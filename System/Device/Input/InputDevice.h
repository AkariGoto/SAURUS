/**
 *  ファイル名
 *		InputDevice.h
 *  説明
 *		入力デバイスの処理クラス
 *  日付
 *		作成日: 2008/03/10(MON)		更新日: 2008/03/10(MON)
 */

#ifndef __InputDevice_h__
#define __InputDevice_h__

/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */
#include "InputDeviceInterface.h"
 
/**
 *	----------------------------------------------------------------------
 *		InputDeviceクラス
 *	----------------------------------------------------------------------
 */
class InputDevice : public InputDeviceInterface
{
/**
 *	------------------------------------------------------------
 *		メンバ変数
 *	------------------------------------------------------------
 */
public:

protected:
/**
 *		デバイスオブジェクトへのインターフェイス
 */
	LPDIRECTINPUTDEVICE8 lpDirectInputDevice;

/**
 *		デバイスエフェクトクラスへのインターフェイス
 */
	InputEffectInterface* lpInputEffect;

private:

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
	InputDevice(LPDIRECTINPUTDEVICE8 pDIDevice);
	virtual ~InputDevice();

/**
 *	----------------------------------------
 *	オーバーライド関数
 *	----------------------------------------
 */
	/**
	 *	説明
 	 *		オブジェクトがNullオブジェクトかどうかを調べる
	 */
	virtual bool isNull() const {return false;}


	/**
	 *	説明
	 *		デバイスバッファの取得
	 *	引数
	 *		outBufData: デバイスバッファを格納するDIDEVICEOBJECTDATA構造体の配列
	 *		elementNum: 配列の要素数
	 *	戻り値
	 *		バッファの取得数
	 */
	virtual DWORD getBuffer(DIDEVICEOBJECTDATA* outDIData, const DWORD elementNum);
	
	/**
	 *	説明
	 *		デバイスバッファの情報を消去
	 */
	virtual void flushBuffer(void);

	/**
	 *	説明
	 *		デバイスのアクセス権の取得
	 */
	virtual void acquire() { lpDirectInputDevice->Acquire(); }

	/**
	 *	説明
	 *		デバイスのアクセス権の解放
	 */
	virtual void unacquire() {lpDirectInputDevice->Unacquire();}

protected:
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
	virtual bool getState(DWORD size, LPVOID lpState);

public:
/**
 *	----------------------------------------
 *	DirectInputEffect関連
 *	----------------------------------------
 */
	/**
	 *	説明
	 *		InputEffectクラスへのインターフェイスを取得
	 */
	virtual InputEffectInterface* getEffect() { return lpInputEffect; }

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
	virtual bool createMagnitudeEffect(const DWORD magnitude, const double period);

	/**
	 *	説明
	 *		エフェクトの解放
	 */
	virtual void releaseEffect(void);
};

/**
 *	----------------------------------------------------------------------
 *		NullInputDeviceクラス
 *			Null Objectパターン
 *			nullの代わりに何の処理も行わないオブジェクト
 *	----------------------------------------------------------------------
 */
class NullInputDevice : public InputDeviceInterface
{
/**
 *	------------------------------------------------------------
 *		メンバ変数
 *	------------------------------------------------------------
 */
public:
	NullInputEffect nullInputEffect;

/**
 *	------------------------------------------------------------
 *		メンバ関数
 *	------------------------------------------------------------
 */
public:

/**
 *		コンストラクタとデストラクタ
 */
	NullInputDevice() {}
	virtual ~NullInputDevice() {}

/**
 *	------------------------------------------------------------
 *		オーバーライド関数
 *	------------------------------------------------------------
 */
	/**
	 *	説明
	 *		オブジェクトがNullオブジェクトかどうかを調べる
	 */
	virtual bool isNull() const{return true;}

	/**
	 *	説明
	 *		デバイスバッファの取得
	 *	引数
	 *		outBufData: デバイスバッファを格納するDIDEVICEOBJECTDATA構造体の配列
	 *		elementNum: 配列の要素数
	 *	戻り値
	 *		バッファの取得数
	 */
	virtual DWORD getBuffer(DIDEVICEOBJECTDATA* outBufData, const DWORD elementNum) {return 0;}

	/**
	 *	説明
	 *		デバイスバッファの情報を消去
	 */
	virtual void flushBuffer() {return;}

	/**
	 *	説明
	 *		デバイスのアクセス権の取得
	 */
	virtual void acquire() { return; }

	/**
	 *	説明
	 *		デバイスのアクセス権の解放
	 */
	virtual void unacquire() { return; }

	/**
	 *	説明
	 *		デバイス状態を取得する
	 *	引数
	 *		lpState: デバイス状態を格納する領域のアドレス
	 *	戻り値
	 *		true: 取得成功
	 *		false: それ以外
	 */
	virtual bool getState(LPVOID lpState) {return true;}

/**
 *	----------------------------------------
 *	DirectInputEffect関連
 *	----------------------------------------
 */
	/**
	 *	説明
	 *		InputEffectクラスへのインターフェイスを取得
	 */
	virtual InputEffectInterface* getEffect() { return &nullInputEffect; }

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
	virtual bool createMagnitudeEffect(const DWORD magnitude, const double period) {return true;}

	/**
	 *	説明
	 *		エフェクトの解放
	 */
	virtual void releaseEffect(void) {return;}
};

#endif	/// __InputDevice_h__