/**
 *  ファイル名
 *		InputController.h
 *  説明
 *		DirectInputによる入力の処理クラス
 *  日付
 *		作成日: 2008/02/05(MON)		更新日: 2008/02/19(MON)
 */

#ifndef __InputController_h__
#define __InputController_h__

/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */
#include "..\..\..\Utility\ArrayList.h"

#include "..\..\BasicDirectX.h"
#include "JoyPad.h"
#include "JoyPadState.h"

/**
 *	----------------------------------------------------------------------
 *		InputControllerクラス
 *			Singleton パターン
 *			クラスのインスタンスが1つしか生成されない
 *	----------------------------------------------------------------------
 */
class InputController
{
/**
 *	------------------------------------------------------------
 *		メンバ変数
 *	------------------------------------------------------------
 */
public:

private:
/**
 *	----------------------------------------
 *	DirectInput関連
 *	----------------------------------------
 */
	/**
	 *		DirectInput8インターフェイス
	 *		DirectInput8オブジェクトを管理する
	 */
	LPDIRECTINPUT8		lpDirectInput;
	/// DirectInputに関連付けるウィンドウハンドル
	HWND					windowHandle;

/**
 *	----------------------------------------
 *	JoyPadオブジェクト
 *	----------------------------------------
 */
	/// JoyPad本体
	ArrayList<InputDeviceInterface*>	joyPads;

	/// JoyPad状態
	ArrayList<JoyPadState*>			joyPadStates;

	/// JoyPadオブジェクト列挙引数構造体
	struct DIENUMOBJARGS
	{
		InputController*	ptrInputController;
		LPDIRECTINPUTDEVICE8 ptrDIDevice;
	};

/**
 *	------------------------------------------------------------
 *		メンバ関数
 *	------------------------------------------------------------
 */
protected:
/**
 *	----------------------------------------
 *	コンストラクタとデストラクタ
 *	----------------------------------------
 */
	/**
	 *	デフォルトコンストラクタ
	 *	シングルトン化
	 */
	InputController();

public:
	/// デストラクタ
	virtual ~InputController();

/**
 *	説明
 *		インスタンスの取得関数
 */
	static InputController& getInstance(void)
	{
		static InputController theInput;
		return theInput;
	}

/**
 *	説明
 *		初期化
 *		DirectInputを初期化してInputControllerを使用可能にする
 */
	bool initialize(const HWND hWnd);
	
/**
 *	説明
 *		リソースの解放
 *		DirectInputとInputControllerが使用していたリソース
 */	
	void release(void);

/**
 *	----------------------------------------
 *	デバイス
 *	----------------------------------------
 */
	/**
	 *		デバイスのアクセス権
	 */
	/// アクセス権の取得
	virtual void acquire();
	/// アクセス権の解放
	virtual void unacquire();

	/**
	 *		デバイスの更新
	 */
	/// デバイスバッファの更新
	HRESULT updateDeviceData(void);
	/// デバイス状態の更新
	HRESULT updateDeviceState(void);

/**
 *	----------------------------------------
 *	JoyPad関連
 *	----------------------------------------
 */
	/**
	 *		JoyPadデバイス生成
	 *		引数
	 *			joyPadNum: 初期化するJoyPadの数
	 */
	DWORD createJoyPad(const int joyPadNum);

	/**
	 *		JoyPadデバイスの解放
	 */
	void releaseJoyPad(void);

	/**
	 *		JoyPadを制御するためのインターフェイスを取得
	 *		引数
	 *			id: JoyPadのID（0～）
	 */
	InputDeviceInterface* getJoyPad(int id) const{return joyPads[id];}

	/**
	 *		JoyPad状態を取得
	 *		引数
	 *			id: JoyPadのID（0～）
	 */
	JoyPadState*	getJoyPadState(int id) const{return joyPadStates[id];}

private:
/**
 *	コピーコンストラクタの禁止
 */	
	InputController(const InputController& inputController);
 
/**
 *	代入演算子の禁止
 */
	InputController& operator =(const InputController& inputController);

/**
 *	デバイスの解放を行うヘルプ関数
 */
	void releaseDevice(InputDeviceInterface* pDevice);

/**
 *	----------------------------------------
 *	JoyPad関連
 *	----------------------------------------
 */
	/// JoyPad初期化
	BOOL initializeJoyPad(const LPDIDEVICEINSTANCE pDevInst, LPVOID userData);

	/// JoyPadオブジェクト設定
	BOOL setJoyPadObjectProp(const LPDIDEVICEOBJECTINSTANCE pDevObjInst, LPVOID userData);

	/// JoyPadのバッファ更新
	HRESULT updateJoyPadData(void);

	/// JoyPad状態更新
	HRESULT updateJoyPadState(void);

/**
 *	----------------------------------------
 *	コールバック関数
 *	----------------------------------------
 */
	/**
	 *		JoyPad列挙コールバック
	 */
	static BOOL CALLBACK enumJoyPadProcedure(LPDIDEVICEINSTANCE pDevInst, LPVOID userData);

	/**
	 *		JoyPad軸列挙コールバック
	 */
	static BOOL CALLBACK enumJoyPadObjectProcedure(LPDIDEVICEOBJECTINSTANCE pDevObjInst, LPVOID userData);

 };

/**
 *	----------------------------------------
 *	inline関数
 *	----------------------------------------
 */
/**
 *	インスタンスの取得の省略呼び出し
 */
inline InputController& diInputCtrl(void) { return InputController::getInstance(); }

/**
 *		JoyPad取得の省略呼び出し
 *		引数
 *			id: JoyPadのID（0～）
 */
inline InputDeviceInterface* diJoyPad(int id) { return diInputCtrl().getJoyPad(id); }

/**
 *		JoyPadState取得の省略呼び出し
 *		引数
 *			id: JoyPadのID（0～）
 */
inline JoyPadState* diJoyPadState(int id) { return diInputCtrl().getJoyPadState(id); }

#endif	/// __InputController_h__