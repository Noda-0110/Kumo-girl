//#CRT_SECURE_NO_WARNINGS
//�w�b�_�t�@�C���ǂݍ���
#include "game.h"		//�Q�[�����Ǘ�����ׂ̃w�b�_�t�@�C���@�����܂ł��Q�[���̊Ǘ�
#include "keyboard.h"	//�L�[�{�[�h�̏���
#include "FPS.h"		//FPS�̏���
#include "Map.h"		//FPS�̏���

enum MUKI
{
	muki_none,
	muki_ue,
	muki_hidari,
	muki_shita,
	muki_migi
};

//�O���[�o���ϐ�
//�V�[�����Ǘ�����ϐ�
GAME_SCENE GameScene;		//���݂̃Q�[���̃V�[��
GAME_SCENE OldGameScene;	//�O����Q�[���̃V�[��
GAME_SCENE NextGameScene;	//���̃Q�[���̃V�[��


//��ʂ̐؂�ւ�
BOOL IsFadeOut = FALSE;	//�t�F�[�h�A�E�g
BOOL IsFadeIn = FALSE;	//�t�F�[�h�C��

int FadeTimeMill = 2000;					//�؂�ւ��~���b
int FadeTimeMax = FadeTimeMill / 1000 * 60;	//�~���b���t���[���a�ɕϊ�

//�t�F�[�h�A�E�g
int FadeOutCntInit = 0;				//�����l
int FadeOutCnt = FadeOutCntInit;	//�t�F�[�h�A�E�g�̃J�E���^
int FadeOutCntMax = FadeTimeMax;	//�t�F�[�h�A�E�g�̃J�E���^MAX

//�t�F�[�h�C��
int FadeInCntInit = FadeTimeMax;	//�����l
int FadeInCnt = FadeInCntInit;		//�t�F�[�h�C���̃J�E���^
int FadeInCntMax = FadeTimeMax;				//�t�F�[�h�C���̃J�E���^MAX

//PushEnter�̓_��
int PushEnterCnt = 0;
const int PushEnterCntMAX = 60;
BOOL PushEnterBrink = FALSE;

//BGM�̍\����
AUDIO TitleBGM;
AUDIO PlayBGM;
AUDIO EdBGM;

AUDIO PianoBGM;

//�w�i�̍\����
IMAGE TitlePic;
IMAGE EdPic;

//���S�̓ǂݍ���
IMAGE Logo;

//PushEnter�̍\����
IMAGE EnterPic;
IMAGE EdEnter;

//�����o���̍\����
IMAGE Hukidasi;

//�v���C���[�̍\���� charchip�̊�
DIVIMAGE PlayerDIVIMG;
MUKI muki = muki_shita;

//�S�[���̍\����
Goal Kumo;

//�}�b�v�\����
MAP_DATA map;

//�C�x���g�}�X
RECT bedevent;
RECT pianoevent;
RECT kurozevent;
RECT bookevent;

//�v���g�^�C�v�錾
VOID Title(VOID);		//�^�C�g�����
VOID TitleProc(VOID);	//�^�C�g����ʁi�����j
VOID TitleDraw(VOID);	//�^�C�g����ʁi�`��j

VOID Play(VOID);		//�v���C���
VOID PlayProc(VOID);	//�v���C��ʁi�����j
VOID PlayDraw(VOID);	//�v���C��ʁi�`��j
VOID EventDraw(VOID);	//�C�x���g�}�X�̕`��

VOID End(VOID);			//�G���h���
VOID EndProc(VOID);		//�G���h��ʁi�����j
VOID EndDraw(VOID);		//�G���h��ʁi�`��j

VOID Change(VOID);		//�؂�ւ����
VOID ChangeProc(VOID);	//�؂�ւ���ʁi�����j
VOID ChangeDraw(VOID);	//�؂�ւ���ʁi�`��j

//���y�̍Đ�
VOID PlayAudio(AUDIO audio);
//���y�̒�~
VOID StopAudio(AUDIO* audio);
//�������ĉ摜��ǂݍ���
BOOL LoadImageDivMem(DIVIMAGE* div, const char* path, int bunkatuYoko, int bunkatuTate);
//�����摜�̓����蔻��X�V
VOID CollUpdateDivImage(DIVIMAGE* div);



VOID ChangeScene(GAME_SCENE scene);	//�V�[���؂�ւ�

VOID collUpdateplayer(CHARACTOR* chara);	//�����蔻��̗̈���X�V

VOID collUpdateGoal(Goal* chara);	//�����蔻��̗̈���X�V

