/**
 *  ファイル名
 *		OpenGLObject.h
 *  説明
 *		OpenGLによる物体描画クラス(Windows専用)
 *  日付
 *		作成日: 2007/04/01(SAT)		更新日: 2007/04/17(MON)
 */

#ifndef __OpenGLObject_h__
#define __OpenGLObject_h__

/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */
#include "OpenGL.h"

namespace Graphic
{
/**
 *	----------------------------------------------------------------------
 *		OpenGLObjectクラス
 *	----------------------------------------------------------------------
 */
class OpenGLObject : public OpenGL
{
/**
 *	------------------------------------------------------------
 *		メンバ変数
 *	------------------------------------------------------------
 */
protected:
/**
 *	----------------------------------------
 *	ピッキングに関するもの
 *	----------------------------------------
 */
	/// 最終的に選択したオブジェクトの名前番号を格納する
	unsigned int nameNumberOfHitObj[NAME_ARRAY_SIZE];	

/**
 *	----------------------------------------
 *	フラグ
 *	----------------------------------------
 */
	/// 座標軸を描画するかどうか
	bool isCoordinateAxisDrawn;

	/// メッシュの床を描画するかどうか
	bool isGridFloorDrawn;

	/// 床描画のためのディスプレイリスト
	int gridFloorDisplayListID;

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
	OpenGLObject();

	/// デフォルトコンストラクタ
	explicit OpenGLObject(HWND hWnd);

	/// デストラクタ
	virtual ~OpenGLObject();

/**
 *	----------------------------------------
 *	OpenGLのオブジェクト作成・消去
 *	----------------------------------------
 */
	/**
	 *	説明
	 *		OpenGLの初期化を行い、OpenGLObjectに関連づける
	 *	引数
	 *		hWnd: 描画する領域のウィンドウハンドル
	 */
	bool createGLObject(HWND hWnd = NULL);

	/**
	 *	説明
	 *		終了処理
	 */
	void destroyGLObject(void);

/**
 *	------------------------------------------------------------
 *		オーバーライド関数
 *	------------------------------------------------------------
 */
/**
 *	----------------------------------------
 *	シーンの描画
 *	----------------------------------------
 */
	/**
	 *	説明
	 *		OpenGLのオブジェクト描画
	 */
	virtual void drawObjects(GLenum renderMode);

	/**
	 *	説明
	 *		OpenGLのイメージ描画
	 */
	virtual void drawScenes(void);

	/**
	 *	説明
	 *		OpenGLイメージのレンダリング
	 *		OpenGL描画のコールバック関数になる
	 */
	virtual void renderScenes(void);

/**
 *	----------------------------------------
 *	ピッキングに関するもの
 *	----------------------------------------
 */
	/**
	 *	説明
	 *		デバイス（マウスなど）による物体のピックアップ関数
	 *		デバイスによるイベント（マウスだったら左クリックなど）関数内で呼ぶ
	 *	引数
	 *		x: クリックした点のx座標
	 *		y: クリックした点のy座標
	 */
	virtual void pickup(int x, int y);

protected:
	/**
	 *	説明
	 *		セレクションバッファからデプスバッファを並び替えたりしながらヒットしたオブジェクトを識別する
	 *	引数
	 *		hitCount: ヒット数
	 *		selectBuffer:	glSelectBuffer() の結果が格納されているセレクションバッファ
	 *						選択したオブジェクトの名前、セレクションされた位置の奥行きの最小値と最大値などが格納されたバッファ
	 */
	virtual int selectHitObjects(GLuint hitCount, GLuint selectBuffer[]);

	/**
	 *	説明
	 *		ピックアップにより選んだ物体の処理
	 */
	virtual void handlePickupResult(void);

/**
 *	------------------------------------------------------------
 *		オブジェクトの描画に関係するもの
 *	------------------------------------------------------------
 */

public:
/**
 *	----------------------------------------
 *	物体の表示/非表示
 *	----------------------------------------
 */
	/// 床を見せる
	void showGridFloor(void) {isGridFloorDrawn = true; return;}
	/// 床を隠す
	void hideGridFloor(void) {isGridFloorDrawn = false; return;}

	/// 座標軸を見せる
	void showCoordinateAxis(void) {isCoordinateAxisDrawn = true; return;}
	/// 座標軸を隠す
	void hideCoordinateAxis(void) {isCoordinateAxisDrawn = false; return;}

/**
 *	----------------------------------------
 *	3Dモデリング
 *	----------------------------------------
 */
	/// 座標軸描画
	void drawCoordinateAxis(double length = 300.0, double width = 2.0, double scale = 1.0); 

	/**
	 *	説明
	 *		ポリゴンモデリング（法線も計算）
	 *		添字(1, 2, 3)の順にCW（時計回り）で入力
	 *		各頂点の座標を入力
	 */
	void drawPolygonSurface(double x1, double y1, double z1,
							double x2, double y2, double z2,
							double x3, double y3, double z3);
	void drawPolygonSurface(const Math::Vector& point1, const Math::Vector& point2, const Math::Vector& point3);

	/// 円柱描画
	void drawCylinder(double radius, double height);

	/// 球描画
	void drawSphere(double radius);

	/// Box描画(線画)
	void drawBox(double startX, double startY, double startZ, double endX, double endY, double endZ);
	void drawBox(const Math::Vector& startPoint, const Math::Vector& endPoint);

	/// フレーム描画(平面と4隅の垂線)
	void drawFrame(double startX, double startY, double startZ, double endX, double endY, double endZ);
	void drawFrame(const Math::Vector& startPoint, const Math::Vector& endPoint);

	/// 線描画(線画)
	void drawLine(double startX, double startY, double startZ,
						 double endX, double endY, double endZ, 
						 double width = 1.0);
	void drawLine(const Math::Vector& startPoint, const Math::Vector& endPoint, double width = 1.0);

	/// 三角形描画
	void drawTriangle(const Math::Vector& point1, const Math::Vector& point2, const Math::Vector& point3, double width = 1.0);

	/// 四角形描画
	void drawQuadrangle(	const Math::Vector& point1, const Math::Vector& point2, 
									const Math::Vector& point3, const Math::Vector& point4,
									double width = 1.0);

/**
 *	----------------------------------------
 *	2Dモデリング
 *	----------------------------------------
 */
	/**
	 *	説明
	 *		平面板描画(対角線の頂点と高さ)
	 */
	/// 水平面（XY）
	void drawHorizontalPlane(double startX, double startY, double endX, double endY, double z);
	/// 前額面（YZ）
	void drawFrontalPlane(double startY, double startZ, double endY, double endZ, double x);
	/// 矢状面（ZX）
	void drawSagittalPlane(double startZ, double startX, double endZ, double endX, double y);

	/**
	 *	説明
	 *		メッシュ状の床を描画する
	 *	引数
	 *		startX, startY：床の対角線の始点
	 *		endX, endY：床の対角線の終点
	 *		z：床の高さ
	 *		dx, dy：メッシュの間隔
	 */
	void drawGridFloor(void);

/**
 *	----------------------------------------
 *	ディスプレイリスト
 *	----------------------------------------
 */
protected: 
	/**
	 *	説明
	 *		ディスプレイリストの生成
	 */
	void newGLObjDispList(void);
	/**
	 *	説明
	 *		ディスプレイリストの消去
	 */
	void deleteObjDispList(void);	
	
private:

};	/// end of class OpenGLObject

}	/// end of namespace Graphic

#endif	/// __OpenGLObject_h__