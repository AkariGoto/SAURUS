/**
 *  �t�@�C����
 *		TimedMotionProcedure.h
 *  ����
 *		�^�C�}�Ŏ����I�ɌĂ΂����e�v����s�����߂̎菇
 *		MultiMediaTimerCallback�N���X����h��
 *  ���t
 *		�쐬��: 2007/05/12(SAT)		�X�V��: 2007/05/19(SAT)
 */

//  20200820  URG�֘A�R�����g�A�E�g
 
#ifndef __TimedMotionProcedure_h__
#define __TimedMotionProcedure_h__

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include "..\Timer\MultiMediaTimer.h"

namespace System
{

/**
 *	----------------------------------------------------------------------
 *		TimedMotionProcedure�N���X
 *	----------------------------------------------------------------------
 */
class TimedMotionProcedure : public MultiMediaTimerCallback
{
/**
 *	------------------------------------------------------------
 *		�����o�ϐ�
 *	------------------------------------------------------------
 */
private:
/**
 *	�L���[������X���b�hID
 */
	DWORD targetThreadID;
	//DWORD URGtargetThreadID;		//URG�p  20200820

/**
 *	TimedMotionProcedure�I�u�W�F�N�g���Ăяo���^�C�}�ւ̃|�C���^
 */
	MultiMediaTimer* timerPointer;

/**
 *	------------------------------------------------------------
 *		�����o�֐�
 *	------------------------------------------------------------
 */
public:
/**
 *	----------------------------------------
 *	�R���X�g���N�^�ƃf�X�g���N�^
 *	----------------------------------------
 */
	/// �R���X�g���N�^
	TimedMotionProcedure();
	/// �f�X�g���N�^
	virtual ~TimedMotionProcedure();

/**
 *	----------------------------------------
 *	�^�C�}�I�u�W�F�N�g�̎擾�E���
 *	----------------------------------------
 */
	/// �擾
	void acquireTimer(MultiMediaTimer* timer);
	/// ���
	void releaseTimer(void);

/**
 *	----------------------------------------
 *	�X���b�hID�̎擾�E���
 *	----------------------------------------
 */
	/// �擾
	void	setThreadID(DWORD id);
	//void	URGsetThreadID(DWORD id);  20200820
	DWORD	getThreadID(void) const	{ return (targetThreadID); }

/**
 *		MultiMediaTimerCallback�N���X�̉��z�֐�
 *		�^�C�}�Ŏ��ۂɏ��������
 */
/**
 *	----------------------------------------
 *	MultiMediaTimerCallback�N���X�̃I�[�o�[���C�h�֐�
 *	�^�C�}�Ŏ��ۂɏ��������
 *	----------------------------------------
 */
	/// ���[�`���J�n���ɌĂяo������
	virtual void	onStart(void);
	/// ���[�`���I�����ɌĂяo������
	virtual void	onStop(void);
	/**
	 *	����
	 *		�I�[�o�[���C�h�K�{�̃^�C�}���[�`��
	 *		�I�����ɂ�false��Ԃ�
	 */
	virtual bool	onMultiMediaTimer(void);


};	/// end of class TimedMotionProcedure

}	/// end of namespace System;

#endif /// __TimedMotionProcedure_h__