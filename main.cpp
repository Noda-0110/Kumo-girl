//#CRT_SECURE_NO_WARNINGS
//ヘッダファイル読み込み
#include "game.h"		//ゲームを管理する為のヘッダファイル　あくまでもゲームの管理
#include "keyboard.h"	//キーボードの処理
#include "FPS.h"		//FPSの処理
#include "Map.h"		//FPSの処理

enum MUKI
{
	muki_none,
	muki_ue,
	muki_hidari,
	muki_shita,
	muki_migi
};

//グローバル変数
//シーンを管理する変数
GAME_SCENE GameScene;		//現在のゲームのシーン
GAME_SCENE OldGameScene;	//前回もゲームのシーン
GAME_SCENE NextGameScene;	//次のゲームのシーン


//画面の切り替え
BOOL IsFadeOut = FALSE;	//フェードアウト
BOOL IsFadeIn = FALSE;	//フェードイン

int FadeTimeMill = 2000;					//切り替えミリ秒
int FadeTimeMax = FadeTimeMill / 1000 * 60;	//ミリ秒をフレーム病に変換

//フェードアウト
int FadeOutCntInit = 0;				//初期値
int FadeOutCnt = FadeOutCntInit;	//フェードアウトのカウンタ
int FadeOutCntMax = FadeTimeMax;	//フェードアウトのカウンタMAX

//フェードイン
int FadeInCntInit = FadeTimeMax;	//初期値
int FadeInCnt = FadeInCntInit;		//フェードインのカウンタ
int FadeInCntMax = FadeTimeMax;				//フェードインのカウンタMAX

//PushEnterの点滅
int PushEnterCnt = 0;
const int PushEnterCntMAX = 60;
BOOL PushEnterBrink = FALSE;

//BGMの構造体
AUDIO TitleBGM;
AUDIO PlayBGM;
AUDIO EdBGM;

AUDIO PianoBGM;

//背景の構造体
IMAGE TitlePic;
IMAGE EdPic;

//ロゴの読み込み
IMAGE Logo;

//PushEnterの構造体
IMAGE EnterPic;
IMAGE EdEnter;

//吹き出しの構造体
IMAGE Hukidasi;

//プレイヤーの構造体 charchipの器
DIVIMAGE PlayerDIVIMG;
MUKI muki = muki_shita;

//ゴールの構造体
Goal Kumo;

//マップ構造体
MAP_DATA map;

//イベントマス
RECT bedevent;
RECT pianoevent;
RECT kurozevent;
RECT bookevent;

//プロトタイプ宣言
VOID Title(VOID);		//タイトル画面
VOID TitleProc(VOID);	//タイトル画面（処理）
VOID TitleDraw(VOID);	//タイトル画面（描画）

VOID Play(VOID);		//プレイ画面
VOID PlayProc(VOID);	//プレイ画面（処理）
VOID PlayDraw(VOID);	//プレイ画面（描画）
VOID EventDraw(VOID);	//イベントマスの描画

VOID End(VOID);			//エンド画面
VOID EndProc(VOID);		//エンド画面（処理）
VOID EndDraw(VOID);		//エンド画面（描画）

VOID Change(VOID);		//切り替え画面
VOID ChangeProc(VOID);	//切り替え画面（処理）
VOID ChangeDraw(VOID);	//切り替え画面（描画）

//音楽の再生
VOID PlayAudio(AUDIO audio);
//音楽の停止
VOID StopAudio(AUDIO* audio);
//分割して画像を読み込み
BOOL LoadImageDivMem(DIVIMAGE* div, const char* path, int bunkatuYoko, int bunkatuTate);
//分割画像の当たり判定更新
VOID CollUpdateDivImage(DIVIMAGE* div);



VOID ChangeScene(GAME_SCENE scene);	//シーン切り替え

VOID collUpdateplayer(CHARACTOR* chara);	//当たり判定の領域を更新

VOID collUpdateGoal(Goal* chara);	//当たり判定の領域を更新

VOID collUpdateenemy(CHARACTOR* chara);	//当たり判定の領域を更新


BOOL colltouch(RECT a, RECT b);//当たり判定の触れているか触れていないかの判定

