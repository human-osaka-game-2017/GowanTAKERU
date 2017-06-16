#include"MainHitManagement.h"
#include<d3dx9.h>


void HitManage() {

}

bool Circle_Hit(float cx1, float cy1, float r1, float cx2, float cy2, float r2) {

	float sumR = r1 + r2;//2つの円の半径の合計求める
						 // 三平方の定理で中心点の長さを求めている
	float L = pow(cx1 - cx2, 2) + pow(cy1 - cy2, 2);
	L = sqrt(L);

	if (sumR >= L) {//中心点の長さが半径の合計より短かったら真
		return TRUE;
	}
	else if (sumR < L) {//中心点の長さが半径の合計より長かったら偽
		return FALSE;
	}
}