VOID collUpdateenemy(CHARACTOR* chara);	//�����蔻��̗̈���X�V


BOOL colltouch(RECT a, RECT b);//�����蔻��̐G��Ă��邩�G��Ă��Ȃ����̔���

BOOL GameLoad(VOID);	//�Q�[���f�[�^�̓ǂݍ���
BOOL LoadAudio(AUDIO* audio, const char* path, int volume, int playType);	
BOOL LoadImageMem(IMAGE* image, const char* path);		//���y�̓ǂݍ���
VOID GameInit(VOID);	//�Q�[���̏�����

// �v���O������ WinMain ����n�܂�܂�
//Windows�̃v���O���~���O�@�œ����Ă���
//DxLib�́ADirectX�Ƃ����A�Q�[���v���O���~���O���ȒP�Ɉ�����d�g��
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	//�ǉ���
	SetOutApplicationLogValidFlag(FALSE);	//Log.txt���o�͂��Ȃ�

	ChangeWindowMode(TRUE);					//�E�B���h�E���[�h�ɐݒ�
	SetMainWindowText(GAME_TITLE);			//�E�B���h�E�̃^�C�g������
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	//�E�B���h�E�̉𑜓x��ݒ�
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);				//�E�B���h�E�̑傫����ݒ�
	SetBackgroundColor(255, 255, 255);					//�f�t�H���g�̔w�i�F
	SetWindowIconID(GAME_ICON_ID);						//�A�C�R���t�@�C���̓ǂݍ���

	//�ǉ���
	SetWindowStyleMode(GAME_WINDOW_BAR);	//�E�B���h�E�o�[�̏��
	SetWaitVSyncFlag(TRUE);					//�f�B�X�v���C�̐���������L���ɂ���
											//���x�ȏ�����`����s�����ɂ�FALSE

	SetAlwaysRunFlag(TRUE);					//�E�B���h�E�������ƃA�N�e�B�u�ɂ���


	if (DxLib_Init() == -1)		//�c�w���C�u���������������@-1���A���Ă����狭���I��
	{
		// �G���[���N�����璼���ɏI��
		return -1;
	}

	//�_�u���o�b�t�@�����O��L��
	SetDrawScreen(DX_SCREEN_BACK);


	//�ŏ��̃V�[���̓^�C�g�����
	GameScene == GAME_SCENE_TITLE;

	//�Q�[���S�̂̏�����

	//�Q�[���̓ǂݍ���
	if (!GameLoad())
	{
		//�f�[�^�̓ǂݍ���
		DxLib_End();	//DXLib�I��
		return -1;		//�ُ�I��
	}

	//�Q�[���̏�����
	GameInit();

	//�������[�v�@�󂯎�葱����
	while (1)
	{

		if (ProcessMessage() != 0) { break; }	//���b�Z�[�W���󂯎�葱����
		if (ClearDrawScreen() != 0) { break; }	//��ʂ���������

		//�L�[�{�[�h�̓��͍X�V
		AllKeyUpdate();

		//FPS�l�̍X�V
		FPSUpdate();

		//ESC�L�[�ŋ����I��
		if (KeyClick(KEY_INPUT_ESCAPE) == TRUE) { break; }

		//�ȑO�̃V�[��������
		if (GameScene != GAME_SCENE_CHANGE)
		{
			OldGameScene = GameScene;
		}

		//�V�[�����Ƃɏ������s��
		switch (GameScene)
		{
		case GAME_SCENE_TITLE:
			Title();	//�^�C�g�����
			break;
		case GAME_SCENE_PLAY:
			Play();		//�v���C���
			break;
		case GAME_SCENE_END:
			End();		//�G���h���
			break;
		case GAME_SCENE_CHANGE:
			Change();	//�؂�ւ����
			break;
		default:
			break;
		}

		//�V�[����؂�ւ���
		if (OldGameScene != GameScene)
		{
			//���݃V�[�����؂�ւ���ʁi�^�C�g���v���C�G���h�̉�ʁj�łȂ��Ƃ�
			if (GameScene != GAME_SCENE_CHANGE)
			{
				NextGameScene = GameScene;		//���̃V�[����ۑ�
				GameScene = GAME_SCENE_CHANGE;	//��ʐ؂�ւ��V�[���ɕς���
			}
		}

		//FPS�l��`��
		//FPSDraw();
		//FPS������҂�
		FPSWait();

		ScreenFlip();	//�_�u���o�b�t�@�����O������ʂ�`��
	}

	//- - - - �f�[�^�̍폜 - - - -

	//���y�̍폜
	DeleteSoundMem(TitleBGM.handle);
	DeleteSoundMem(PlayBGM.handle);
	DeleteSoundMem(EdBGM.handle);
	DeleteSoundMem(PianoBGM.handle);
	//�w�i�̍폜
	DeleteGraph(TitlePic.handle);

	//PushEnter�̍폜
	DeleteGraph(EnterPic.handle);
	DeleteGraph(EdEnter.handle);

	//�����o���̍폜
	DeleteGraph(Hukidasi.handle);

	//�S�[���̍폜
	DeleteGraph(Kumo.img.handle);

	//�v���C���[�̕����摜�폜
	for (int i = 0; i < PlayerDIVIMG.DivMax; i++) { DeleteGraph(PlayerDIVIMG.handle[i]); }

	// �c�w���C�u�����g�p�̏I�������i�����j
	DxLib_End();

	return 0;				// �\�t�g�̏I���i���s�j 
}




