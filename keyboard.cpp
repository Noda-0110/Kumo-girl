//�L�[�{�[�h�̃\�[�X�t�@�C��

//�w�b�_�t�@�C���ǂݍ���
#include "keyboard.h"

//�O���[�o���ϐ�
KEYBOARD keyboard;

//�֐�

/// <summary>
/// �L�[�̓��͏�Ԃ��X�V����
/// </summary>
VOID AllKeyUpdate(VOID)
{
	//���O�̃L�[���͂��Ƃ��Ă����@����AllKey���ȑOOld�ւƈڂ�
	for (int i = 0; i < KEY_KIND_MAX; i++)
	{
		keyboard.OldAllKeyState[i] = keyboard.AllKeyState[i];
	}

	//�S�ẴL�[�̓��͏�Ԃ𓾂�ATempKeyState�̒��ɓ����Ă���
	GetHitKeyStateAll(&keyboard.TempKeyState[0]);

	//������Ă���L�[�̎��Ԃ��X�V����ATempKeyState������
	for (int i = 0; i < KEY_KIND_MAX; i++)
	{
		//�L�[�{�[�h�������Ă���Ƃ��A�O�łȂ��Ƃ�
		if (keyboard.TempKeyState[i] != 0)
		{
			keyboard.AllKeyState[i]++;		//�����Ă��鎞�Ԃ����Z
		}
		//�����Ă��Ȃ��Ƃ�
		else
		{
			keyboard.AllKeyState[i] = 0;	//�����Ă��鎞�Ԃ�0�ɖ߂�
		}
	}
	return;
}

/// <summary>
/// �L�[�������Ă��邩�A�L�[�R�[�h�Ŕ��f����
/// </summary>
/// <param name="KEY_INPUT_">�L�[�R�[�h</param>
/// <returns>�����Ă�����O�łȂ����TRUE</returns>
BOOL KeyDown(int KEY_INPUT_)
{
	if (keyboard.AllKeyState[KEY_INPUT_] != 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/// <summary>
/// �L�[�������Ă��邩�i�������Ƃ��j�A�L�[�R�[�h�Ŕ��f����
/// </summary>
/// <param name="KEY_INPUT_">�L�[�R�[�h</param>
/// <returns>�����Ă�����TRUE</returns>
BOOL KeyUp(int KEY_INPUT_)
{
	//�ȑO�͂O�łȂ��A�����݂͂O�łȂ�
	if (
		keyboard.OldAllKeyState[KEY_INPUT_] != 0	//�ȑO�͉����Ă���
		&& keyboard.AllKeyState[KEY_INPUT_] == 0	//���݂͉����Ă��Ȃ�
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
/// �L�[���N���b�N�������A�L�[�R�[�h�Ŕ��f����
/// </summary>
/// <param name="KEY_INPUT_">�L�[�R�[�h</param>
/// <returns>�N���b�N������TRUE</returns>
BOOL KeyClick(int KEY_INPUT_)
{
	//�ȑO�͂O�łȂ��A�����݂͂O�łȂ�
	if (
		keyboard.OldAllKeyState[KEY_INPUT_] != 0	//�ȑO�͉����Ă���
		&& keyboard.AllKeyState[KEY_INPUT_] == 0	//���݂͉����Ă��Ȃ�
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
/// �L�[�����������Ă��邩���A�L�[�R�[�h�Ŕ��f
/// </summary>
/// <param name="KEY_INPUT_intMilliTime">�L�[�R�[�h</param>
/// <returns>�L�[�����������Ă���~���b</returns>
BOOL KeyDownKeep(int KEY_INPUT_,int MilliTime)
{
	//1�b��1000�~���b
	float MilliSec = 1000.0f;


	//���������Ă��鎞�Ԃ́A�~���b���~�e�o�r�l
	//��@�P�T�O�O�~���b����/�P�O�O�O�~���@���@1.5�b �~�@60FPS = 90
	int UpdateTime = (MilliTime / MilliSec) * 60;

	if (keyboard.AllKeyState[KEY_INPUT_] > UpdateTime)
	{
		return TRUE;	//���������Ă���
	}

	else {
		return FALSE;	//���������Ă��Ȃ�
	}

}