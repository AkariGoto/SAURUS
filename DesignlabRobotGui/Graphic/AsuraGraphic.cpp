
#include "AsuraGraphic.h"

#include "Math/math_constant.h"


namespace designlab_robot_gui::graphic
{

AsuraGraphic::AsuraGraphic(HWND hWnd, AsuraData* asuraData, ViewType type)
{
    // 初期化．
    initializeAsuraGraphic(hWnd, asuraData);

    // 視点を決定．
    SetViewType(type);
}

/// デストラクタ
AsuraGraphic::~AsuraGraphic()
{
    finalizeAsuraGraphic();
}

void AsuraGraphic::initializeAsuraGraphic(HWND hWnd, AsuraData* asuraData)
{
    // フラグ初期化
    isAsuraDrawn = true;
    isSupportPolygonDrawn = false;
    isDisplayListReady = false;
    is_recorded_animation_ = false;

    is_displayed_leg_coordinate_ = false;  // 起動時は脚座標系を表示しない

    // 最初にOpenGLのデフォルト初期化を行う
    if (!OpenGLObject::createGLObject(hWnd))
    {
        return;
    }

    // ポインタが有効だったらポインタをセット
    if (asuraData != nullptr)
    {
        asura_data_ptr_ = asuraData;
    }

    // ディスプレイリスト作成
    newAsuraDisplayList();

    // 影の準備
    GLfloat floorP1[3] = { 1.0f, 0.0f, (GLfloat)GRID_HEIGHT };
    GLfloat floorP2[3] = { 0.0f, 1.0f, (GLfloat)GRID_HEIGHT };
    GLfloat floorP3[3] = { -1.0f, -1.0f, (GLfloat)GRID_HEIGHT };

    calcShadowPlaneEquation(planeFactorOfGridFloor, floorP1, floorP2, floorP3);

    RECT rect;
    GetClientRect(hWnd, &rect);

    // AVI準備
    movieRecorder.initialize((rect.right / 4) * 4, (rect.bottom / 4) * 4, 24);
}

void AsuraGraphic::finalizeAsuraGraphic()
{
    // ディスプレイリスト破棄
    deleteAsuraDisplayList();
}

void AsuraGraphic::newAsuraDisplayList()
{
    using designlab_robot_gui::asura::BODY_LENGTH;
    using designlab_robot_gui::asura::BODY_WIDTH;
    using designlab_robot_gui::asura::BODY_HEIGHT;
    using designlab_robot_gui::asura::LINK1;
    using designlab_robot_gui::asura::LINK2;
    using designlab_robot_gui::asura::LINK3;
    using designlab_robot_gui::asura::FOOT;

    if (isDisplayListReady)
    {
        return;
    }

    // ディスプレイリストの生成
    int bodyList = glGenLists(1);
    int legList = glGenLists(LEG_DISP_LIST_NUM);


    // 胴体用のディスプレイリスト
    bodyDisplayListID = bodyList;

    // 脚用のディスプレイリストの準備
    for (int i = 0; i < LEG_DISP_LIST_NUM; i++)
    {
        legDisplayListID[i] = legList + i;
    }

    // 関節色を設定．
    const GLfloat jointColor[4] = { 0.55f, 0.55f, 0.65f, 1.0f };
    const GLfloat linkColor[4] = { 0.156f, 0.40f, 1.0f, 1.0f };


    // body
    glNewList(bodyDisplayListID, GL_COMPILE);
    setMaterialColor(linkColor);

    DrawBox(-BODY_LENGTH / 2, -BODY_WIDTH / math::ROOT_2 / 2 + 50, -BODY_HEIGHT / 2,
            BODY_LENGTH / 2, BODY_WIDTH / math::ROOT_2 / 2 - 50, BODY_HEIGHT / 2);
    DrawBox(-BODY_LENGTH / math::ROOT_2 / 2 + 50, -BODY_WIDTH / 2, -BODY_HEIGHT / 2,
            BODY_LENGTH / math::ROOT_2 / 2 - 50, BODY_WIDTH / 2, BODY_HEIGHT / 2);

    glEndList();


    // 脚用のディスプレイリストの作成

    // Base
    glNewList(legDisplayListID[0], GL_COMPILE);
    setMaterialColor(jointColor);	///関節	/// 原点
    DrawCylinder(30, BODY_HEIGHT + 10);
    glEndList();

    // 1.Hip
    glNewList(legDisplayListID[1], GL_COMPILE);
    setMaterialColor(jointColor);	///関節	/// 原点
    DrawCylinder(30, BODY_HEIGHT + 10);
    glTranslated(-LINK1, 0, 0);	/// 座標変換

    setMaterialColor(linkColor);
    DrawBox(0, -35, -35, LINK1, 35, 35);
    glEndList();

    /// 2.Thigh
    glNewList(legDisplayListID[2], GL_COMPILE);
    setMaterialColor(jointColor);///関節	
    DrawCylinder(30, 60);
    //DrawCylinder(40, 70);ASURA_20231128変更
    glTranslated(-LINK2, 0, 0);	/// 座標変換
    setMaterialColor(linkColor);
    DrawBox(0, -30, -30, LINK2, 30, 30);
    setMaterialColor(jointColor);///関節	
    DrawCylinder(30, 60);
    //DrawCylinder(40, 110);ASURA_20231128変更
    glEndList();

    // 3.Shank
    glNewList(legDisplayListID[3], GL_COMPILE);
    glTranslated(-LINK3, 0, 0);	/// 座標変換
    setMaterialColor(linkColor);
    DrawBox(0, -25, -25, LINK3, 25, 25);
    setMaterialColor(jointColor);///関節	
    DrawCylinder(30, 60);
    //DrawCylinder(40, 70); ASURA_20231128変更
    glEndList();

    // 4.Foot
    glNewList(legDisplayListID[4], GL_COMPILE);
    glTranslated(-FOOT, 0, 0);  // 座標変換
    setMaterialColor(linkColor);
    DrawBox(0, -25, -25, FOOT, 25, 25);  // DrawBox(3*FOOT/5, -40, -35, FOOT, 40, 35);
    setMaterialColor(jointColor);  // 関節	
    DrawCylinder(30, 60);
    // DrawCylinder(40, 70); ASURA_20231128変更
    glEndList();

    // 準備完了
    isDisplayListReady = true;
}

void AsuraGraphic::deleteAsuraDisplayList()
{
    // ディスプレイリストの消去
    glDeleteLists(bodyDisplayListID, 1);
    glDeleteLists(legDisplayListID[0], LEG_DISP_LIST_NUM);
}

void AsuraGraphic::drawObjects(GLenum renderMode) const
{
    Matrix gAb(math::DH_DIMENSION, math::DH_DIMENSION);

    switch (renderMode)
    {
        case GL_RENDER:
        {
            /// シーンを描画
            glDisable(GL_CULL_FACE);
            /// 座標系描画
            DrawCoordinateAxis(300.0, 5.0, 0.8);
            /// Asura描画
            drawAsura();
            glEnable(GL_CULL_FACE);

            //20201019
            if (is_displayed_leg_coordinate_)
            {
                glDisable(GL_CULL_FACE);
                glPushMatrix();
                /// 脚座標系に変換
                gAb = asura_data_ptr_->leg_base_transformation[1];
                transformOpenGLMatrix(gAb);
                DrawCoordinateAxis(300.0, 5.0, 0.8);
                glPopMatrix();
                glEnable(GL_CULL_FACE);
            }

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

void AsuraGraphic::DrawScenes() const
{
    drawObjects(GL_RENDER);
    return;
}

void AsuraGraphic::DrawShadow()
{
    // 影の光源
    float lightPosition[] = { 60, 80, 3000, 1 };

    // 投影行列：4x4の同時変換行列
    GLfloat shadowMatrix[16];

    // ステンシルバッファクリア値の設定
    glClearStencil(0);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_POSITION, (float*)lightPosition);

    // 平面射影行列の算出
    calcShadowMatrix(shadowMatrix, planeFactorOfGridFloor, lightPosition);


    // 床のステンシルを付ける
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 1, ~0);

    // これから描画するもののステンシル値にすべて１タグをつける
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    // 床の描画
    drawGridFloor();

    /**
     *		カラー・デプスバッファマスクをセットする
     *		これで以下の内容のピクセルの色の値は、書き込まれない。
     */
    glColorMask(0, 0, 0, 0);
    glDepthMask(0);

    /**
     *		床にオブジェクトの影のステンシルを付ける
     */
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_EQUAL, 1, ~0);

    /**
     *		これから描画するもののステンシル値にすべて１タグをつける
     */
    glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
    glDisable(GL_DEPTH_TEST);
    glPushMatrix();
    glMultMatrixf(shadowMatrix);
    glDisable(GL_CULL_FACE);
    drawAsura();
    glEnable(GL_CULL_FACE);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);

