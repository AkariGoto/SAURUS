
#include "OpenGLObject.h"

#include "Math/Matrix/matrix_library.h"


namespace designlab_robot_gui::graphic
{

bool OpenGLObject::createGLObject(HWND hWnd)
{
    // 最初にOpenGLのデフォルト初期化を行う．
    // 継承前のクラスの関数を呼び出す．
    if (!AbstractOpenGLBase::InitializeGL(hWnd))
    {
        // 失敗したら false を返す．
        return false;
    }

    // ディスプレイリスト作成．
    newGLObjDispList();

    // フラグ類の初期化．
    is_drawn_coordinate_axis_ = true;
    is_drawn_grid_floor_ = true;

    return true;
}

void OpenGLObject::destroyGLObject()
{
    is_drawn_coordinate_axis_ = false;
    is_drawn_grid_floor_ = false;

    // ディスプレイリストの破棄．
    deleteObjDispList();

    // 最後に基底クラスの終了処理を呼ぶ．
    AbstractOpenGLBase::FinalizeGL();
}

void OpenGLObject::drawObjects(const GLenum render_mode) const
{
    switch (render_mode)
    {
        case GL_RENDER:
        {
            glDisable(GL_CULL_FACE);

            // 座標系描画．
            DrawCoordinateAxis();

            glEnable(GL_CULL_FACE);
            break;
        }
        case GL_SELECT:
        {
            break;
        }
        default:
        {
            break;
        }
    }
}

void OpenGLObject::DrawScenes() const
{
    drawObjects(GL_RENDER);
}

void OpenGLObject::RenderScenes()
{
    // レンダリングコンテキストをカレントにする
    if (wglMakeCurrent(device_context_handle_ptr_, rendering_context_handle_))
    {
        // バッファをクリア（指定したバッファを特定の色で消去）
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glLoadIdentity();

        // 視点決定．
        setSceneView(scene_width_, scene_height_);

        // 今の座標系を保存しておく．
        glPushMatrix();

        {
            // シーンを描画.
            DrawScenes();
        }

        // 座標系を戻す．
        glPopMatrix();

        // コマンドのフラッシュ．
        glFlush();
    }

    // バッファの切替．
    // バックバッファをフロントバッファに切り替え，新しい画像を見せる．
    SwapBuffers(wglGetCurrentDC());

    wglMakeCurrent(device_context_handle_ptr_, nullptr);
}

void OpenGLObject::DrawCoordinateAxis(const double length, const double width,
                                      const double scale) const
{
    if (!is_drawn_coordinate_axis_)
    {
        return;
    }

    // 球の半径．
    const double radius = 50.0;

    glPushMatrix();

    // x軸の描画（赤）
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialAmbDiffRed);
    glLineWidth((GLfloat)(width * scale));
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f((GLfloat)(length * scale), 0.0f, 0.0f);
    glEnd();

    glTranslatef((GLfloat)(length * scale), 0.0f, 0.0f);
    DrawSphere(radius * scale);
    glPopMatrix();

    glPushMatrix();

    // y軸の描画（緑）
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialAmbDiffGreen);
    glLineWidth((GLfloat)(width * scale));
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, (GLfloat)(length * scale), 0.0f);
    glEnd();

    glTranslatef(0.0f, (GLfloat)(length * scale), 0.0f);
    DrawSphere(radius * scale);
    glPopMatrix();

    glPushMatrix();

    // z軸の描画（青）
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialAmbDiffBlue);
    glLineWidth((GLfloat)(width * scale));
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, (GLfloat)(length * scale));
    glEnd();

    glTranslatef(0.0f, 0.0f, (GLfloat)(length * scale));
    DrawSphere(radius * scale);
    glPopMatrix();

    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialAmbDiffYellow);
    DrawSphere(radius * scale);
}