BOOL GameLoad(VOID);	//ゲームデータの読み込み
BOOL LoadAudio(AUDIO* audio, const char* path, int volume, int playType);	
BOOL LoadImageMem(IMAGE* image, const char* path);		//音楽の読み込み
VOID GameInit(VOID);	//ゲームの初期化

// プログラムは WinMain から始まります
//Windowsのプログラミング法で動いている
//DxLibは、DirectXという、ゲームプログラミングを簡単に扱える仕組み
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	//追加分
	SetOutApplicationLogValidFlag(FALSE);	//Log.txtを出力しない

	ChangeWindowMode(TRUE);					//ウィンドウモードに設定
	SetMainWindowText(GAME_TITLE);			//ウィンドウのタイトル文字
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	//ウィンドウの解像度を設定
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);				//ウィンドウの大きさを設定
	SetBackgroundColor(255, 255, 255);					//デフォルトの背景色
	SetWindowIconID(GAME_ICON_ID);						//アイコンファイルの読み込み

	//追加分
	SetWindowStyleMode(GAME_WINDOW_BAR);	//ウィンドウバーの状態
	SetWaitVSyncFlag(TRUE);					//ディスプレイの垂直同期を有効にする
											//高度な処理や描画を行う時にはFALSE

	SetAlwaysRunFlag(TRUE);					//ウィンドウをずっとアクティブにする


	if (DxLib_Init() == -1)		//ＤＸライブラリ初期化処理　-1が帰ってきたら強制終了
	{
		// エラーが起きたら直ちに終了
		return -1;
	}

	//ダブルバッファリングを有効
	SetDrawScreen(DX_SCREEN_BACK);


	//最初のシーンはタイトル画面
	GameScene == GAME_SCENE_TITLE;

	//ゲーム全体の初期化

	//ゲームの読み込み
	if (!GameLoad())
	{
		//データの読み込み
		DxLib_End();	//DXLib終了
		return -1;		//異常終了
	}

	//ゲームの初期化
	GameInit();

	//無限ループ　受け取り続ける
	while (1)
	{

		if (ProcessMessage() != 0) { break; }	//メッセージを受け取り続ける
		if (ClearDrawScreen() != 0) { break; }	//画面を消去する

		//キーボードの入力更新
		AllKeyUpdate();

		//FPS値の更新
		FPSUpdate();

		//ESCキーで強制終了
		if (KeyClick(KEY_INPUT_ESCAPE) == TRUE) { break; }

		//以前のシーンを所得
		if (GameScene != GAME_SCENE_CHANGE)
		{
			OldGameScene = GameScene;
		}

		//シーンごとに処理を行う
		switch (GameScene)
		{
		case GAME_SCENE_TITLE:
			Title();	//タイトル画面
			break;
		case GAME_SCENE_PLAY:
			Play();		//プレイ画面
			break;
		case GAME_SCENE_END:
			End();		//エンド画面
			break;
		case GAME_SCENE_CHANGE:
			Change();	//切り替え画面
			break;
		default:
			break;
		}

		//シーンを切り替える
		if (OldGameScene != GameScene)
		{
			//現在シーンが切り替え画面（タイトルプレイエンドの画面）でないとき
			if (GameScene != GAME_SCENE_CHANGE)
			{
				NextGameScene = GameScene;		//次のシーンを保存
				GameScene = GAME_SCENE_CHANGE;	//画面切り替えシーンに変える
			}
		}

		//FPS値を描画
		//FPSDraw();
		//FPS処理を待つ
		FPSWait();

		ScreenFlip();	//ダブルバッファリングした画面を描画
	}

	//- - - - データの削除 - - - -

	//音楽の削除
	DeleteSoundMem(TitleBGM.handle);
	DeleteSoundMem(PlayBGM.handle);
	DeleteSoundMem(EdBGM.handle);
	DeleteSoundMem(PianoBGM.handle);
	//背景の削除
	DeleteGraph(TitlePic.handle);

	//PushEnterの削除
	DeleteGraph(EnterPic.handle);
	DeleteGraph(EdEnter.handle);

	//吹き出しの削除
	DeleteGraph(Hukidasi.handle);

	//ゴールの削除
	DeleteGraph(Kumo.img.handle);

	//プレイヤーの分割画像削除
	for (int i = 0; i < PlayerDIVIMG.DivMax; i++) { DeleteGraph(PlayerDIVIMG.handle[i]); }

	// ＤＸライブラリ使用の終了処理（準備）
	DxLib_End();

	return 0;				// ソフトの終了（実行） 
}