    /**
     *		ビットマスクを解除
     */
    glColorMask(1, 1, 1, 1);
    glDepthMask(1);

    glCullFace(GL_FRONT);
    glEnable(GL_CULL_FACE);
    glEnable(GL_AUTO_NORMAL);


    /**
     *		影をつける
     */
    glStencilFunc(GL_EQUAL, 2, ~0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.1f, 0.1f, 0.1f, 0.5f);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);
    drawGridFloor();
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_STENCIL_TEST);

    glDisable(GL_AUTO_NORMAL);
    glDisable(GL_NORMALIZE);
}

void AsuraGraphic::RenderScenes()
{
    // レンダリングコンテキストをカレントにする
    if (wglMakeCurrent(device_context_handle_ptr_, rendering_context_handle_))
    {
        // バッファをクリア（指定したバッファを特定の色で消去）
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glLoadIdentity();

        // 視点決定
        setSceneView(scene_width_, scene_height_);


        // オブジェクトの描画

        // 今の座標系を保存しておく
        glPushMatrix();

        // シーンを描画
        DrawScenes();

        // 影描画
        DrawShadow();

        // 支持脚多角形描画
        drawSupportPolygon();


        // 座標系を戻す
        glPopMatrix();

        // コマンドのフラッシュ
        if (is_recorded_animation_)
        {
            movieRecorder.recordFrame();
        }
        else
        {
            glFlush();
        }
    }

    SwapBuffers(wglGetCurrentDC());

    wglMakeCurrent(device_context_handle_ptr_, NULL);

    return;
}