void OpenGLObject::DrawCylinder(const double radius, const double height) const
{
    // quadric object を一つ生成する．
    GLUquadricObj* quad = gluNewQuadric();

    // 面の塗り潰しを指定する（線画ではなく陰影をつけた円柱を描く）
    gluQuadricDrawStyle(quad, GLU_FILL);

    // スムースシェーディングを行うよう設定する．
    gluQuadricNormals(quad, GLU_SMOOTH);

    glPushMatrix();
    glTranslated((GLdouble)0, 0, (GLdouble)(-0.5 * height));
    gluCylinder(quad, (GLdouble)radius, (GLdouble)radius, (GLdouble)height,
                (GLint)16, (GLint)1);
    gluDisk(quad, (GLdouble)0, (GLdouble)radius, (GLint)16, (GLint)1);
    glTranslated((GLdouble)0, 0, (GLdouble)height);
    gluDisk(quad, (GLdouble)0, (GLdouble)radius, (GLint)16, (GLint)1);
    glPopMatrix();

    // 生成した quadric object を削除．
    gluDeleteQuadric(quad);
}


void OpenGLObject::DrawSphere(const double radius) const
{
    // quadric object を一つ生成する．
    GLUquadricObj* quad = gluNewQuadric();

    // 面の塗り潰しを指定する(線画ではなく陰影をつけた円柱を描く)
    gluQuadricDrawStyle(quad, GLU_FILL);

    // スムースシェーディングを行うよう設定する．
    gluQuadricNormals(quad, GLU_SMOOTH);

    // 球面．
    gluSphere(quad, (GLdouble)radius, (GLint)16, (GLint)16);

    // 生成した quadric object を削除．
    gluDeleteQuadric(quad);
}

void OpenGLObject::DrawBox(const double start_x, const double start_y, const double start_z,
                           const double endX, const double endY, const double endZ) const
{
    // 第1面描画(x-y平面に平行な面)
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f((GLfloat)start_x, (GLfloat)endY, (GLfloat)endZ);
    glVertex3f((GLfloat)start_x, (GLfloat)start_y, (GLfloat)endZ);
    glVertex3f((GLfloat)endX, (GLfloat)start_y, (GLfloat)endZ);
    glVertex3f((GLfloat)endX, (GLfloat)endY, (GLfloat)endZ);
    glEnd();

    // 第2面描画(y-z平面に平行な面)
    glBegin(GL_POLYGON);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f((GLfloat)endX, (GLfloat)endY, (GLfloat)endZ);
    glVertex3f((GLfloat)endX, (GLfloat)start_y, (GLfloat)endZ);
    glVertex3f((GLfloat)endX, (GLfloat)start_y, (GLfloat)start_z);
    glVertex3f((GLfloat)endX, (GLfloat)endY, (GLfloat)start_z);
    glEnd();

    // 第3面描画(x-y平面に平行な面)
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f((GLfloat)endX, (GLfloat)endY, (GLfloat)start_z);
    glVertex3f((GLfloat)endX, (GLfloat)start_y, (GLfloat)start_z);
    glVertex3f((GLfloat)start_x, (GLfloat)start_y, (GLfloat)start_z);
    glVertex3f((GLfloat)start_x, (GLfloat)endY, (GLfloat)start_z);
    glEnd();

    // 第4面描画(y-z平面に平行な面)
    glBegin(GL_POLYGON);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f((GLfloat)start_x, (GLfloat)endY, (GLfloat)start_z);
    glVertex3f((GLfloat)start_x, (GLfloat)start_y, (GLfloat)start_z);
    glVertex3f((GLfloat)start_x, (GLfloat)start_y, (GLfloat)endZ);
    glVertex3f((GLfloat)start_x, (GLfloat)endY, (GLfloat)endZ);
    glEnd();

    // 第5面描画(x-z平面に平行な面)
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f((GLfloat)start_x, (GLfloat)start_y, (GLfloat)endZ);
    glVertex3f((GLfloat)start_x, (GLfloat)start_y, (GLfloat)start_z);
    glVertex3f((GLfloat)endX, (GLfloat)start_y, (GLfloat)start_z);
    glVertex3f((GLfloat)endX, (GLfloat)start_y, (GLfloat)endZ);
    glEnd();

    // 第6面描画(x-z平面に平行な面)
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f((GLfloat)start_x, (GLfloat)endY, (GLfloat)start_z);
    glVertex3f((GLfloat)start_x, (GLfloat)endY, (GLfloat)endZ);
    glVertex3f((GLfloat)endX, (GLfloat)endY, (GLfloat)endZ);
    glVertex3f((GLfloat)endX, (GLfloat)endY, (GLfloat)start_z);
    glEnd();
}