// - - - - データロード - - - - //

/// <summary>
/// ゲームのデータを読み込み
/// </summary>
/// <returns>読み込めたらTRUE / 読み込めなかったらFALSE</returns>
BOOL GameLoad()
{

	//音楽の諸々の読み込み
	if (!LoadAudio(&TitleBGM, ".\\Audio\\Title画面.mp3",255,DX_PLAYTYPE_LOOP)) { return FALSE; }
	if (!LoadAudio(&PlayBGM, ".\\Audio\\Play画面.mp3",255,DX_PLAYTYPE_LOOP)) { return FALSE; }
	if (!LoadAudio(&EdBGM, ".\\Audio\\Ed画面.mp3",255,DX_PLAYTYPE_LOOP)) { return FALSE; }
	if (!LoadAudio(&PianoBGM, ".\\Audio\\少女の演奏.mp3",255,DX_PLAYTYPE_LOOP)) { return FALSE; }

	//背景の諸々の読み込み
	//タイトル画面の背景
	if (LoadImageMem(&TitlePic, ".\\Image\\タイトル画面.png") == FALSE) { return FALSE; }
	//if (LoadImageMem(&TitlePic, ".\\Image\\タイトル画面2.png") == FALSE) { return FALSE; }
	
	TitlePic.x = 0;
	TitlePic.y = 0;
	TitlePic.IsDraw = TRUE;

	if (LoadImageMem(&EdPic, ".\\Image\\エンド画面.png") == FALSE) { return FALSE; }
	EdPic.x = 0;
	EdPic.y = 0;
	EdPic.IsDraw = TRUE;

	//エンターの読み込み
	if (!LoadImageMem(&EnterPic, ".\\Image\\Enter.png")) { return FALSE; }
	if (!LoadImageMem(&EdEnter, ".\\Image\\EdEnter.png")) { return FALSE; }

	//吹き出しの読み込み
	if (!LoadImageMem(&Hukidasi, ".\\Image\\吹き出し.png")) { return FALSE; }

	//ロゴの読み込み
	if (!LoadImageMem(&Logo, ".\\Image\\TitleLogo.png")) { return FALSE; }

	//プレイヤーの読み込み
	if (LoadImageDivMem(&PlayerDIVIMG, ".\\Image\\charachip.png", 3, 4) == FALSE) { return FALSE; }

	//マップの読み込み

	//サンプルマップデータを読み込み
	if (LoadCSVMap(
		IMG_PATH_MAP1,
		CSV_PATH_MAP1_SHITA,
		CSV_PATH_MAP1_NAKA,
		CSV_PATH_MAP1_NAKA_ATARI,
		CSV_PATH_MAP1_UE,
		&map,
		MAP1_YOKO_DIV, MAP1_TATE_DIV) == FALSE) {
		return FALSE;
	}

	//ゴールの情報を所得
	GetGraphSize(Kumo.img.handle, &Kumo.img.width, &Kumo.img.height);
	if (!LoadImageMem(&Kumo.img, ".\\Image\\クモ.png")) { return FALSE; }



	return TRUE;
}
// - - - - - データロード - - - - - //

