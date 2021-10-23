//ヘッダファイル読み込み
#include "map.h"
#include "game.h"

/// <summary>
/// CSVファイルを読み込む
/// </summary>
/// <param name="ImgPath">マップチップの画像ファイルのパス</param>
/// <param name="ShitaPath"></param>
/// <param name="NakaAtariPath"></param>
/// <param name="UePath"></param>
/// <param name="map"></param>
/// <param name="divYoko"></param>
/// <param name="divTate"></param>
/// <returns></returns>
BOOL LoadCSVMap(const char* ImgPath, const char* ShitaPath, const char* NakaPath, const char* NakaAtariPath,
	const char* UePath, MAP_DATA* map, int divYoko, int divTate) {
	//マップチップ読み込み
		{
			int IsLoad = -1;	//画像を読み込めたか

			//一時的に画像のハンドルを用意する
			int TempHandle = LoadGraph(ImgPath);


			//読み込みエラー
			if (TempHandle == -1)
			{
				MessageBox(
					GetMainWindowHandle(),
					ImgPath,
					"画像読み込みエラー",
					MB_OK
				);
				return FALSE;
			}

			//画像の幅と高さ
			int width = -1;
			int height = -1;
			GetGraphSize(TempHandle, &width, &height);

			//分割して読みこみ
			IsLoad = LoadDivGraph(
				ImgPath,					//画像のパス
				divYoko * divTate,			//分割総数
				divYoko, divTate,			//横の分割,縦の分割
				width / divYoko, height / divTate,	//画像一つ分の幅、高さ
				map->handle					//連続で管理する配列の先頭アドレス
			);

			//分割エラー
			if (IsLoad == -1)
			{
				MessageBox(
					GetMainWindowHandle(),
					ImgPath,
					"画像分割エラー",
					MB_OK
				);

				return FALSE;	//読み込み失敗
			}

			//情報を設定
			map->DivMax = divYoko * divTate;
			GetGraphSize(map->handle[0], &map->width, &map->height);

			//画像を描画
			map->IsDraw = TRUE;

			//一時的に読みこんだハンドルを開放
			DeleteGraph(TempHandle);
		}

		FILE* fp;
		char GetChar = '\0';
		int LoopCnt = 0;
		errno_t FileErr = NULL;
		int IsEOF = NULL;

		//下CSVを読み取り
		{
			FileErr = fopen_s(&fp, ShitaPath, "r");
			if (FileErr != NULL)
			{
				MessageBox(GetMainWindowHandle(), ShitaPath, CSV_LOAD_ERR_TITLE, MB_OK);
				return FALSE;
			}

			//以下正常に読み込んだ場合の処理

			//実際にデータを格納
			IsEOF = NULL; LoopCnt = 0;
			while (IsEOF != EOF)	//ファイルの最後まで繰り返す
			{
				//ファイルから数値を一つ読み込み、配列に格納
				//[LoopCnt / MAP1_YOKO_MAX][LoopCnt % MAP1_YOKO_MAX]は
				//例えば[3][3]の配列の時、LoopCntが５であれば
				//[5 / 3 = 1][5 % 3 = 2]となり、一次元配列の位置を二次元配列として変換できる
				IsEOF = fscanf_s(
					fp,
					CSV_MAP_FORMAT,	//データが[XX,]で書かれていることがポイント
					&map->CSV_shita[LoopCnt / MAP1_YOKO_MAX][LoopCnt % MAP1_YOKO_MAX]
				);
				//次のデータへ
				LoopCnt++;
			}
			fclose(fp);
		}

		//中CSVを読み取り
		{
			FileErr = fopen_s(&fp, NakaPath, "r");
			if (FileErr != NULL)
			{
				MessageBox(GetMainWindowHandle(), NakaPath, CSV_LOAD_ERR_TITLE, MB_OK);
				return FALSE;
			}

			//以下正常に読み込んだ場合の処理

			//実際にデータを格納
			IsEOF = NULL; LoopCnt = 0;
			while (IsEOF != EOF)	//ファイルの最後まで繰り返す
			{
				//ファイルから数値を一つ読み込み、配列に格納
				//[LoopCnt / MAP1_YOKO_MAX][LoopCnt % MAP1_YOKO_MAX]は
				//例えば[3][3]の配列の時、LoopCntが５であれば
				//[5 / 3 = 1][5 % 3 = 2]となり、一次元配列の位置を二次元配列として変換できる
				IsEOF = fscanf_s(
					fp,
					CSV_MAP_FORMAT,	//データが[XX,]で書かれていることがポイント
					&map->CSV_naka[LoopCnt / MAP1_YOKO_MAX][LoopCnt % MAP1_YOKO_MAX]
				);
				//次のデータへ
				LoopCnt++;
			}
			fclose(fp);
		}

		//中あたりCSVを読み取り
		{
			FileErr = fopen_s(&fp, NakaAtariPath, "r");
			if (FileErr != NULL)
			{
				MessageBox(GetMainWindowHandle(), NakaAtariPath, CSV_LOAD_ERR_TITLE, MB_OK);
				return FALSE;
			}

			//以下正常に読み込んだ場合の処理

			//実際にデータを格納
			IsEOF = NULL; LoopCnt = 0;
			while (IsEOF != EOF)	//ファイルの最後まで繰り返す
			{
				//ファイルから数値を一つ読み込み、配列に格納
				//[LoopCnt / MAP1_YOKO_MAX][LoopCnt % MAP1_YOKO_MAX]は
				//例えば[3][3]の配列の時、LoopCntが５であれば
				//[5 / 3 = 1][5 % 3 = 2]となり、一次元配列の位置を二次元配列として変換できる
				IsEOF = fscanf_s(
					fp,
					CSV_MAP_FORMAT,	//データが[XX,]で書かれていることがポイント
					&map->CSV_naka_atari[LoopCnt / MAP1_YOKO_MAX][LoopCnt % MAP1_YOKO_MAX]
				);
				//次のデータへ
				LoopCnt++;
			}
			fclose(fp);
		}

		//上CSVを読み取り
		{
			FileErr = fopen_s(&fp, UePath, "r");
			if (FileErr != NULL)
			{
				MessageBox(GetMainWindowHandle(), UePath, CSV_LOAD_ERR_TITLE, MB_OK);
				return FALSE;
			}

			//以下正常に読み込んだ場合の処理

			//実際にデータを格納
			IsEOF = NULL; LoopCnt = 0;
			while (IsEOF != EOF)	//ファイルの最後まで繰り返す
			{
				//ファイルから数値を一つ読み込み、配列に格納
				//[LoopCnt / MAP1_YOKO_MAX][LoopCnt % MAP1_YOKO_MAX]は
				//例えば[3][3]の配列の時、LoopCntが５であれば
				//[5 / 3 = 1][5 % 3 = 2]となり、一次元配列の位置を二次元配列として変換できる
				IsEOF = fscanf_s(
					fp,
					CSV_MAP_FORMAT,	//データが[XX,]で書かれていることがポイント
					&map->CSV_ue[LoopCnt / MAP1_YOKO_MAX][LoopCnt % MAP1_YOKO_MAX]
				);
				//次のデータへ
				LoopCnt++;
			}
			fclose(fp);
		}

		//マップの当たり判定を作成
		{
			for (int tate = 0; tate < MAP1_TATE_MAX; tate++)
			{
				for (int yoko = 0; yoko < MAP1_YOKO_MAX; yoko++)
				{
					//とおれないIDの場合
					if (map->CSV_naka_atari[tate][yoko] == MAP_STOP_ID)
					{
						//当たり判定を作成
						map->coll[tate][yoko].left = (yoko + 0) * map->width + 1;
						map->coll[tate][yoko].right = (yoko + 1) * map->width - 1;
						map->coll[tate][yoko].top = (tate + 0) * map->height + 1;
						map->coll[tate][yoko].bottom = (tate + 1) * map->height - 1;
					}
					else {
						map->coll[tate][yoko].left = 0;
						map->coll[tate][yoko].right = 0;
						map->coll[tate][yoko].top = 0;
						map->coll[tate][yoko].bottom = 0;
					}

					//マップの場所を指定
					map->x[tate][yoko] = (yoko + 0) * map->width;
					map->y[tate][yoko] = (tate + 0) * map->height;

				}
			}
		}

		return TRUE;
}


