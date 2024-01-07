
#include "pch.h"

#include "Graphic/OpenGL/abstract_open_gl_base.h"



namespace designlab_robot_gui::graphic
{

bool AbstractOpenGLBase::InitializeGL(HWND hWnd)
{
    // 引数チェック．
    if (!::IsWindow(hWnd))
    {
        std::cerr << "Error: [AbstractOpenGLBase::initializeOpenGL] Invalid Window Handle"
            << std::endl;
        return false;
    }

    // ハンドルの代入
    window_handle_ptr = hWnd;
    device_context_handle_ptr = ::GetDC(window_handle_ptr);

    // ピクセルフォーマットの設定．
    if (!SetWindowPixelFormat())
    {
        std::cerr << "Error: [AbstractOpenGLBase::initializeOpenGL] SetWindowPixelFormat Error"
            << std::endl;
        return false;
    }

    // GLコンテキストの設定．
    if (!CreateGLContext())
    {
        std::cerr << "Error: [AbstractOpenGLBase::initializeOpenGL] CreateGLContext Error"
            << std::endl;
        return false;
    }

    // OpenGLの各機能の開始．
    SetOpenGLFunctions();

    // 照明の設定．
    setWorldLightings();

    // 描画するシーンのサイズを格納．
    RECT sceneSize;
    ::GetClientRect(window_handle_ptr, &sceneSize);
    scene_width = sceneSize.right;
    scene_height = sceneSize.bottom;

    return true;
}


void AbstractOpenGLBase::FinalizeGL()
{
    // レンダリングコンテキストをカレントからはずす．
    if (wglGetCurrentContext())
    {
        wglMakeCurrent(NULL, NULL);
    }

    if (rendering_context_handle != NULL)
    {
        wglDeleteContext(rendering_context_handle);
        rendering_context_handle = NULL;
    }

    if (device_context_handle_ptr != NULL)
    {
        ::ReleaseDC(window_handle_ptr, device_context_handle_ptr);
    }
}

void AbstractOpenGLBase::SetOpenGLFunctions()
{
    /**
     *	バッファを初期化するカラー情報を指定
     *	glClearで使用されるカラー
     */
    glClearColor(0.7f, 0.8f, 0.9f, 1.0f);		// 背景色の決定

    // デプスバッファクリア値の設定
    glClearDepth(1.0f);

    // Z（デプス）バッファの使用開始
    glEnable(GL_DEPTH_TEST);

    /**
     *	深度バッファー比較に使用される値を指定
     *	同一か、手前にあるもので上がきしていく
     */
    glDepthFunc(GL_LEQUAL);

    /**
     *	シェーディング設定
     *	プリミティブの面を各頂点の色から計算されたグラデーションで塗る
     */
    glShadeModel(GL_SMOOTH);

    /**
     *	次の行列演算ターゲットを選択する
     *		オブジェクト描画モード
     */
    glMatrixMode(GL_MODELVIEW);

}

/**
 *	説明
 *		GLの設定のリセット
 */
void AbstractOpenGLBase::ClearOpenGLSettings(void)
{
    // レンダリングコンテキストの初期化
    if (rendering_context_handle != NULL)
    {
        wglDeleteContext(rendering_context_handle);
        rendering_context_handle = NULL;
    }

    // デバイスコンテキストハンドルの初期化
    if (device_context_handle_ptr != NULL)
        ::ReleaseDC(window_handle_ptr, device_context_handle_ptr);

    // 視点をパースペクティブに
    view_type = ViewType::PERSPECTIVE;

}

void AbstractOpenGLBase::setWorldLightings()
{
    // 照明効果の決定．

     // 環境光．
    ambientLight0[0] = 1.0f;
    ambientLight0[1] = 1.0f;
    ambientLight0[2] = 1.0f;
    ambientLight0[3] = 0.5f;

    // 拡散光．
    diffuseLight0[0] = 1.0f;
    diffuseLight0[1] = 1.0f;
    diffuseLight0[2] = 1.0f;
    diffuseLight0[3] = 1.0f;

    // 鏡面光．
    specularLight0[0] = 1.0f;
    specularLight0[1] = 1.0f;
    specularLight0[2] = 1.0f;
    specularLight0[3] = 1.0f;

    // 光源位置．
    positionLight0[0] = 0.5f;
    positionLight0[1] = -0.5f;
    positionLight0[2] = 1.0f;
    positionLight0[3] = 0.0f;

    // スポットライト方向．
    directionLight0[0] = 0.0f;
    directionLight0[1] = 0.0f;
    directionLight0[2] = -1.0f;

    // ここではGL_LIGHT0の一つの光源のみ設定する．
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight0);
    glLightfv(GL_LIGHT0, GL_POSITION, positionLight0);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
}

