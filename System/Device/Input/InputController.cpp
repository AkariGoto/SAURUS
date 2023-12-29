/**
 *  ファイル名
 *		InputController.cpp
 *  説明
 *		DirectInputによる入力の処理クラス
 *  日付
 *		作成日: 2008/02/05(MON)		更新日: 2008/02/19(MON)
 */ 
 
/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */
#include "InputController.h"
#include "..\..\ComLib.h"
#include "..\..\..\Utility\SafeOperation.h"
#include "JoyPadConst.h"

/**
 *	----------------------------------------------------------------------
 *		InputControllerクラス
 *	----------------------------------------------------------------------
 */

/**
 *	------------------------------------------------------------
 *		InputControllerクラスのメンバ関数定義
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	コンストラクタとデストラクタ
 *	----------------------------------------
 */
InputController::InputController() : lpDirectInput(NULL), windowHandle(NULL)
{
	/// COMライブラリの初期化	
	ComLib::initialize();
}

InputController::~InputController()
{
	release();
	
	/// COMライブラリの解放
	ComLib::uninitialize();	
}

/**
 *	----------------------------------------
 *	DirectInput関連
 *	----------------------------------------
 */
/**
 *	説明
 *		初期化
 *		DirectInputを初期化してInputControllerを使用可能にする
 */
