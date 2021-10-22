//#CRT_SECURE_NO_WARNINGS
//ヘッダファイル読み込み
#include "game.h"		//ゲームを管理する為のヘッダファイル　あくまでもゲームの管理
#include "keyboard.h"	//キーボードの処理
#include "FPS.h"		//FPSの処理



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

//背景の構造体
IMAGE TitlePic;
IMAGE PlayPic;
IMAGE EdPic;

//プロトタイプ宣言
VOID Title(VOID);		//タイトル画面
VOID TitleProc(VOID);	//タイトル画面（処理）
VOID TitleDraw(VOID);	//タイトル画面（描画）

VOID Play(VOID);		//プレイ画面
VOID PlayProc(VOID);	//プレイ画面（処理）
VOID PlayDraw(VOID);	//プレイ画面（描画）

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



VOID ChangeScene(GAME_SCENE scene);	//シーン切り替え

VOID collUpdateplayer(CHARACTOR* chara);	//当たり判定の領域を更新

VOID collUpdateGoal(CHARACTOR* chara);	//当たり判定の領域を更新

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
		FPSDraw();
		//FPS処理を待つ
		FPSWait();

		ScreenFlip();	//ダブルバッファリングした画面を描画
	}

	//- - - - データの開放 - - - -

	//音楽の削除
	DeleteSoundMem(TitleBGM.handle);
	DeleteSoundMem(PlayBGM.handle);
	DeleteSoundMem(EdBGM.handle);
	//背景の開放
	DeleteGraph(TitlePic.handle);
	DeleteGraph(PlayPic.handle);
	DeleteGraph(EdPic.handle);

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

	//背景の諸々の読み込み
	//タイトル画面の背景
	if (LoadImageMem(&TitlePic, ".\\Image\\タイトル画面.png") == FALSE) { return FALSE; }
	TitlePic.x = 0;
	TitlePic.y = 0;
	TitlePic.IsDraw = TRUE;

	if (LoadImageMem(&EdPic, ".\\Image\\エンド画面.png") == FALSE) { return FALSE; }
	EdPic.x = 0;
	EdPic.y = 0;
	EdPic.IsDraw = TRUE;
	return TRUE;

}
// - - - - - データロード - - - - - //

VOID GameInit(VOID)
{


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

	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//BGMの停止
		StopAudio(&TitleBGM);

		//プレイ画面への遷移
		ChangeScene(GAME_SCENE_PLAY);

		return;
	}

	PlayAudio(TitleBGM);

	return;
}
/// <summary>
/// タイトル画面　処理 
/// </summary>
VOID TitleDraw(VOID)
{
	DrawGraph(TitlePic.x, TitlePic.y, TitlePic.handle, TRUE);

	DrawString(0, 0, "タイトル画面", GetColor(0, 0, 0));
	return;
}


/// <summary>
/// プレイ画面
/// </summary>
VOID Play(VOID)
{
	PlayProc();
	PlayDraw();
	return;
}
/// <summary>
/// プレイ画面　処理
/// </summary>
VOID PlayProc(VOID)
{

	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//BGMの停止
		StopAudio(&PlayBGM);

		//エンド画面への遷移
		ChangeScene(GAME_SCENE_END);

		return;
	}
	PlayAudio(PlayBGM);

	return;
}
/// <summary>
/// プレイ画面　描画
/// </summary>
VOID PlayDraw(VOID)

{
	DrawString(0, 0, "プレイ画面", GetColor(0, 0, 0));
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
	DrawGraph(EdPic.x, EdPic.y, EdPic.handle, TRUE);

	DrawString(0, 0, "エンド画面", GetColor(0, 0, 0));
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

	DrawString(0, 0, "切り替え画面", GetColor(0, 0, 0));
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
VOID collUpdateGoal(CHARACTOR* chara)
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