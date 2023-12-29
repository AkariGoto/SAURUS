/**
 *  File: TimedMotionProcedure.cpp
 *
 *  Description: �^�C�}�Ŏ����I�ɌĂ΂����e�v����s�����߂̎菇
 *				 MultiMediaTimerCallback�N���X����h��
 *
 *  Created: 2007/05/12(Sat)
 *  Updated: 2007/05/191(Sat)
 *
 *  Copyright (C) Tokyo Institute of Technology Hirose Lab.
 */

 //  20200820  URG�֘A�R�����g�A�E�g

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include "TimedMotionProcedure.h"
#include "..\WinUserMessage.h"

namespace System
{

/**
 *	----------------------------------------------------------------------
 *		TimedMotionProcedure�N���X
 *	----------------------------------------------------------------------
 */

/**
 *	------------------------------------------------------------
 *		TimedMotionProcedure�N���X�̃����o�֐���`
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	�R���X�g���N�^�ƃf�X�g���N�^
 *	----------------------------------------
 */
/// �R���X�g���N�^
TimedMotionProcedure::TimedMotionProcedure()
{
	targetThreadID = 0;
	//URGtargetThreadID = 0;  20200820
}

/// �f�X�g���N�^
TimedMotionProcedure::~TimedMotionProcedure()
{
}

/**
 *	����
 *		�Ăяo����̃X���b�h��ID��ݒ�
 */
void TimedMotionProcedure::setThreadID(DWORD id)
{
	if ( id != 0 )
		targetThreadID = id;

	return;
}
/**
 *	����
 *		�Ăяo����̃X���b�h��ID��ݒ�		//URG�p
 */
/*  20200820
void TimedMotionProcedure::URGsetThreadID(DWORD id)
{
	if ( id != 0 )
		URGtargetThreadID = id;

	return;
}
*/

/**
 *	����
 *		�^�C�}�R�[���o�b�N�J�n���ɌĂяo�����
 */
void TimedMotionProcedure::onStart(void)
{
	return;
}

/**
 *	����
 *		�^�C�}�R�[���o�b�N�I�����ɌĂяo�����
 */
void TimedMotionProcedure::onStop(void)
{
	return;
}

/**
 *	����
 *		�I�[�o�[���C�h�K�{�̃^�C�}���[�`��
 *		�I�����ɂ�false��Ԃ�
 */
bool TimedMotionProcedure::onMultiMediaTimer(void)
{
	///200ms���J�E���g����t���O
	//static int URG_200ms_Flag=0;  20200820

	if ( targetThreadID != 0 )
	{
		/// �w�肵���X���b�h�փ��b�Z�[�W�ʒm
		::PostThreadMessage(targetThreadID, WM_PLAN_GENERATE_MOTION, NULL, NULL);
	}

	///URG�ɂ�鑪�����s��
	/*  20200820
	if ( URGtargetThreadID != 0 )
	{
		if(URG_200ms_Flag == 10)
		{
			::PostThreadMessage(URGtargetThreadID, WM_URG_GENERATE_MOTION, NULL, NULL);
			URG_200ms_Flag=0;
		}
		URG_200ms_Flag=URG_200ms_Flag+1;
	}
	*/

	return true;
}

}	/// end of namespace System;