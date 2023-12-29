/**
 *  ファイル名
 *		InputDevice.cpp
 *  説明
 *		入力デバイスの処理クラス
 *  日付
 *		作成日: 2008/03/10(MON)		更新日: 2008/03/10(MON)
 */

/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */
#include "InputDevice.h"

/**
 *	----------------------------------------------------------------------
 *		InputDeviceクラス
 *	----------------------------------------------------------------------
 */

/**
 *	------------------------------------------------------------
 *		InputDeviceクラスのメンバ関数定義
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	コンストラクタとデストラクタ
 *	----------------------------------------
 */
InputDevice::InputDevice(LPDIRECTINPUTDEVICE8 pDIDevice)
{
	DEBUG_ASSERT( pDIDevice != NULL );
	
	lpDirectInputDevice = pDIDevice;
	
	/// 参照カウンタインクリメント
	lpDirectInputDevice->AddRef();

	/// エフェクトインターフェイスの初期化
	lpInputEffect = new NullInputEffect();
}

/**
 *		デストラクタ
 */
InputDevice::~InputDevice()
{
	lpDirectInputDevice->Unacquire();
	lpDirectInputDevice->Release();

	delete lpInputEffect;
}

/**
 *	説明
 *		デバイスバッファの取得
 *	引数
 *		outBufData: デバイスバッファを格納するDIDEVICEOBJECTDATA構造体の配列
 *		elementNum: 配列の要素数
 *	戻り値
 *		バッファの取得数
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
 *	説明
 *		デバイスバッファの情報を消去
 */
void InputDevice::flushBuffer(void)
{
	DWORD items = INFINITE;
	lpDirectInputDevice->GetDeviceData( sizeof(DIDEVICEOBJECTDATA), NULL, &items, 0 );
	return;
}

/**
 *	説明
 *		デバイス状態を取得する
 *	引数
 *		size: 格納する領域（2つ目の引数）のサイズ
 *		lpState: デバイス状態を格納する領域のアドレス
 *	戻り値
 *		true: 取得成功
 *		false: それ以外
 */
bool InputDevice::getState(DWORD size, LPVOID lpState)
{
	HRESULT hResult;

	hResult = lpDirectInputDevice->GetDeviceState(size, lpState);
	if ( hResult == DI_OK )
		return true;

	DEBUG_TRACE( TEXT("Error: [InputDevice::getState] DI8 GetDeviceState() failed\n") );
	::ZeroMemory(lpState, size);
		
	/// アクセス権の再取得
	if ( hResult == DIERR_INPUTLOST || hResult == DIERR_NOTACQUIRED )
		lpDirectInputDevice->Acquire();

	return false;
}

/**
 *	----------------------------------------
 *	DirectInputEffect関連
 *	----------------------------------------
 */
/**
 *	説明
 *		エフェクトの生成
 *	引数
 *		magnitude: 振動の強さ
 *		period: エフェクトの間隔
 *	戻り値
 *		true: 成功
 *		false: 失敗
 */
bool InputDevice::createMagnitudeEffect(const DWORD magnitude, const double period)
{

/**	
 *		エフェクト周期設定
 */
	DIPERIODIC diprd;
	::ZeroMemory(&diprd, sizeof(diprd));
	diprd.dwMagnitude	= magnitude;
	diprd.lOffset			= 0;
	diprd.dwPhase			= 0;
	diprd.dwPeriod			= (DWORD)( period * DI_SECONDS );

/**
 *		エフェクト設定
 */
	DWORD	axes[]		= {DIJOFS_X, DIJOFS_Y};		/// エフェクト軸
	LONG		Direction[]	= {0, 0};								/// エフェクト方向

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
 *		エフェクトを生成する
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
 *		エフェクトオブジェクト生成
 */
	delete lpInputEffect;
	lpInputEffect = new InputEffect(pDIEffect);
	pDIEffect->Release();

	return true;


}


/**
 *	説明
 *		エフェクトを解放する
 */
void InputDevice::releaseEffect(void)
{
	/// Nullオブジェクトの場合は解放しない
	if ( lpInputEffect->isNull() ) return;

	/// エフェクトを解放し、Nullオブジェクトを設定
	delete lpInputEffect;
	lpInputEffect = new NullInputEffect();
}

