<?xml version='1.0' encoding='UTF-8'?>
<Project Type="Project" LVVersion="12008004">
	<Item Name="My Computer" Type="My Computer">
		<Property Name="server.app.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.control.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.tcp.enabled" Type="Bool">false</Property>
		<Property Name="server.tcp.port" Type="Int">0</Property>
		<Property Name="server.tcp.serviceName" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.tcp.serviceName.default" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.vi.callsEnabled" Type="Bool">true</Property>
		<Property Name="server.vi.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="specify.custom.address" Type="Bool">false</Property>
		<Item Name="SetAtributes.vi" Type="VI" URL="../SetAtributes.vi"/>
		<Item Name="Dependencies" Type="Dependencies">
			<Item Name="vi.lib" Type="Folder">
				<Item Name="VISA Configure Serial Port" Type="VI" URL="/&lt;vilib&gt;/Instr/_visa.llb/VISA Configure Serial Port"/>
				<Item Name="VISA Configure Serial Port (Instr).vi" Type="VI" URL="/&lt;vilib&gt;/Instr/_visa.llb/VISA Configure Serial Port (Instr).vi"/>
				<Item Name="VISA Configure Serial Port (Serial Instr).vi" Type="VI" URL="/&lt;vilib&gt;/Instr/_visa.llb/VISA Configure Serial Port (Serial Instr).vi"/>
				<Item Name="VISA Find Search Mode.ctl" Type="VI" URL="/&lt;vilib&gt;/Instr/_visa.llb/VISA Find Search Mode.ctl"/>
			</Item>
			<Item Name="DevicePortFinder.vi" Type="VI" URL="../DevicePortFinder.vi"/>
			<Item Name="GetVariable.vi" Type="VI" URL="../GetVariable.vi"/>
			<Item Name="SetVariable.vi" Type="VI" URL="../SetVariable.vi"/>
		</Item>
		<Item Name="Build Specifications" Type="Build">
			<Item Name="SetSettings" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{4036E2B8-E9D8-4617-8E7A-4A7647574884}</Property>
				<Property Name="App_INI_GUID" Type="Str">{AB4103D8-4A94-44FE-9DE0-02B256CAE4BF}</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{6CCE811C-1169-4E5B-B249-0F203C4B044A}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">SetSettings</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../builds/NI_AB_PROJECTNAME/SetSettings</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{7C3C98CB-99CA-41D1-B9FC-F55B0D0F2B28}</Property>
				<Property Name="Destination[0].destName" Type="Str">SetSettings.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../builds/NI_AB_PROJECTNAME/SetSettings/SetSettings.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../builds/NI_AB_PROJECTNAME/SetSettings/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Source[0].itemID" Type="Str">{A4544625-4FCA-4B8A-9414-09FD48EEF85C}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/SetAtributes.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">SetSettings</Property>
				<Property Name="TgtF_fileVersion.major" Type="Int">1</Property>
				<Property Name="TgtF_internalName" Type="Str">SetSettings</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2014 </Property>
				<Property Name="TgtF_productName" Type="Str">SetSettings</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{1C917CF1-A905-4868-9AED-F549DB2B8813}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">SetSettings.exe</Property>
			</Item>
		</Item>
	</Item>
</Project>
