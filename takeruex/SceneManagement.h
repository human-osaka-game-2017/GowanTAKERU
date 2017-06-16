#ifndef SCENEMANAGEMENT_H
#define SCENEMANAGEMENT_H

#include<d3dx9.h>

struct SceneSelect {
	bool MainScene;
};

//シーンを分けてくれる。描画コントロールも呼んでます。
//初期化ー＞処理ー＞リリースをやってます
void SceneManage();

#endif