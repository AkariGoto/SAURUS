/**
 *  ファイル名
 *		AsuraGraphic.h
 *  説明
 *		OpenGLを用いてTITAN Xを描画するクラス
 *  日付
 *		作成日: 2007/05/12(SAT)		更新日: 2008/07/07(MON)
 */

//  20200819  クローラ関連コメントアウト
//  20201019  脚先のワールド座標と脚座標の切り替え

#ifndef __AsuraGraphic_h__
#define __AsuraGraphic_h__

/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */
#include "OpenGL\OpenGLObject.h"
#include "OpenGL\OpenGLText.h"
#include "OpenGL\OpenGLMovie.h"

#include "..\Data\AsuraData.h"


namespace Graphic
{

/**
 *	----------------------------------------------------------------------
 *		定数宣言
 *	----------------------------------------------------------------------
 */
const int LEG_DISP_LIST_NUM = Asura::LEG_JOINT_NUM+2;

/*  20200819
const int TRACK_DISP_LIST_NUM = Asura::TRACK_JOINT_NUM+2;
*/

/**
 *	----------------------------------------------------------------------
 *		AsuraGraphicクラス
 *	----------------------------------------------------------------------
 */
class AsuraGraphic : public OpenGLObject
{
/**
 *	------------------------------------------------------------
 *		メンバ変数
 *	------------------------------------------------------------
 */
private:
	/**
	 *	Asuraを描画するためのデータソース
	 */
	Data::AsuraData* asuraDataSrcPtr;
	
	/**
	 *	ムービーレコーダ
	 */
	OpenGLMovie movieRecorder;

	/**
	 *	描画のためのOpenGLのディスプレイリスト
	 */
	int bodyDisplayListID;
	int legDisplayListID[LEG_DISP_LIST_NUM];

	/*  20200819
	int trackDisplayListID[TRACK_DISP_LIST_NUM];
	*/

	/**
	 *	影描画用変数
	 */
	GLfloat planeFactorOfGridFloor[4];


	/**
	 *	フラグ
	 */
	/// 描画用
	bool isAsuraDrawn;
	bool isSupportPolygonDrawn;
	bool isDisplayListReady;

	/// AVI保存用
	bool isAnimationRecorded;

//20201019
	bool legCoordinate;  //脚座標を表示するか否かのフラグ


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
	/// デフォルトコンストラクタ
	explicit AsuraGraphic(HWND hWnd = NULL, Data::AsuraData* asuraData = NULL, ViewType type = PERSPECTIVE);

	/// デストラクタ
	virtual ~AsuraGraphic();

/**
 *	----------------------------------------
 *	OpenGLの設定の初期化・終了処理
 *	----------------------------------------
 */
	/// 初期化
	void initializeAsuraGraphic(HWND hWnd = NULL, Data::AsuraData* asuraData = NULL);
	/// 終了処理
	void finalizeAsuraGraphic(void);

/**
 *	説明
 *		ディスプレイリストの生成・消去
 */
	void newAsuraDisplayList(void);
	void deleteAsuraDisplayList(void);

/**
 *	----------------------------------------
 *	描画に関係するもの
 *	----------------------------------------
 */
	/**
	 *	説明
	 *		OpenGLのオブジェクト描画
	 */
	virtual void drawObjects(GLenum renderMode);

/**
 *	説明
 *		OpenGLイメージ描画
 */
	virtual void drawScenes(void);

/**
 *	説明
 *		オブジェクトの影描画
 */
	virtual void drawShadow(void);

	/**
	 *	説明
	 *		OpenGLイメージのレンダリング
	 *		OpenGL描画のコールバック関数になる
	 */
	virtual void renderScenes(void);

/**
 *	説明
 *		シーンの視点をViewTypeにより設定
 */
	virtual void setSceneView(double width, double height);

/**
 *	Asuraの描画
 *		drawAsura			: Asuraの全体描画
 *		drawBody			: Asuraの本体描画
 *		drawLeg				: Asuraの脚描画
*/
	void drawAsura(void);
	void drawBody(void);
	void drawLeg(int legNo);
	void drawTrack(int trackNo);

/**
 *	支持脚多角形(三角形or四角形)の描画
 */
	void drawSupportPolygon(void);

/**
 *	支持脚多角形(三角形or四角形)を見せる/隠す
 */
	/// 見せる
	void showSupportPolygon(void) { isSupportPolygonDrawn = true; return; }
	/// 隠す
	void hideSupportPolygon(void) { isSupportPolygonDrawn = false; return; }

/**
 *	AVIを保存するかどうか
 */
	/// 録画する
	void startRecording(void) {isAnimationRecorded = true; return;}
	/// 録画停止する
	void stopRecording(void) {isAnimationRecorded = false; return;}

	//20201019  脚座標系のフラグの関数
	void legCoordinateOn(void) { legCoordinate = true; }  //脚座標系を表示する
	void legCoordinateOff(void) { legCoordinate = false; }  //脚座標系を表示しない

private:
	/**
	 *		OpenGL座標変換のためのヘルプ関数
	 *			OpenGL用の4x4座標変換
	 */
	void transformOpenGLMatrix(const Math::Matrix& matrix);

	/**
	 *	説明
	 *		影を投影する平面の係数を計算
	 *	引数
	 *		planeEq: 平面の方程式の係数配列
	 *		p0: 平面上の頂点
	 *		p1: 平面上の頂点
	 *		p2: 平面上の頂点
	 */
	void calcShadowPlaneEquation(GLfloat planeEq[], GLfloat p0[], GLfloat p1[], GLfloat p2[]);

	/**
	 *	説明
	 *		目的の影を投影する行列
	 *	引数
	 *		matrix: 投影した行列
	 *		plnaeEq: 平面の方程式の係数配列（要素数4）
	 *		lightPos: 光源の同時座標位置（要素数4）
	 */
	 void calcShadowMatrix(GLfloat matrix[], GLfloat planeEq[], GLfloat lightPos[]);

};

}	/// end of namespace Graphic

#endif	/// __AsuraGraphic_h__