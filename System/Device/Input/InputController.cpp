/**
 *  �t�@�C����
 *		InputController.cpp
 *  ����
 *		DirectInput�ɂ����͂̏����N���X
 *  ���t
 *		�쐬��: 2008/02/05(MON)		�X�V��: 2008/02/19(MON)
 */ 
 
/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include "InputController.h"
#include "..\..\ComLib.h"
#include "..\..\..\Utility\SafeOperation.h"
#include "JoyPadConst.h"

/**
 *	----------------------------------------------------------------------
 *		InputController�N���X
 *	----------------------------------------------------------------------
 */

/**
 *	------------------------------------------------------------
 *		InputController�N���X�̃����o�֐���`
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	�R���X�g���N�^�ƃf�X�g���N�^
 *	----------------------------------------
 */
InputController::InputController() : lpDirectInput(NULL), windowHandle(NULL)
{
	/// COM���C�u�����̏�����	
	ComLib::initialize();
}

InputController::~InputController()
{
	release();
	
	/// COM���C�u�����̉��
	ComLib::uninitialize();	
}

/**
 *	----------------------------------------
 *	DirectInput�֘A
 *	----------------------------------------
 */
/**
 *	����
 *		������
 *		DirectInput������������InputController���g�p�\�ɂ���
 */
bool InputController::initialize(const HWND hWnd)
{
	HINSTANCE hInstance;
	HRESULT hResult;
	
	release();
	
	/**
	 *		�E�B���h�E�n���h���̕ۑ�
	 *		�f�o�C�X�I�u�W�F�N�g�𐶐�����Ƃ��Ɏg�p
	 */
	if ( hWnd == NULL )
		return false;
	else
		windowHandle = hWnd;

	/**
	 *		DirectInput8�̐����ƃC���^�[�t�F�C�X�̎擾
	 *
	 *		DirectInput8Create�͐����Ə��������s���̂����A
	 *		�Y������o�[�W���������݂��Ȃ��Ƌ����I�����Ă��܂��̂�CoCreateInstance���g�p����
	 *		DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&lpDirectInput, NULL );
	 */
	hResult = CoCreateInstance(CLSID_DirectInput8, NULL, CLSCTX_INPROC_SERVER, IID_IDirectInput8, (LPVOID*)&lpDirectInput );
	if ( hResult != S_OK )
	{	
		DEBUG_TRACE( TEXT("Error: [InputController::initialize] CoCreateInstance failed\n") );
		 return false;
	}
	
	hInstance = (HINSTANCE)(LONG_PTR)::GetWindowLong(hWnd, GWL_HINSTANCE);
	hResult = lpDirectInput->Initialize(hInstance, DIRECTINPUT_VERSION);
	
	if( hResult != DI_OK ) 
	{
		DEBUG_TRACE( TEXT("Error: [InputController::initialize] DI8 initialize failed\n") );
		release();
		
		return false;
	}
	
	return true;
}


/**
 *	����
 *		���\�[�X�̉��
 *		DirectInput��InputController���g�p���Ă������\�[�X
 */
void InputController::release(void)
{
/**
 *		�f�o�C�X�̉��
 */
	/// JoyPad�̉��
	releaseJoyPad();

	windowHandle = NULL;
	
	/// DirectInput8�̉��
	SAFE_RELEASE(lpDirectInput);
}

/**
 *	----------------------------------------
 *	�f�o�C�X�̃A�N�Z�X��
 *	----------------------------------------
 */
/// �A�N�Z�X���̎擾
void InputController::acquire()
{
	if ( lpDirectInput == NULL )
	{
		DEBUG_TRACE( TEXT("Error: [InputController::createJoyPad] DI8 not initialized \n") );
		return;
	}

	int i;
	for (i=0; i<joyPads.getCount(); i++)
		joyPads[i]->acquire();
}

/// �A�N�Z�X���̉��
void InputController::unacquire()
{
	if ( lpDirectInput == NULL )
	{
		DEBUG_TRACE( TEXT("Error: [InputController::createJoyPad] DI8 not initialized \n") );
		return;
	}

	int i;
	for (i=0; i<joyPads.getCount(); i++)
		joyPads[i]->unacquire();
}