// - - - - �f�[�^���[�h - - - - //

/// <summary>
/// �Q�[���̃f�[�^��ǂݍ���
/// </summary>
/// <returns>�ǂݍ��߂���TRUE / �ǂݍ��߂Ȃ�������FALSE</returns>
BOOL GameLoad()
{

	//���y�̏��X�̓ǂݍ���
	if (!LoadAudio(&TitleBGM, ".\\Audio\\Title���.mp3",255,DX_PLAYTYPE_LOOP)) { return FALSE; }
	if (!LoadAudio(&PlayBGM, ".\\Audio\\Play���.mp3",255,DX_PLAYTYPE_LOOP)) { return FALSE; }
	if (!LoadAudio(&EdBGM, ".\\Audio\\Ed���.mp3",255,DX_PLAYTYPE_LOOP)) { return FALSE; }
	if (!LoadAudio(&PianoBGM, ".\\Audio\\�����̉��t.mp3",255,DX_PLAYTYPE_LOOP)) { return FALSE; }

	//�w�i�̏��X�̓ǂݍ���
	//�^�C�g����ʂ̔w�i
	if (LoadImageMem(&TitlePic, ".\\Image\\�^�C�g�����.png") == FALSE) { return FALSE; }
	//if (LoadImageMem(&TitlePic, ".\\Image\\�^�C�g�����2.png") == FALSE) { return FALSE; }
	
	TitlePic.x = 0;
	TitlePic.y = 0;
	TitlePic.IsDraw = TRUE;

	if (LoadImageMem(&EdPic, ".\\Image\\�G���h���.png") == FALSE) { return FALSE; }
	EdPic.x = 0;
	EdPic.y = 0;
	EdPic.IsDraw = TRUE;

	//�G���^�[�̓ǂݍ���
	if (!LoadImageMem(&EnterPic, ".\\Image\\Enter.png")) { return FALSE; }
	if (!LoadImageMem(&EdEnter, ".\\Image\\EdEnter.png")) { return FALSE; }

	//�����o���̓ǂݍ���
	if (!LoadImageMem(&Hukidasi, ".\\Image\\�����o��.png")) { return FALSE; }

	//���S�̓ǂݍ���
	if (!LoadImageMem(&Logo, ".\\Image\\TitleLogo.png")) { return FALSE; }

	//�v���C���[�̓ǂݍ���
	if (LoadImageDivMem(&PlayerDIVIMG, ".\\Image\\charachip.png", 3, 4) == FALSE) { return FALSE; }

	//�}�b�v�̓ǂݍ���

	//�T���v���}�b�v�f�[�^��ǂݍ���
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

	//�S�[���̏�������
	GetGraphSize(Kumo.img.handle, &Kumo.img.width, &Kumo.img.height);
	if (!LoadImageMem(&Kumo.img, ".\\Image\\�N��.png")) { return FALSE; }



	return TRUE;
}
// - - - - - �f�[�^���[�h - - - - - //