VOID GameInit(VOID)
{
	//ロゴの位置
	Logo.x = 0;
	Logo.y = -60;

	//PushEnterの位置
	EnterPic.x = 385;
	EnterPic.y = -10;

	//エンドエンターの位置
	EdEnter.x = 400;
	EdEnter.y = -15;

	//吹き出しの位置
	Hukidasi.x = 490;
	Hukidasi.y = 100;

	//プレイヤーの位置
	PlayerDIVIMG.x = 288;
	PlayerDIVIMG.y = 370;

	muki = muki_shita;

	//ゴール初期化
	Kumo.img.x = 120;
	Kumo.img.y = 120;
	Kumo.speed = 200;
	Kumo.img.IsDraw = TRUE;

	//当たり判定(ゴール)
	collUpdateGoal(&Kumo);

	//イベントの発生位置
	//ベッドの位置
	bedevent.top = map.height * (8.5) + 1;
	bedevent.left = map.width * (12) + 1;
	bedevent.bottom = map.height * (10 + 1) + 1;
	bedevent.right = map.width * (12 + 1) + 1;
	//ピアノの位置
	pianoevent.top = map.height * (3.5) + 1;
	pianoevent.left = map.width * (11) + 1;
	pianoevent.bottom = map.height * (4.5 + 1) + 1;
	pianoevent.right = map.width * (11 + 1) + 1;
	//クローゼットの位置
	kurozevent.top = map.height * (4) + 1;
	kurozevent.left = map.width * (2) + 1;
	kurozevent.bottom = map.height * (4 + 1) + 1;
	kurozevent.right = map.width * (2 + 1) + 1;
	//本棚の位置
	bookevent.top = map.height * (4) + 1;
	bookevent.left = map.width * (8) + 1;
	bookevent.bottom = map.height * (4 + 1) + 1;
	bookevent.right = map.width * (7.5 + 1) + 1;
}

/// <summary>
/// 音楽をメモリへと読み込む
/// </summary>
/// <param name="audio">Audio構造体のアドレス &～～～</param>
/// <param name="path">Audioのパス　.\\Audio\\～～～</param>
/// <param name="volume">ボリューム</param>
/// <param name="playType">DX_PLAYTYPE_LOOP or DX_PLAYTYPE_BACK</param>
/// <returns></returns>
BOOL LoadAudio(AUDIO* audio, const char* path, int volume, int playType)
{
	strcpyDx(audio->path,path);
	audio->handle = LoadSoundMem(audio->path);

	//音楽を読み込みができなかった
	if (audio->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//メインのウィンドウハンドル
			audio->path,				//メッセージ本文
			"音楽読み込みエラー！",	//メッセージタイトル
			MB_OK					//ボタン
		);
		return FALSE;				//エラー終了
	}

	audio->volume = volume;
	audio->playType = playType;

	return TRUE;
}

// ------------  シーン関数　　------------ //
VOID ChangeScene(GAME_SCENE scene)
{
	GameScene = scene;	//シーンを切り替え
	IsFadeIn = FALSE;	//フェードインしない
	IsFadeOut = TRUE;	//フェードアウトする

	return;
}

/// <summary>
/// タイトル画面
/// </summary>
VOID Title(VOID)
{
	TitleProc();	//処理
	TitleDraw();	//描画

	return;
}
/// <summary>
/// タイトル画面　処理
/// </summary>
VOID TitleProc(VOID)
{
	GameInit();
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//BGMの停止
		StopAudio(&TitleBGM);

		//プレイ画面への遷移
		ChangeScene(GAME_SCENE_PLAY);

		return;
	}

	//音楽を鳴らす
	PlayAudio(TitleBGM);


	return;
}
/// <summary>
/// タイトル画面　処理 
/// </summary>
VOID TitleDraw(VOID)
{
	//背景の描画
	DrawGraph(TitlePic.x, TitlePic.y, TitlePic.handle, TRUE);
	//ロゴの描画
	DrawGraph(Logo.x, Logo.y, Logo.handle, TRUE);
	//PushEnterの描画
	if (PushEnterCnt < PushEnterCntMAX) { PushEnterCnt++; }
	else
	{
		if (PushEnterBrink == TRUE)PushEnterBrink = FALSE;
		else if (PushEnterBrink == FALSE)PushEnterBrink = TRUE;

		PushEnterCnt = 0;	//カウンタを初期化
	}

	if (PushEnterBrink == TRUE)
	{
		//半透明にする
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)PushEnterCnt / PushEnterCntMAX) * 255);

		//PushEnterの描画
		DrawGraph(EnterPic.x, EnterPic.y, EnterPic.handle, TRUE);

		//半透明終了
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	if (PushEnterBrink == FALSE)
	{
		//半透明にする
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)(PushEnterCntMAX - PushEnterCnt) / PushEnterCntMAX) * 255);

		//PushEnterの描画
		DrawGraph(EnterPic.x, EnterPic.y, EnterPic.handle, TRUE);

		//半透明終了
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	//DrawString(0, 0, "タイトル画面", GetColor(0, 0, 0));
	return;
}


