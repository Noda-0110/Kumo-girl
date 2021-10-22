//FPS�̃\�[�X�t�@�C��

//�w�b�_�t�@�C���ǂݍ���
#include "FPS.h"

//�O���[�o���ϐ�
FPS fps;	//FPS�̊Ǘ�(FPS�͍\����)

//�֐�

/// <summary>
/// FPS�l���v�����A�l���X�V����
/// </summary>
/// <param name=""></param>
VOID FPSUpdate(VOID)
{
	if (FALSE == fps.IsInitFlg)
	{
		//����J�n�������}�C�N���P�ʂŏ���
		fps.startTime = GetNowHiPerformanceCount();	//Windows���N�����Ă���o�߂������ԁi�}�C�N���b�j

		fps.IsInitFlg = TRUE;	//�t���O�𗧂Ă�
	}

	//���݂̎������}�C�N���b�P�ʂŎ擾
	fps.NowTime = GetNowHiPerformanceCount();

	//�O��擾���Ԃ���̌o�ߎ��Ԃ�b�i�����j�ɕϊ����Ă���Z�b�g
	fps.DeltaTime = (fps.NowTime - fps.OldTime) / 1000000.0f;

	//���񏊓��������Ԃ�ۑ�
	fps.OldTime = fps.NowTime;

	//1�t���[���ځ`FPS�ݒ�l�܂ł́A�J�E���g�A�b�v
	if (fps.count < fps.sampleRate)
	{
		//�J�E���^�𑝂₷
		fps.count++;
	}
	else {
		//FPS�ݒ�l�̃t���[���ŁA����FPS���v�Z

		//���݂̎�������0�t���[���ڂ̎��Ԃ�����FPS�̐��l�Ŋ���
		//���݂�FPS�̒l���o��
		fps.DrawValue = 1000000.f / ((fps.NowTime - fps.startTime) / (float)fps.sampleRate);

		fps.startTime = GetNowHiPerformanceCount();	//Windows���N�����Ă���o�߂������ԁi�}�C�N���b�j

		//�J�E���^������
		fps.count = 1;
	}
	return;
}

/// <summary>
/// ����FPS�l��`�悷��
/// </summary>
/// <param name=""></param>
VOID FPSDraw(VOID)
{
	if (GAME_DEBUG == TRUE) {
		//�������`��
		DrawFormatString(5,700, GetColor(0, 0, 0), "FPS:%.1f", fps.DrawValue);
	}
	return;
}


/// <summary>
/// FPS�ŏ�����҂�
/// </summary>
/// <param name=""></param>
VOID FPSWait(VOID)
{
	//���݂̎����[�ŏ��̎����ŁA���݂������Ă��鎞������������
	LONGLONG resulTtime = fps.NowTime - fps.startTime;

	//�҂ׂ��~���b���i1�b/FPS�l�@*�@���݂̃t���[�������猻�݂������Ă��鎞��������
	LONGLONG waitTime = 1000000.0f / fps.Value * fps.count - resulTtime;

	//�}�C�N���b����~���b�ɕϊ�
	waitTime /= 1000.0f;

	//���������������珈����҂�
	if (WaitTimer > 0)
	{
		WaitTimer(waitTime);	//�����~���b��
	}


	//����������OFF�ɂ��Ă��邩
	if (GetWaitVSyncFlag() == FALSE)
	{
		//FPS�ő�l�łȂ�
		if (fps.Value < GAME_FPS_MAX)
		{
			//��b�Ԃ�FPS�l�����A�҂��Ԃ����������́A������FPS�l�������Ă��悢
			//�҂���10�~�� <= 1�b/60FPS = 16.6666�~���@���������ł���
			if (waitTime > 0
				&& waitTime <= 1000.0f / fps.Value)
			{
				fps.Value++;
			}
			else
			{
				//FPS�l���ǂ����Ă��炸�A�x������FPS�l��������
				if (fps.Value > GAME_FPS_MIN)
				{
					fps.Value--;
				}
			}
		}
	}
}