/**
 *	----------------------------------------
 *	�f�o�C�X�̍X�V
 *	----------------------------------------
 */
/// �f�o�C�X�o�b�t�@�̍X�V
HRESULT InputController::updateDeviceData(void)
{
	return S_OK;
}

/// �f�o�C�X��Ԃ̍X�V
HRESULT InputController::updateDeviceState(void)
{
	bool joyResult = updateJoyPadState() == S_OK;

	if (joyResult) return S_OK;
	else return E_FAIL;
}

/**
 *	----------------------------------------
 *	JoyPad�֘A
 *	----------------------------------------
 */
/**
 *	����
 *		JoyPad�f�o�C�X����
 */
DWORD InputController::createJoyPad(const int joyPadNum)
{
	if ( lpDirectInput == NULL )
	{
		DEBUG_TRACE( TEXT("Error: [InputController::createJoyPad] DI8 not initialized \n") );
		return 0;
	}

	releaseJoyPad();

	if ( joyPadNum == 0 )
		return 0;

	/// �W���C�p�b�h�̐��ɍ��킹�AArrayList�̍œK��
	joyPads.setCapacity(joyPadNum);
	joyPadStates.setCapacity(joyPadNum);

	/// JoyPad��񋓂��A�f�o�C�X�I�u�W�F�N�g�𐶐�����
	lpDirectInput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)enumJoyPadProcedure, this, DIEDFL_ATTACHEDONLY);

	/// �����ł���JoyPad�̃J�E���g
	int i;
	int joyCounter = 0;
	for (i=0; i<joyPads.getCount(); i++)
	{
		joyCounter++;
	}

	/// �s������ArrayList��NULL�I�u�W�F�N�g��ݒ�
	for (i=joyCounter; i<joyPadNum; i++)
	{
		if ( i != 0 ) { joyPads.remove(i); }
		joyPads.add( new NullInputDevice() );

		if ( i != 0 ) { joyPadStates.remove(i); }
		joyPadStates.add( new JoyPadState() );
	}

	return joyCounter;
}

/**
 *	����
 *		JoyPad�̉�����s��
 */
void InputController::releaseJoyPad(void)
{
	int i;
	for (i=0; i<joyPads.getCount(); i++)
		delete joyPads[i];

	for (i=0; i<joyPadStates.getCount(); i++)
		delete joyPadStates[i];

	joyPads.clear();
	joyPadStates.clear();
}

/**
 *	����
 *		JoyPad�̏�����
 */