/// <summary>
/// マップ描画
/// </summary>
/// <param name="map">マップ構造体</param>
/// <returns></returns>
/// <returns></returns>
VOID DrawMap(MAP_DATA map)
{
	//画像を描画できるとき
	if (map.IsDraw == TRUE)
	{
		//下レイヤ
		for (int tate = 0; tate < MAP1_TATE_MAX; tate++)
		{
			for (int yoko = 0; yoko < MAP1_YOKO_MAX; yoko++)
			{
				DrawGraph(map.x[tate][yoko],
					map.y[tate][yoko],
					map.handle[map.CSV_shita[tate][yoko]], TRUE);
			}
		}

		//中レイヤ
		for (int tate = 0; tate < MAP1_TATE_MAX; tate++)
		{
			for (int yoko = 0; yoko < MAP1_YOKO_MAX; yoko++)
			{
				DrawGraph(map.x[tate][yoko],
					map.y[tate][yoko],
					map.handle[map.CSV_naka[tate][yoko]], TRUE);
			}
		}

		//ここにプレイヤーの画像を描画
		DrawDivImage(&PlayerDIVIMG);

		//上レイヤ
		for (int tate = 0; tate < MAP1_TATE_MAX; tate++)
		{
			for (int yoko = 0; yoko < MAP1_YOKO_MAX; yoko++)
			{
				DrawGraph(map.x[tate][yoko],
					map.y[tate][yoko],
					map.handle[map.CSV_ue[tate][yoko]], TRUE);
			}
		}
	}

	//デバックモード時は
	if (GAME_DEBUG == TRUE)
	{
		for (int tate = 0; tate < MAP1_TATE_MAX; tate++)
		{
			for (int yoko = 0; yoko < MAP1_YOKO_MAX; yoko++)
			{
				DrawRect(map.coll[tate][yoko], GetColor(255, 255, 255), FALSE);
			}
		}
	}
	return;
}

