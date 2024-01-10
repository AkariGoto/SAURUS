
#ifndef __OpenGL_h__
#define __OpenGL_h__

#include <windows.h>

#include <gl/GL.h>
#include <gl/GLU.h>

#include "Graphic/OpenGL/camera_view.h"
#include "Graphic/OpenGL/open_gl_parameter.h"

#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")


namespace designlab_robot_gui::graphic
{

class AbstractOpenGLBase
{
public:
    //! デフォルトコンストラクタは default (なにもしない)． 
    AbstractOpenGLBase() = default;

    //! @brief コンストラクタ．
    explicit AbstractOpenGLBase(HWND hWnd)
    {
        InitializeGL(hWnd);
    }

    //! @brief デストラクタ．継承する場合は必ず virtual にすること．
    virtual ~AbstractOpenGLBase()
    {
        FinalizeGL();
    }


    //! @brief OpenGLイメージの描画を行う．
    virtual void RenderScenes();


    //! @brief カメラ視点の変更を開始する．
    //! @param[in] mode カメラ視点の変更モード．
    //! @param[in] x マウスのX座標．
    //! @param[in] y マウスのY座標．
    void BeginCameraViewControl(CameraView::Mode mode, int x, int y);

    //! @brief カメラ視点の変更を終了する．
    void EndCameraViewControl();

    //! @brief カメラ視点の変更を行う．
    //! @param[in] x マウスのX座標．
    //! @param[in] y マウスのY座標．
    void DoCameraViewControl(int x, int y);

protected:
    virtual void DrawScenes() const;

    /**
 *	説明
 *		シーンの視点をViewTypeにより設定
 *	引数
 *		width: シーンの横幅
 *		height: シーンの高さ
    */
    virtual void setSceneView(double width, double height);

    //! @brief カメラの視点を設定する．
    //! AbstractOpenGLBase で実装．
    //! @param[in] type 視点タイプ．
    constexpr void SetViewType(const ViewType type)
    {
        view_type_ = type;
    }

    /**
 *	説明
 *		シーンの視点を引数により設定
 *	引数
 *		distance_, azimuth_, elevation：視点への距離，方位角，仰角
 *		centerX, centerY, centerZ：視界の中心位置の参照点座標
 *		width: シーンの横幅
 *		height: シーンの高さ
 */
    void setViewPoint(double distance, double azimuth, double elevation,
              double centerX = 0.0, double centerY = 0.0, double centerZ = 0.0,
              double width = 1.0, double height = 1.0);

    //! @brief マテリアルカラーを取得する．
    //! AbstractOpenGLBaseで実装．
    const GLfloat* selectMaterialColor(MaterialColor color);

    //! @brief 初期化処理を行う．
    //! AbstractOpenGLBaseで実装．
    //! @param[in] window_handle 描画する領域のウィンドウハンドル．
    //! @retval true 成功．
    //! @retval false 失敗．
    bool InitializeGL(HWND window_handle = nullptr);

    //! @brief 終了処理を行う．
    void FinalizeGL();

    //! @brief マテリアルカラーを設定する．
    void setMaterialColor(double red, double green, double blue, double alpha);

    //! @brief マテリアルカラーを設定する．
    void setMaterialColor(const GLfloat* materialAmbDiffColor);

    //! @brief マテリアルカラーを設定する．
    void setMaterialColor(MaterialColor color);


    HWND	window_handle_ptr_{ nullptr };  //!< ウィンドウハンドル．
    HDC		device_context_handle_ptr_{ nullptr };  //!< デバイスコンテキストハンドル．
    HGLRC	rendering_context_handle_{ nullptr };  //!< レンダリングコンテキストハンドル．

    int	scene_width_{ 0 };   //!< シーンの横幅．
    int scene_height_{ 0 };  //!< シーンの高さ．


    GLfloat ambientLight0[4];  //!< 環境光．
    GLfloat diffuseLight0[4];  //!< 拡散光．
    GLfloat specularLight0[4];  //!< 鏡面光．
    GLfloat positionLight0[4];  //!< 光源位置．
    GLfloat directionLight0[3];  //!< スポットライト方向．

    CameraView camera_view_;  //!< カメラ視点オブジェクト．

    ViewType view_type_{ ViewType::PERSPECTIVE };  //!< 視点タイプを表す列挙型．

private:
    //! @brief ピクセルフォーマットの設定．
    //! @retval true 成功．
    //! @retval false 失敗．
    bool SetWindowPixelFormat();

    //! @brief 環境光の設定．
    //! OpenGLではGL_LIGHT0からGL_LIGHT7までの８つの光源を設定できる．
    void setWorldLightings();

    //! @brief OpenGLのコンテキストを作成する．
    //! @retval true 成功．
    //! @retval false 失敗．
    bool CreateGLContext();

    //! @brief OpenGLの機能設定を行う．
    void SetOpenGLFunctions();

    //! @brief OpenGLの設定をリセットする．
    void ClearOpenGLSettings();
};

}	/// end of namespace Graphic

#endif	/// __OpenGL_h__