VOID GameInit(VOID)
{
	//���S�̈ʒu
	Logo.x = 0;
	Logo.y = -60;

	//PushEnter�̈ʒu
	EnterPic.x = 385;
	EnterPic.y = -10;

	//�G���h�G���^�[�̈ʒu
	EdEnter.x = 400;
	EdEnter.y = -15;

	//�����o���̈ʒu
	Hukidasi.x = 490;
	Hukidasi.y = 100;

	//�v���C���[�̈ʒu
	PlayerDIVIMG.x = 288;
	PlayerDIVIMG.y = 370;

	muki = muki_shita;

	//�S�[��������
	Kumo.img.x = 120;
	Kumo.img.y = 120;
	Kumo.speed = 200;
	Kumo.img.IsDraw = TRUE;

	//�����蔻��(�S�[��)
	collUpdateGoal(&Kumo);

	//�C�x���g�̔����ʒu
	//�x�b�h�̈ʒu
	bedevent.top = map.height * (8.5) + 1;
	bedevent.left = map.width * (12) + 1;
	bedevent.bottom = map.height * (10 + 1) + 1;
	bedevent.right = map.width * (12 + 1) + 1;
	//�s�A�m�̈ʒu
	pianoevent.top = map.height * (3.5) + 1;
	pianoevent.left = map.width * (11) + 1;
	pianoevent.bottom = map.height * (4.5 + 1) + 1;
	pianoevent.right = map.width * (11 + 1) + 1;
	//�N���[�[�b�g�̈ʒu
	kurozevent.top = map.height * (4) + 1;
	kurozevent.left = map.width * (2) + 1;
	kurozevent.bottom = map.height * (4 + 1) + 1;
	kurozevent.right = map.width * (2 + 1) + 1;
	//�{�I�̈ʒu
	bookevent.top = map.height * (4) + 1;
	bookevent.left = map.width * (8) + 1;
	bookevent.bottom = map.height * (4 + 1) + 1;
	bookevent.right = map.width * (7.5 + 1) + 1;
}

/// <summary>
/// ���y���������ւƓǂݍ���
/// </summary>
/// <param name="audio">Audio�\���̂̃A�h���X &�`�`�`</param>
/// <param name="path">Audio�̃p�X�@.\\Audio\\�`�`�`</param>
/// <param name="volume">�{�����[��</param>
/// <param name="playType">DX_PLAYTYPE_LOOP or DX_PLAYTYPE_BACK</param>
/// <returns></returns>
BOOL LoadAudio(AUDIO* audio, const char* path, int volume, int playType)
{
	strcpyDx(audio->path,path);
	audio->handle = LoadSoundMem(audio->path);

	//���y��ǂݍ��݂��ł��Ȃ�����
	if (audio->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//���C���̃E�B���h�E�n���h��
			audio->path,				//���b�Z�[�W�{��
			"���y�ǂݍ��݃G���[�I",	//���b�Z�[�W�^�C�g��
			MB_OK					//�{�^��
		);
		return FALSE;				//�G���[�I��
	}

	audio->volume = volume;
	audio->playType = playType;

	return TRUE;
}

// ------------  �V�[���֐��@�@------------ //
VOID ChangeScene(GAME_SCENE scene)
{
	GameScene = scene;	//�V�[����؂�ւ�
	IsFadeIn = FALSE;	//�t�F�[�h�C�����Ȃ�
	IsFadeOut = TRUE;	//�t�F�[�h�A�E�g����

	return;
}

/// <summary>
/// �^�C�g�����
/// </summary>
VOID Title(VOID)
{
	TitleProc();	//����
	TitleDraw();	//�`��

	return;
}
/// <summary>
/// �^�C�g����ʁ@����
/// </summary>
VOID TitleProc(VOID)
{
	GameInit();
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//BGM�̒�~
		StopAudio(&TitleBGM);

		//�v���C��ʂւ̑J��
		ChangeScene(GAME_SCENE_PLAY);

		return;
	}

	//���y��炷
	PlayAudio(TitleBGM);


	return;
}
/// <summary>
/// �^�C�g����ʁ@���� 
/// </summary>
VOID TitleDraw(VOID)
{
	//�w�i�̕`��
	DrawGraph(TitlePic.x, TitlePic.y, TitlePic.handle, TRUE);
	//���S�̕`��
	DrawGraph(Logo.x, Logo.y, Logo.handle, TRUE);
	//PushEnter�̕`��
	if (PushEnterCnt < PushEnterCntMAX) { PushEnterCnt++; }
	else
	{
		if (PushEnterBrink == TRUE)PushEnterBrink = FALSE;
		else if (PushEnterBrink == FALSE)PushEnterBrink = TRUE;

		PushEnterCnt = 0;	//�J�E���^��������
	}

	if (PushEnterBrink == TRUE)
	{
		//�������ɂ���
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)PushEnterCnt / PushEnterCntMAX) * 255);

		//PushEnter�̕`��
		DrawGraph(EnterPic.x, EnterPic.y, EnterPic.handle, TRUE);

		//�������I��
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	if (PushEnterBrink == FALSE)
	{
		//�������ɂ���
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)(PushEnterCntMAX - PushEnterCnt) / PushEnterCntMAX) * 255);

		//PushEnter�̕`��
		DrawGraph(EnterPic.x, EnterPic.y, EnterPic.handle, TRUE);

		//�������I��
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	//DrawString(0, 0, "�^�C�g�����", GetColor(0, 0, 0));
	return;
}