/// <summary>
/// プレイ画面
/// </summary>
VOID Play(VOID)
{
	PlayProc();
	EventDraw();
	PlayDraw();
	return;
}
/// <summary>
/// プレイ画面　処理
/// </summary>
VOID PlayProc(VOID)
{
	if(GAME_DEBUG==TRUE)
	{
		if (KeyClick(KEY_INPUT_RETURN) == TRUE)
		{
			//BGMの停止
			StopAudio(&PlayBGM);

			//エンド画面への遷移
			ChangeScene(GAME_SCENE_END);

			return;
		}
	}
	

	PlayAudio(PlayBGM);
	//マップの当たり判定を取得
	{
		muki = muki_none;	//最初は向きを無しにする
		DIVIMAGE dummy = PlayerDIVIMG;	//当たり判定のダミーを作成 
		//ダミーを先行させて当たらなければダミーが本体に代わる
		if (KeyDown(KEY_INPUT_W)) { muki = muki_ue; dummy.y--; }
		else if (KeyDown(KEY_INPUT_S)) { muki = muki_shita; dummy.y++; }
		if (KeyDown(KEY_INPUT_A)) { muki = muki_hidari; dummy.x--; }
		else if (KeyDown(KEY_INPUT_D)) { muki = muki_migi; dummy.x++; }

		CollUpdateDivImage(&dummy);	//当たり判定の更新

		if (CollMap(dummy.coll, map) == FALSE)
		{
			PlayerDIVIMG = dummy;	//ダミー情報を戻す
		}
		

		
		collUpdateGoal(&Kumo);
		//プレイヤーがゴールに当たった時
		if (colltouch(dummy.coll, Kumo.coll) == TRUE)
		{
			//BGMを止める
			StopSoundMem(PlayBGM.handle);

			ChangeScene(GAME_SCENE_END);
			return;
		}
	}

	return;
}

/// <summary>
/// プレイ画面　描画
/// </summary>
VOID PlayDraw(VOID)
{
	{DrawDivImage(&PlayerDIVIMG); }

	DrawMap(map);

	DrawGraph(Kumo.img.x, Kumo.img.y, Kumo.img.handle, TRUE);
	//ロゴの描画
	DrawGraph(430, 530, Logo.handle, TRUE);

	if(GAME_DEBUG==TRUE)
	DrawRect(bookevent, GetColor(255, 0, 0), TRUE);

	DrawString(500, 50, "クモに会いに行こう", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// イベントマスの処理
/// </summary>
VOID EventDraw(VOID)
{
	//吹き出しの描画
	DrawGraph(Hukidasi.x, Hukidasi.y, Hukidasi.handle, TRUE);

	//イベントの処理
	if (CheckCollRectToRect(PlayerDIVIMG.coll, bedevent) == TRUE)
	{
		DrawString(TEXT_X, TEXT_Y, "私のベッドだ\n外で寝るから\nあまり使わない", GetColor(0, 0, 0));

	}
	if (CheckCollRectToRect(PlayerDIVIMG.coll, pianoevent) == TRUE)
	{
		StopSoundMem(PlayBGM.handle);
		PlayAudio(PianoBGM);
		DrawString(TEXT_X, TEXT_Y, "ピアノ\n誕生日に貰った物\n少しだけ引ける", GetColor(0, 0, 0));
	}
	else
	{
		StopSoundMem(PianoBGM.handle);
	}

	if (CheckCollRectToRect(PlayerDIVIMG.coll, kurozevent) == TRUE)
	{
		DrawString(TEXT_X, TEXT_Y, "クローゼット\n今はまだ\n着替える必要はない", GetColor(0, 0, 0));
	}

	if (CheckCollRectToRect(PlayerDIVIMG.coll, bookevent) == TRUE)
	{
		DrawString(TEXT_X, TEXT_Y, "本棚\n洞窟や火山など\n冒険の本がある", GetColor(0, 0, 0));
	}
	return;
}

/// <summary>
/// エンド画面
/// </summary>
VOID End(VOID)
{
	EndProc();
	EndDraw();
	return;
}
/// <summary>
/// エンド画面　処理
/// </summary>
VOID EndProc(VOID)
{

	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//BGMの停止
		StopAudio(&EdBGM);

		ChangeScene(GAME_SCENE_TITLE);

		return;
	}

	PlayAudio(EdBGM);

	return;
}

/// <summary>
/// エンド画面 描画
/// </summary>
VOID EndDraw(VOID)
{
	//背景の描画
	DrawGraph(EdPic.x, EdPic.y, EdPic.handle, TRUE);
	//ロゴの描画
	DrawGraph(Logo.x, Logo.y, Logo.handle, TRUE);
	//PushEnterの描画
	if (PushEnterCnt < PushEnterCntMAX) { PushEnterCnt++; }
	else
	{
		if (PushEnterBrink == TRUE)PushEnterBrink = FALSE;
		else if (PushEnterBrink == FALSE)PushEnterBrink = TRUE;

		PushEnterCnt = 0;	//カウンタを初期化
	}

	if (PushEnterBrink == TRUE)
	{
		//半透明にする
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)PushEnterCnt / PushEnterCntMAX) * 255);

		//PushEnterの描画
		DrawGraph(EdEnter.x, EdEnter.y, EdEnter.handle, TRUE);

		//半透明終了
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	if (PushEnterBrink == FALSE)
	{
		//半透明にする
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)(PushEnterCntMAX - PushEnterCnt) / PushEnterCntMAX) * 255);

		//PushEnterの描画
		DrawGraph(EdEnter.x, EdEnter.y, EdEnter.handle, TRUE);

		//半透明終了
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	//DrawString(0, 0, "エンド画面", GetColor(0, 0, 0));
	return;
}