void AsuraGraphic::setSceneView(const double width, const double height)
{
    // view_type_ により視点を変更する．
    switch (view_type_)
    {
        case ViewType::PERSPECTIVE:
        {
            setViewPoint(camera_view_.GetDistance(),
                         camera_view_.GetAzimuth(),
                         camera_view_.GetElevation(),
                         camera_view_.GetViewCenterPos(1),
                         camera_view_.GetViewCenterPos(2),
                         camera_view_.GetViewCenterPos(3),
                         width, height);
            break;
        }
        case ViewType::TOP:
        {
            setViewPoint(2500.0, -90.0, 90.0,
                         asura_data_ptr_->body_position(1),
                         asura_data_ptr_->body_position(2),
                         asura_data_ptr_->body_position(3) - 100,
                         width, height);

            break;
        }
        case ViewType::SIDE:
        {
            setViewPoint(1500.0, -90.0, 0.0,
                         asura_data_ptr_->body_position(1),
                         asura_data_ptr_->body_position(2),
                         asura_data_ptr_->body_position(3) - 100,
                         width, height);

            break;
        }
        case ViewType::FRONT:
        {
            setViewPoint(1500.0, 0.0, 0.0,
                         asura_data_ptr_->body_position(1),
                         asura_data_ptr_->body_position(2),
                         asura_data_ptr_->body_position(3) - 100,
                         width, height);
            break;
        }
        default:
        {
            break;
        }
    }
}

