/**
 *  ファイル名
 *		JoyPad.cpp
 *  説明
 *		ジョイパッドクラス
 *  日付
 *		作成日: 2008/03/10(MON)		更新日: 2008/03/10(MON)
 */

/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */
#include "JoyPad.h"

/**
 *	----------------------------------------------------------------------
 *		InputEffectクラス
 *	----------------------------------------------------------------------
 */

/**
 *	------------------------------------------------------------
 *		InputEffectクラスのメンバ関数定義
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	コンストラクタとデストラクタ
 *	----------------------------------------
 */
JoyPad::JoyPad(LPDIRECTINPUTDEVICE8 pDIDevice) : InputDevice(pDIDevice)
{
}


/**
 *	デストラクタ
 */
JoyPad::~JoyPad()
{
}

/**
 *	----------------------------------------
 *	オーバーライド関数
 *	----------------------------------------
 */
/**
 *		デバイスバッファの取得
 *		引数
 *			outBufData: デバイスバッファを格納するDIDEVICEOBJECTDATA構造体の配列
 *			elementNum: 配列の要素数
 *		戻り値
 *			バッファの取得数
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
 *		デバイスバッファの情報を消去
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
 *		デバイス状態を取得する
 *		引数
 *			size: 格納する領域（2つ目の引数）のサイズ
 *			lpState: デバイス状態を格納する領域のアドレス
 *		戻り値
 *		true: 取得成功
 *			false: それ以外
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
