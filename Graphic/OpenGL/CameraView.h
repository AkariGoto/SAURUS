/**
 *  ファイル名
 *		CameraView.h
 *  説明
 *		カメラビューを生成する（主にマウス動作に対応）
 *  日付
 *		作成日: 2007/06/04(MON)		更新日: 2007/06/13(WED)
 */

#ifndef __CameraView_h__
#define __CameraView_h__

/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */
#include <cmath>
#include "../../Math/MathLibrary.h"

namespace Graphic
{


/**
 *	----------------------------------------------------------------------
 *		CameraViewクラス
 *	----------------------------------------------------------------------
 */
class CameraView
{
/**
 *	------------------------------------------------------------
 *		メンバ変数
 *	------------------------------------------------------------
 */
public:
/**
 *		変更モード
 *			PAN			: 並進
 *			SPIN		: 回転
 *			ZOOM		: 縮尺
 *			STOP		: 停止
 */
	enum Mode{PAN, SPIN, ZOOM, STOP};

private:
/**
 *		カメラ情報
 *			azimuth				: 方位角(アジマス)[deg]
 *			elevation			: 仰角(エレベイション)[deg]
 *			distance				: 距離
 */
	double		azimuth;
	double		elevation;
	double		distance;

/**
 *		視点の中心位置
 */
	double viewCenterPosition[3];

/**
 *		視点の初期情報
 *			initAzimuth				: 方位角(アジマス)[deg]
 *			initElevation			: 仰角(エレベイション)[deg]
 *			initDistance			: 距離
 */
	double		initAzimuth;
	double		initElevation;
	double		initDistance;

/**
 *		視点の初期中心位置
 */
	double		initViewCenterPosition[Const::THREE_DIMENSION];

/**
 *		視点の2次元初期位置
 */
	int initX;
	int initY;

/**
 *		モード状態
 */
	Mode	modeState;

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
	CameraView();
	virtual ~CameraView();

/**
 *	----------------------------------------
 *	主な操作
 *	----------------------------------------
 */
	/**
	 *	説明
	 *		視点変更開始
	 *	引数
	 *		mode: 変更モード
	 *		x: 起点のx座標
	 *		y: 起点のy座標
	 */
	void beginViewControl(CameraView::Mode mode, int x, int y);

	/**
	 *	視点変更終了
	 */
	void endViewControl(void);

	/**
	 *	説明
	 *		モードによるViewの変更
	 */
	void doViewControl(int x, int y);

/**
 *		視野の変更の各種操作
 *			pan		: 並進操作
 *			Rotate			: 回転操作
 *			Scale			: 縮尺操作
 */
	void	pan(int x, int y);
	void	spin(int x, int y);
	void	zoom(int x, int y);


/**
 *	----------------------------------------
 *	セット関数
 *	----------------------------------------
 */
/**
 *	説明
 *		画面上の操作起点となる点をセット
 */
	void setControlPoint(int x, int y);

/**
 *	説明
 *		変数の値をデフォルトにセット
 */
	void setDefaultCondition(void);


/**
 *	説明
 *		変更モードを設定
 */
	void setMode(CameraView::Mode mode);

/**
 *	----------------------------------------
 *	アクセス関数
 *	----------------------------------------
 */
	double getAzimuth(void) const {return azimuth;}
	double getElevation(void) const {return elevation;}
	double getDistance(void) const {return distance;}
	double getViewCenterPosition(int num) const
			{
				assert( 1<= num && num <= Const::THREE_DIMENSION);
				return viewCenterPosition[num-1];
			}

/**
 *	説明
 *		変更モードを取得
 */
	CameraView::Mode getMode(void) const{ return modeState; }




private:
/**
 *	説明
 *		視線方向の取得
 */
	void getEyeDirection(double* x, double* y, double* z);

/**
 *	説明
 *		視野の上方向の取得
 */
	void getUpDirection(double* x, double* y, double* z);

};	/// end of namespace CameraView

}	/// end of namespace Graphic	

#endif /// __CameraView_h__