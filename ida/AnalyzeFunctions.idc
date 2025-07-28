extern HUD_DrawPos;
extern HUD_DrawNum;
extern HUD_ResetScore;
extern HUD_DrawScore;
extern HUD_ResetRings;
extern HUD_DrawRings;
extern HUD_DrawMins;
extern HUD_DrawSecs;
extern HUD_DrawLives;
extern HUD_DrawBonus;
extern HUD_ResetNumber;
extern HUD_DrawHexNum;
extern HUD_DrawCounter;
extern ContScrCounter;
extern LoadCollision;

static DefineKnownFunctions(void)
{
	HUD_DrawPos = StartFuncFromOp(UpdateHUD + 8, "HUD_DrawPos", 0);
	HUD_DrawNum = StartFuncFromOp(UpdateHUD + 0x4E, "HUD_DrawNum", 0);
	HUD_ResetScore = StartFuncFromOp(UpdateHUD + 0x60, "HUD_ResetScore", 0);
	HUD_DrawScore = StartFuncFromOp(UpdateHUD + 0x76, "HUD_DrawScore", 0);
	HUD_ResetRings = StartFuncFromOp(UpdateHUD + 0x84, "HUD_ResetRings", 0);
	HUD_DrawRings = StartFuncFromOp(UpdateHUD + 0xAC, "HUD_DrawRings", 0);
	HUD_DrawMins = StartFuncFromOp(UpdateHUD + 0x114, "HUD_DrawMins", 0);
	HUD_DrawSecs = StartFuncFromOp(UpdateHUD + 0x126, "HUD_DrawSecs", 0);
	HUD_DrawLives = StartFuncFromOp(UpdateHUD + 0x16A, "HUD_DrawLives", 0);
	HUD_DrawBonus = StartFuncFromOp(UpdateHUD + 0x194, "HUD_DrawBonus", 0);
	HUD_ResetNumber = StartFuncFromOp(HUD_ResetRings + 0x12, "HUD_ResetNumber", 0);
	HUD_DrawHexNum = StartFuncFromOp(HUD_DrawPos + 0xC, "HUD_DrawHexNum", 0);
	HUD_DrawCounter = StartFunction(HUD_DrawScore + 8, "HUD_DrawCounter");
	ContScrCounter = StartFunction(HUD_DrawCounter + 0x58, "ContScrCounter");
	
	if (zone == 2) {
		LoadCollision = StartFunction(UpdateGlobalAnims - 0xA, "LoadCollision");
	} else {
		LoadCollision = StartFunction(UpdateGlobalAnims - 0x32, "LoadCollision");
	}
}
