/**
 *  �t�@�C����
 *		JoyPad.cpp
 *  ����
 *		�W���C�p�b�h�N���X
 *  ���t
 *		�쐬��: 2008/03/10(MON)		�X�V��: 2008/03/10(MON)
 */

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include "JoyPad.h"

/**
 *	----------------------------------------------------------------------
 *		InputEffect�N���X
 *	----------------------------------------------------------------------
 */

/**
 *	------------------------------------------------------------
 *		InputEffect�N���X�̃����o�֐���`
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	�R���X�g���N�^�ƃf�X�g���N�^
 *	----------------------------------------
 */
JoyPad::JoyPad(LPDIRECTINPUTDEVICE8 pDIDevice) : InputDevice(pDIDevice)
{
}


/**
 *	�f�X�g���N�^
 */
JoyPad::~JoyPad()
{
}

/**
 *	----------------------------------------
 *	�I�[�o�[���C�h�֐�
 *	----------------------------------------
 */
/**
 *		�f�o�C�X�o�b�t�@�̎擾
 *		����
 *			outBufData: �f�o�C�X�o�b�t�@���i�[����DIDEVICEOBJECTDATA�\���̂̔z��
 *			elementNum: �z��̗v�f��
 *		�߂�l
 *			�o�b�t�@�̎擾��
 */
DWORD JoyPad::getBuffer(DIDEVICEOBJECTDATA* outDIData, const DWORD elementNum)
{
	HRESULT hResult;
	
	hResult = lpDirectInputDevice->Poll();
	if ( DX_FAILED(hResult) )
	{
		lpDirectInputDevice->Acquire();
		return 0;
	}
	
	return InputDevice::getBuffer(outDIData, elementNum);
}
	
/**
 *		�f�o�C�X�o�b�t�@�̏�������
 */
void JoyPad::flushBuffer(void)
{
	HRESULT hResult;

	hResult = lpDirectInputDevice->Poll();
	if ( DX_FAILED(hResult) )
	{
		lpDirectInputDevice->Acquire();
		return;
	}
	return InputDevice::flushBuffer();
}

/**
 *		�f�o�C�X��Ԃ��擾����
 *		����
 *			size: �i�[����̈�i2�ڂ̈����j�̃T�C�Y
 *			lpState: �f�o�C�X��Ԃ��i�[����̈�̃A�h���X
 *		�߂�l
 *		true: �擾����
 *			false: ����ȊO
 */
bool JoyPad::getState(LPVOID lpState)
{
	HRESULT hResult;

	hResult = lpDirectInputDevice->Poll();
	if ( DX_FAILED(hResult) )
	{
		lpDirectInputDevice->Acquire();
		return false;
	}
	return InputDevice::getState(sizeof(DIJOYSTATE2), lpState);
}