BOOL InputController::initializeJoyPad(const LPDIDEVICEINSTANCE pDevInst, LPVOID userData)
{
#ifdef _DEBUG
	if ( lpDirectInput == NULL )
	{
		DEBUG_TRACE( TEXT("Error: [InputController::initializeJoyPad] DI8 not initialized \n") );
		return DIENUM_STOP;
	}
#endif
	
	HRESULT hResult;		/// DirectX�̊֐�����

	/// �f�o�C�X�I�u�W�F�N�g�쐬
	LPDIRECTINPUTDEVICE8 pDIDevice;
	hResult = lpDirectInput->CreateDevice(pDevInst->guidInstance, &pDIDevice, NULL);
	if ( hResult != DI_OK )
	{
		DEBUG_TRACE( TEXT("Error: [InputController::initializeJoyPad] DI8 CreateDevice() failed\n") );
		return DIENUM_CONTINUE;
	}

	/// �f�o�C�X�̔\�͎擾
	DIDEVCAPS diDevCaps;
	diDevCaps.dwSize = sizeof(DIDEVCAPS);
	hResult = pDIDevice->GetCapabilities( &diDevCaps );
	if( hResult != DI_OK )
	{
        // �W���C�X�e�B�b�N�\�͂̎擾�Ɏ��s
		DEBUG_TRACE( TEXT("Error: [InputController::initializeJoyPad] DI8 GetCapabilities() failed\n") );
		pDIDevice->Release();
        return DIENUM_CONTINUE;
    }


	/// �f�[�^�t�H�[�}�b�g�ݒ�
	hResult = pDIDevice->SetDataFormat(&c_dfDIJoystick2);
	if ( hResult != DI_OK )
	{
		DEBUG_TRACE( TEXT("Error: [InputController::initializeJoyPad] DI8 SetDataFormat() failed\n") );
		pDIDevice->Release();
		return DIENUM_CONTINUE;
	}

	/// �������x���ݒ�
	hResult = pDIDevice->SetCooperativeLevel(windowHandle, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if ( hResult != DI_OK )
	{
		DEBUG_TRACE( TEXT("Error: [InputController::initializeJoyPad] DI8 SetCooperativeLevel() failed\n") );
		pDIDevice->Release();
		return DIENUM_CONTINUE;
	}

	/// DIPROPDWORD�\���̂̐ݒ�
	DIPROPDWORD dipd;
	::ZeroMemory(&dipd, sizeof(dipd));
	dipd.diph.dwSize		= sizeof(dipd);
	dipd.diph.dwHeaderSize	= sizeof(dipd.diph);
	dipd.diph.dwObj			= 0;
	dipd.diph.dwHow			= DIPH_DEVICE;
	dipd.dwData				= DIPROPAXISMODE_ABS;

	/// �o�b�t�@�T�C�Y�ݒ�
	hResult = pDIDevice->SetProperty(DIPROP_AXISMODE, &dipd.diph);
	if ( DX_FAILED(hResult) )
	{
		DEBUG_TRACE( TEXT("Error: [InputController::initializeJoyPad] DI8 SetProperty() failed\n") );
		pDIDevice->Release();
		return DIENUM_CONTINUE;
	}

	/// �I�u�W�F�N�g��񋓂��v���p�e�B��ݒ�
	DIENUMOBJARGS		args = {this, pDIDevice};	/// �\���̂�2�Ԗڂ̕ϐ���p����
	hResult = pDIDevice->EnumObjects( (LPDIENUMDEVICEOBJECTSCALLBACK)enumJoyPadObjectProcedure, &args, DIDFT_AXIS );
	if ( DX_FAILED(hResult) )
	{
		pDIDevice->Release();
		return DIENUM_CONTINUE;
	}

	/// �A�N�Z�X���擾
	pDIDevice->Acquire();

	/// JoyPad��ArrayList�ւ̒ǉ�
	joyPads.add( new JoyPad(pDIDevice) );
	joyPadStates.add ( new JoyPadState() );

	pDIDevice->Release();

	return DIENUM_CONTINUE;

}

/// JoyPad�I�u�W�F�N�g�ݒ�
BOOL InputController::setJoyPadObjectProp(const LPDIDEVICEOBJECTINSTANCE pDevObjInst, LPVOID userData)
{
	HRESULT hResult;
	LPDIRECTINPUTDEVICE8 pDIDevice = ( (DIENUMOBJARGS*)userData )->ptrDIDevice;

#ifdef _DEBUG
	if (pDIDevice == NULL)
	{
		DEBUG_TRACE( TEXT("Error: [InputController::setJoyPadObjectProp] DI8 not initialized\n") );
		return DIENUM_STOP;
	}
#endif

	/// ���͈͐ݒ�
	if ( (pDevObjInst->dwType & DIDFT_AXIS) != 0 )
	{
		DIPROPRANGE dipr;
		::ZeroMemory(&dipr, sizeof(dipr));
		dipr.diph.dwSize		= sizeof(dipr);
		dipr.diph.dwHeaderSize	= sizeof(dipr.diph);
		dipr.diph.dwHow			= DIPH_BYID;
		dipr.diph.dwObj			= pDevObjInst->dwType;
		dipr.lMin				= DIJOY_MIN_AXIS_VALUE*DIJOY_AXIS_SCALE;
		dipr.lMax				= DIJOY_MAX_AXIS_VALUE*DIJOY_AXIS_SCALE;

		hResult = pDIDevice->SetProperty(DIPROP_RANGE, &dipr.diph);
		if ( DX_FAILED( hResult ) )
			DEBUG_TRACE( TEXT("Error: [InputController::setJoyPadObjectProp] DI8 SetProperty() failed\n") );
	}

	/// �f�b�h�]�[���ݒ�
	DIPROPDWORD dipdw;
	dipdw.diph.dwSize				= sizeof(dipdw);
	dipdw.diph.dwHeaderSize			= sizeof(dipdw.diph);
	dipdw.diph.dwHow				= DIPH_BYID;
	dipdw.diph.dwObj				= pDevObjInst->dwType;
	dipdw.dwData					= DIJOY_DEAD_ZONE;

	hResult = pDIDevice->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
	if ( DX_FAILED( hResult ) )
		DEBUG_TRACE( TEXT("Error: [InputController::setJoyPadObjectProp] DI8 SetProperty() failed\n") );

	return DIENUM_CONTINUE;
}



/**
 *	------------------------------------------------------------
 *		InputController�N���X��private�ȃ����o�֐�
 *	------------------------------------------------------------
 */
void InputController::releaseDevice(InputDeviceInterface* pDevice)
{
	/// pDevice��NULL�܂���Null�I�u�W�F�N�g�̏ꍇ�͉�����Ȃ�
	if ( pDevice == NULL || pDevice->isNull() )
		return;
	
	/// �f�o�C�X��������ANULL�I�u�W�F�N�g��ݒ�
	delete pDevice;
	pDevice = new NullInputDevice();

}

/**
 *	----------------------------------------
 *	JoyPad�֘A
 *	----------------------------------------
 */
/// JoyPad�̃o�b�t�@�X�V
HRESULT InputController::updateJoyPadData(void)
{
	return S_OK;
}

/// JoyPad��ԍX�V
HRESULT InputController::updateJoyPadState(void)
{
	DIJOYSTATE2 dijs;	/// JoyPad�̏�Ԃ��i�[�����DIJOYSTATE�\����
	::ZeroMemory(&dijs, sizeof(DIJOYSTATE2));
	bool result;				/// �֐��̌���
	int i, j;					/// �J�E���^

	if ( joyPads.getCount() < 0 ||  joyPads.getCount() == joyPadStates.getCount() )

	for (i=0; i< joyPads.getCount(); i++)
	{
		/// �f�o�C�X�̏�Ԃ��X�V
		result = joyPads[i]->getState(&dijs);

		/// 
		if ( result )
		{
			joyPadStates[i]->setAxisTransX(dijs.lX);
			joyPadStates[i]->setAxisTransY(dijs.lY);
			joyPadStates[i]->setAxisTransZ(dijs.lZ);
			joyPadStates[i]->setAxisRotX(dijs.lRx);
			joyPadStates[i]->setAxisRotY(dijs.lRy);
			joyPadStates[i]->setAxisRotZ(dijs.lRz);
			
			for (j=0; j<DIJOY_MAX_BUTTON_NUM; j++)
			{
				bool isPressed = ( dijs.rgbButtons[j] == 0x80);
				joyPadStates[i]->setButtonPressed(j, isPressed);
			}

		}
		else
		{
			/// �G���[�Ȃ���͖���
			joyPadStates[i]->clear();
		}
	}
	return S_OK;
}


/**
 *	----------------------------------------
 *	�R�[���o�b�N�֐�
 *	----------------------------------------
 */
/**
 *		JoyPad�񋓃R�[���o�b�N
 */
BOOL CALLBACK InputController::enumJoyPadProcedure(LPDIDEVICEINSTANCE pDevInst, LPVOID userData)
{
	return ( (InputController*)userData )->initializeJoyPad(pDevInst, userData);
}

/**
 *		JoyPad���񋓃R�[���o�b�N
 */
BOOL CALLBACK InputController::enumJoyPadObjectProcedure(LPDIDEVICEOBJECTINSTANCE pDevObjInst, LPVOID userData)
{
	return ( (DIENUMOBJARGS*)userData )->ptrInputController->setJoyPadObjectProp(pDevObjInst, userData);
}