/// <summary>
/// �v���C���
/// </summary>
VOID Play(VOID)
{
	PlayProc();
	EventDraw();
	PlayDraw();
	return;
}
/// <summary>
/// �v���C��ʁ@����
/// </summary>
VOID PlayProc(VOID)
{
	if(GAME_DEBUG==TRUE)
	{
		if (KeyClick(KEY_INPUT_RETURN) == TRUE)
		{
			//BGM�̒�~
			StopAudio(&PlayBGM);

			//�G���h��ʂւ̑J��
			ChangeScene(GAME_SCENE_END);

			return;
		}
	}
	

	PlayAudio(PlayBGM);
	//�}�b�v�̓����蔻����擾
	{
		muki = muki_none;	//�ŏ��͌����𖳂��ɂ���
		DIVIMAGE dummy = PlayerDIVIMG;	//�����蔻��̃_�~�[���쐬 
		//�_�~�[���s�����ē�����Ȃ���΃_�~�[���{�̂ɑ���
		if (KeyDown(KEY_INPUT_W)) { muki = muki_ue; dummy.y--; }
		else if (KeyDown(KEY_INPUT_S)) { muki = muki_shita; dummy.y++; }
		if (KeyDown(KEY_INPUT_A)) { muki = muki_hidari; dummy.x--; }
		else if (KeyDown(KEY_INPUT_D)) { muki = muki_migi; dummy.x++; }

		CollUpdateDivImage(&dummy);	//�����蔻��̍X�V

		if (CollMap(dummy.coll, map) == FALSE)
		{
			PlayerDIVIMG = dummy;	//�_�~�[����߂�
		}
		

		
		collUpdateGoal(&Kumo);
		//�v���C���[���S�[���ɓ���������
		if (colltouch(dummy.coll, Kumo.coll) == TRUE)
		{
			//BGM���~�߂�
			StopSoundMem(PlayBGM.handle);

			ChangeScene(GAME_SCENE_END);
			return;
		}
	}

	return;
}

/// <summary>
/// �v���C��ʁ@�`��
/// </summary>
VOID PlayDraw(VOID)
{
	{DrawDivImage(&PlayerDIVIMG); }

	DrawMap(map);

	DrawGraph(Kumo.img.x, Kumo.img.y, Kumo.img.handle, TRUE);
	//���S�̕`��
	DrawGraph(430, 530, Logo.handle, TRUE);

	if(GAME_DEBUG==TRUE)
	DrawRect(bookevent, GetColor(255, 0, 0), TRUE);

	DrawString(500, 50, "�N���ɉ�ɍs����", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// �C�x���g�}�X�̏���
/// </summary>
VOID EventDraw(VOID)
{
	//�����o���̕`��
	DrawGraph(Hukidasi.x, Hukidasi.y, Hukidasi.handle, TRUE);

	//�C�x���g�̏���
	if (CheckCollRectToRect(PlayerDIVIMG.coll, bedevent) == TRUE)
	{
		DrawString(TEXT_X, TEXT_Y, "���̃x�b�h��\n�O�ŐQ�邩��\n���܂�g��Ȃ�", GetColor(0, 0, 0));

	}
	if (CheckCollRectToRect(PlayerDIVIMG.coll, pianoevent) == TRUE)
	{
		StopSoundMem(PlayBGM.handle);
		PlayAudio(PianoBGM);
		DrawString(TEXT_X, TEXT_Y, "�s�A�m\n�a�����ɖ������\n��������������", GetColor(0, 0, 0));
	}
	else
	{
		StopSoundMem(PianoBGM.handle);
	}

	if (CheckCollRectToRect(PlayerDIVIMG.coll, kurozevent) == TRUE)
	{
		DrawString(TEXT_X, TEXT_Y, "�N���[�[�b�g\n���͂܂�\n���ւ���K�v�͂Ȃ�", GetColor(0, 0, 0));
	}

	if (CheckCollRectToRect(PlayerDIVIMG.coll, bookevent) == TRUE)
	{
		DrawString(TEXT_X, TEXT_Y, "�{�I\n���A��ΎR�Ȃ�\n�`���̖{������", GetColor(0, 0, 0));
	}
	return;
}

/// <summary>
/// �G���h���
/// </summary>
VOID End(VOID)
{
	EndProc();
	EndDraw();
	return;
}
/// <summary>
/// �G���h��ʁ@����
/// </summary>
VOID EndProc(VOID)
{

	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//BGM�̒�~
		StopAudio(&EdBGM);

		ChangeScene(GAME_SCENE_TITLE);

		return;
	}

	PlayAudio(EdBGM);

	return;
}

/// <summary>
/// �G���h��� �`��
/// </summary>
VOID EndDraw(VOID)
{
	//�w�i�̕`��
	DrawGraph(EdPic.x, EdPic.y, EdPic.handle, TRUE);
	//���S�̕`��
	DrawGraph(Logo.x, Logo.y, Logo.handle, TRUE);
	//PushEnter�̕`��
	if (PushEnterCnt < PushEnterCntMAX) { PushEnterCnt++; }
	else
	{
		if (PushEnterBrink == TRUE)PushEnterBrink = FALSE;
		else if (PushEnterBrink == FALSE)PushEnterBrink = TRUE;

		PushEnterCnt = 0;	//�J�E���^��������
	}

	if (PushEnterBrink == TRUE)
	{
		//�������ɂ���
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)PushEnterCnt / PushEnterCntMAX) * 255);

		//PushEnter�̕`��
		DrawGraph(EdEnter.x, EdEnter.y, EdEnter.handle, TRUE);

		//�������I��
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	if (PushEnterBrink == FALSE)
	{
		//�������ɂ���
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)(PushEnterCntMAX - PushEnterCnt) / PushEnterCntMAX) * 255);

		//PushEnter�̕`��
		DrawGraph(EdEnter.x, EdEnter.y, EdEnter.handle, TRUE);

		//�������I��
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	//DrawString(0, 0, "�G���h���", GetColor(0, 0, 0));
	return;
}


