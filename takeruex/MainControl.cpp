#include"MainControl.h"
#include"PlayerControl.h"
#include"EnemyControl.h"
#include"BulletControl.h"
#include"MapControl.h"
#include"MainHitManagement.h"
#include"MoveManagement.h"

void MainControl() {

	PlayerControl();
	EnemyControl();
	BulletControl();
	MapComtrol();
	HitManage();
	MoveManage();
}



double Calculate_rad(float x1, float y1, float x2, float y2) {

	double rad;

	double lengthX = x2 - x1;
	double lengthY = y2 - y1;

	if (0 < lengthX) {
		if (lengthY < 0) {

			rad = atan(lengthY / lengthX);

		}
		else {

			rad = atan(lengthY / lengthX) + (2 * D3DX_PI);

		}
	}
	else if (lengthX < 0) {

		rad = atan(lengthY / lengthX) + D3DX_PI;

	}
	else if (lengthX == 0) {
		if (0 < lengthY) {

			rad = 3 / 2 * D3DX_PI;

		}
		else {

			rad = D3DX_PI / 2;

		}
	}

	rad = (2 * D3DX_PI) - rad;

	return rad;
}

//struct VertexInfo {
//	D3DXVECTOR2 Pos;
//	MapNumXY mapNum;
//};
//
//void DataUpdate(VertexInfo* vertexInfo, D3DXVECTOR2& currentPos, float width, float height);
//
//void PreventIntoPos(D3DXVECTOR2& oldPos, D3DXVECTOR2* currentPos,float width,float height) {
//
//	VertexInfo vertex[4];
//
//	DataUpdate(vertex, *currentPos, width, height);
//
//	D3DXVECTOR2 frDiff;
//	frDiff.x = currentPos->x - oldPos.x;
//	frDiff.y = currentPos->x - oldPos.x;
//
//	//?ŒÂ‚É‚Ü‚½‚ª‚Á‚Ä‚¢‚é(‹«ŠEü‚ÌŒÂ”j
//	int widthSizeNum = width / TIPSIZE;
//	int heightSizeNum = height / TIPSIZE;
//
//	if (frDiff.x < 0) {
//		if (frDiff.y < 0) {
//			//¶ã‚ÉŒü‚©‚Á‚Ä“®‚¢‚Ä‚¢‚é
//
//			D3DXVECTOR2 tmpPos = vertex[0].Pos;
//
//			//‘¤–Ê‚¾‚¯‚ğŒ©‚Ü‚·
//			//ã‚Ì–Ê‚ª“–‚½‚Á‚Ä‚¢‚é‚Æ‚«
//			for (int i = 0; i < widthSizeNum + 1; i++) {
//				tmpPos.x += TIPSIZE*i;
//				if (MapKindSpecifyForPos(&tmpPos) != NOTHING) {
//
//					float diffY = (vertex[0].mapNum.NumY*TIPSIZE) - vertex[0].Pos.y;
//
//					float diffX = frDiff.x;
//					diffX *= diffY / frDiff.y;
//
//					currentPos->y += diffY;
//					currentPos->x += diffX;
//					break;
//				}
//			}
//			DataUpdate(vertex, *currentPos, width, height);
//			tmpPos.x = vertex[0].Pos.x;
//			//¶‚Ì–Ê‚ª“–‚½‚Á‚Ä‚¢‚é‚Æ‚«
//			for (int i = 0; i < heightSizeNum; i++) {
//				tmpPos.y += TIPSIZE*i;
//				if (MapKindSpecifyForPos(&tmpPos) != NOTHING) {
//
//					float diffX = (vertex[0].mapNum.NumX*TIPSIZE) - vertex[0].Pos.x;
//
//					float diffY = frDiff.y;
//					diffY *= diffX / frDiff.x;
//
//					currentPos->y += diffY;
//					currentPos->x += diffX;
//					break;
//				}
//			}
//			DataUpdate(vertex, *currentPos, width, height);
//		}
//		else if (0 < frDiff.y) {
//			//¶‰º‚ÉŒü‚©‚Á‚Ä“®‚¢‚Ä‚¢‚é
//
//			D3DXVECTOR2 tmpPos = vertex[3].Pos;
//
//			//‘¤–Ê‚¾‚¯‚ğŒ©‚Ü‚·
//			//¶‚Ì–Ê‚ª“–‚½‚Á‚Ä‚¢‚é‚Æ‚«
//			for (int i = 0; i < heightSizeNum; i++) {
//				tmpPos.y -= TIPSIZE*i;
//				if (MapKindSpecifyForPos(&tmpPos) != NOTHING) {
//
//					float diffX = (vertex[3].mapNum.NumX*TIPSIZE) - vertex[3].Pos.x;
//
//					float diffY = frDiff.y;
//					diffY *= diffX / frDiff.x;
//
//					currentPos->y += diffY;
//					currentPos->x += diffX;
//					break;
//				}
//			}
//			DataUpdate(vertex, *currentPos, width, height);
//			tmpPos.x = vertex[3].Pos.x;
//			//‰º‚Ì–Ê‚ª“–‚½‚Á‚Ä‚¢‚é‚Æ‚«
//			for (int i = 0; i < widthSizeNum + 1; i++) {
//				tmpPos.x += TIPSIZE*i;
//				if (MapKindSpecifyForPos(&tmpPos) != NOTHING) {
//
//					float diffY = vertex[3].Pos.y - (vertex[3].mapNum.NumY*TIPSIZE);
//
//					float diffX = frDiff.x;
//					diffX *= diffY / frDiff.y;
//
//					currentPos->y += diffY;
//					currentPos->x += diffX;
//					break;
//				}
//			}
//
//			DataUpdate(vertex, *currentPos, width, height);
//		}
//		else if (frDiff.y == 0) {
//			//¶‚ÉŒü‚©‚Á‚Ä“®‚¢‚Ä‚¢‚é	
//			D3DXVECTOR2 tmpPos = vertex[0].Pos;
//
//			//¶‚Ì–Ê‚ª“–‚½‚Á‚Ä‚¢‚é‚Æ‚«
//			for (int i = 0; i < heightSizeNum; i++) {
//				tmpPos.y += TIPSIZE*i;
//				if (MapKindSpecifyForPos(&tmpPos) != NOTHING) {
//
//					float diffX = (vertex[0].mapNum.NumX*TIPSIZE) - vertex[0].Pos.x;
//
//					float diffY = frDiff.y;
//					diffY *= diffX / frDiff.x;
//
//					currentPos->y += diffY;
//					currentPos->x += diffX;
//					break;
//				}
//			}
//		}
//	}
//	else if (0 < frDiff.x) {
//		if (frDiff.y < 0) {
//
//		}
//		else if (0 < frDiff.y) {
//
//		}
//		else if (frDiff.y == 0) {
//
//		}
//	}
//	else if (frDiff.x == 0) {
//		if (frDiff.y < 0) {
//
//		}
//		else if (0 < frDiff.y) {
//
//		}
//		else if (frDiff.y == 0) {
//
//		}
//	}
//}
//
//void DataUpdate(VertexInfo* vertexInfo, D3DXVECTOR2& currentPos, float width, float height){
//
//	vertexInfo[1].Pos.x = vertexInfo[2].Pos.x = currentPos.x + width / 2;
//	vertexInfo[0].Pos.x = vertexInfo[3].Pos.x = currentPos.x - width / 2;
//	vertexInfo[0].Pos.y = vertexInfo[1].Pos.y = currentPos.y - height / 2;
//	vertexInfo[3].Pos.y = vertexInfo[2].Pos.y = currentPos.y + height / 2;
//
//
//	MapchipNumberSpecify(&vertexInfo[0].mapNum, &vertexInfo[0].Pos);
//	MapchipNumberSpecify(&vertexInfo[1].mapNum, &vertexInfo[1].Pos);
//	MapchipNumberSpecify(&vertexInfo[2].mapNum, &vertexInfo[2].Pos);
//	MapchipNumberSpecify(&vertexInfo[3].mapNum, &vertexInfo[3].Pos);
//}