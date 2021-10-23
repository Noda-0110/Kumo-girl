//�w�b�_�t�@�C���ǂݍ���
#include "map.h"
#include "game.h"

/// <summary>
/// CSV�t�@�C����ǂݍ���
/// </summary>
/// <param name="ImgPath">�}�b�v�`�b�v�̉摜�t�@�C���̃p�X</param>
/// <param name="ShitaPath"></param>
/// <param name="NakaAtariPath"></param>
/// <param name="UePath"></param>
/// <param name="map"></param>
/// <param name="divYoko"></param>
/// <param name="divTate"></param>
/// <returns></returns>
BOOL LoadCSVMap(const char* ImgPath, const char* ShitaPath, const char* NakaPath, const char* NakaAtariPath,
	const char* UePath, MAP_DATA* map, int divYoko, int divTate) {
	//�}�b�v�`�b�v�ǂݍ���
		{
			int IsLoad = -1;	//�摜��ǂݍ��߂���

			//�ꎞ�I�ɉ摜�̃n���h����p�ӂ���
			int TempHandle = LoadGraph(ImgPath);


			//�ǂݍ��݃G���[
			if (TempHandle == -1)
			{
				MessageBox(
					GetMainWindowHandle(),
					ImgPath,
					"�摜�ǂݍ��݃G���[",
					MB_OK
				);
				return FALSE;
			}

			//�摜�̕��ƍ���
			int width = -1;
			int height = -1;
			GetGraphSize(TempHandle, &width, &height);

			//�������ēǂ݂���
			IsLoad = LoadDivGraph(
				ImgPath,					//�摜�̃p�X
				divYoko * divTate,			//��������
				divYoko, divTate,			//���̕���,�c�̕���
				width / divYoko, height / divTate,	//�摜����̕��A����
				map->handle					//�A���ŊǗ�����z��̐擪�A�h���X
			);

			//�����G���[
			if (IsLoad == -1)
			{
				MessageBox(
					GetMainWindowHandle(),
					ImgPath,
					"�摜�����G���[",
					MB_OK
				);

				return FALSE;	//�ǂݍ��ݎ��s
			}

			//����ݒ�
			map->DivMax = divYoko * divTate;
			GetGraphSize(map->handle[0], &map->width, &map->height);

			//�摜��`��
			map->IsDraw = TRUE;

			//�ꎞ�I�ɓǂ݂��񂾃n���h�����J��
			DeleteGraph(TempHandle);
		}

		FILE* fp;
		char GetChar = '\0';
		int LoopCnt = 0;
		errno_t FileErr = NULL;
		int IsEOF = NULL;

		//��CSV��ǂݎ��
		{
			FileErr = fopen_s(&fp, ShitaPath, "r");
			if (FileErr != NULL)
			{
				MessageBox(GetMainWindowHandle(), ShitaPath, CSV_LOAD_ERR_TITLE, MB_OK);
				return FALSE;
			}

			//�ȉ�����ɓǂݍ��񂾏ꍇ�̏���

			//���ۂɃf�[�^���i�[
			IsEOF = NULL; LoopCnt = 0;
			while (IsEOF != EOF)	//�t�@�C���̍Ō�܂ŌJ��Ԃ�
			{
				//�t�@�C�����琔�l����ǂݍ��݁A�z��Ɋi�[
				//[LoopCnt / MAP1_YOKO_MAX][LoopCnt % MAP1_YOKO_MAX]��
				//�Ⴆ��[3][3]�̔z��̎��ALoopCnt���T�ł����
				//[5 / 3 = 1][5 % 3 = 2]�ƂȂ�A�ꎟ���z��̈ʒu��񎟌��z��Ƃ��ĕϊ��ł���
				IsEOF = fscanf_s(
					fp,
					CSV_MAP_FORMAT,	//�f�[�^��[XX,]�ŏ�����Ă��邱�Ƃ��|�C���g
					&map->CSV_shita[LoopCnt / MAP1_YOKO_MAX][LoopCnt % MAP1_YOKO_MAX]
				);
				//���̃f�[�^��
				LoopCnt++;
			}
			fclose(fp);
		}

		//��CSV��ǂݎ��
		{
			FileErr = fopen_s(&fp, NakaPath, "r");
			if (FileErr != NULL)
			{
				MessageBox(GetMainWindowHandle(), NakaPath, CSV_LOAD_ERR_TITLE, MB_OK);
				return FALSE;
			}

			//�ȉ�����ɓǂݍ��񂾏ꍇ�̏���

			//���ۂɃf�[�^���i�[
			IsEOF = NULL; LoopCnt = 0;
			while (IsEOF != EOF)	//�t�@�C���̍Ō�܂ŌJ��Ԃ�
			{
				//�t�@�C�����琔�l����ǂݍ��݁A�z��Ɋi�[
				//[LoopCnt / MAP1_YOKO_MAX][LoopCnt % MAP1_YOKO_MAX]��
				//�Ⴆ��[3][3]�̔z��̎��ALoopCnt���T�ł����
				//[5 / 3 = 1][5 % 3 = 2]�ƂȂ�A�ꎟ���z��̈ʒu��񎟌��z��Ƃ��ĕϊ��ł���
				IsEOF = fscanf_s(
					fp,
					CSV_MAP_FORMAT,	//�f�[�^��[XX,]�ŏ�����Ă��邱�Ƃ��|�C���g
					&map->CSV_naka[LoopCnt / MAP1_YOKO_MAX][LoopCnt % MAP1_YOKO_MAX]
				);
				//���̃f�[�^��
				LoopCnt++;
			}
			fclose(fp);
		}

		//��������CSV��ǂݎ��
		{
			FileErr = fopen_s(&fp, NakaAtariPath, "r");
			if (FileErr != NULL)
			{
				MessageBox(GetMainWindowHandle(), NakaAtariPath, CSV_LOAD_ERR_TITLE, MB_OK);
				return FALSE;
			}

			//�ȉ�����ɓǂݍ��񂾏ꍇ�̏���

			//���ۂɃf�[�^���i�[
			IsEOF = NULL; LoopCnt = 0;
			while (IsEOF != EOF)	//�t�@�C���̍Ō�܂ŌJ��Ԃ�
			{
				//�t�@�C�����琔�l����ǂݍ��݁A�z��Ɋi�[
				//[LoopCnt / MAP1_YOKO_MAX][LoopCnt % MAP1_YOKO_MAX]��
				//�Ⴆ��[3][3]�̔z��̎��ALoopCnt���T�ł����
				//[5 / 3 = 1][5 % 3 = 2]�ƂȂ�A�ꎟ���z��̈ʒu��񎟌��z��Ƃ��ĕϊ��ł���
				IsEOF = fscanf_s(
					fp,
					CSV_MAP_FORMAT,	//�f�[�^��[XX,]�ŏ�����Ă��邱�Ƃ��|�C���g
					&map->CSV_naka_atari[LoopCnt / MAP1_YOKO_MAX][LoopCnt % MAP1_YOKO_MAX]
				);
				//���̃f�[�^��
				LoopCnt++;
			}
			fclose(fp);
		}

		//��CSV��ǂݎ��
		{
			FileErr = fopen_s(&fp, UePath, "r");
			if (FileErr != NULL)
			{
				MessageBox(GetMainWindowHandle(), UePath, CSV_LOAD_ERR_TITLE, MB_OK);
				return FALSE;
			}

			//�ȉ�����ɓǂݍ��񂾏ꍇ�̏���

			//���ۂɃf�[�^���i�[
			IsEOF = NULL; LoopCnt = 0;
			while (IsEOF != EOF)	//�t�@�C���̍Ō�܂ŌJ��Ԃ�
			{
				//�t�@�C�����琔�l����ǂݍ��݁A�z��Ɋi�[
				//[LoopCnt / MAP1_YOKO_MAX][LoopCnt % MAP1_YOKO_MAX]��
				//�Ⴆ��[3][3]�̔z��̎��ALoopCnt���T�ł����
				//[5 / 3 = 1][5 % 3 = 2]�ƂȂ�A�ꎟ���z��̈ʒu��񎟌��z��Ƃ��ĕϊ��ł���
				IsEOF = fscanf_s(
					fp,
					CSV_MAP_FORMAT,	//�f�[�^��[XX,]�ŏ�����Ă��邱�Ƃ��|�C���g
					&map->CSV_ue[LoopCnt / MAP1_YOKO_MAX][LoopCnt % MAP1_YOKO_MAX]
				);
				//���̃f�[�^��
				LoopCnt++;
			}
			fclose(fp);
		}

		//�}�b�v�̓����蔻����쐬
		{
			for (int tate = 0; tate < MAP1_TATE_MAX; tate++)
			{
				for (int yoko = 0; yoko < MAP1_YOKO_MAX; yoko++)
				{
					//�Ƃ���Ȃ�ID�̏ꍇ
					if (map->CSV_naka_atari[tate][yoko] == MAP_STOP_ID)
					{
						//�����蔻����쐬
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

					//�}�b�v�̏ꏊ���w��
					map->x[tate][yoko] = (yoko + 0) * map->width;
					map->y[tate][yoko] = (tate + 0) * map->height;

				}
			}
		}

		return TRUE;
}


/// <summary>
/// �}�b�v�`��
/// </summary>
/// <param name="map">�}�b�v�\����</param>
/// <returns></returns>
/// <returns></returns>
VOID DrawMap(MAP_DATA map)
{
	//�摜��`��ł���Ƃ�
	if (map.IsDraw == TRUE)
	{
		//�����C��
		for (int tate = 0; tate < MAP1_TATE_MAX; tate++)
		{
			for (int yoko = 0; yoko < MAP1_YOKO_MAX; yoko++)
			{
				DrawGraph(map.x[tate][yoko],
					map.y[tate][yoko],
					map.handle[map.CSV_shita[tate][yoko]], TRUE);
			}
		}

		//�����C��
		for (int tate = 0; tate < MAP1_TATE_MAX; tate++)
		{
			for (int yoko = 0; yoko < MAP1_YOKO_MAX; yoko++)
			{
				DrawGraph(map.x[tate][yoko],
					map.y[tate][yoko],
					map.handle[map.CSV_naka[tate][yoko]], TRUE);
			}
		}

		//�����Ƀv���C���[�̉摜��`��
		DrawDivImage(&PlayerDIVIMG);

		//�ヌ�C��
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

	//�f�o�b�N���[�h����
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
	//�����蔻��`�F�b�N
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
/// ��`�̈擯�m�̓����蔻�������֐�
/// </summary>
/// <param name="a">�̈�A</param>
/// <param name="b">�̈�B</param>
/// <returns></returns>
BOOL CheckCollRectToRect(RECT a, RECT b)
{
	if (a.left < b.right &&
		a.top < b.bottom &&
		a.right > b.left &&
		a.bottom > b.top
		)
	{
		return TRUE;	//�������Ă���
	}

	return FALSE;		//�������Ă��Ȃ�
}

/// <summary>
/// RECT�𗘗p���Ďl�p��`��
/// </summary>
/// <param name="r">RECT�\����</param>
/// <param name="color">�`�悷��F</param>
/// <param name="b">����h��Ԃ��Ȃ��Ȃ�FALSE/�h��Ԃ��Ȃ�TRUE</param>
/// <returns></returns>
VOID DrawRect(RECT r, unsigned int color, bool b)
{
	//��������ɕ`��
	DrawBox(r.left, r.top, r.right, r.bottom, color, b);
	return;
}