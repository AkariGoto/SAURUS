/**
 *  ファイル名
 *		InputDeviceInterface.h
 *  説明
 *		DirectInputを用いたデバイスの処理クラス
 *  日付
 *		作成日: 2008/03/10(MON)		更新日: 2008/03/10(MON)
 */

#ifndef __InputDeviceInterface_h__
#define __InputDeviceInterface_h__


/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */
#include "..\..\BasicDirectX.h"
#include "InputEffect.h"

/**
 *	----------------------------------------------------------------------
 *		マクロ、型宣言
 *	----------------------------------------------------------------------
 */
/// バッファサイズ
static const int DIDEV_BUFSIZE = 16;

/**
 *	----------------------------------------------------------------------
 *		InputDeviceInterfaceクラス
 *	----------------------------------------------------------------------
 */
class InputDeviceInterface
{

public:

	
/**
 *	------------------------------------------------------------
 *		メンバ変数
 *	------------------------------------------------------------
 */
public:
	

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
	InputDeviceInterface() {}
	virtual ~InputDeviceInterface() {}

/**
 *	----------------------------------------
 *	オーバーライド関数
 *	----------------------------------------
 */
	/**
	 *	説明
	 *		オブジェクトがNullオブジェクトかどうかを調べる
	 */
	virtual bool isNull() const = 0;

	/**
	 *	説明
	 *		デバイスバッファの取得
	 *	引数
	 *		outBufData: デバイスバッファを格納するDIDEVICEOBJECTDATA構造体の配列
	 *		elementNum: 配列の要素数
	 *	戻り値
	 *		バッファの取得数
	 */
	virtual DWORD getBuffer(DIDEVICEOBJECTDATA* outBufData, const DWORD elementNum = DIDEV_BUFSIZE) = 0;
	
	/**
	 *	説明
	 *		デバイスバッファの情報を消去
	 */
	virtual void flushBuffer(void) = 0;	

	/**
	 *	説明
	 *		デバイスのアクセス権の取得
	 */
	virtual void acquire() = 0;

	/**
	 *		デバイスのアクセス権の解放
	 */
	virtual void unacquire() = 0;

	/**
	 *	説明
	 *		デバイス状態を取得する
	 *	引数
	 *		lpState: デバイス状態を格納する領域のアドレス
	 *	戻り値
	 *		true: 取得成功
	 *		false: それ以外
	 */
	virtual bool getState(LPVOID lpState) = 0;

/**
 *	----------------------------------------
 *	DirectInputEffect関連
 *	----------------------------------------
 */
	/**
	 *	説明
	 *		InputEffectクラスへのインターフェイスを取得
	 */
	virtual InputEffectInterface* getEffect() = 0;

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
	virtual bool createMagnitudeEffect(const DWORD magnitude, const double period) = 0;

	/**
	 *	説明
	 *		エフェクトの解放
	 */
	virtual void releaseEffect(void) = 0;
};

#endif	/// __InputDeviceInterface_h__