BOOL CollMap(RECT rect, MAP_DATA map)
{
	//当たり判定チェック
	for (int tate = 0; tate < MAP1_TATE_MAX; tate++)
	{
		for (int yoko = 0; yoko < MAP1_YOKO_MAX; yoko++)
		{
			if (CheckCollRectToRect(rect, map.coll[tate][yoko]) == TRUE)
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}


/// <summary>
/// 矩形領域同士の当たり判定をする関数
/// </summary>
/// <param name="a">領域A</param>
/// <param name="b">領域B</param>
/// <returns></returns>
BOOL CheckCollRectToRect(RECT a, RECT b)
{
	if (a.left < b.right &&
		a.top < b.bottom &&
		a.right > b.left &&
		a.bottom > b.top
		)
	{
		return TRUE;	//当たっている
	}

	return FALSE;		//当たっていない
}

/// <summary>
/// RECTを利用して四角を描画
/// </summary>
/// <param name="r">RECT構造体</param>
/// <param name="color">描画する色</param>
/// <param name="b">中を塗りつぶさないならFALSE/塗りつぶすならTRUE</param>
/// <returns></returns>
VOID DrawRect(RECT r, unsigned int color, bool b)
{
	//引数を基に描画
	DrawBox(r.left, r.top, r.right, r.bottom, color, b);
	return;
}