void AsuraGraphic::drawAsura() const
{
    if (!isAsuraDrawn)
    {
        return;
    }

    // 胴体描画
    drawBody();

    // ロコモーション様式により描画を分ける．
    if (asura_data_ptr_->locomotion_style == asura::LocomotionStyle::LEGGED)
    {
        int i = 2;  // 2脚目のみを描画．

        drawLeg(i);
    }
}

void AsuraGraphic::drawBody() const
{
    // グローバルから胴体座標への変換行列．
    Matrix gAb(math::DH_DIMENSION, math::DH_DIMENSION);

    glPushMatrix();

    // 胴体座標系に変換．
    gAb = asura_data_ptr_->body_transformation;
    transformOpenGLMatrix(gAb);
    glCallList(bodyDisplayListID);
    glPopMatrix();
}

void AsuraGraphic::drawLeg(const int legNo) const
{
    if (!isDisplayListReady)
    {
        return;
    }

    // 脚の引数チェック
    assert(1 <= legNo && legNo <= asura::LEG_NUM);

    Matrix trans(math::DH_DIMENSION, math::DH_DIMENSION);	// 座標変換用の行列
    Matrix bAl(math::DH_DIMENSION, math::DH_DIMENSION);		// 胴体 -> 脚


    glPushMatrix();

    /// 脚のベース座標系を取得して，脚座標に変換
    glPushMatrix();
    bAl = asura_data_ptr_->leg_base_transformation[legNo - 1];
    transformOpenGLMatrix(bAl);
    glCallList(legDisplayListID[0]);
    glPopMatrix();

    /// hip
    glPushMatrix();
    trans = asura_data_ptr_->leg_joint_transformation[legNo - 1][0];
    transformOpenGLMatrix(trans);
    glCallList(legDisplayListID[1]);
    glPopMatrix();

    /// shank
    glPushMatrix();
    trans = asura_data_ptr_->leg_joint_transformation[legNo - 1][1];
    transformOpenGLMatrix(trans);
    glCallList(legDisplayListID[2]);
    glPopMatrix();

    /// knee
    glPushMatrix();
    trans = asura_data_ptr_->leg_joint_transformation[legNo - 1][2];
    transformOpenGLMatrix(trans);
    glCallList(legDisplayListID[3]);
    glPopMatrix();

    /// foot
    glPushMatrix();
    trans = asura_data_ptr_->leg_foot_transformation[legNo - 1];
    transformOpenGLMatrix(trans);
    glCallList(legDisplayListID[4]);
    glPopMatrix();

    glPopMatrix();
}




void AsuraGraphic::drawSupportPolygon()
{
    // 描画フラグ
    if (!isSupportPolygonDrawn)
    {
        return;
    }

    int i;
    designlab_robot_gui::asura::LegPhase phase[designlab_robot_gui::asura::LEG_NUM];

    for (i = 0; i < designlab_robot_gui::asura::LEG_NUM; i++)
    {
        phase[i] = asura_data_ptr_->leg_phase[i];
    }

    /// 支持脚多角形の色決定
    setMaterialColor(selectMaterialColor(designlab_robot_gui::graphic::MaterialColor::YELLOW));

    if (phase[0] == designlab_robot_gui::asura::LegPhase::SWING)
    {
        drawTriangle(asura_data_ptr_->leg_foot_position[1], asura_data_ptr_->leg_foot_position[2],
                     asura_data_ptr_->leg_foot_position[3], 2.0);
    }
    else if (phase[1] == designlab_robot_gui::asura::LegPhase::SWING)
    {
        drawTriangle(asura_data_ptr_->leg_foot_position[0], asura_data_ptr_->leg_foot_position[2],
                     asura_data_ptr_->leg_foot_position[3], 2.0);
    }
    else if (phase[2] == designlab_robot_gui::asura::LegPhase::SWING)
    {
        drawTriangle(asura_data_ptr_->leg_foot_position[0], asura_data_ptr_->leg_foot_position[1],
                     asura_data_ptr_->leg_foot_position[3], 2.0);
    }
    else if (phase[3] == designlab_robot_gui::asura::LegPhase::SWING)
    {
        drawTriangle(asura_data_ptr_->leg_foot_position[0], asura_data_ptr_->leg_foot_position[1],
                     asura_data_ptr_->leg_foot_position[2], 2.0);
    }
    else
    {
        drawQuadrangle(asura_data_ptr_->leg_foot_position[0], asura_data_ptr_->leg_foot_position[1],
                    asura_data_ptr_->leg_foot_position[3], asura_data_ptr_->leg_foot_position[2],
                    2.0);
    }
}