void OpenGLObject::DrawBox(const Vector& startPoint, const Vector& endPoint)
{
    using designlab_robot_gui::math::THREE_DIMENSION;

    // 引数チェック
    assert(startPoint.getSize() == THREE_DIMENSION);
    assert(endPoint.getSize() == THREE_DIMENSION);

    DrawBox(startPoint(1), startPoint(2), startPoint(3), endPoint(1), endPoint(2), endPoint(3));

    return;
}

void OpenGLObject::drawFrame(double startX, double startY, double startZ, double endX, double endY, double endZ)
{
    // 色をグレーに設定
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialAmbDiffGray);
    // 線幅決定
    glLineWidth(4.0f);

    // 平面描画
    glBegin(GL_LINE_LOOP);
    glVertex3f((GLfloat)endX, (GLfloat)endY, (GLfloat)startZ);
    glVertex3f((GLfloat)endX, (GLfloat)startY, (GLfloat)startZ);
    glVertex3f((GLfloat)startX, (GLfloat)startY, (GLfloat)startZ);
    glVertex3f((GLfloat)startX, (GLfloat)endY, (GLfloat)startZ);
    glEnd();

    // 以下で垂線描画
    glBegin(GL_LINES);
    glVertex3f((GLfloat)endX, (GLfloat)startY, (GLfloat)endZ);
    glVertex3f((GLfloat)endX, (GLfloat)startY, (GLfloat)startZ);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f((GLfloat)startX, (GLfloat)startY, (GLfloat)startZ);
    glVertex3f((GLfloat)startX, (GLfloat)startY, (GLfloat)endZ);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f((GLfloat)endX, (GLfloat)endY, (GLfloat)startZ);
    glVertex3f((GLfloat)endX, (GLfloat)endY, (GLfloat)endZ);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f((GLfloat)startX, (GLfloat)endY, (GLfloat)startZ);
    glVertex3f((GLfloat)startX, (GLfloat)endY, (GLfloat)endZ);
    glEnd();
}

void OpenGLObject::drawFrame(const Vector& startPoint, const Vector& endPoint)
{
    assert(startPoint.getSize() == math::THREE_DIMENSION);
    assert(endPoint.getSize() == math::THREE_DIMENSION);

    drawFrame(startPoint(1), startPoint(2), startPoint(3),
              endPoint(1), endPoint(2), endPoint(3));
}


void OpenGLObject::DrawLine(const double start_x, const double start_y, const double start_z,
                            const double end_x, const double end_y, const  double end_z,
                            const double width) const
{
    // 線の太さ決定．
    glLineWidth((GLfloat)width);

    // 線の描画．
    glBegin(GL_LINES);
    glVertex3f((GLfloat)start_x, (GLfloat)start_y, (GLfloat)start_z);
    glVertex3f((GLfloat)end_x, (GLfloat)end_y, (GLfloat)end_z);
    glEnd();
}

void OpenGLObject::DrawLine(const Vector& startPoint, const Vector& endPoint,
                            const double width) const
{
    assert(startPoint.getSize() == math::THREE_DIMENSION);
    assert(endPoint.getSize() == math::THREE_DIMENSION);

    // 実際に描画する．
    DrawLine(startPoint(1), startPoint(2), startPoint(3),
             endPoint(1), endPoint(2), endPoint(3), width);
}

// 三角形描画
void OpenGLObject::drawTriangle(const Vector& point1, const Vector& point2,
                                const Vector& point3, double width)
{
    using designlab_robot_gui::math::THREE_DIMENSION;

    // 引数チェック
    assert(point1.getSize() == THREE_DIMENSION);
    assert(point2.getSize() == THREE_DIMENSION);
    assert(point3.getSize() == THREE_DIMENSION);

    // 実際に描画
    DrawLine(point1, point2, width);
    DrawLine(point2, point3, width);
    DrawLine(point3, point1, width);

}

// 四角形描画
void OpenGLObject::drawQuadrangle(const Vector& point1, const Vector& point2,
                                  const Vector& point3, const Vector& point4,
                                  double width)
{
    using designlab_robot_gui::math::THREE_DIMENSION;

    // 引数チェック
    assert(point1.getSize() == THREE_DIMENSION);
    assert(point2.getSize() == THREE_DIMENSION);
    assert(point3.getSize() == THREE_DIMENSION);
    assert(point4.getSize() == THREE_DIMENSION);

    DrawLine(point1, point2, width);
    DrawLine(point2, point3, width);
    DrawLine(point3, point1, width);
    DrawLine(point1, point2, width);
}

