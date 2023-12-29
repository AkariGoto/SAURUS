/**
 *  ファイル名
 *		JoyPad.h
 *  説明
 *		ジョイパッドクラス
 *  日付
 *		作成日: 2008/03/10(MON)		更新日: 2008/03/10(MON)
 */

#ifndef __JoyPad_h__
#define __JoyPad_h__

/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */
#include "InputDevice.h"

/**
 *	----------------------------------------------------------------------
 *		JoyPadクラス
 *	----------------------------------------------------------------------
 */
class JoyPad : public InputDevice
{
public:

/**
 *	------------------------------------------------------------
 *		メンバ変数
 *	------------------------------------------------------------
 */
  
/**
 *	------------------------------------------------------------
 *		メンバ関数
 *	------------------------------------------------------------
 */
public:
/**
 *	----------------------------------------
 *	コンストラクタとデストラクタ
 *	----------------------------------------
 */
	JoyPad(LPDIRECTINPUTDEVICE8 pDIDevice);
	virtual ~JoyPad();

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
	virtual DWORD getBuffer(DIDEVICEOBJECTDATA* outDIData, const DWORD elementNum = DIDEV_BUFSIZE);
	
	/**
	 *		デバイスバッファの情報を消去
	 */
	virtual void flushBuffer(void);

	/**
	 *		デバイス状態を取得する
	 *		引数
	 *			size: 格納する領域（2つ目の引数）のサイズ
	 *			lpState: デバイス状態を格納する領域のアドレス
	 *		戻り値
	 *		true: 取得成功
	 *			false: それ以外
	 */
	virtual bool getState(LPVOID lpState);

};

#endif	/// __JoyPad_h__