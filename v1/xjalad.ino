#include <SD.h>
#include "Wire.h"
#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/sleep.h>

#define DS1307_I2C_ADDRESS 0x68


#define LED0 2 //RED
#define LED1 3 //BLUE
#define LED2 7
#define LED3 8
#define LED4 9

#define CS 10
#define SW0_0 4
#define SW0_1 5
#define SW1_0 A6
#define SW1_1 A7
#define FSR0 A2 


#define MAX 0.8
#define MIN 0.2
//second -15624
//200 Hz - 3125
#define TIMER1_PERIOD 78
#define WFREQ 200 //Hz
#define WLAST 5
#define N 200*5

const int periods[3]={5,15,60};
byte chosenPeriod=5;
byte seconds=0;
int msecs=0;
volatile int n=0;
volatile boolean needToMeasure =true;
boolean samplesReady = false;
float tempSum;
Sd2Card card;
SdVolume volume;
SdFile root;
File myFile;
// month+day+hour+minute
char fileName[13];

// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return ( (val/16*10) + (val%16) );
}




String parseDateTimeValue(byte n){
  if (n<10)
    return ("0"+String(n, DEC));
  return String(n, DEC);
}


// Gets the date and time from the ds1307
void getDateDs1307(byte *second,
          byte *minute,
          byte *hour,
          byte *dayOfWeek,
          byte *dayOfMonth,
          byte *month,
          byte *year)
{
  // Reset the register pointer
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  Wire.write(0);
  Wire.endTransmission();
  
  Wire.requestFrom(DS1307_I2C_ADDRESS, 7);

  // A few of these need masks because certain bits are control bits
  *second     = bcdToDec(Wire.read() & 0x7f);
  *minute     = bcdToDec(Wire.read());
  *hour       = bcdToDec(Wire.read() & 0x3f);  // Need to change this if 12 hour am/pm
  *dayOfWeek  = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month      = bcdToDec(Wire.read());
  *year       = bcdToDec(Wire.read());
}

String getTimeStamp(void){
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  getDateDs1307(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
 
  return (parseDateTimeValue(hour)+":"+parseDateTimeValue(minute)+":"+parseDateTimeValue(second)+"  "+parseDateTimeValue(dayOfMonth)+"/"+parseDateTimeValue(month)+"/"+parseDateTimeValue(year));
}

String getTimeStampForFileName(void){
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  getDateDs1307(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
 
  return (parseDateTimeValue(month)+parseDateTimeValue(dayOfMonth)+parseDateTimeValue(hour)+parseDateTimeValue(minute)) ;
}

boolean cardSetup(void){

  pinMode(CS, OUTPUT);     // change this to 53 on a mega
  // we'll use the initialization code from the utility libraries
  // since we're just testing if the card is working!
  if (!card.init(SPI_HALF_SPEED, CS)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card is inserted?");
    Serial.println("* Is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    return false;
  } else {
   Serial.println("Wiring is correct and a card is present."); 
  }
  
  if (!SD.begin()) {
    Serial.println("initialization failed!");

  }
  Serial.println("initialization done.");
  
  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
  if (!volume.init(card)) {
    Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    return false;
  }
  root.openRoot(volume);
  return true;
}

void pinSetup(void){
  pinMode(SW1_0, INPUT);           // set pin to input
  pinMode(SW1_1, INPUT);           // set pin to input
  pinMode(FSR0,INPUT);
  
  //digitalWrite(pin, HIGH);       // turn on pullup resistors
}

byte readFreqSwitch(void){
  if (analogRead(SW1_0)>1022) return 0;
  if (analogRead(SW1_1)>1022) return 2;
  return 1;
} 

byte readModeSwitch(void){
  if (digitalRead(SW0_0)) return 0;
  if (digitalRead(SW0_1)) return 2;
  return 1;
}

float readSensor(byte n){
  
  switch (n){
    case 0: return analogRead(FSR0)/1023.0; break;
    case 1: return analogRead(FSR0)/1023.0; break;
    case 2: return analogRead(FSR0)/1023.0; break;
    case 3: return analogRead(FSR0)/1023.0; break;
  }
  //WARNING
  return -1;
}

String processSamples(){
  char buf [7];
  float average=tempSum/n;
  
  digitalWrite(LED2,LOW);
  digitalWrite(LED3,LOW);
  digitalWrite(LED4,LOW);
  
  if (average>MAX)
    digitalWrite(LED2,HIGH);
  else if (average<MIN)
    digitalWrite(LED4,HIGH);
  else
    digitalWrite(LED3,HIGH);

  dtostrf(average,1,3,buf);
  return (buf);
}

boolean createFile(){
  /*
  byte findex=1;
  while (true){
    if (!SD.exists("s.txt"))
      break;
    findex++;
  }
  */
  (String(getTimeStampForFileName()+".txt")).toCharArray(fileName,13);
  //fileName=getTimeStampForFileName();
  myFile=SD.open(fileName,FILE_WRITE);

  
  return true;
   
}

void configureInterrupt(int CV){
  cli();
  //set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = CV;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler
  // TIMER1_PRESCALER
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  sei();
}


void setup(void){
  Wire.begin();
  Serial.begin(9600);
 
  //setDateDs1307(15, 25, 16, 1, 22, 7, 13);
  Serial.println("Started!");
  
}


ISR(TIMER1_COMPA_vect){
  //Serial.println("second");
  
  if ((msecs+=5)>=1000) {
    seconds++;
    msecs=0;
  }
  
  if (needToMeasure){
    if (n<N){
      tempSum+=readSensor(0);
      n++;
    }
    else {
      samplesReady=true;
      needToMeasure=false;
      Serial.println("Samples ready");
    }
  }
   
  if (seconds>=chosenPeriod){
    seconds=0;
    msecs=0;
    n=0;
    tempSum=0;
    needToMeasure=true;
   // myFile.println(getTimeStamp()+' '+measure());
    
  }
  
}

void loop(void) {
 
  
  //Serial print mode
  
  if (readModeSwitch()==0){
    digitalWrite(LED1,HIGH);
    
    while (readModeSwitch()==0){
      char buf [7];
      String s=getTimeStamp()+' '+processSamples();
      //dtostrf(readSensor(0),1,3,buf);
      
       Serial.println(s);
       delay(500);
    }
    
    digitalWrite(LED1,LOW);
  }
  // Mem Card Store Mode
  if (readModeSwitch()==2){
     
    if (cardSetup() && createFile()){
     digitalWrite(LED0,HIGH);//RED
    chosenPeriod=15;//periods[readFreqSwitch()];
    seconds=0;
    msecs=0;
    n=0;
    needToMeasure=true;
    samplesReady=false;
    configureInterrupt(TIMER1_PERIOD);
    Serial.println("Mem Card start");
    while (readModeSwitch()==2){
      
       //Serial.println(measure());
       if (samplesReady){
         samplesReady=false;
         String s=getTimeStamp()+' '+processSamples();
         myFile=SD.open(fileName,FILE_WRITE);
         Serial.println(fileName);
         if (myFile){
           myFile.println(s);
           myFile.close();
           Serial.println(s);
         }
         else 
           Serial.println("Opening file failed");
         
       }
       //sleep()
       
       set_sleep_mode(SLEEP_MODE_PWR_SAVE);
        
        sleep_enable();
        sleep_mode();
        /* wake up here */
        sleep_disable();
       
    }
    
    digitalWrite(LED0,LOW);
    }
  }  
  
}