bool InputController::initialize(const HWND hWnd)
{
	HINSTANCE hInstance;
	HRESULT hResult;
	
	release();
	
	/**
	 *		ウィンドウハンドルの保存
	 *		デバイスオブジェクトを生成するときに使用
	 */
	if ( hWnd == NULL )
		return false;
	else
		windowHandle = hWnd;

	/**
	 *		DirectInput8の生成とインターフェイスの取得
	 *
	 *		DirectInput8Createは生成と初期化を行うのだが、
	 *		該当するバージョンが存在しないと強制終了してしまうのでCoCreateInstanceを使用する
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
 *	説明
 *		リソースの解放
 *		DirectInputとInputControllerが使用していたリソース
 */
void InputController::release(void)
{
/**
 *		デバイスの解放
 */
	/// JoyPadの解放
	releaseJoyPad();

	windowHandle = NULL;
	
	/// DirectInput8の解放
	SAFE_RELEASE(lpDirectInput);
}

/**
 *	----------------------------------------
 *	デバイスのアクセス権
 *	----------------------------------------
 */
/// アクセス権の取得
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

/// アクセス権の解放
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
 *	デバイスの更新
 *	----------------------------------------
 */
/// デバイスバッファの更新
HRESULT InputController::updateDeviceData(void)
{
	return S_OK;
}

/// デバイス状態の更新
HRESULT InputController::updateDeviceState(void)
{
	bool joyResult = updateJoyPadState() == S_OK;

	if (joyResult) return S_OK;
	else return E_FAIL;
}

/**
 *	----------------------------------------
 *	JoyPad関連
 *	----------------------------------------
 */
/**
 *	説明
 *		JoyPadデバイス生成
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

	/// ジョイパッドの数に合わせ、ArrayListの最適化
	joyPads.setCapacity(joyPadNum);
	joyPadStates.setCapacity(joyPadNum);

	/// JoyPadを列挙し、デバイスオブジェクトを生成する
	lpDirectInput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)enumJoyPadProcedure, this, DIEDFL_ATTACHEDONLY);

	/// 生成できたJoyPadのカウント
	int i;
	int joyCounter = 0;
	for (i=0; i<joyPads.getCount(); i++)
	{
		joyCounter++;
	}

	/// 不足したArrayListにNULLオブジェクトを設定
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
 *	説明
 *		JoyPadの解放を行う
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
 *	説明
 *		JoyPadの初期化
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
	
	HRESULT hResult;		/// DirectXの関数結果

	/// デバイスオブジェクト作成
	LPDIRECTINPUTDEVICE8 pDIDevice;
	hResult = lpDirectInput->CreateDevice(pDevInst->guidInstance, &pDIDevice, NULL);
	if ( hResult != DI_OK )
	{
		DEBUG_TRACE( TEXT("Error: [InputController::initializeJoyPad] DI8 CreateDevice() failed\n") );
		return DIENUM_CONTINUE;
	}

	/// デバイスの能力取得
	DIDEVCAPS diDevCaps;
	diDevCaps.dwSize = sizeof(DIDEVCAPS);
	hResult = pDIDevice->GetCapabilities( &diDevCaps );
	if( hResult != DI_OK )
	{
        // ジョイスティック能力の取得に失敗
		DEBUG_TRACE( TEXT("Error: [InputController::initializeJoyPad] DI8 GetCapabilities() failed\n") );
		pDIDevice->Release();
        return DIENUM_CONTINUE;
    }


	/// データフォーマット設定
	hResult = pDIDevice->SetDataFormat(&c_dfDIJoystick2);
	if ( hResult != DI_OK )
	{
		DEBUG_TRACE( TEXT("Error: [InputController::initializeJoyPad] DI8 SetDataFormat() failed\n") );
		pDIDevice->Release();
		return DIENUM_CONTINUE;
	}

	/// 協調レベル設定
	hResult = pDIDevice->SetCooperativeLevel(windowHandle, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if ( hResult != DI_OK )
	{
		DEBUG_TRACE( TEXT("Error: [InputController::initializeJoyPad] DI8 SetCooperativeLevel() failed\n") );
		pDIDevice->Release();
		return DIENUM_CONTINUE;
	}

	/// DIPROPDWORD構造体の設定
	DIPROPDWORD dipd;
	::ZeroMemory(&dipd, sizeof(dipd));
	dipd.diph.dwSize		= sizeof(dipd);
	dipd.diph.dwHeaderSize	= sizeof(dipd.diph);
	dipd.diph.dwObj			= 0;
	dipd.diph.dwHow			= DIPH_DEVICE;
	dipd.dwData				= DIPROPAXISMODE_ABS;

	/// バッファサイズ設定
	hResult = pDIDevice->SetProperty(DIPROP_AXISMODE, &dipd.diph);
	if ( DX_FAILED(hResult) )
	{
		DEBUG_TRACE( TEXT("Error: [InputController::initializeJoyPad] DI8 SetProperty() failed\n") );
		pDIDevice->Release();
		return DIENUM_CONTINUE;
	}

	/// オブジェクトを列挙しプロパティを設定
	DIENUMOBJARGS		args = {this, pDIDevice};	/// 構造体の2番目の変数を用いる
	hResult = pDIDevice->EnumObjects( (LPDIENUMDEVICEOBJECTSCALLBACK)enumJoyPadObjectProcedure, &args, DIDFT_AXIS );
	if ( DX_FAILED(hResult) )
	{
		pDIDevice->Release();
		return DIENUM_CONTINUE;
	}

	/// アクセス権取得
	pDIDevice->Acquire();

	/// JoyPadのArrayListへの追加
	joyPads.add( new JoyPad(pDIDevice) );
	joyPadStates.add ( new JoyPadState() );

	pDIDevice->Release();

	return DIENUM_CONTINUE;

}

/// JoyPadオブジェクト設定
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

	/// 軸範囲設定
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

	/// デッドゾーン設定
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
 *		InputControllerクラスのprivateなメンバ関数
 *	------------------------------------------------------------
 */
void InputController::releaseDevice(InputDeviceInterface* pDevice)
{
	/// pDeviceがNULLまたはNullオブジェクトの場合は解放しない
	if ( pDevice == NULL || pDevice->isNull() )
		return;
	
	/// デバイスを解放し、NULLオブジェクトを設定
	delete pDevice;
	pDevice = new NullInputDevice();

}

/**
 *	----------------------------------------
 *	JoyPad関連
 *	----------------------------------------
 */
/// JoyPadのバッファ更新
HRESULT InputController::updateJoyPadData(void)
{
	return S_OK;
}

/// JoyPad状態更新
HRESULT InputController::updateJoyPadState(void)
{
	DIJOYSTATE2 dijs;	/// JoyPadの状態が格納されるDIJOYSTATE構造体
	::ZeroMemory(&dijs, sizeof(DIJOYSTATE2));
	bool result;				/// 関数の結果
	int i, j;					/// カウンタ

	if ( joyPads.getCount() < 0 ||  joyPads.getCount() == joyPadStates.getCount() )

	for (i=0; i< joyPads.getCount(); i++)
	{
		/// デバイスの状態を更新
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
			/// エラーなら入力無し
			joyPadStates[i]->clear();
		}
	}
	return S_OK;
}


/**
 *	----------------------------------------
 *	コールバック関数
 *	----------------------------------------
 */
/**
 *		JoyPad列挙コールバック
 */
BOOL CALLBACK InputController::enumJoyPadProcedure(LPDIDEVICEINSTANCE pDevInst, LPVOID userData)
{
	return ( (InputController*)userData )->initializeJoyPad(pDevInst, userData);
}

/**
 *		JoyPad軸列挙コールバック
 */
BOOL CALLBACK InputController::enumJoyPadObjectProcedure(LPDIDEVICEOBJECTINSTANCE pDevObjInst, LPVOID userData)
{
	return ( (DIENUMOBJARGS*)userData )->ptrInputController->setJoyPadObjectProp(pDevObjInst, userData);
}