//#CRT_SECURE_NO_WARNINGS
//�w�b�_�t�@�C���ǂݍ���
#include "game.h"		//�Q�[�����Ǘ�����ׂ̃w�b�_�t�@�C���@�����܂ł��Q�[���̊Ǘ�
#include "keyboard.h"	//�L�[�{�[�h�̏���
#include "FPS.h"		//FPS�̏���



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

//�w�i�̍\����
IMAGE TitlePic;
IMAGE PlayPic;
IMAGE EdPic;

//�v���g�^�C�v�錾
VOID Title(VOID);		//�^�C�g�����
VOID TitleProc(VOID);	//�^�C�g����ʁi�����j
VOID TitleDraw(VOID);	//�^�C�g����ʁi�`��j

VOID Play(VOID);		//�v���C���
VOID PlayProc(VOID);	//�v���C��ʁi�����j
VOID PlayDraw(VOID);	//�v���C��ʁi�`��j

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



VOID ChangeScene(GAME_SCENE scene);	//�V�[���؂�ւ�

VOID collUpdateplayer(CHARACTOR* chara);	//�����蔻��̗̈���X�V

VOID collUpdateGoal(CHARACTOR* chara);	//�����蔻��̗̈���X�V

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
		FPSDraw();
		//FPS������҂�
		FPSWait();

		ScreenFlip();	//�_�u���o�b�t�@�����O������ʂ�`��
	}

	//- - - - �f�[�^�̊J�� - - - -

	//���y�̍폜
	DeleteSoundMem(TitleBGM.handle);
	DeleteSoundMem(PlayBGM.handle);
	DeleteSoundMem(EdBGM.handle);
	//�w�i�̊J��
	DeleteGraph(TitlePic.handle);
	DeleteGraph(PlayPic.handle);
	DeleteGraph(EdPic.handle);

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

	//�w�i�̏��X�̓ǂݍ���
	//�^�C�g����ʂ̔w�i
	if (LoadImageMem(&TitlePic, ".\\Image\\�^�C�g�����.png") == FALSE) { return FALSE; }
	TitlePic.x = 0;
	TitlePic.y = 0;
	TitlePic.IsDraw = TRUE;

	if (LoadImageMem(&EdPic, ".\\Image\\�G���h���.png") == FALSE) { return FALSE; }
	EdPic.x = 0;
	EdPic.y = 0;
	EdPic.IsDraw = TRUE;
	return TRUE;

}
// - - - - - �f�[�^���[�h - - - - - //

VOID GameInit(VOID)
{


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

	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//BGM�̒�~
		StopAudio(&TitleBGM);

		//�v���C��ʂւ̑J��
		ChangeScene(GAME_SCENE_PLAY);

		return;
	}

	PlayAudio(TitleBGM);

	return;
}
/// <summary>
/// �^�C�g����ʁ@���� 
/// </summary>
VOID TitleDraw(VOID)
{
	DrawGraph(TitlePic.x, TitlePic.y, TitlePic.handle, TRUE);

	DrawString(0, 0, "�^�C�g�����", GetColor(0, 0, 0));
	return;
}


/// <summary>
/// �v���C���
/// </summary>
VOID Play(VOID)
{
	PlayProc();
	PlayDraw();
	return;
}
/// <summary>
/// �v���C��ʁ@����
/// </summary>
VOID PlayProc(VOID)
{

	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//BGM�̒�~
		StopAudio(&PlayBGM);

		//�G���h��ʂւ̑J��
		ChangeScene(GAME_SCENE_END);

		return;
	}
	PlayAudio(PlayBGM);

	return;
}
/// <summary>
/// �v���C��ʁ@�`��
/// </summary>
VOID PlayDraw(VOID)

{
	DrawString(0, 0, "�v���C���", GetColor(0, 0, 0));
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
	DrawGraph(EdPic.x, EdPic.y, EdPic.handle, TRUE);

	DrawString(0, 0, "�G���h���", GetColor(0, 0, 0));
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

	DrawString(0, 0, "�؂�ւ����", GetColor(0, 0, 0));
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
VOID collUpdateGoal(CHARACTOR* chara)
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