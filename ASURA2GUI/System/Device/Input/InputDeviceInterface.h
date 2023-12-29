/**
 *  �t�@�C����
 *		InputDeviceInterface.h
 *  ����
 *		DirectInput��p�����f�o�C�X�̏����N���X
 *  ���t
 *		�쐬��: 2008/03/10(MON)		�X�V��: 2008/03/10(MON)
 */

#ifndef __InputDeviceInterface_h__
#define __InputDeviceInterface_h__


/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include "..\..\BasicDirectX.h"
#include "InputEffect.h"

/**
 *	----------------------------------------------------------------------
 *		�}�N���A�^�錾
 *	----------------------------------------------------------------------
 */
/// �o�b�t�@�T�C�Y
static const int DIDEV_BUFSIZE = 16;

/**
 *	----------------------------------------------------------------------
 *		InputDeviceInterface�N���X
 *	----------------------------------------------------------------------
 */
class InputDeviceInterface
{

public:

	
/**
 *	------------------------------------------------------------
 *		�����o�ϐ�
 *	------------------------------------------------------------
 */
public:
	

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
	InputDeviceInterface() {}
	virtual ~InputDeviceInterface() {}

/**
 *	----------------------------------------
 *	�I�[�o�[���C�h�֐�
 *	----------------------------------------
 */
	/**
	 *	����
	 *		�I�u�W�F�N�g��Null�I�u�W�F�N�g���ǂ����𒲂ׂ�
	 */
	virtual bool isNull() const = 0;

	/**
	 *	����
	 *		�f�o�C�X�o�b�t�@�̎擾
	 *	����
	 *		outBufData: �f�o�C�X�o�b�t�@���i�[����DIDEVICEOBJECTDATA�\���̂̔z��
	 *		elementNum: �z��̗v�f��
	 *	�߂�l
	 *		�o�b�t�@�̎擾��
	 */
	virtual DWORD getBuffer(DIDEVICEOBJECTDATA* outBufData, const DWORD elementNum = DIDEV_BUFSIZE) = 0;
	
	/**
	 *	����
	 *		�f�o�C�X�o�b�t�@�̏�������
	 */
	virtual void flushBuffer(void) = 0;	

	/**
	 *	����
	 *		�f�o�C�X�̃A�N�Z�X���̎擾
	 */
	virtual void acquire() = 0;

	/**
	 *		�f�o�C�X�̃A�N�Z�X���̉��
	 */
	virtual void unacquire() = 0;

	/**
	 *	����
	 *		�f�o�C�X��Ԃ��擾����
	 *	����
	 *		lpState: �f�o�C�X��Ԃ��i�[����̈�̃A�h���X
	 *	�߂�l
	 *		true: �擾����
	 *		false: ����ȊO
	 */
	virtual bool getState(LPVOID lpState) = 0;

/**
 *	----------------------------------------
 *	DirectInputEffect�֘A
 *	----------------------------------------
 */
	/**
	 *	����
	 *		InputEffect�N���X�ւ̃C���^�[�t�F�C�X���擾
	 */
	virtual InputEffectInterface* getEffect() = 0;

	/**
	 *	����
	 *		�G�t�F�N�g�̐���
	 *	����
	 *		magnitude: �U���̋���
	 *		period: �G�t�F�N�g�̊Ԋu
	 *	�߂�l
	 *		true: ����
	 *		false: ���s
	 */
	virtual bool createMagnitudeEffect(const DWORD magnitude, const double period) = 0;

	/**
	 *	����
	 *		�G�t�F�N�g�̉��
	 */
	virtual void releaseEffect(void) = 0;
};

#endif	/// __InputDeviceInterface_h__