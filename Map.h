#pragma once

//ヘッダ読み込み
#include "game.h"

//エラーメッセージ
#define CSV_LOAD_ERR_TITLE		TEXT("CSV読み込みエラー")
#define CSV_DATANUM_ERR_TITLE	TEXT("CSV数エラー")

//マップチップの画像ファイル
#define IMG_PATH_MAP1	TEXT(".\\Map\\mapchip.png")

//マップデータのファイルパス
#define CSV_PATH_MAP1_SHITA			TEXT(".\\Map\\クモの守人　少女の自宅._shita.csv")
#define CSV_PATH_MAP1_NAKA			TEXT(".\\Map\\クモの守人　少女の自宅._naka.csv")
#define CSV_PATH_MAP1_NAKA_ATARI	TEXT(".\\Map\\クモの守人　少女の自宅._naka_atari.csv")
#define CSV_PATH_MAP1_UE			TEXT(".\\Map\\クモの守人　少女の自宅._ue.csv")

//マップの書式指定子
#define  CSV_MAP_FORMAT			TEXT("%d,")

#define MAP1_YOKO_MAX	15	//マップ横サイズ
#define MAP1_TATE_MAX	15	//マップ縦サイズ

#define MAP1_YOKO_DIV	60	//マップ横分割
#define MAP1_TATE_DIV	60	//マップ縦分割

#define MAP_MOVE_ID		56	//通れるマップのID　　〇
#define MAP_STOP_ID		57	//通れないマップのID  ×
#define MAP_NONE_ID		58	//何もないマップのID　透明


//マップデータ構造体(サンプル)
struct MAP_DATA
{
	//CSVデータ（分割画像のIDが入る）
	int CSV_shita[MAP1_TATE_MAX][MAP1_YOKO_MAX];
	int CSV_naka[MAP1_TATE_MAX][MAP1_YOKO_MAX];
	int CSV_naka_atari[MAP1_TATE_MAX][MAP1_YOKO_MAX];
	int CSV_ue[MAP1_TATE_MAX][MAP1_YOKO_MAX];

	//マップハンドル
	int handle[MAP1_TATE_DIV * MAP1_YOKO_DIV];

	//マップの場所
	int x[MAP1_TATE_MAX][MAP1_YOKO_MAX];
	int y[MAP1_TATE_MAX][MAP1_YOKO_MAX];

	int width;
	int height;

	int DivMax;	//分割数　forで回す

	BOOL IsDraw = FALSE;	//描画するか

	//当たり判定
	RECT coll[MAP1_TATE_MAX][MAP1_YOKO_MAX];
};

extern BOOL LoadCSVMap(
	const char* ImgPath,
	const char* ShitaPath,
	const char* NakaPath,
	const char* NakaAtariPath,
	const char* UePath,
	MAP_DATA* map,
	int divYoko,
	int divTate
);

extern BOOL LoadCSVMap(const char* ImgPath, const char* ShitaPath, const char* NakaPath, const char* NakaAtariPath,
	const char* UePath, MAP_DATA* map, int divYoko, int divTate);


extern VOID DrawMap(MAP_DATA map);
extern BOOL CollMap(RECT rect, MAP_DATA map);

extern VOID DrawRect(RECT r, unsigned int color, bool b);
extern BOOL CheckCollRectToRect(RECT a, RECT b);