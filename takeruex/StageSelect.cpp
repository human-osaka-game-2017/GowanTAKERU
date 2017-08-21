#include"StageSelect.h"
#include"FileManagement.h"
#include<stdio.h>

STAGE_ID g_Stage_ID = STAGE_1;


STAGE_ID GetStage_ID() {
	return g_Stage_ID;
}

void StageSelect(STAGE_ID nextStage) {
	g_Stage_ID = nextStage;
}