void AbstractOpenGLBase::drawScenes()
{
    // 行列モード切り替え
    glMatrixMode(GL_MODELVIEW);
    // 行列初期化
    glLoadIdentity();

    glDisable(GL_CULL_FACE);

    glEnable(GL_CULL_FACE);

    return;
}

void AbstractOpenGLBase::renderScenes()
{
    // レンダリングコンテキストをカレントにする
    if (wglMakeCurrent(device_context_handle_ptr, rendering_context_handle))
    {
        // バッファをクリア（指定したバッファを特定の色で消去）
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        // 行列初期化
        glLoadIdentity();

        // 視点決定
        setSceneView(scene_width, scene_height);

        // シーンを描画
        glPushMatrix();
        drawScenes();
        glPopMatrix();

        // コマンドのフラッシュ
        glFlush();

    }

    /**
     *		バッファの切替
     *			バックバッファをフロントバッファに切り替え，新しい画像を見せる
     */
    SwapBuffers(wglGetCurrentDC());

    wglMakeCurrent(device_context_handle_ptr, NULL);
}

/**
 *	説明
 *		描画領域のリサイズ処理
 */
void AbstractOpenGLBase::resizeScenes(int width, int height)
{
    // 行列モードの切り替え
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    /**
     *	gluPerspective(fovy, aspect, zNear, zFar)
     *		fovy: 視野の垂直方向画角（角度）。大きいほど広角レンズ、小さいほど望遠レンズになる。
     *			  180度以上や-も使えるが、45～50度あたりが標準レンズ（人の目に近い）。
     *		aspect: 描画範囲の縦横比。Viewportのwidth/Heightをセットすれば、物体の縦横比が正しく描画される。
     *		zNear: 視点からどれだけ離れた位置から表示するか。必ず0より大きく、かつZFarよりも小さな値を入れる。
     *		zFar: 視点からどれだけ離れた位置まで表示するか。必ず0より大きく、かつZNearよりも大きな値を入れる。
     */
    gluPerspective(PERS_DEFAULT_FOVY, (GLdouble)width / (GLdouble)height, PERS_DEFAULT_NEAR, PERS_DEFAULT_FAR);

    /**
     *	void glViewport(GLint x , GLint y , GLsizei width , GLsizei height);
     *	作成画像を、どこに描画するかを設定する
     *	デフォルトでは、X 座標、Y 座標共に 0、ウィンドウの幅と高さで描画される
     *		x, y: 描画範囲の原点位置を指定。単位はピクセル。ここでは左下が（0, 0）、上方向が+yで設定する。
     *		width, height: 描画範囲の幅、高さを設定。単位はピクセル。
     *
     */
    glViewport(0, 0, (GLint)width, (GLint)height);

    // シーンのサイズを格納
    scene_width = width;
    scene_height = height;

    // 行列モードをモデルビューに戻す
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void AbstractOpenGLBase::setSceneView(const double width, const double height)
{
    // ビュータイプにより視点を変更
    switch (view_type)
    {
        case ViewType::PERSPECTIVE:
        {
            setViewPoint(cameraView.getDistance(),
                         cameraView.getAzimuth(),
                         cameraView.getElevation(),
                         cameraView.getViewCenterPosition(1),
                         cameraView.getViewCenterPosition(2),
                         cameraView.getViewCenterPosition(3),
                         width, height);
            break;
        }
        case ViewType::TOP:
        {
            setViewPoint(300, -90.0, 90.0, 0, 0, 0, width, height);
            break;
        }
        case ViewType::SIDE:
        {
            setViewPoint(300, -90.0, 0.0, 0, 0, 0, width, height);
            break;
        }
        case ViewType::FRONT:
        {
            setViewPoint(300, 0.0, 0.0, 0, 0, 0, width, height);
            break;
        }
        default:
        {
            break;
        }
    }

    return;
}

void AbstractOpenGLBase::setViewPoint(double distance, double azimuth, double elevation,
              double centerX, double centerY, double centerZ,
              double width, double height
)

{
    // 行列モード切り替え
    glMatrixMode(GL_PROJECTION);
    // 行列初期化
    glLoadIdentity();

    glViewport(0, 0, (GLint)width, (GLint)height);
    gluPerspective((GLdouble)45.0,						// 視野角度[deg]
            (GLdouble)(width / height),			// アスペクト比(ウィンドウの幅/高さ)
            (GLdouble)(distance * 0.1),			// 視点から最も近い点までの距離
            (GLdouble)(distance * 3.0)			// 視点から最も遠い点までの距離
    );

    // 行列モード切り替え
    glMatrixMode(GL_MODELVIEW);
    // 行列初期化
    glLoadIdentity();

    /**
     *	ここから gluLookAt のための引数計算
     */
    double theta = azimuth;
    double phi = elevation;

    // 視点の位置
    double eyeX;
    double eyeY;
    double eyeZ;

    // 視界の中心位置の参照点
    double refCenterX;
    double refCenterY;
    double refCenterZ;

    // 視界の上方向のベクトル
    double upDirectionX;
    double upDirectionY;
    double upDirectionZ;

    eyeX = distance * cos(phi * designlab_robot_gui::math::PI / 180) * cos(theta * designlab_robot_gui::math::PI / 180);
    eyeY = distance * cos(phi * designlab_robot_gui::math::PI / 180) * sin(theta * designlab_robot_gui::math::PI / 180);
    eyeZ = distance * sin(phi * designlab_robot_gui::math::PI / 180);

    refCenterX = centerX;
    refCenterY = centerY;
    refCenterZ = centerZ;

    upDirectionX = distance * cos((phi + 90) * designlab_robot_gui::math::PI / 180) * cos(theta * designlab_robot_gui::math::PI / 180);
    upDirectionY = distance * cos((phi + 90) * designlab_robot_gui::math::PI / 180) * sin(theta * designlab_robot_gui::math::PI / 180);
    upDirectionZ = distance * sin((phi + 90) * designlab_robot_gui::math::PI / 180);

    eyeX += refCenterX;
    eyeY += refCenterY;
    eyeZ += refCenterZ;

    // 視点と視線の向きを決定
    gluLookAt((GLdouble)eyeX, (GLdouble)eyeY, (GLdouble)eyeZ,
          (GLdouble)refCenterX, (GLdouble)refCenterY, (GLdouble)refCenterZ,
          (GLdouble)upDirectionX, (GLdouble)upDirectionY, (GLdouble)upDirectionZ);
}

bool AbstractOpenGLBase::setWindowHandle(HWND hWnd)
{
    // 引数チェック
    if (!::IsWindow(hWnd))
    {
        std::cerr << "Error: [AbstractOpenGLBase::setWindowHandle] Invalid Window Handle"
            << std::endl;
        return false;
    }

    return true;
}

void AbstractOpenGLBase::setViewType(const ViewType type)
{
    view_type = type;
}

void AbstractOpenGLBase::setMaterialColor(double red, double green, double blue, double alpha)
{
    GLfloat materialAmbDiff[] = { (GLfloat)red, (GLfloat)green,
                                  (GLfloat)blue, (GLfloat)alpha };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, materialAmbDiff);

    GLfloat materialSpecular[] = { (GLfloat)(red * 0.01), (GLfloat)(green * 0.01),
                                   (GLfloat)(blue * 0.01), (GLfloat)alpha };

    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialSpecular);
}

