<?xml version='1.0' encoding='UTF-8'?>
<Project Type="Project" LVVersion="14008000">
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
		<Item Name="AlloysLogger.vi" Type="VI" URL="../AlloysLogger.vi"/>
		<Item Name="CriticalOpalescenceLogger.vi" Type="VI" URL="../CriticalOpalescenceLogger.vi"/>
		<Item Name="FranckHertzLogger.vi" Type="VI" URL="../FranckHertzLogger.vi"/>
		<Item Name="ThermalDiffusivity.vi" Type="VI" URL="../ThermalDiffusivity.vi"/>
		<Item Name="Dependencies" Type="Dependencies">
			<Item Name="vi.lib" Type="Folder">
				<Item Name="BuildHelpPath.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/BuildHelpPath.vi"/>
				<Item Name="Check Special Tags.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Check Special Tags.vi"/>
				<Item Name="Clear Errors.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Clear Errors.vi"/>
				<Item Name="Convert property node font to graphics font.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Convert property node font to graphics font.vi"/>
				<Item Name="Details Display Dialog.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Details Display Dialog.vi"/>
				<Item Name="DialogType.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/DialogType.ctl"/>
				<Item Name="DialogTypeEnum.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/DialogTypeEnum.ctl"/>
				<Item Name="Error Code Database.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Error Code Database.vi"/>
				<Item Name="ErrWarn.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/ErrWarn.ctl"/>
				<Item Name="eventvkey.ctl" Type="VI" URL="/&lt;vilib&gt;/event_ctls.llb/eventvkey.ctl"/>
				<Item Name="ex_CorrectErrorChain.vi" Type="VI" URL="/&lt;vilib&gt;/express/express shared/ex_CorrectErrorChain.vi"/>
				<Item Name="Find Tag.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Find Tag.vi"/>
				<Item Name="Format Message String.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Format Message String.vi"/>
				<Item Name="General Error Handler Core CORE.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/General Error Handler Core CORE.vi"/>
				<Item Name="General Error Handler.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/General Error Handler.vi"/>
				<Item Name="Get File Extension.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/libraryn.llb/Get File Extension.vi"/>
				<Item Name="Get String Text Bounds.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Get String Text Bounds.vi"/>
				<Item Name="Get Text Rect.vi" Type="VI" URL="/&lt;vilib&gt;/picture/picture.llb/Get Text Rect.vi"/>
				<Item Name="GetHelpDir.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/GetHelpDir.vi"/>
				<Item Name="GetRTHostConnectedProp.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/GetRTHostConnectedProp.vi"/>
				<Item Name="Longest Line Length in Pixels.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Longest Line Length in Pixels.vi"/>
				<Item Name="LVBoundsTypeDef.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/miscctls.llb/LVBoundsTypeDef.ctl"/>
				<Item Name="LVRectTypeDef.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/miscctls.llb/LVRectTypeDef.ctl"/>
				<Item Name="Not Found Dialog.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Not Found Dialog.vi"/>
				<Item Name="Search and Replace Pattern.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Search and Replace Pattern.vi"/>
				<Item Name="Set Bold Text.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Set Bold Text.vi"/>
				<Item Name="Set String Value.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Set String Value.vi"/>
				<Item Name="subFile Dialog.vi" Type="VI" URL="/&lt;vilib&gt;/express/express input/FileDialogBlock.llb/subFile Dialog.vi"/>
				<Item Name="TagReturnType.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/TagReturnType.ctl"/>
				<Item Name="Three Button Dialog CORE.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Three Button Dialog CORE.vi"/>
				<Item Name="Three Button Dialog.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Three Button Dialog.vi"/>
				<Item Name="Trim Whitespace.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Trim Whitespace.vi"/>
				<Item Name="VISA Configure Serial Port" Type="VI" URL="/&lt;vilib&gt;/Instr/_visa.llb/VISA Configure Serial Port"/>
				<Item Name="VISA Configure Serial Port (Instr).vi" Type="VI" URL="/&lt;vilib&gt;/Instr/_visa.llb/VISA Configure Serial Port (Instr).vi"/>
				<Item Name="VISA Configure Serial Port (Serial Instr).vi" Type="VI" URL="/&lt;vilib&gt;/Instr/_visa.llb/VISA Configure Serial Port (Serial Instr).vi"/>
				<Item Name="whitespace.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/whitespace.ctl"/>
				<Item Name="Write Spreadsheet String.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/file.llb/Write Spreadsheet String.vi"/>
				<Item Name="Write To Spreadsheet File (DBL).vi" Type="VI" URL="/&lt;vilib&gt;/Utility/file.llb/Write To Spreadsheet File (DBL).vi"/>
				<Item Name="Write To Spreadsheet File (I64).vi" Type="VI" URL="/&lt;vilib&gt;/Utility/file.llb/Write To Spreadsheet File (I64).vi"/>
				<Item Name="Write To Spreadsheet File (string).vi" Type="VI" URL="/&lt;vilib&gt;/Utility/file.llb/Write To Spreadsheet File (string).vi"/>
				<Item Name="Write To Spreadsheet File.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/file.llb/Write To Spreadsheet File.vi"/>
			</Item>
			<Item Name="FGV_COM_PortHandler.vi" Type="VI" URL="../FGV_COM_PortHandler.vi"/>
			<Item Name="FGV_DataLoggerControl.vi" Type="VI" URL="../FGV_DataLoggerControl.vi"/>
			<Item Name="Serial - Settings.ctl" Type="VI" URL="../Serial - Settings.ctl"/>
			<Item Name="SVI_AL_Parser.vi" Type="VI" URL="../SVI_AL_Parser.vi"/>
			<Item Name="SVI_Calculate_checksum.vi" Type="VI" URL="../SVI_Calculate_checksum.vi"/>
			<Item Name="SVI_CheckAndIncrementFilename.vi" Type="VI" URL="../SVI_CheckAndIncrementFilename.vi"/>
			<Item Name="SVI_CO_Parser.vi" Type="VI" URL="../SVI_CO_Parser.vi"/>
			<Item Name="SVI_ConvertStringBufferAndBuildGraphs.vi" Type="VI" URL="../SVI_ConvertStringBufferAndBuildGraphs.vi"/>
			<Item Name="SVI_ExtractReadings.vi" Type="VI" URL="../SVI_ExtractReadings.vi"/>
			<Item Name="SVI_FH_Parser.vi" Type="VI" URL="../SVI_FH_Parser.vi"/>
			<Item Name="SVI_SerialReceive.vi" Type="VI" URL="../SVI_SerialReceive.vi"/>
			<Item Name="SVI_SerialSendAndReceive.vi" Type="VI" URL="../SVI_SerialSendAndReceive.vi"/>
			<Item Name="SVI_SerialSendAndReceiveChecksumWithACKNAK.vi" Type="VI" URL="../SVI_SerialSendAndReceiveChecksumWithACKNAK.vi"/>
			<Item Name="SVI_SetupSerialPort.vi" Type="VI" URL="../SVI_SetupSerialPort.vi"/>
			<Item Name="SVI_SpeciesChecker.vi" Type="VI" URL="../SVI_SpeciesChecker.vi"/>
			<Item Name="SVI_TD_Parser.vi" Type="VI" URL="../SVI_TD_Parser.vi"/>
			<Item Name="Typedef_DataLogger_Actions.ctl" Type="VI" URL="../Typedef_DataLogger_Actions.ctl"/>
			<Item Name="Typedef_FindPortActions.ctl" Type="VI" URL="../Typedef_FindPortActions.ctl"/>
			<Item Name="Typedef_TeachLabArduinos.ctl" Type="VI" URL="../Typedef_TeachLabArduinos.ctl"/>
		</Item>
		<Item Name="Build Specifications" Type="Build">
			<Item Name="AlloysLogger" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{2B990E93-CA98-4CA0-97BB-7661CC283DD5}</Property>
				<Property Name="App_INI_GUID" Type="Str">{A732379F-8099-47C1-8117-78034AF9F5B5}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{70934624-7150-4DDE-83A3-8E63B818DC6F}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">AlloysLogger</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../builds/AlloysLogger</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToProject</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{53512840-8919-4E48-ADC5-3D8FB77CD7C8}</Property>
				<Property Name="Bld_version.build" Type="Int">8</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">AlloysLogger.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../builds/AlloysLogger/AlloysLogger.exe</Property>
				<Property Name="Destination[0].path.type" Type="Str">relativeToProject</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../builds/AlloysLogger/data</Property>
				<Property Name="Destination[1].path.type" Type="Str">relativeToProject</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Source[0].itemID" Type="Str">{08F40DCB-204B-4BE3-B953-55F4B56B83C0}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/AlloysLogger.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">AlloysLogger