/// <summary>
/// �؂�ւ����
/// </summary>
VOID Change(VOID)
{
	ChangeProc();
	ChangeDraw();
	return;
}
/// <summary>
/// �؂�ւ���� ����
/// </summary>
VOID ChangeProc(VOID)
{
	//�t�F�[�h�C��
	if (IsFadeIn == TRUE)
	{
		if (FadeInCnt > FadeInCntMax)
		{
			FadeInCnt--;	//�J�E���^�����炷�A�����ɂ��Ă���
		}
		else
		{
			//�t�F�[�h�C�������̏I���� �����J�E���^��MAX�𒴂����Ƃ�
			FadeInCnt = FadeInCntInit;	//�J�E���^������
			IsFadeIn = FALSE;			//�t�F�[�h�C�������̏I��
		}

	}
	//�t�F�[�h�A�E�g
	if (IsFadeOut == TRUE)
	{
		if (FadeOutCnt < FadeOutCntMax)
		{
			FadeOutCnt++;	//�J�E���^�𑝂₷�A�F��Z���ɂ��Ă���
		}
		else
		{
			//�t�F�[�h�C�������̏I���� �����J�E���^��MAX�𒴂����Ƃ�
			FadeOutCnt = FadeOutCntInit;	//�J�E���^������
			IsFadeOut = FALSE;			//�t�F�[�h�C�������̏I��
		}

	}

	//�؂�ւ������I���H
	if (IsFadeIn == FALSE && IsFadeOut == FALSE)
	{
		//�t�F�[�h�A�E�g���t�F�[�h�C�������Ă��Ȃ�
		GameScene = NextGameScene;	//���̃V�[���ɐ؂�ւ�
		OldGameScene = GameScene;	//�ȑO�̃V�[���ɍX�V
	}
	return;
}

