/**
 *  ファイル名
 *		CameraView.cpp
 *  説明
 *		カメラビューを生成する（主にマウス動作に対応）
 *  日付
 *		作成日: 2007/06/04(MON)		更新日: 2007/06/13(WED)
 */

//初期カメラ位置設定  20200822

/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */
#include "CameraView.h"

using namespace std;
using namespace Math;
using namespace Const;

namespace Graphic
{
/**
 *	----------------------------------------------------------------------
 *		CameraViewクラス
 *	----------------------------------------------------------------------
 */

/**
 *	------------------------------------------------------------
 *		CameraViewクラスのメンバ関数定義
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	コンストラクタとデストラクタ
 *	----------------------------------------
 */
/// デフォルトコンストラクタ
CameraView::CameraView()
{
	setDefaultCondition();
}

/// デストラクタ
CameraView::~CameraView()
{
}

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
void  CameraView::beginViewControl(CameraView::Mode mode, int x, int y)
{
	modeState = mode;

	initX = x;
	initY = y;

	initAzimuth		= azimuth;
	initElevation	= elevation;
	initDistance	= distance;

	for (int i=0; i<THREE_DIMENSION; i++)
		initViewCenterPosition[i] = viewCenterPosition[i];
			
	return;
}

/**
 *	視点変更終了
 */
void  CameraView::endViewControl(void)
{
	modeState = CameraView::STOP;
}

/**
 *	説明
 *		モードによるViewの変更
 */
void  CameraView::doViewControl(int x, int y)
{
	switch ( modeState )
	{
		case CameraView::PAN:
			pan(x, y);
			break;
		
		case CameraView::SPIN:
			spin(x, y);
			break;

		case CameraView::ZOOM:
			zoom(x, y);
			break;

		case CameraView::STOP:
			break;
			
		default:
			return;
			break;
	}
}

/**
 *	説明
 *		視野の変更の各種操作
 */
/**
 *	----------------------------------------
 *	視野の変更の各種操作
 *	----------------------------------------
 */
/**
 *	並進操作
 */
void CameraView::pan(int x, int y)
{
	double dx = (double)(x - initX);
	double dy = (double)(y - initY);

	double upview[3];
	double view[3];
	double right[3];

	getUpDirection(&upview[0], &upview[1], &upview[2]);

	getEyeDirection(&view[0], &view[1], &view[2]);

	outerProduct(view[0], view[1], view[2],
				 upview[0], upview[1], upview[2],
				 &right[0], &right[1], &right[2]
				 );

	double vx = 5.0E-4;
	double vy = 5.0E-4;

	for (int i=0; i<THREE_DIMENSION; i++)
	{
		viewCenterPosition[i] = ( vx*dx*right[i] + vy*dy*upview[i] ) + initViewCenterPosition[i];
	}

	return;
}

/**
 *	回転操作
 */
void CameraView::spin(int x, int y)
{
	double dx = (double)(x - initX);
	double dy = (double)(y - initY);

	azimuth		= (-0.5)*(double)dx + initAzimuth;
	elevation	= (0.5)*(double)dy + initElevation;

	return;
}

/**
 *	縮尺操作
 */
void CameraView::zoom(int x, int y)
{
	double dx = (double)(x - initX);
	double dy = (double)(y - initY);

	double dl = dy;

	distance = (100.0)*dl + initDistance;

	return;
}

/**
 *	----------------------------------------
 *	セット関数
 *	----------------------------------------
 */
/**
 *	説明
 *		画面上の操作起点となる点をセット
 */
void CameraView::setControlPoint(int x, int y)
{
	initX = x;
	initY = y;

	initAzimuth		= azimuth;
	initElevation	= elevation;
	initDistance	= distance;

	for (int i=0; i<THREE_DIMENSION; i++)
		initViewCenterPosition[i] = viewCenterPosition[i];
			
	return;
}

/**
 *	説明
 *		変数の値をデフォルトにセット
 */
void CameraView::setDefaultCondition(void)
{
	initX = 0;
	initY = 0;

	/*
	azimuth		= 45;
	elevation	= 45;
	distance	= 2500;
	*/
	//20200822
	azimuth = 90;
	elevation = 15;
	distance = 2500;

	for (int i=0; i<THREE_DIMENSION; i++)
		viewCenterPosition[i] = 0.0;

	setControlPoint(0, 0);

	modeState = CameraView::STOP;

	return;
}

/**
 *	説明
 *		変更モードを設定
 */
void CameraView::setMode(CameraView::Mode mode)
{
	modeState = mode;
}



/**
 *	説明
 *		視線方向の取得
 */
void CameraView::getEyeDirection(double* x, double* y, double* z)
{
	double xo = cos(elevation*PI/180)*cos(azimuth*PI/180);
	double yo = cos(elevation*PI/180)*sin(azimuth*PI/180);
	double zo = sin(elevation*PI/180);

	*x = xo;
	*y = yo;
	*z = zo; 

	return;
}

/**
 *	説明
 *		視野の上方向の取得
 */
void CameraView::getUpDirection(double* x, double* y, double* z)
{
	double xo = distance*cos( (elevation+90)*PI/180 )*cos(azimuth*PI/180);
	double yo = distance*cos( (elevation+90)*PI/180 )*sin(azimuth*PI/180);
	double zo = distance*sin( (elevation+90)*PI/180 );
 
	*x = xo;
	*y = yo;
	*z = zo;
	
	return;
}

}	/// end of namespace Graphic