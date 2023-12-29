/**
 *  ファイル名
 *		JoyPadState.h
 *  説明
 *		ジョイパッドの状態を保持するクラス
 *  日付
 *		作成日: 2008/03/10(MON)		更新日: 2008/03/10(MON)
 */

#ifndef __JoyPadState_h__
#define __JoyPadState_h__


/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */
#include "..\..\BasicDirectX.h"
#include "JoyPadConst.h"

/**
 *	----------------------------------------------------------------------
 *		JoyPadStateクラス
 *	----------------------------------------------------------------------
 */
class JoyPadState
{
/**
 *	------------------------------------------------------------
 *		メンバ変数
 *	------------------------------------------------------------
 */
public:

private:
/**
 *		軸の値
 */
 	/// 並進
	int AxisTransX;
	int AxisTransY;
	int AxisTransZ;
	
	/// 回転
	int AxisRotX;
	int AxisRotY;
	int AxisRotZ;

/**
 *	ボタン
 */
	bool buttons[DIJOY_MAX_BUTTON_NUM];


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
	JoyPadState();
	virtual ~JoyPadState();

/**
 *		メンバ変数のクリア
 */
	virtual void clear(void);

/**
 *	----------------------------------------
 *	セット関数
 *	----------------------------------------
 */
 	/**
 	 *		軸の並進値
 	 */
	virtual void setAxisTransX(int xAxis) { AxisTransX = xAxis; }
	virtual void setAxisTransY(int yAxis) { AxisTransY = yAxis; }
	virtual void setAxisTransZ(int zAxis) { AxisTransZ = zAxis; }	

 	/**
 	 *		軸の回転値
 	 */
	virtual void setAxisRotX(int xAxis) { AxisRotX = xAxis; }
	virtual void setAxisRotY(int yAxis) { AxisRotY = yAxis; }
	virtual void setAxisRotZ(int zAxis) { AxisRotZ = zAxis; }

	/**
	 *		ボタン
	 *		引数
	 *			id: 対象ボタンID
	 *		戻り値
	 *			ボタンが押されていればtrue
	 */
	virtual void setButtonPressed(int id, bool pressed)
	{
		DEBUG_ASSERT( (0 <= id ) && (id < DIJOY_MAX_BUTTON_NUM) );
		
		buttons[id] = pressed;
	}

/**
 *	----------------------------------------
 *	アクセス関数
 *	----------------------------------------
 */
 	/**
 	 *		軸の並進値
 	 */
	virtual int getAxisTransX(void) { return AxisTransX; }
	virtual int getAxisTransY(void) { return AxisTransY; }
	virtual int getAxisTransZ(void) { return AxisTransZ; }

 	/**
 	 *		軸の並進値
 	 */
	virtual int getAxisRotX(void) { return AxisRotX; }
	virtual int getAxisRotY(void) { return AxisRotY; }
	virtual int getAxisRotZ(void) { return AxisRotZ; }

	/**
	 *		ボタンが押されているか
	 *		引数
	 *			id 対象ボタンID
	 *		戻り値
	 *			ボタンが押されていればtrue
	 */
	virtual bool isButtonPressed(int id) const
	{
		DEBUG_ASSERT( (0 <= id ) && (id < DIJOY_MAX_BUTTON_NUM) );
		return buttons[id];
	}


private:

};

#endif	/// __JoyPadState_h__