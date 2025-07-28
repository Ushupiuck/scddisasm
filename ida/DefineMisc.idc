extern VInterrupt;
extern HInterrupt;
extern ObjectIndex;
extern PLCLists;
extern StageDataIndex;
extern SectionRanges;
extern SectionInitPLCs;
extern SectionUpdatePLCs;
extern SpawnStageObjs_Init;
extern SpawnStageObjs_Main;
extern VInt_General;
extern DoVIntUpdates;
extern StageObjNull;
extern CheckObjDespawn2;
extern debugSpeed;
extern sectCount;

static DefineMisc(void)
{
	auto idx, idx2, count;
	
	VInterrupt = 0;
	HInterrupt = 0;
	ObjectIndex = 0;
	PLCLists = 0;
	StageDataIndex = 0;
	SectionRanges = 0;
	SectionInitPLCs = 0;
	SectionUpdatePLCs = 0;
	SpawnStageObjs_Init = 0;
	SpawnStageObjs_Main = 0;
	VInt_General = 0;
	DoVIntUpdates = 0;
	StageObjNull = 0;
	CheckObjDespawn2 = 0;
	debugSpeed = 0;
	sectCount = 0;
	
	SetMacro(0x200000, 0x100, "MMD\t0, WORDRAM2M, 0, JmpTo_Start, JmpTo_HInt, JmpTo_VInt");
	
	StartFunction(0x200100, "JmpTo_Start");
	StartFunction(0x200106, "JmpTo_Error");
	StartFunction(0x20010C, "JmpTo_HInt");
	StartFunction(0x200112, "JmpTo_VInt");
	StartFuncFromOp(0x200100, "Start", 0);
	StartFuncFromOp(0x200106, "ErrorTrap", 0);
	HInterrupt = StartFuncFromOp(0x20010C, "HInterrupt", 0);
	VInterrupt = StartFuncFromOp(0x200112, "VInterrupt", 0);
	StartFuncFromOp(0x200166, "InitVDP", 0);
	StartFuncFromOp(0x20016A, "InitControllers", 0);
	StartFunction(0x200180, "GameModes");
	StartFuncFromOp(0x200180, "StageStart", 0);
	StartFunction(0x200184, "PaletteCycle");
	
	ObjectIndex = NameFromOp(UpdateObjects + 0x10, "ObjectIndex", 0);
	OpDecimal(UpdateObjects + 0x16, 0);
	
	PLCLists = NameFromOp(AddPLCs + 4, "PLCLists", 0);
	StageDataIndex = NameFromOp(LoadStageData + 2, "StageDataIndex", 0);
	
	debugSpeed = MakeName(DebugMode + 0x190, "debugSpeed");
	MakeName(DebugMode + 0x194, "DebugItemIndex");
	count = Byte(DebugMode + 0x194);
	MakeWord(DebugMode + 0x194);
	SetManualInsn(DebugMode + 0x194, "DBSTART");
	for (idx = 0; idx < count; idx = idx + 1) {
		auto debugItem = DebugMode + 0x196 + (idx * 12);
		MakeStructEx(debugItem, -1, "DBGITEM");

		auto debugMap = Dword(debugItem + 2);
		if (substr(Name(debugMap), 0, 4) != "Spr_") {
			MakeName(debugMap, "Spr_" + sprintf("%X", debugMap));
			AnalyzeMappings(debugMap, "@Spr_" + sprintf("%X", debugMap), 0);
		}
	}

	for (idx = 0; idx < Word(PLCLists); idx = idx + 2) {
		auto plcStart = SetTableOff(PLCLists, idx, "");

		if (idx == 0) {
			MakeName(plcStart, "PLC_Stage");
		} else if (idx == 2) {
			MakeName(plcStart, "PLC_Standard");
		} else if (idx == 4) {
			MakeName(plcStart, "PLC_SectInit0");
		} else if (idx == 0x20) {
			MakeName(plcStart, "PLC_Results");
		} else if (idx == 0x24) {
			MakeName(plcStart, "PLC_Signpost");
		}
		
		if (time == 1) {
			if (idx == 8) {
				MakeName(plcStart, "PLC_Animals");
			} else if (idx == 0xA) {
				MakeName(plcStart, "PLC_ProjAnimals");
			}
		}
		
		MakeWord(plcStart);
		count = Word(plcStart) + 1;
		for (idx2 = 0; idx2 < count; idx2 = idx2 + 1) {
			SetDwordPointer(plcStart + 2 + (idx2 * 6), "");
			MakeWord(plcStart + 6 + (idx2 * 6));
		}
	}
		
	if (LoadSectionArt > 0) {
		SectionRanges = NameFromOp(LoadSectionArt, "SectionRanges", 0);
		SectionInitPLCs = NameFromOp(LoadSectionArt + 0x1A, "SectionInitPLCs", 0);
		SectionUpdatePLCs = NameFromOp(UpdateSectionArt + 0x24, "SectionUpdatePLCs", 0);

		auto done = 0;
		sectCount = 0;
		while (done == 0) {
			MakeWord(SectionRanges + (sectCount * 2));
			if (Word(SectionRanges + (sectCount * 2)) == 0xFFFF) {
				done = 1;
			}
			sectCount = sectCount + 1;
		}
		
		for (idx = 0; idx < sectCount; idx = idx + 1) {
			MakeWord(SectionInitPLCs + (idx * 2));
			MakeWord(SectionUpdatePLCs + (idx * 2));
			
			auto plcInit = PLCLists + Word(PLCLists + (Word(SectionInitPLCs + (idx * 2)) * 2));
			auto plcUpdate = PLCLists + Word(PLCLists + (Word(SectionUpdatePLCs + (idx * 2)) * 2));
			
			if (substr(Name(plcInit), 0, 4) != "PLC_") {
				MakeName(plcInit, "PLC_SectInit" + sprintf("%X", idx));
			}
			if (substr(Name(plcUpdate), 0, 4) != "PLC_") {
				MakeName(plcUpdate, "PLC_SectUpdate" + sprintf("%X", idx));
			}
		}
	}
	
	SpawnStageObjs_Init = SetTableOffFunc(SpawnStageObjects + 0xE, 0, "SpawnStageObjs_Init");
	SpawnStageObjs_Main = SetTableOffFunc(SpawnStageObjects + 0xE, 2, "SpawnStageObjs_Main");
	
	SetTableOff(StageObjLayouts, 0, "");
	StageObjNull = SetTableOff(StageObjLayouts, 2, "StageObjNull");
	ForceFormattedWordArray(StageObjects - 8, 0, 4, 4, -1);
	ForceFormattedWordArray(StageObjNull, 0, 3, 3, -1);
	
	for (idx = 0; idx < Word(StageLayouts); idx = idx + 2) {
		SetTableOff(StageLayouts, idx, "");
	}
	
	for (idx = PaletteTable; idx < Dword(PaletteTable); idx = idx + 8) {
		ForceDword(idx);
		OpOff(idx, 0, 0);
		ForceWord(idx + 4);
		ForceWord(idx + 6);
	}
	
	auto vintPos = VInterrupt;
	while (Word(vintPos) != 0x4EBB) {
		vintPos = vintPos + 2;
	}
	auto vintTable = NameFromOp(vintPos, "VInt_Index", 0);
	
	SetTableOffFunc(vintTable, 0, "VInt_Lag");
	VInt_General = SetTableOffFunc(vintTable, 2, "VInt_General");
	SetTableOffFunc(vintTable, 4, "VInt_S1Title");
	SetTableOffFunc(vintTable, 6, "VInt_Unk6");
	SetTableOffFunc(vintTable, 8, "VInt_Stage");
	SetTableOffFunc(vintTable, 0xA, "VInt_S1SpecStg");
	SetTableOffFunc(vintTable, 0xC, "VInt_StageLoad");
	SetTableOffFunc(vintTable, 0xE, "VInt_UnkE");
	SetTableOffFunc(vintTable, 0x10, "VInt_Pause");
	SetTableOffFunc(vintTable, 0x12, "VInt_PalFade");
	SetTableOffFunc(vintTable, 0x14, "VInt_S1SegaScr");
	SetTableOffFunc(vintTable, 0x16, "VInt_S1ContScr");
	SetTableOffFunc(vintTable, 0x18, "VInt_StageLoad");
	
	DoVIntUpdates = StartFuncFromOp(VInt_General, "DoVIntUpdates", 0);
	
	CheckObjDespawn2 = CheckObjDespawn + 4;

	idx = ObjectIndex;
	while (idx < ObjNull)
	{
		SetDwordPointer(idx, "");
		idx = idx + 4;
	}
	
	if (WaterEvents > 0)
	{
		idx = WaterEvents + 0x2C;
		SetTableOffFunc(idx, 0, "WaterEvents_Act1");
		SetTableOffFunc(idx, 2, "WaterEvents_Act2");
		SetTableOffFunc(idx, 4, "WaterEvents_Act3");
	}
}