For Support, contact:
andrew.murray@manchester.ac.uk
matthew.harvey@manchester.ac.uk
matthew.a.harvey@gmail.com</Property>
				<Property Name="TgtF_internalName" Type="Str">AlloysLogger</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">AlloysLogger</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{5E57730C-D0AA-4FCF-8637-DA367EF705F9}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">AlloysLogger.exe</Property>
			</Item>
			<Item Name="CriticalOpalescenceLogger" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{165D270F-773A-4795-9184-9FDB3FE441BB}</Property>
				<Property Name="App_INI_GUID" Type="Str">{0040537A-CDF3-40E6-80BC-FD84360E4DD3}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{5859F1EB-D99A-4153-B40E-BDB9B800A33B}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">CriticalOpalescenceLogger</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../builds/CriticalOpalescenceLogger</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToProject</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{E2739B4A-A16F-4887-A7C6-55D1635C13E8}</Property>
				<Property Name="Bld_version.build" Type="Int">8</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">CriticalOpalescenceLogger.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../builds/CriticalOpalescenceLogger/CriticalOpalescenceLogger.exe</Property>
				<Property Name="Destination[0].path.type" Type="Str">relativeToProject</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../builds/CriticalOpalescenceLogger/data</Property>
				<Property Name="Destination[1].path.type" Type="Str">relativeToProject</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Source[0].itemID" Type="Str">{08F40DCB-204B-4BE3-B953-55F4B56B83C0}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/CriticalOpalescenceLogger.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">AlloysLogger