/// <summary>
/// 切り替え画面
/// </summary>
VOID Change(VOID)
{
	ChangeProc();
	ChangeDraw();
	return;
}
/// <summary>
/// 切り替え画面 処理
/// </summary>
VOID ChangeProc(VOID)
{
	//フェードイン
	if (IsFadeIn == TRUE)
	{
		if (FadeInCnt > FadeInCntMax)
		{
			FadeInCnt--;	//カウンタを減らす、透明にしていく
		}
		else
		{
			//フェードイン処理の終了時 透明カウンタのMAXを超えたとき
			FadeInCnt = FadeInCntInit;	//カウンタ初期化
			IsFadeIn = FALSE;			//フェードイン処理の終了
		}

	}
	//フェードアウト
	if (IsFadeOut == TRUE)
	{
		if (FadeOutCnt < FadeOutCntMax)
		{
			FadeOutCnt++;	//カウンタを増やす、色を濃くにしていく
		}
		else
		{
			//フェードイン処理の終了時 透明カウンタのMAXを超えたとき
			FadeOutCnt = FadeOutCntInit;	//カウンタ初期化
			IsFadeOut = FALSE;			//フェードイン処理の終了
		}

	}

	//切り替え処理終了？
	if (IsFadeIn == FALSE && IsFadeOut == FALSE)
	{
		//フェードアウトもフェードインもしていない
		GameScene = NextGameScene;	//次のシーンに切り替え
		OldGameScene = GameScene;	//以前のシーンに更新
	}
	return;
}

/// <summary>
/// 切り替え画面　描画
/// </summary>
VOID ChangeDraw(VOID)
{
	//以前のシーンを描画
	switch (OldGameScene)
	{
	case GAME_SCENE_TITLE:
		TitleDraw();	//タイトル画面の描画
		break;
	case GAME_SCENE_PLAY:
		PlayDraw();		//プレイ画面の描画
		break;
	case GAME_SCENE_END:
		EndDraw();		//エンド画面の描画
		break;
	default:
		break;
	}

	//フェードイン
	if (IsFadeIn == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)FadeInCnt / FadeInCntMax) * 255);	//パーセンテージ

	}
	//フェードアウト
	if (IsFadeOut == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)FadeOutCnt / FadeOutCntMax) * 255);	//パーセンテージ

	}

	//四角を描画
	DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(0, 0, 0), TRUE);

	//半透明終了
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//DrawString(0, 0, "切り替え画面", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// 当たり判定の更新(プレイヤー専用)
/// </summary>
/// <param name="coll">当たり判定の領域</param>
VOID collUpdateplayer(CHARACTOR* chara)
{
	chara->coll.left = chara->img.x + 30;		//当たり判定を微調整
	chara->coll.top = chara->img.y + 25;		//当たり判定を微調整
	chara->coll.right = chara->img.x + chara->img.width - 15;	//当たり判定を微調整
	chara->coll.bottom = chara->img.y + chara->img.height - 20;	//当たり判定を微調整

	return;
}

