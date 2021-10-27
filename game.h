#pragma once

//�Q�[���S�̂̃w�b�_�t�@�C��
// 
//�w�b�_�[�t�@�C���̓ǂݍ���
#include "DxLib.h"		//DxLib���g�����ɕK�v


//�}�N����`
#define GAME_TITLE "�N���Ə���"	//�Q�[���^�C�g��

//�Q�[����ʂ̕��@����
#define GAME_WIDTH  720
#define GAME_HEIGHT 720

//�Q�[���̐F��
#define GAME_COLOR  32

//�Q�[����ICON��ID
#define GAME_ICON_ID 333

//�E�C���h�E�o�[�̎��
#define GAME_WINDOW_BAR 0

//�f�o�b�N���[�h TRUE/�`��@FALSE/�`�悵�Ȃ�
#define GAME_DEBUG FALSE

//�p�X�̒���
#define PATH_MAX	255
//�n���h���̍ő吔
#define IMGDIV_MAX	128

//��ʊO�̕����̈ʒu
#define TEXT_X 540
#define TEXT_Y 160

//�񋓌^
enum GAME_SCENE {
	GAME_SCENE_TITLE,
	GAME_SCENE_PLAY,
	GAME_SCENE_END,
	GAME_SCENE_CHANGE //�G�t�F�N�g���o���V�[��
};//�Q�[���̃V�[��

//�\���̂̒�`

//�摜�̍\����
struct IMAGE
{
	int handle = -1;	//�摜�̃n���h���i�Ǘ��ԍ��j
	char path[255];		//�摜�̏ꏊ�i�p�X�j

	int x;		//X�ʒu
	int y;		//Y�ʒu
	int width;	//��
	int height;	//����

	BOOL IsDraw = FALSE;//�摜���`��ł��邩
};

//�L�����N�^�̍\����
struct CHARACTOR
{
	IMAGE img;	//�摜�\����
	int speed = 1;

	RECT coll;	//�㉺���E�����蔻��̗̈� RECT�͎l�p�`�̈ʒu��������
};

//���y�̍\����
struct AUDIO
{
	int handle = -1;	//���y�̃n���h��
	char path[255];		//���y�̃p�X

	int volume = -1;	//�{�����[���iMIN 0�@�`�@�Q�T�T�@MAX�j
	int playType = -1;
};

struct DIVIMAGE
{
	int DivMax;						//������
	int handle[IMGDIV_MAX];			//�摜�n���h��
	int nowIndex = 0;				//���݂̃n���h���z��̃C���f�b�N�X
	char path[PATH_MAX];			//�p�X

	int x = -1;						//X�ʒu
	int y = -1;						//Y�ʒu
	int width = -1;					//��
	int height = -1;				//����

	int DivYoko = -1;				//���̕�����
	int DivTate = -1;				//�c�̕�����

	RECT coll;						//�����蔻��̗̈�(�l�p)

	BOOL IsDraw = FALSE;			//�`��ł��邩�H

	int AnimCnt = 0;				//�A�j���[�V�����J�E���^
	int AnimCntMAX = 2;				//�A�j���[�V�����J�E���^MAX
	BOOL IsAnimLoop = FALSE;		//�A�j���[�V���������[�v�����邩�H
};

//�S�[���̍\����
struct Goal
{
	IMAGE img;
	int speed = -1;

	RECT coll;
};


extern VOID ChangeScene(GAME_SCENE scene);	//�V�[���؂�ւ�

extern VOID collUpdateplayer(CHARACTOR* chara);	//�����蔻��̗̈���X�V

extern VOID collUpdateGoal(Goal* chara);	//�����蔻��̗̈���X�V

extern VOID collUpdateenemy(CHARACTOR* chara);	//�����蔻��̗̈���X�V

extern BOOL colltouch(RECT a, RECT b);//�����蔻��̐G��Ă��邩�G��Ă��Ȃ����̔���

extern BOOL IventProc(RECT a, RECT b);

//�Q�[���f�[�^�̓ǂݍ���
extern BOOL GameLoad(VOID);
//���y�̏��
extern BOOL LoadAudio(AUDIO* audio, const char* path, int volume, int playType);
//���y�̓ǂݍ���
extern BOOL LoadImageMem(IMAGE* image, const char* path);
//���y�̍Đ�
extern VOID PlayAudio(AUDIO audio);
//���y�̒�~
extern VOID StopAudio(AUDIO* audio);

extern DIVIMAGE PlayerDIVIMG;
//�����摜�̕`��
extern VOID DrawDivImage(DIVIMAGE* image);
//�������ĉ摜��ǂݍ���
extern BOOL LoadImageDivMem(DIVIMAGE* div, const char* path, int bunkatuYoko, int bunkatuTate);
//�����摜�̓����蔻��X�V
extern VOID CollUpdateDivImage(DIVIMAGE* div);