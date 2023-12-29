/**
 *  �t�@�C����
 *		InputDevice.cpp
 *  ����
 *		���̓f�o�C�X�̏����N���X
 *  ���t
 *		�쐬��: 2008/03/10(MON)		�X�V��: 2008/03/10(MON)
 */

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include "InputDevice.h"

/**
 *	----------------------------------------------------------------------
 *		InputDevice�N���X
 *	----------------------------------------------------------------------
 */

/**
 *	------------------------------------------------------------
 *		InputDevice�N���X�̃����o�֐���`
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	�R���X�g���N�^�ƃf�X�g���N�^
 *	----------------------------------------
 */
InputDevice::InputDevice(LPDIRECTINPUTDEVICE8 pDIDevice)
{
	DEBUG_ASSERT( pDIDevice != NULL );
	
	lpDirectInputDevice = pDIDevice;
	
	/// �Q�ƃJ�E���^�C���N�������g
	lpDirectInputDevice->AddRef();

	/// �G�t�F�N�g�C���^�[�t�F�C�X�̏�����
	lpInputEffect = new NullInputEffect();
}

/**
 *		�f�X�g���N�^
 */
InputDevice::~InputDevice()
{
	lpDirectInputDevice->Unacquire();
	lpDirectInputDevice->Release();

	delete lpInputEffect;
}

/**
 *	����
 *		�f�o�C�X�o�b�t�@�̎擾
 *	����
 *		outBufData: �f�o�C�X�o�b�t�@���i�[����DIDEVICEOBJECTDATA�\���̂̔z��
 *		elementNum: �z��̗v�f��
 *	�߂�l
 *		�o�b�t�@�̎擾��
 */
DWORD InputDevice::getBuffer(DIDEVICEOBJECTDATA* outDIData, const DWORD elementNum)
{
	DWORD items = elementNum;
	HRESULT hResult;

	hResult = lpDirectInputDevice->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), outDIData, &items, 0);
	if ( DX_FAILED( hResult ) )
	{
		DEBUG_TRACE( TEXT("Error: [InputDevice::getBuffer] DI8 GetDeviceData() failed\n") );
		::ZeroMemory(outDIData, sizeof(DIDEVICEOBJECTDATA)*items );
		return 0;
	}

	return items;
}

/**
 *	����
 *		�f�o�C�X�o�b�t�@�̏�������
 */
void InputDevice::flushBuffer(void)
{
	DWORD items = INFINITE;
	lpDirectInputDevice->GetDeviceData( sizeof(DIDEVICEOBJECTDATA), NULL, &items, 0 );
	return;
}

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
bool InputDevice::getState(DWORD size, LPVOID lpState)
{
	HRESULT hResult;

	hResult = lpDirectInputDevice->GetDeviceState(size, lpState);
	if ( hResult == DI_OK )
		return true;

	DEBUG_TRACE( TEXT("Error: [InputDevice::getState] DI8 GetDeviceState() failed\n") );
	::ZeroMemory(lpState, size);
		
	/// �A�N�Z�X���̍Ď擾
	if ( hResult == DIERR_INPUTLOST || hResult == DIERR_NOTACQUIRED )
		lpDirectInputDevice->Acquire();

	return false;
}

/**
 *	----------------------------------------
 *	DirectInputEffect�֘A
 *	----------------------------------------
 */
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
bool InputDevice::createMagnitudeEffect(const DWORD magnitude, const double period)
{

/**	
 *		�G�t�F�N�g�����ݒ�
 */
	DIPERIODIC diprd;
	::ZeroMemory(&diprd, sizeof(diprd));
	diprd.dwMagnitude	= magnitude;
	diprd.lOffset			= 0;
	diprd.dwPhase			= 0;
	diprd.dwPeriod			= (DWORD)( period * DI_SECONDS );

/**
 *		�G�t�F�N�g�ݒ�
 */
	DWORD	axes[]		= {DIJOFS_X, DIJOFS_Y};		/// �G�t�F�N�g��
	LONG		Direction[]	= {0, 0};								/// �G�t�F�N�g����

	DIEFFECT dieft;
	::ZeroMemory(&dieft, sizeof(dieft));
	dieft.dwSize							= sizeof(dieft);
	dieft.dwFlags							= DIEFF_POLAR | DIEFF_OBJECTOFFSETS;
	dieft.dwDuration						= (DWORD)( period * DI_SECONDS );
	dieft.dwSamplePeriod					= 0;
	dieft.dwGain							= DI_FFNOMINALMAX;
	dieft.dwTriggerButton					= DIEB_NOTRIGGER;
	dieft.dwTriggerRepeatInterval			= 0;
	dieft.cAxes								= sizeof(axes) / sizeof(axes[0]);
	dieft.rgdwAxes							= axes;
	dieft.rglDirection						= Direction;
	dieft.lpEnvelope						= NULL;
	dieft.cbTypeSpecificParams				= sizeof(diprd);
	dieft.lpvTypeSpecificParams				= &diprd;

/**
 *		�G�t�F�N�g�𐶐�����
 */
	IDirectInputEffect* pDIEffect;
	HRESULT hResult;

	hResult = lpDirectInputDevice->CreateEffect(GUID_Square, &dieft, & pDIEffect, NULL);
	if ( hResult != DI_OK )
	{
		DEBUG_TRACE( TEXT("Error: [InputDevice::createMagnitudeEffect] DI8 CreateEffect() failed\n") );
		releaseEffect();
		return false;
	}

/**
 *		�G�t�F�N�g�I�u�W�F�N�g����
 */
	delete lpInputEffect;
	lpInputEffect = new InputEffect(pDIEffect);
	pDIEffect->Release();

	return true;


}


/**
 *	����
 *		�G�t�F�N�g���������
 */
void InputDevice::releaseEffect(void)
{
	/// Null�I�u�W�F�N�g�̏ꍇ�͉�����Ȃ�
	if ( lpInputEffect->isNull() ) return;

	/// �G�t�F�N�g��������ANull�I�u�W�F�N�g��ݒ�
	delete lpInputEffect;
	lpInputEffect = new NullInputEffect();
}