/// <summary>
/// 当たり判定の更新(ゴール用)
/// </summary>
/// <param name="coll">当たり判定の領域</param>
VOID collUpdateGoal(Goal* chara)
{
	chara->coll.left = chara->img.x;
	chara->coll.top = chara->img.y;
	chara->coll.right = chara->img.x + chara->img.width;
	chara->coll.bottom = chara->img.y + chara->img.height;

	return;
}

/// <summary>
/// 当たり判定の更新(敵用)
/// </summary>
/// <param name="coll">当たり判定の領域</param>
VOID collUpdateenemy(CHARACTOR* chara)
{
	chara->coll.left = chara->img.x;
	chara->coll.top = chara->img.y;
	chara->coll.right = chara->img.x + chara->img.width;
	chara->coll.bottom = chara->img.y + chara->img.height;

	return;
}


/// <summary>
/// 矩形同士の当たり判定
/// </summary>
/// <param name="a">何らかの矩形"a"</param>
/// <param name="b">何らかの矩形"b"</param>
/// <returns>当たったらTRUE / 当たらなければFALSE</returns>
BOOL colltouch(RECT a, RECT b)
{
	if (
		a.left < b.right &&	//pの左辺x < gの右辺x座標
		a.right > b.left &&	//pの右辺x < gの左辺x座標
		a.bottom > b.top &&	//pの上辺y < gの下辺y座標
		a.top < b.bottom	//pの下辺y < gの上辺y座標
		)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/// <summary>
/// タイトルロゴの読み込み
/// </summary>
/// <param name="image"></param>
/// <param name="path"></param>
/// <returns></returns>
BOOL LoadImageMem(IMAGE* image, const char* path)
{
	//画像を読み込み
	strcpyDx(image->path, path);
	image->handle = LoadGraph(image->path);	//画像を読み込み

	//画像が読み込めなかったとき、エラー（ー１）が入る
	if (image->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//メインのウィンドウハンドル
			image->path,				//メッセージ本文
			"画像読み込みエラー！",	//メッセージタイトル
			MB_OK					//ボタン
		);
		return FALSE;				//エラー終了
	}

	//画像の幅と高さを所得
	GetGraphSize(image->handle, &image->width, &image->height);
	return TRUE;

}


/// <summary>
/// 画像を分割してメモリに読み込み
/// </summary>
/// <param name="handle">ハンドル配列の先頭アドレス</param>
/// <param name="path">画像のパス</param>
/// <param name="bunkatuYoko">分割するときの横の数</param>
/// <param name="bunkatuTate">分割するときの縦の数</param>
/// <returns></returns>
BOOL LoadImageDivMem(DIVIMAGE* div, const char* path, int bunkatuYoko, int bunkatuTate)
{
	int IsLoad = -1;	//画像が読み込めたか？

	//一時的に画像のハンドルを用意する
	int TempHandle = LoadGraph(path);

	//読み込みエラー
	if (TempHandle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//ウィンドウハンドル
			path,					//本文
			"画像読み込みエラー",	//タイトル
			MB_OK					//ボタン
		);

		return FALSE;
	}

	//画像の幅と高さを取得
	int Width = -1;		//幅
	int Height = -1;	//高さ
	GetGraphSize(TempHandle, &Width, &Height);

	//分割読み込み
	IsLoad = LoadDivGraph(
		path,							//画像のパス
		bunkatuYoko * bunkatuTate,		//分割総数
		bunkatuYoko, bunkatuTate,					//横と縦の分割
		Width / bunkatuYoko, Height / bunkatuTate,	//画像1つの幅,高さ
		div->handle									//連続で管理する配列の先頭アドレス
	);

	//分割エラー
	if (IsLoad == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//ウィンドウハンドル
			path,					//本文
			"画像分割エラー",		//タイトル
			MB_OK					//ボタン
		);

		return FALSE;	//読み込み失敗
	}

	//情報を設定
	div->DivYoko = bunkatuYoko;
	div->DivTate = bunkatuTate;
	div->DivMax = div->DivYoko * div->DivTate;
	div->x = 0;
	div->y = 0;
	GetGraphSize(div->handle[0], &div->width, &div->height);
	div->AnimCnt = 0;
	div->IsAnimLoop = FALSE;
	div->nowIndex = 0;

	//当たり判定更新
	CollUpdateDivImage(div);

	//画像描画する
	div->IsDraw = TRUE;

	//一時的に読み込んだハンドルを解放
	DeleteGraph(TempHandle);

	return TRUE;
}