void AsuraGraphic::transformOpenGLMatrix(const Matrix& matrix) const
{
    /// 変換行列要素
    double T[16];

    int k = 0;

    // 行列要素を配列に格納
    for (int i = 1; i <= math::DH_DIMENSION; i++)
    {
        for (int j = 1; j <= math::DH_DIMENSION; j++)
        {
            T[k] = matrix(j, i);
            k++;
        }
    }

    // 現在のマトリクスと掛ける
    glMultMatrixd(T);
}

void AsuraGraphic::calcShadowPlaneEquation(GLfloat planeEq[], GLfloat p0[],
                                           GLfloat p1[], GLfloat p2[])
{
    GLfloat vector0[3], vector1[3];

    // 平面の法線ベクトルを求めるため、外積計算に用いる2つのベクトルを求める
    vector0[0] = p1[0] - p0[0];
    vector0[1] = p1[1] - p0[1];
    vector0[2] = p1[2] - p0[2];

    vector1[0] = p2[0] - p0[0];
    vector1[1] = p2[1] - p0[1];
    vector1[2] = p2[2] - p0[2];

    // 平面の方程式 ax+ by + cz = d のa, b, c, d を求める
    planeEq[0] = vector0[1] * vector1[2] - vector0[2] * vector1[1];
    planeEq[1] = -(vector0[0] * vector1[2] - vector0[2] * vector1[0]);
    planeEq[2] = vector0[0] * vector1[1] - vector0[1] * vector1[0];

    planeEq[3] = -(planeEq[0] * p0[0] + planeEq[1] * p0[1] + planeEq[2] * p0[2]);
}

void AsuraGraphic::calcShadowMatrix(GLfloat matrix[], GLfloat planeEq[], GLfloat lightPos[])
{
    GLfloat dot;

    // 平面と光源の内積を計算
    dot = planeEq[0] * lightPos[0] +
        planeEq[1] * lightPos[1] +
        planeEq[2] * lightPos[2] +
        planeEq[3] * lightPos[3];

    matrix[0] = dot - lightPos[0] * planeEq[0];
    matrix[4] = 0.f - lightPos[0] * planeEq[1];
    matrix[8] = 0.f - lightPos[0] * planeEq[2];
    matrix[12] = 0.f - lightPos[0] * planeEq[3];

    matrix[1] = 0.f - lightPos[1] * planeEq[0];
    matrix[5] = dot - lightPos[1] * planeEq[1];
    matrix[9] = 0.f - lightPos[1] * planeEq[2];
    matrix[13] = 0.f - lightPos[1] * planeEq[3];

    matrix[2] = 0.f - lightPos[2] * planeEq[0];
    matrix[6] = 0.f - lightPos[2] * planeEq[1];
    matrix[10] = dot - lightPos[2] * planeEq[2];
    matrix[14] = 0.f - lightPos[2] * planeEq[3];

    matrix[3] = 0.f - lightPos[3] * planeEq[0];
    matrix[7] = 0.f - lightPos[3] * planeEq[1];
    matrix[11] = 0.f - lightPos[3] * planeEq[2];
    matrix[15] = dot - lightPos[3] * planeEq[3];
}

}  // namespce Graphic
