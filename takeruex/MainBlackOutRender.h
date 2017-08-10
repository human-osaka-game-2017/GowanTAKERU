#ifndef MAINBLACKOUTRENDER_H
#define MAINBLACKOUTRENDER_H

//60フレームで完了するので30フレームで真っ暗になる
#define FINISHFRM 60

//チェックポイントに戻るときに使います
bool* GetMainBlackOutflg();


void MainBlackOutRender();

#endif