/// <summary>
/// 分割画像の描画
/// </summary>
/// <param name="image">描画する画像構造体</param>
/// <returns></returns>
/// <returns></returns>
VOID DrawDivImage(DIVIMAGE* image)
{
	//画像が描画できるときは
	if (image->IsDraw == TRUE)
	{
		DrawGraph(image->x, image->y, image->handle[image->nowIndex], TRUE);

		//アニメーションのカウントアップ
		if (image->AnimCnt < image->AnimCntMAX) { image->AnimCnt++; }
		else
		{

			//右向きのとき(画像によって数字が違うので、合わせること！)
			if (muki == muki_migi)
			{
				if (image->nowIndex >= 6 && image->nowIndex < 8)
				{
					image->nowIndex++;
				}
				else
				{
					image->nowIndex = 6;
				}
			}

			//左向きのとき(画像によって数字が違うので、合わせること！)
			if (muki == muki_hidari)
			{
				if (image->nowIndex >= 3 && image->nowIndex < 5)
				{
					image->nowIndex++;
				}
				else
				{
					image->nowIndex = 3;
				}
			}

			//上向きのとき(画像によって数字が違うので、合わせること！)
			if (muki == muki_ue)
			{
				if (image->nowIndex >= 9 && image->nowIndex < 11)
				{
					image->nowIndex++;
				}
				else
				{
					image->nowIndex = 9;
				}
			}

			//下向きのとき(画像によって数字が違うので、合わせること！)
			if (muki == muki_shita)
			{
				if (image->nowIndex >= 0 && image->nowIndex < 2)
				{
					image->nowIndex++;
				}
				else
				{
					image->nowIndex = 0;
				}
			}

			//向き無しのときは、直前の向きの真ん中の画像にする(画像に合わせて決めてネ)
			if (muki == muki_none)
			{
				if (image->nowIndex >= 6 && image->nowIndex <= 8) { image->nowIndex = 7; }
				if (image->nowIndex >= 3 && image->nowIndex <= 5) { image->nowIndex = 4; }
				if (image->nowIndex >= 9 && image->nowIndex <= 11) { image->nowIndex = 10; }
				if (image->nowIndex >= 0 && image->nowIndex <= 2) { image->nowIndex = 1; }
			}

			image->AnimCnt = 0;	//カウンタ0クリア
		}
	}

	//デバッグモードのときは
	if (GAME_DEBUG == TRUE)
	{
		//当たり判定を描画
		DrawRect(image->coll, GetColor(255, 255, 255), FALSE);
	}
	return;
}


/// <summary>
/// 当たり判定の領域更新（分割画像）
/// </summary>
/// <param name="img">画像構造体のポインタ</param>
VOID CollUpdateDivImage(DIVIMAGE* div)
{
	div->coll.left = div->x;
	div->coll.top = div->y;

	div->coll.right = div->x + div->width;
	div->coll.bottom = div->y + div->height;

	return;
}

//音楽の再生
VOID PlayAudio(AUDIO audio)
{
	//ループ再生でないなら、そのまま再生
	if (audio.playType == DX_PLAYTYPE_BACK)
	{
		//音楽の再生
		PlaySoundMem(audio.handle, audio.playType);
	}
	//ループ再生のとき、音楽が再生されていないなら、再生
	else if (audio.playType == DX_PLAYTYPE_LOOP)
	{
		if (CheckSoundMem(audio.handle) == 0)
		{
			//音楽の再生
			PlaySoundMem(audio.handle, audio.playType);
		}
	}

	return;
}

VOID StopAudio(AUDIO* audio)
{
	StopSoundMem(audio->handle);

	return;
}