void AbstractOpenGLBase::setMaterialColor(const GLfloat* materialAmbDiffColor)
{
    setMaterialColor(materialAmbDiffColor[0], materialAmbDiffColor[1],
                     materialAmbDiffColor[2], materialAmbDiffColor[3]);
}

void AbstractOpenGLBase::setMaterialColor(const MaterialColor color)
{
    const GLfloat* materialAmbDiffColor = selectMaterialColor(color);

    setMaterialColor(materialAmbDiffColor[0], materialAmbDiffColor[1],
                     materialAmbDiffColor[2], materialAmbDiffColor[3]);
}


const GLfloat* AbstractOpenGLBase::selectMaterialColor(const MaterialColor color)
{
    switch (color)
    {
        case MaterialColor::RED:
        {
            return materialAmbDiffRed;
        }
        case MaterialColor::GREEN:
        {
            return materialAmbDiffGreen;
        }
        case MaterialColor::BLUE:
        {
            return materialAmbDiffBlue;
        }
        case MaterialColor::YELLOW:
        {
            return materialAmbDiffYellow;
        }
        case MaterialColor::GRAY:
        {
            return materialAmbDiffGray;
        }
        case MaterialColor::BLACK:
        {
            return materialAmbDiffBlack;
        }
        case MaterialColor::WHITE:
        {
            return materialAmbDiffWhite;
        }
        case MaterialColor::SKY:
        {
            return materialAmbDiffSky;
        }
        case MaterialColor::ORANGE:
        {
            return materialAmbDiffOrange;
        }
        case MaterialColor::PURPLE:
        {
            return materialAmbDiffPurple;
        }
        default:
        {
            return nullptr;
        }
    }

    return nullptr;
}

