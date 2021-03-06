#pragma once

//ゲーム全体のヘッダファイル
// 
//ヘッダーファイルの読み込み
#include "DxLib.h"		//DxLibを使う時に必要


//マクロ定義
#define GAME_TITLE "クモと少女"	//ゲームタイトル

//ゲーム画面の幅　高さ
#define GAME_WIDTH  720
#define GAME_HEIGHT 720

//ゲームの色域
#define GAME_COLOR  32

//ゲームのICONのID
#define GAME_ICON_ID 333

//ウインドウバーの種類
#define GAME_WINDOW_BAR 0

//デバックモード TRUE/描画　FALSE/描画しない
#define GAME_DEBUG FALSE

//パスの長さ
#define PATH_MAX	255
//ハンドルの最大数
#define IMGDIV_MAX	128

//画面外の文字の位置
#define TEXT_X 540
#define TEXT_Y 160

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

//音楽の構造体
struct AUDIO
{
	int handle = -1;	//音楽のハンドル
	char path[255];		//音楽のパス

	int volume = -1;	//ボリューム（MIN 0　〜　２５５　MAX）
	int playType = -1;
};

struct DIVIMAGE
{
	int DivMax;						//分割数
	int handle[IMGDIV_MAX];			//画像ハンドル
	int nowIndex = 0;				//現在のハンドル配列のインデックス
	char path[PATH_MAX];			//パス

	int x = -1;						//X位置
	int y = -1;						//Y位置
	int width = -1;					//幅
	int height = -1;				//高さ

	int DivYoko = -1;				//横の分割数
	int DivTate = -1;				//縦の分割数

	RECT coll;						//当たり判定の領域(四角)

	BOOL IsDraw = FALSE;			//描画できるか？

	int AnimCnt = 0;				//アニメーションカウンタ
	int AnimCntMAX = 2;				//アニメーションカウンタMAX
	BOOL IsAnimLoop = FALSE;		//アニメーションをループさせるか？
};

//ゴールの構造体
struct Goal
{
	IMAGE img;
	int speed = -1;

	RECT coll;
};


extern VOID ChangeScene(GAME_SCENE scene);	//シーン切り替え

extern VOID collUpdateplayer(CHARACTOR* chara);	//当たり判定の領域を更新

extern VOID collUpdateGoal(Goal* chara);	//当たり判定の領域を更新

extern VOID collUpdateenemy(CHARACTOR* chara);	//当たり判定の領域を更新

extern BOOL colltouch(RECT a, RECT b);//当たり判定の触れているか触れていないかの判定

extern BOOL IventProc(RECT a, RECT b);

//ゲームデータの読み込み
extern BOOL GameLoad(VOID);
//音楽の情報
extern BOOL LoadAudio(AUDIO* audio, const char* path, int volume, int playType);
//音楽の読み込み
extern BOOL LoadImageMem(IMAGE* image, const char* path);
//音楽の再生
extern VOID PlayAudio(AUDIO audio);
//音楽の停止
extern VOID StopAudio(AUDIO* audio);

extern DIVIMAGE PlayerDIVIMG;
//分割画像の描画
extern VOID DrawDivImage(DIVIMAGE* image);
//分割して画像を読み込み
extern BOOL LoadImageDivMem(DIVIMAGE* div, const char* path, int bunkatuYoko, int bunkatuTate);
//分割画像の当たり判定更新
extern VOID CollUpdateDivImage(DIVIMAGE* div);