For Support, contact:
andrew.murray@manchester.ac.uk
matthew.harvey@manchester.ac.uk
matthew.a.harvey@gmail.com</Property>
				<Property Name="TgtF_internalName" Type="Str">CriticalOpalescenceLogger</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">CriticalOpalescenceLogger</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{6C30D907-2692-41D4-90DD-4FF8C6C211F3}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">CriticalOpalescenceLogger.exe</Property>
			</Item>
			<Item Name="FranckHertzLogger" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{C22634D7-76CD-4BBD-9FE4-8991759AA7AC}</Property>
				<Property Name="App_INI_GUID" Type="Str">{71727861-B72B-4871-BE38-B7727A9BBA7B}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{A2DB8445-B8F4-4A03-A8C0-81FBC53C5933}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">FranckHertzLogger</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../builds/FranckHertzLogger</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToProject</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{96519F24-7024-4E87-8541-970CC10760CA}</Property>
				<Property Name="Bld_version.build" Type="Int">8</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">FranckHertzLogger.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../builds/FranckHertzLogger/FranckHertzLogger.exe</Property>
				<Property Name="Destination[0].path.type" Type="Str">relativeToProject</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../builds/FranckHertzLogger/data</Property>
				<Property Name="Destination[1].path.type" Type="Str">relativeToProject</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Source[0].itemID" Type="Str">{08F40DCB-204B-4BE3-B953-55F4B56B83C0}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/FranckHertzLogger.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">AlloysLogger
For Support, contact:
andrew.murray@manchester.ac.uk
matthew.harvey@manchester.ac.uk
matthew.a.harvey@gmail.com</Property>
				<Property Name="TgtF_internalName" Type="Str">FranckHertzLogger</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">FranckHertzLogger</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{AA0DA18C-B1DF-499F-A875-59985B5031D5}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">FranckHertzLogger.exe</Property>
			</Item>
			<Item Name="ThermalDiffusivity" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{FAC8FF44-52ED-48B2-A338-62A649F0EB3B}</Property>
				<Property Name="App_INI_GUID" Type="Str">{11B519BE-29E4-46C0-94DB-83806883095D}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{C61E9B0F-1290-4711-821D-1DFD7104B531}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">ThermalDiffusivity</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../builds/ThermalDiffusivity</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToProject</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{014EFC3F-5502-4E34-81DF-1F517AB9962D}</Property>
				<Property Name="Bld_version.build" Type="Int">9</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">ThermalDiffusivity.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../builds/ThermalDiffusivity/ThermalDiffusivity.exe</Property>
				<Property Name="Destination[0].path.type" Type="Str">relativeToProject</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../builds/ThermalDiffusivity/data</Property>
				<Property Name="Destination[1].path.type" Type="Str">relativeToProject</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Source[0].itemID" Type="Str">{A6364C8F-9979-454C-AE97-CD8F4E21C835}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/FranckHertzLogger.vi</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="Source[2].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[2].itemID" Type="Ref">/My Computer/ThermalDiffusivity.vi</Property>
				<Property Name="Source[2].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[2].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">3</Property>
				<Property Name="TgtF_fileDescription" Type="Str">ThermalDiffusivity
For Support, contact:
andrew.murray@manchester.ac.uk
matthew.harvey@manchester.ac.uk
matthew.a.harvey@gmail.com</Property>
				<Property Name="TgtF_internalName" Type="Str">ThermalDiffusivity</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">ThermalDiffusivity</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{1956DD17-6AAD-4C4E-9121-4C38D0246E4A}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">ThermalDiffusivity.exe</Property>
			</Item>
		</Item>
	</Item>
</Project>
