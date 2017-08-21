#ifndef STAGESELECT_H
#define STAGESELECT_H


enum STAGE_ID {
	STAGE_1,
	STAGE_2,
	STAGE_3,
	STAGE_4,
	STAGE_5
};

STAGE_ID GetStage_ID();
void StageSelect(STAGE_ID nextStage);

#endif
