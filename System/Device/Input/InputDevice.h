/**
 *  �t�@�C����
 *		InputDevice.h
 *  ����
 *		���̓f�o�C�X�̏����N���X
 *  ���t
 *		�쐬��: 2008/03/10(MON)		�X�V��: 2008/03/10(MON)
 */

#ifndef __InputDevice_h__
#define __InputDevice_h__

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include "InputDeviceInterface.h"
 
/**
 *	----------------------------------------------------------------------
 *		InputDevice�N���X
 *	----------------------------------------------------------------------
 */
class InputDevice : public InputDeviceInterface
{
/**
 *	------------------------------------------------------------
 *		�����o�ϐ�
 *	------------------------------------------------------------
 */
public:

protected:
/**
 *		�f�o�C�X�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
 */
	LPDIRECTINPUTDEVICE8 lpDirectInputDevice;

/**
 *		�f�o�C�X�G�t�F�N�g�N���X�ւ̃C���^�[�t�F�C�X
 */
	InputEffectInterface* lpInputEffect;

private:

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
	InputDevice(LPDIRECTINPUTDEVICE8 pDIDevice);
	virtual ~InputDevice();

/**
 *	----------------------------------------
 *	�I�[�o�[���C�h�֐�
 *	----------------------------------------
 */
	/**
	 *	����
 	 *		�I�u�W�F�N�g��Null�I�u�W�F�N�g���ǂ����𒲂ׂ�
	 */
	virtual bool isNull() const {return false;}


	/**
	 *	����
	 *		�f�o�C�X�o�b�t�@�̎擾
	 *	����
	 *		outBufData: �f�o�C�X�o�b�t�@���i�[����DIDEVICEOBJECTDATA�\���̂̔z��
	 *		elementNum: �z��̗v�f��
	 *	�߂�l
	 *		�o�b�t�@�̎擾��
	 */
	virtual DWORD getBuffer(DIDEVICEOBJECTDATA* outDIData, const DWORD elementNum);
	
	/**
	 *	����
	 *		�f�o�C�X�o�b�t�@�̏�������
	 */
	virtual void flushBuffer(void);

	/**
	 *	����
	 *		�f�o�C�X�̃A�N�Z�X���̎擾
	 */
	virtual void acquire() { lpDirectInputDevice->Acquire(); }

	/**
	 *	����
	 *		�f�o�C�X�̃A�N�Z�X���̉��
	 */
	virtual void unacquire() {lpDirectInputDevice->Unacquire();}

protected:
	/**
	 *	����
	 *		�f�o�C�X��Ԃ��擾����
	 *	����
	 *		size: �i�[����̈�i2�ڂ̈����j�̃T�C�Y
	 *		lpState: �f�o�C�X��Ԃ��i�[����̈�̃A�h���X
	 *	�߂�l
	 *		true: �擾����
	 *		false: ����ȊO
	 */
	virtual bool getState(DWORD size, LPVOID lpState);

public:
/**
 *	----------------------------------------
 *	DirectInputEffect�֘A
 *	----------------------------------------
 */
	/**
	 *	����
	 *		InputEffect�N���X�ւ̃C���^�[�t�F�C�X���擾
	 */
	virtual InputEffectInterface* getEffect() { return lpInputEffect; }

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
	virtual bool createMagnitudeEffect(const DWORD magnitude, const double period);

	/**
	 *	����
	 *		�G�t�F�N�g�̉��
	 */
	virtual void releaseEffect(void);
};

/**
 *	----------------------------------------------------------------------
 *		NullInputDevice�N���X
 *			Null Object�p�^�[��
 *			null�̑���ɉ��̏������s��Ȃ��I�u�W�F�N�g
 *	----------------------------------------------------------------------
 */
class NullInputDevice : public InputDeviceInterface
{
/**
 *	------------------------------------------------------------
 *		�����o�ϐ�
 *	------------------------------------------------------------
 */
public:
	NullInputEffect nullInputEffect;

/**
 *	------------------------------------------------------------
 *		�����o�֐�
 *	------------------------------------------------------------
 */
public:

/**
 *		�R���X�g���N�^�ƃf�X�g���N�^
 */
	NullInputDevice() {}
	virtual ~NullInputDevice() {}

/**
 *	------------------------------------------------------------
 *		�I�[�o�[���C�h�֐�
 *	------------------------------------------------------------
 */
	/**
	 *	����
	 *		�I�u�W�F�N�g��Null�I�u�W�F�N�g���ǂ����𒲂ׂ�
	 */
	virtual bool isNull() const{return true;}

	/**
	 *	����
	 *		�f�o�C�X�o�b�t�@�̎擾
	 *	����
	 *		outBufData: �f�o�C�X�o�b�t�@���i�[����DIDEVICEOBJECTDATA�\���̂̔z��
	 *		elementNum: �z��̗v�f��
	 *	�߂�l
	 *		�o�b�t�@�̎擾��
	 */
	virtual DWORD getBuffer(DIDEVICEOBJECTDATA* outBufData, const DWORD elementNum) {return 0;}

	/**
	 *	����
	 *		�f�o�C�X�o�b�t�@�̏�������
	 */
	virtual void flushBuffer() {return;}

	/**
	 *	����
	 *		�f�o�C�X�̃A�N�Z�X���̎擾
	 */
	virtual void acquire() { return; }

	/**
	 *	����
	 *		�f�o�C�X�̃A�N�Z�X���̉��
	 */
	virtual void unacquire() { return; }

	/**
	 *	����
	 *		�f�o�C�X��Ԃ��擾����
	 *	����
	 *		lpState: �f�o�C�X��Ԃ��i�[����̈�̃A�h���X
	 *	�߂�l
	 *		true: �擾����
	 *		false: ����ȊO
	 */
	virtual bool getState(LPVOID lpState) {return true;}

/**
 *	----------------------------------------
 *	DirectInputEffect�֘A
 *	----------------------------------------
 */
	/**
	 *	����
	 *		InputEffect�N���X�ւ̃C���^�[�t�F�C�X���擾
	 */
	virtual InputEffectInterface* getEffect() { return &nullInputEffect; }

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
	virtual bool createMagnitudeEffect(const DWORD magnitude, const double period) {return true;}

	/**
	 *	����
	 *		�G�t�F�N�g�̉��
	 */
	virtual void releaseEffect(void) {return;}
};

#endif	/// __InputDevice_h__