void AbstractOpenGLBase::BeginCameraViewControl(const CameraView::Mode mode, const  int x, const int y)
{
    cameraView.BeginViewControl(mode, x, y);
}

void AbstractOpenGLBase::EndCameraViewControl()
{
    cameraView.EndViewControl();
}

void AbstractOpenGLBase::DoCameraViewControl(const int x, const  int y)
{
    cameraView.DoViewControl(x, y);
}

bool AbstractOpenGLBase::SetWindowPixelFormat()
{
    PIXELFORMATDESCRIPTOR pfd;

    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);  // この構造体のサイズ．
    pfd.nVersion = 1;  // Version number : must be 1．

    // ウィンドウへの描画をサポート，OpenGLをサポート，ダブルバッファリングをサポート．
    // mesoscopic か stereoscopic をサポート．
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_STEREO_DONTCARE;

    pfd.iPixelType = PFD_TYPE_RGBA;     // RGBA タイプ．
    pfd.cColorBits = 32;                // 利用できるカラーの数を決める．
    pfd.cRedBits = 8;                   // RGBAカラーバッファの赤ビット数．
    pfd.cRedShift = 16;                 // RGBAカラーバッファの赤ビットのためのシフトカウント．
    pfd.cGreenBits = 8;                 // RGBAカラーバッファの緑ビット数．
    pfd.cGreenShift = 8;                // RGBAカラーバッファの緑ビットのためのシフトカウント．
    pfd.cBlueBits = 8;                  // RGBAカラーバッファの青ビット数．
    pfd.cBlueShift = 0;                 // RGBAカラーバッファの青ビットのためのシフトカウント．
    pfd.cAlphaBits = 0;                 // RGBAカラーバッファのアルファビット数．
    pfd.cAlphaShift = 0;                // RGBAカラーバッファのアルファビットのためのシフトカウント．
    pfd.cAccumBits = 64;                // アキュームレーションバッファのピクセル当りのビット数．
    pfd.cAccumRedBits = 16;             // アキュームレーションバッファのピクセル当りの赤ビット数．
    pfd.cAccumGreenBits = 16;           // アキュームレーションバッファのピクセル当りの緑ビット数．
    pfd.cAccumBlueBits = 16;            // アキュームレーションバッファのピクセル当りの青ビット数．
    pfd.cAccumAlphaBits = 0;            // アキュームレーションバッファのピクセル当りのアルファビット数．
    pfd.cDepthBits = 32;                // デプスバッファのピクセル当りのビット数．
    pfd.cStencilBits = 8;               // ステンシルバッファのピクセル当りのビット数．
    pfd.cAuxBuffers = 0;                // 補助バッファの数. Win32上ではサポート外．
    pfd.iLayerType = PFD_MAIN_PLANE;    // レイヤタイプの記述. 現在は使用されてない．
    pfd.bReserved = 0;                  // オーバーレイとアンダーレイの plane の数. (0でなければならない)
    pfd.dwLayerMask = 0;                // レイヤマスクの記述. 現在では使用されていない．
    pfd.dwVisibleMask = 0;              // ビジブルマスクの設定．
    pfd.dwDamageMask = 0;               // ダメージマスクの設定. 現在では使用されていない．

    // 要求されたピクセルフォーマットに最も近いピクセルフォーマットを返す
    int pixelFormat = ChoosePixelFormat(device_context_handle_ptr, &pfd);

    if (pixelFormat == 0)		// Choose default
    {
        pixelFormat = 1;

        // 指定したピクセルフォーマットに関する情報を得る．
        if (DescribePixelFormat(device_context_handle_ptr, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd) == 0)
        {
            // 失敗した場合は false を返す．
            return false;
        }
    }

    // 指定したデバイスコンテキストのピクセルフォーマットを設定する
    if (!SetPixelFormat(device_context_handle_ptr, pixelFormat, &pfd))
    {
        // 失敗した場合は false を返す．
        return false;
    }

    return true;
}


bool AbstractOpenGLBase::CreateGLContext()
{
    // OpenGLのレンダリングコンテキストを作成．
    rendering_context_handle = wglCreateContext(device_context_handle_ptr);

    if (!rendering_context_handle)
    {
        // 作成不可ならば，false を返す．
        return false;
    }

    // 現在のレンダリングコンテキストにセット．
    if (!wglMakeCurrent(device_context_handle_ptr, rendering_context_handle))
    {
        // 失敗した場合は false を返す．
        return false;
    }

    return true;
}


}  // namespace designlab_robot_gui::graphic
