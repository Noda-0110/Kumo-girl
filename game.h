#pragma once

//ゲーム全体のヘッダファイル
// 
//ヘッダーファイルの読み込み
#include "DxLib.h"		//DxLibを使う時に必要


//マクロ定義
#define GAME_TITLE "ゲームタイトル"	//ゲームタイトル
#define GAME_WIDTH  720		//ゲーム画面の幅
#define GAME_HEIGHT 720			//ゲーム画面の高さ
#define GAME_COLOR  32			//ゲームの色域

#define GAME_ICON_ID 333			//ゲームのICONのID


#define GAME_WINDOW_BAR 0	//ウインドウバーの種類

#define GAME_DEBUG TRUE		//デバックモード


//列挙型
enum GAME_SCENE {
	GAME_SCENE_TITLE,
	GAME_SCENE_PLAY,
	GAME_SCENE_END,
	GAME_SCENE_CHANGE //エフェクトを出すシーン
};//ゲームのシーン

//構造体の定義

//画像の構造体
struct IMAGE
{
	int handle = -1;	//画像のハンドル（管理番号）
	char path[255];		//画像の場所（パス）

	int x;		//X位置
	int y;		//Y位置
	int width;	//幅
	int height;	//高さ

	BOOL IsDraw = FALSE;//画像が描画できるか
};

//キャラクタの構造体
struct CHARACTOR
{
	IMAGE img;	//画像構造体
	int speed = 1;

	RECT coll;	//上下左右当たり判定の領域 RECTは四角形の位置を扱える
};

//動画の構造体
struct MOVIE
{
	int handle = -1;	//動画のハンドル
	char path[255];		//動画のパス

	int x;		//ｘ位置
	int y;		//ｙ位置
	int width;	//幅
	int height;	//高さ
	int Volume = 255;	//ボリューム（最小）0～255（最大）
};

//音楽の構造体
struct AUDIO
{
	int handle = -1;	//音楽のハンドル
	char path[255];		//音楽のパス

	int volume = -1;	//ボリューム（MIN 0　～　２５５　MAX）
	int playType = -1;
};