/// <summary>
/// �؂�ւ���ʁ@�`��
/// </summary>
VOID ChangeDraw(VOID)
{
	//�ȑO�̃V�[����`��
	switch (OldGameScene)
	{
	case GAME_SCENE_TITLE:
		TitleDraw();	//�^�C�g����ʂ̕`��
		break;
	case GAME_SCENE_PLAY:
		PlayDraw();		//�v���C��ʂ̕`��
		break;
	case GAME_SCENE_END:
		EndDraw();		//�G���h��ʂ̕`��
		break;
	default:
		break;
	}

	//�t�F�[�h�C��
	if (IsFadeIn == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)FadeInCnt / FadeInCntMax) * 255);	//�p�[�Z���e�[�W

	}
	//�t�F�[�h�A�E�g
	if (IsFadeOut == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)FadeOutCnt / FadeOutCntMax) * 255);	//�p�[�Z���e�[�W

	}

	//�l�p��`��
	DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(0, 0, 0), TRUE);

	//�������I��
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//DrawString(0, 0, "�؂�ւ����", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// �����蔻��̍X�V(�v���C���[��p)
/// </summary>
/// <param name="coll">�����蔻��̗̈�</param>
VOID collUpdateplayer(CHARACTOR* chara)
{
	chara->coll.left = chara->img.x + 30;		//�����蔻��������
	chara->coll.top = chara->img.y + 25;		//�����蔻��������
	chara->coll.right = chara->img.x + chara->img.width - 15;	//�����蔻��������
	chara->coll.bottom = chara->img.y + chara->img.height - 20;	//�����蔻��������

	return;
}

/// <summary>
/// �����蔻��̍X�V(�S�[���p)
/// </summary>
/// <param name="coll">�����蔻��̗̈�</param>
VOID collUpdateGoal(Goal* chara)
{
	chara->coll.left = chara->img.x;
	chara->coll.top = chara->img.y;
	chara->coll.right = chara->img.x + chara->img.width;
	chara->coll.bottom = chara->img.y + chara->img.height;

	return;
}

/// <summary>
/// �����蔻��̍X�V(�G�p)
/// </summary>
/// <param name="coll">�����蔻��̗̈�</param>
VOID collUpdateenemy(CHARACTOR* chara)
{
	chara->coll.left = chara->img.x;
	chara->coll.top = chara->img.y;
	chara->coll.right = chara->img.x + chara->img.width;
	chara->coll.bottom = chara->img.y + chara->img.height;

	return;
}


/// <summary>
/// ��`���m�̓����蔻��
/// </summary>
/// <param name="a">���炩�̋�`"a"</param>
/// <param name="b">���炩�̋�`"b"</param>
/// <returns>����������TRUE / ������Ȃ����FALSE</returns>
BOOL colltouch(RECT a, RECT b)
{
	if (
		a.left < b.right &&	//p�̍���x < g�̉E��x���W
		a.right > b.left &&	//p�̉E��x < g�̍���x���W
		a.bottom > b.top &&	//p�̏��y < g�̉���y���W
		a.top < b.bottom	//p�̉���y < g�̏��y���W
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
/// �^�C�g�����S�̓ǂݍ���
/// </summary>
/// <param name="image"></param>
/// <param name="path"></param>
/// <returns></returns>
BOOL LoadImageMem(IMAGE* image, const char* path)
{
	//�摜��ǂݍ���
	strcpyDx(image->path, path);
	image->handle = LoadGraph(image->path);	//�摜��ǂݍ���

	//�摜���ǂݍ��߂Ȃ������Ƃ��A�G���[�i�[�P�j������
	if (image->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//���C���̃E�B���h�E�n���h��
			image->path,				//���b�Z�[�W�{��
			"�摜�ǂݍ��݃G���[�I",	//���b�Z�[�W�^�C�g��
			MB_OK					//�{�^��
		);
		return FALSE;				//�G���[�I��
	}

	//�摜�̕��ƍ���������
	GetGraphSize(image->handle, &image->width, &image->height);
	return TRUE;

}


/// <summary>
/// �摜�𕪊����ă������ɓǂݍ���
/// </summary>
/// <param name="handle">�n���h���z��̐擪�A�h���X</param>
/// <param name="path">�摜�̃p�X</param>
/// <param name="bunkatuYoko">��������Ƃ��̉��̐�</param>
/// <param name="bunkatuTate">��������Ƃ��̏c�̐�</param>
/// <returns></returns>
BOOL LoadImageDivMem(DIVIMAGE* div, const char* path, int bunkatuYoko, int bunkatuTate)
{
	int IsLoad = -1;	//�摜���ǂݍ��߂����H

	//�ꎞ�I�ɉ摜�̃n���h����p�ӂ���
	int TempHandle = LoadGraph(path);

	//�ǂݍ��݃G���[
	if (TempHandle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//�E�B���h�E�n���h��
			path,					//�{��
			"�摜�ǂݍ��݃G���[",	//�^�C�g��
			MB_OK					//�{�^��
		);

		return FALSE;
	}

	//�摜�̕��ƍ������擾
	int Width = -1;		//��
	int Height = -1;	//����
	GetGraphSize(TempHandle, &Width, &Height);

	//�����ǂݍ���
	IsLoad = LoadDivGraph(
		path,							//�摜�̃p�X
		bunkatuYoko * bunkatuTate,		//��������
		bunkatuYoko, bunkatuTate,					//���Əc�̕���
		Width / bunkatuYoko, Height / bunkatuTate,	//�摜1�̕�,����
		div->handle									//�A���ŊǗ�����z��̐擪�A�h���X
	);

	//�����G���[
	if (IsLoad == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//�E�B���h�E�n���h��
			path,					//�{��
			"�摜�����G���[",		//�^�C�g��
			MB_OK					//�{�^��
		);

		return FALSE;	//�ǂݍ��ݎ��s
	}

	//����ݒ�
	div->DivYoko = bunkatuYoko;
	div->DivTate = bunkatuTate;
	div->DivMax = div->DivYoko * div->DivTate;
	div->x = 0;
	div->y = 0;
	GetGraphSize(div->handle[0], &div->width, &div->height);
	div->AnimCnt = 0;
	div->IsAnimLoop = FALSE;
	div->nowIndex = 0;

	//�����蔻��X�V
	CollUpdateDivImage(div);

	//�摜�`�悷��
	div->IsDraw = TRUE;

	//�ꎞ�I�ɓǂݍ��񂾃n���h�������
	DeleteGraph(TempHandle);

	return TRUE;
}


