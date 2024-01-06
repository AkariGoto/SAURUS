/**
 *  ファイル名
 *		OpenGL.h
 *  説明
 *		OpenGL関係の描画基礎クラス(Windows専用)
 *  日付
 *		作成日: 2007/04/01(SAT)		更新日: 2007/04/15(SAT)
 */

#ifndef __OpenGL_h__
#define __OpenGL_h__

 /**
  *	----------------------------------------------------------------------
  *		ヘッダファイルインクルード
  *	----------------------------------------------------------------------
  */
#include <windows.h>
#include <iostream>

#include <gl/GL.h>
#include <gl/GLU.h>
  //#include <gl/GLAUX.h>

#include "OpenGLParameter.h"
#include "CameraView.h"
/**
 *	----------------------------------------------------------------------
 *		リンカへの通知
 *	----------------------------------------------------------------------
 */
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")
 //#pragma comment (lib, "glaux.lib")



namespace Graphic
{
/**
 *	----------------------------------------------------------------------
 *		OpenGLクラス
 *	----------------------------------------------------------------------
 */
class OpenGL
{
    /**
     *	------------------------------------------------------------
     *		メンバ変数
     *	------------------------------------------------------------
     */
protected:
    /**
     *	ハンドル
     *		windowHandle：ウィンドウ
     *		deviceContextHandle：デバイスコンテキスト
     *		renderingContextHandle：レンダリングコンテキスト
     */
    HWND	windowHandle;
    HDC		deviceContextHandle;
    HGLRC	renderingContextHandle;

    /**
     *	描画するサイズ
     */
    int	sceneWidth;
    int sceneHeight;

    /**
     *	ライティング
     */
    GLfloat ambientLight0[4];		/// 環境光
    GLfloat diffuseLight0[4];		/// 拡散光
    GLfloat specularLight0[4];		/// 鏡面光
    GLfloat positionLight0[4];		/// 光源位置
    GLfloat directionLight0[3];	/// スポットライト方向

    /**
     *	視点に関係するもの
     */

     /**
      *	カメラ視点オブジェクト
      */
    CameraView cameraView;

    /// 視点タイプ
    ViewType viewType;

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
    OpenGL();

    /// コンストラクタ
    explicit OpenGL(HWND hWnd);

    /// デストラクタ
    virtual ~OpenGL();

    /**
     *	----------------------------------------
     *	OpenGLの設定に関係するもの
     *	----------------------------------------
     */
     /**
      *	説明
      *		初期化
      *	引数
      *		hWnd: 描画する領域のウィンドウハンドル
      */
    bool initializeGL(HWND hWnd = NULL);

    /**
     *	説明
     *		終了処理
     */
    void finalizeGL(void);

    /**
     *	説明
     *		OpenGLの機能設定
     */
    void setOpenGLFunctions(void);

    /**
     *	説明
     *		GLの設定のリセット
     */
    void clearOpenGLSettings(void);

    /**
     *	----------------------------------------
     *	描画に関係するもの
     *	----------------------------------------
     */
     /**
      *	説明
      *		照明効果の設定
      *		OpenGLではGL_LIGHT0からGL_LIGHT7までの８つの光源を設定できる
      */
    void setWorldLightings(void);

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
     *	説明
     *		描画領域のリサイズ処理
     */
    void resizeScenes(int width, int height);

    /**
     *	----------------------------------------
     *	視点の設定
     *	----------------------------------------
     */
     /**
      *	説明
      *		シーンの視点をViewTypeにより設定
      *	引数
      *		width: シーンの横幅
      *		height: シーンの高さ
      */
    virtual void setSceneView(double width, double height);

    /**
     *	説明
     *		シーンの視点を引数により設定
     *	引数
     *		distance, azimuth, elevation：視点への距離，方位角，仰角
     *		centerX, centerY, centerZ：視界の中心位置の参照点座標
     *		width: シーンの横幅
     *		height: シーンの高さ
     */
    void setViewPoint(double distance, double azimuth, double elevation,
              double centerX = 0.0, double centerY = 0.0, double centerZ = 0.0,
              double width = 1.0, double height = 1.0);

    /**
     *	----------------------------------------
     *	セット関数
     *	----------------------------------------
     */
    bool setWindowHandle(HWND hWnd);
    void setViewType(ViewType type);

    /**
     *	説明
     *		マテリアルカラーの設定
     */
    void setMaterialColor(double red, double green, double blue, double alpha);
    void setMaterialColor(const GLfloat* materialAmbDiffColor);
    void setMaterialColor(COLOR color);

    /**
     *	----------------------------------------
     *	アクセス関数
     *	----------------------------------------
     */
    HDC getDeviceContextHandle(void) const { return deviceContextHandle; }
    HWND getWindowHandle(void) const { return windowHandle; }
    ViewType getViewType(void) const { return viewType; }

    /**
     *	説明
     *		マテリアルカラーの取得
       */
    const GLfloat* selectMaterialColor(COLOR color);

    /**
     *	----------------------------------------
     *	カメラオブジェクト
     *	----------------------------------------
     */
     /**
      *	カメラオブジェクトの取得
      */
    CameraView& getCameraView(void) { return cameraView; }

    /**
     *	カメラ視点変更開始
     */
    void beginCameraViewControl(CameraView::Mode mode, int x, int y);

    /**
     *	カメラ視点変更終了
     */
    void endCameraViewControl(void);

    /**
     *	カメラ視点変更
     */
    void doCameraViewControl(int x, int y);

    /**
     *	----------------------------------------
     *	OpenGLの初期化のための補助関数
     *	----------------------------------------
     */
private:

    /**
     *	説明
     *		ピクセルフォーマットの設定
     */
    bool setWindowPixelFormat(void);
    /**
     *	説明
     *		コンテキストの設定
     */
    bool createGLContext(void);

};

}	/// end of namespace Graphic

#endif	/// __OpenGL_h__