/**
 *	----------------------------------------
 *	2Dモデリング
 *	----------------------------------------
 */
 /**
  *		平面板描画(対角線の頂点と高さ)
  */
  // 水平面（XY
void OpenGLObject::drawHorizontalPlane(double startX, double startY, double endX, double endY, double z)
{
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f((GLfloat)startX, (GLfloat)startY, (GLfloat)z);
    glVertex3f((GLfloat)startX, (GLfloat)endY, (GLfloat)z);
    glVertex3f((GLfloat)endX, (GLfloat)endY, (GLfloat)z);
    glVertex3f((GLfloat)endX, (GLfloat)startY, (GLfloat)z);
    glEnd();
}

// 前額面（YZ）
void OpenGLObject::drawFrontalPlane(double startY, double startZ, double endY, double endZ, double x)
{
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f((GLfloat)x, (GLfloat)startY, (GLfloat)startZ);
    glVertex3f((GLfloat)x, (GLfloat)startY, (GLfloat)endZ);
    glVertex3f((GLfloat)x, (GLfloat)endY, (GLfloat)endZ);
    glVertex3f((GLfloat)x, (GLfloat)endY, (GLfloat)startZ);
    glEnd();
}

// 矢状面（ZX）
void OpenGLObject::drawSagittalPlane(double startZ, double startX, double endZ, double endX, double y)
{
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f((GLfloat)startX, (GLfloat)y, (GLfloat)startZ);
    glVertex3f((GLfloat)startX, (GLfloat)y, (GLfloat)endZ);
    glVertex3f((GLfloat)endX, (GLfloat)y, (GLfloat)endZ);
    glVertex3f((GLfloat)endX, (GLfloat)y, (GLfloat)startZ);
    glEnd();
}

void OpenGLObject::drawGridFloor(void)
{
    if (!is_drawn_grid_floor_)
    {
        return;
    }

    glCallList(grid_floor_display_list_id_);
}

void OpenGLObject::newGLObjDispList()
{
    // ディスプレイリストIDの作成．
    grid_floor_display_list_id_ = glGenLists(1);

    const GLfloat glidColor[4] = { 0.1f, 0.1f, 0.3f, 1.0f };
    const GLfloat planeColor[4] = { 0.2f, 0.4, 0.6f, 1.0f };

    // ディスプレイリストの作成．
    glNewList(grid_floor_display_list_id_, GL_COMPILE);

    setMaterialColor(planeColor);

    // 平面の描画．
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f((GLfloat)GRID_START_X, (GLfloat)GRID_START_Y, (GLfloat)GRID_HEIGHT);
    glVertex3f((GLfloat)GRID_START_X, (GLfloat)GRID_END_Y, (GLfloat)GRID_HEIGHT);
    glVertex3f((GLfloat)GRID_END_X, (GLfloat)GRID_END_Y, (GLfloat)GRID_HEIGHT);
    glVertex3f((GLfloat)GRID_END_X, (GLfloat)GRID_START_Y, (GLfloat)GRID_HEIGHT);
    glEnd();


    // グリッドの描画．

    // 色の選択．
    setMaterialColor(glidColor);

    Vector start(3);
    Vector end(3);

    // 高さ固定．
    start(3) = GRID_HEIGHT + 1.0;
    end(3) = GRID_HEIGHT + 1.0;

    // x方向の描画．
    start(2) = GRID_START_Y;
    end(2) = GRID_END_Y;

    for (double x = GRID_START_X; x <= GRID_END_X; x += GRID_DX)
    {
        start(1) = x;
        end(1) = x;
        DrawLine(start, end, 1.5);
    }

    // y方向の描画．
    start(1) = GRID_START_X;
    end(1) = GRID_END_X;

    for (double y = GRID_START_Y; y <= GRID_END_Y; y += GRID_DY)
    {
        start(2) = y;
        end(2) = y;
        DrawLine(start, end, 1.5);
    }

    // ディスプレイリストの保存．
    glEndList();
}

void OpenGLObject::deleteObjDispList()
{
    // ディスプレイリストの消去
    glDeleteLists(grid_floor_display_list_id_, 1);
}

}	// end of namespace Graphic