/// <summary>
/// �����摜�̕`��
/// </summary>
/// <param name="image">�`�悷��摜�\����</param>
/// <returns></returns>
/// <returns></returns>
VOID DrawDivImage(DIVIMAGE* image)
{
	//�摜���`��ł���Ƃ���
	if (image->IsDraw == TRUE)
	{
		DrawGraph(image->x, image->y, image->handle[image->nowIndex], TRUE);

		//�A�j���[�V�����̃J�E���g�A�b�v
		if (image->AnimCnt < image->AnimCntMAX) { image->AnimCnt++; }
		else
		{

			//�E�����̂Ƃ�(�摜�ɂ���Đ������Ⴄ�̂ŁA���킹�邱�ƁI)
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

			//�������̂Ƃ�(�摜�ɂ���Đ������Ⴄ�̂ŁA���킹�邱�ƁI)
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

			//������̂Ƃ�(�摜�ɂ���Đ������Ⴄ�̂ŁA���킹�邱�ƁI)
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

			//�������̂Ƃ�(�摜�ɂ���Đ������Ⴄ�̂ŁA���킹�邱�ƁI)
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

			//���������̂Ƃ��́A���O�̌����̐^�񒆂̉摜�ɂ���(�摜�ɍ��킹�Č��߂ăl)
			if (muki == muki_none)
			{
				if (image->nowIndex >= 6 && image->nowIndex <= 8) { image->nowIndex = 7; }
				if (image->nowIndex >= 3 && image->nowIndex <= 5) { image->nowIndex = 4; }
				if (image->nowIndex >= 9 && image->nowIndex <= 11) { image->nowIndex = 10; }
				if (image->nowIndex >= 0 && image->nowIndex <= 2) { image->nowIndex = 1; }
			}

			image->AnimCnt = 0;	//�J�E���^0�N���A
		}
	}

	//�f�o�b�O���[�h�̂Ƃ���
	if (GAME_DEBUG == TRUE)
	{
		//�����蔻���`��
		DrawRect(image->coll, GetColor(255, 255, 255), FALSE);
	}
	return;
}


/// <summary>
/// �����蔻��̗̈�X�V�i�����摜�j
/// </summary>
/// <param name="img">�摜�\���̂̃|�C���^</param>
VOID CollUpdateDivImage(DIVIMAGE* div)
{
	div->coll.left = div->x;
	div->coll.top = div->y;

	div->coll.right = div->x + div->width;
	div->coll.bottom = div->y + div->height;

	return;
}

//���y�̍Đ�
VOID PlayAudio(AUDIO audio)
{
	//���[�v�Đ��łȂ��Ȃ�A���̂܂܍Đ�
	if (audio.playType == DX_PLAYTYPE_BACK)
	{
		//���y�̍Đ�
		PlaySoundMem(audio.handle, audio.playType);
	}
	//���[�v�Đ��̂Ƃ��A���y���Đ�����Ă��Ȃ��Ȃ�A�Đ�
	else if (audio.playType == DX_PLAYTYPE_LOOP)
	{
		if (CheckSoundMem(audio.handle) == 0)
		{
			//���y�̍Đ�
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