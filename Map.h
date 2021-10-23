#pragma once

//�w�b�_�ǂݍ���
#include "game.h"

//�G���[���b�Z�[�W
#define CSV_LOAD_ERR_TITLE		TEXT("CSV�ǂݍ��݃G���[")
#define CSV_DATANUM_ERR_TITLE	TEXT("CSV���G���[")

//�}�b�v�`�b�v�̉摜�t�@�C��
#define IMG_PATH_MAP1	TEXT(".\\Map\\mapchip.png")

//�}�b�v�f�[�^�̃t�@�C���p�X
#define CSV_PATH_MAP1_SHITA			TEXT(".\\Map\\�N���̎�l�@�����̎���._shita.csv")
#define CSV_PATH_MAP1_NAKA			TEXT(".\\Map\\�N���̎�l�@�����̎���._naka.csv")
#define CSV_PATH_MAP1_NAKA_ATARI	TEXT(".\\Map\\�N���̎�l�@�����̎���._naka_atari.csv")
#define CSV_PATH_MAP1_UE			TEXT(".\\Map\\�N���̎�l�@�����̎���._ue.csv")

//�}�b�v�̏����w��q
#define  CSV_MAP_FORMAT			TEXT("%d,")

#define MAP1_YOKO_MAX	15	//�}�b�v���T�C�Y
#define MAP1_TATE_MAX	15	//�}�b�v�c�T�C�Y

#define MAP1_YOKO_DIV	60	//�}�b�v������
#define MAP1_TATE_DIV	60	//�}�b�v�c����

#define MAP_MOVE_ID		56	//�ʂ��}�b�v��ID�@�@�Z
#define MAP_STOP_ID		57	//�ʂ�Ȃ��}�b�v��ID  �~
#define MAP_NONE_ID		58	//�����Ȃ��}�b�v��ID�@����


//�}�b�v�f�[�^�\����(�T���v��)
struct MAP_DATA
{
	//CSV�f�[�^�i�����摜��ID������j
	int CSV_shita[MAP1_TATE_MAX][MAP1_YOKO_MAX];
	int CSV_naka[MAP1_TATE_MAX][MAP1_YOKO_MAX];
	int CSV_naka_atari[MAP1_TATE_MAX][MAP1_YOKO_MAX];
	int CSV_ue[MAP1_TATE_MAX][MAP1_YOKO_MAX];

	//�}�b�v�n���h��
	int handle[MAP1_TATE_DIV * MAP1_YOKO_DIV];

	//�}�b�v�̏ꏊ
	int x[MAP1_TATE_MAX][MAP1_YOKO_MAX];
	int y[MAP1_TATE_MAX][MAP1_YOKO_MAX];

	int width;
	int height;

	int DivMax;	//�������@for�ŉ�

	BOOL IsDraw = FALSE;	//�`�悷�邩

	//�����蔻��
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