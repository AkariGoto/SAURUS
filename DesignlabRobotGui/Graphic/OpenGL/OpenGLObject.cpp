
#include "OpenGLObject.h"

#include "Math/Matrix/matrix_library.h"


namespace designlab_robot_gui::graphic
{

OpenGLObject::OpenGLObject()
{
}

/// コンストラクタ
OpenGLObject::OpenGLObject(HWND hWnd)
{
    /**
     *	OpenGLObjectの作成
     */
    createGLObject(hWnd);
}

/// デストラクタ
OpenGLObject::~OpenGLObject()
{
    destroyGLObject();
}



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
bool OpenGLObject::createGLObject(HWND hWnd)
{
    /// 最初にOpenGLのデフォルト初期化を行う
    if (!AbstractOpenGLBase::InitializeGL(hWnd))
        return false;

    /// ディスプレイリスト作成
    newGLObjDispList();

    /**
     *	フラグ類の初期化
     */
    isCoordinateAxisDrawn = true;
    isGridFloorDrawn = true;

    return true;
}

/**
 *	説明
 *		終了処理
 */
void OpenGLObject::destroyGLObject(void)
{
    /**
     *	フラグ類の初期化
     */
    isCoordinateAxisDrawn = false;
    isGridFloorDrawn = false;

    /// ディスプレイリストの破棄
    deleteObjDispList();

    /// 最後に基底クラスの終了処理
    AbstractOpenGLBase::FinalizeGL();

    return;
}

/**
 *	------------------------------------------------------------
 *		オーバーライド関数
 *	------------------------------------------------------------
 */
 /**
  *	----------------------------------------
  *	描画に関係するもの
  *	----------------------------------------
  */
  /**
   *	説明
   *		OpenGLのオブジェクト描画
   */
void OpenGLObject::drawObjects(GLenum renderMode)
{
    switch (renderMode)
    {
        case GL_RENDER:
            glDisable(GL_CULL_FACE);
            /// 座標系描画
            drawCoordinateAxis();
            glEnable(GL_CULL_FACE);
            break;

        case GL_SELECT:
            break;

        default:
            break;
    }
}

/**
 *	説明
 *		OpenGLのオブジェクト描画
 */
void OpenGLObject::drawScenes(void)
{
    drawObjects(GL_RENDER);
    return;
}

/**
 *	説明
 *		OpenGLイメージのレンダリング
 *		OpenGL描画のコールバック関数になる
 */
void OpenGLObject::renderScenes(void)
{
    /// レンダリングコンテキストをカレントにする
    if (wglMakeCurrent(device_context_handle_ptr, rendering_context_handle))
    {
        /// バッファをクリア（指定したバッファを特定の色で消去）
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glLoadIdentity();

        /// 視点決定
        setSceneView(scene_width, scene_height);

        /// 今の座標系を保存しておく
        glPushMatrix();
        {

            /// シーンを描画
            drawScenes();

        }
        /// 座標系を戻す
        glPopMatrix();

        /// コマンドのフラッシュ
        glFlush();
    }

    /**
     *		バッファの切替
     *			バックバッファをフロントバッファに切り替え，新しい画像を見せる
     */
    SwapBuffers(wglGetCurrentDC());

    wglMakeCurrent(device_context_handle_ptr, NULL);

    return;
}

/**
 *	----------------------------------------
 *	ピッキングに関するもの
 *	----------------------------------------
 */
 /**
  *	説明
  *		マウスによる物体のピックアップ関数
  *		マウスイベント（大体は左クリック）関数内で呼ぶ
  *	引数
  *		x: クリックした点のx座標
  *		y: クリックした点のy座標
  */
void OpenGLObject::pickup(int x, int y)
{
    GLuint selectBuf[SELECT_BUFF_SIZE];		/// セレクションバッファ
    GLint hitCount = 0;					/// ヒットカウント
    GLint viewPort[4] = { 0,0,0,0 };		/// ビューポート		
    float currentAspect;				/// 現在のアスペクト比

    wglMakeCurrent(device_context_handle_ptr, rendering_context_handle);

    /**
     *	セレクション開始
     */
     /// 現在のビューポートを代入
    glGetIntegerv(GL_VIEWPORT, viewPort);

    /**
     *	バッファの選択
     *
     *	glSelectBuffer(GLsizei size, GLuint *buffer)
     *	buffer に GLuint 型の配列、size にその要素数を指定する。
     *	・この配列に選択したオブジェクトの名前、セレクションされた位置の奥行きの最小値と最大値などが格納されるから、
     *	　要素数はオブジェクト数にしたがって、多めに指定しておく。
     *	・セレクションモードに入る前（glRenderMode(GL_SELECT) を実行する前）に実行する必要がある。
     */
    glSelectBuffer(SELECT_BUFF_SIZE, selectBuf);

    /// セレクションモードに入る
    (void)glRenderMode(GL_SELECT);

    /**
     *	オブジェクトにつける名前（整数値）を登録しておくネームスタックを初期化する。
     *	これはセレクションモードに入った後（glRenderMode(GL_SELECT) を実行した後）に行わなければ無視される
     */
    glInitNames();

    /**
     *	プロジェクションモード
     *	セレクションの処理は視点座標系で行う
     */
    glMatrixMode(GL_PROJECTION);

    /**
     *	セレクションモードのときのみ下の行列計算を反映させるために
     *	Push（現在の透視変換マトリクスを保存）する
     */
    glPushMatrix();
    /// 透視変換マトリクスを初期化する
    glLoadIdentity();
    /**
     *	ピッキング行列の乗算
     *		表示領域がマウスポインタの周囲だけになるように変換行列を設定する。
     *		マウスの座標系は、スクリーンの座標系に対して上下が反転しているのでそれを補正する
     */
    gluPickMatrix((GLdouble)x, (GLdouble)(viewPort[3] - y), 5.0, 5.0, viewPort);

    /**
     *	通常の描画と同じ透視変換マトリクスを設定する
     *		ピックアップ時と現在見えてるものとのずれを防ぐため
     */
    currentAspect = (float)viewPort[2] / (float)viewPort[3];
    gluPerspective(PERS_DEFAULT_FOVY, currentAspect, PERS_DEFAULT_NEAR, PERS_DEFAULT_FAR);

    /// ここでモデルビューモードにする
    glMatrixMode(GL_MODELVIEW);

    /// セレクションモードで描画する
    drawObjects(GL_SELECT);
    /// プロジェクションモードに戻す
    glMatrixMode(GL_PROJECTION);

    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);			//モデルビューモードへ戻す

    hitCount = glRenderMode(GL_RENDER);

    /// ヒットしたオブジェクトを選ぶ
    selectHitObjects(hitCount, selectBuf);

    /// ヒットした結果により、処理を行う
    handlePickupResult();

    wglMakeCurrent(NULL, NULL);
}

/**
 *	説明
 *		セレクションバッファからデプスバッファを並び替えたりしながらヒットしたオブジェクトを識別する
 *	引数
 *		hitCount: ヒット数
 *		selectBuffer:	glSelectBuffer() の結果が格納されているセレクションバッファ
 *						選択したオブジェクトの名前、セレクションされた位置の奥行きの最小値と最大値などが格納されたバッファ
 */
int OpenGLObject::selectHitObjects(GLuint hitCount, GLuint selectBuffer[])
{
    /**
     *	ローカル変数
     */
    GLuint i;				/// カウンタ
    unsigned int nameIndex;	/// 名前列で使用するカウンタ

    GLuint nameCount;		/// ヒットしたオブジェクトの名前階層の数
    GLuint* work_pointer;	/// 作業用ポインタ
    GLuint minZ;			/// z（深度）の最小値
    GLuint maxZ;			/// z（深度）の最大値

    double nearDepth;		/// ヒットしたオブジェクトのデプス範囲（手前）
    double farDepth;		/// ヒットしたオブジェクトのデプス範囲（奥）
    double minDepth;		/// ヒットしたポリゴンの中の最小デプス値

    unsigned int nameArray[NAME_ARRAY_SIZE];	/// 名前階層の配列（NAME_ARRAY_SIZEは最大階層数）

    /**
     *	セレクションバッファに格納されるデータ
     *		1.ネームスタックに積まれた名前の個数（名前階層の個数）
     *		2.選択範囲を横切るプリミティブの頂点のデプス値の最小値
     *		3.選択範囲を横切るプリミティブの頂点のデプス値の最大値
     *		4.ネームスタック（= オブジェクトの名前）[n個]
     *	これらがヒットしたオブジェクト数だけ、順番に確保されている
     *
     *	つまり、m個ヒットしたなら、m * (1 + 2 + n)分のunsigned int の配列が確保される
     *	 [階層の深さ] [Z値(最小)] [Z値(最大)][階層中の番号]... [階層の深さ]・・・
     *
     *	例えば、
     *		[2][1.5][1.8][2][1] となっている、「ピックアップ」した「ポリゴンデータ」は、
     *	つまり
     *		階層の深さ：2
     *		奥行き値(手前側)：1.5
     *		奥行き値(奥側)：1.8
     *		階層中の番号：2-1
       */

       /// ヒットしていない
    if (hitCount <= 0) return -1;

    /// 最小深度の初期化
    minDepth = 10.0;

    /// セレクションバッファのポインタを作業用ポインタへ渡す
    work_pointer = (GLuint*)selectBuffer;

    /// ヒット数分確認する
    for (i = 0; i < hitCount; i++)
    {
        /**
         *	----------------------------------------
         *	STEP1 ヒットしたオブジェクト個々の処理
         *	----------------------------------------
         */
         /// 名前の個数を格納
        nameCount = *work_pointer;

        /// ポインタのインクリメントで、デプス値の最小値
        work_pointer++;
        minZ = *work_pointer;
        nearDepth = (double)minZ / 0x7fffffff;

        /// ポインタのインクリメントで、デプス値の最大値
        work_pointer++;
        maxZ = *work_pointer;
        farDepth = (double)minZ / 0x7fffffff;


        /**
         *	ポインタのインクリメントで、名前列の取得
         *	なお、名前の個数だけ取得する
         */
        for (nameIndex = 0; nameIndex < nameCount; nameIndex++)
        {
            work_pointer++;
            nameArray[nameIndex] = *work_pointer;
        }

        if (NAME_ARRAY_SIZE < nameCount)
        {
            std::cerr << "Error: [OpenGLObject::selectHitObjects] Name array size error"
                << std::endl;
            continue;
        }

        /**
         *	----------------------------------------
         *	STEP2 最小デプス（最も手前にあるもの）を探索
         *	----------------------------------------
         */

         /// 最小値の比較
        if (nearDepth < minDepth)
        {
            /// 最小値の更新
            minDepth = nearDepth;

            for (nameIndex = 0; nameIndex < nameCount; nameIndex++)
            {
                nameNumberOfHitObj[nameIndex] = nameArray[nameIndex];
            }
        }
    }

    return 1;
}

/**
 *	説明
 *		ピックアップにより選んだ物体の処理
 */
void OpenGLObject::handlePickupResult(void)
{
    return;
}

/**
 *	------------------------------------------------------------
 *		オブジェクトの描画に関係するもの
 *	------------------------------------------------------------
 */
 /**
  *	----------------------------------------
  *	3Dモデリング
  *	----------------------------------------
  */
  /// 座標軸描画
void OpenGLObject::drawCoordinateAxis(double length, double width, double scale)
{
    if (!isCoordinateAxisDrawn)
        return;

    /// 球の半径
    const double RADIUS = 50.0;

    glPushMatrix();
    /**
     *	x軸の描画（赤）
     */
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialAmbDiffRed);
    glLineWidth((GLfloat)(width * scale));
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f((GLfloat)(length * scale), 0.0f, 0.0f);
    glEnd();

    glTranslatef((GLfloat)(length * scale), 0.0f, 0.0f);
    drawSphere(RADIUS * scale);
    glPopMatrix();

    glPushMatrix();
    /**
     *	y軸の描画（緑）
     */
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialAmbDiffGreen);
    glLineWidth((GLfloat)(width * scale));
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, (GLfloat)(length * scale), 0.0f);
    glEnd();

    glTranslatef(0.0f, (GLfloat)(length * scale), 0.0f);
    drawSphere(RADIUS * scale);
    glPopMatrix();

    glPushMatrix();
    /**
     *	z軸の描画（青）
     */
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialAmbDiffBlue);
    glLineWidth((GLfloat)(width * scale));
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, (GLfloat)(length * scale));
    glEnd();

    glTranslatef(0.0f, 0.0f, (GLfloat)(length * scale));
    drawSphere(RADIUS * scale);
    glPopMatrix();

    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialAmbDiffYellow);
    drawSphere(RADIUS * scale);
}

/**
 *	説明
 *		ポリゴンモデリング（成分）
 *		添字(1, 2, 3)の順にCW（時計回り）で入力
 *		各頂点の座標を入力
 */
void OpenGLObject::drawPolygonSurface(double x1, double y1, double z1,
                            double x2, double y2, double z2,
                            double x3, double y3, double z3)
{
    /// ローカル変数の宣言
    double norm;
    double x, y, z;
    double vx1, vy1, vz1, vx2, vy2, vz2;

    /// ベクトルの差を計算
    vx1 = x1 - x2;
    vy1 = y1 - y2;
    vz1 = z1 - z2;

    vx2 = x3 - x2;
    vy2 = y3 - y2;
    vz2 = z3 - z2;

    /// 外積計算
    designlab_robot_gui::math::outerProduct(vx1, vy1, vz1, vx2, vy2, vz2, &x, &y, &z);
    norm = sqrt(x * x + y * y + z * z);

    if (norm == 0)
    {
        std::cerr << "Error: [AbstractOpenGLBase::drawPolygonSurface] argument error\n" << std::endl;
        return;
    }

    x = x / norm;
    y = y / norm;
    z = z / norm;

    glBegin(GL_POLYGON);
    glNormal3f((GLfloat)x, (GLfloat)y, (GLfloat)z);
    glVertex3f((GLfloat)x1, (GLfloat)y1, (GLfloat)z1);
    glVertex3f((GLfloat)x2, (GLfloat)y2, (GLfloat)z2);
    glVertex3f((GLfloat)x3, (GLfloat)y3, (GLfloat)z3);
    glEnd();



    return;
}

/**
 *		ポリゴンモデリング（ベクトル）
 *		添字(1, 2, 3)の順にCW（時計回り）で入力
 *		各頂点の座標を入力
 */
void OpenGLObject::drawPolygonSurface(const Vector& point1,
                                      const Vector& point2,
                                      const Vector& point3)
{
    using designlab_robot_gui::math::THREE_DIMENSION;

    /// 引数チェック
    assert(point1.getSize() == THREE_DIMENSION);
    assert(point2.getSize() == THREE_DIMENSION);
    assert(point3.getSize() == THREE_DIMENSION);

    drawPolygonSurface(point1(1), point1(2), point1(3), point2(1), point2(2), point2(3), point3(1), point3(2), point3(3));
}

/// 円柱描画
void OpenGLObject::drawCylinder(double radius, double height)
{
    /// quadric object を一つ生成する
    GLUquadricObj* quad = gluNewQuadric();

    /// 面の塗り潰しを指定する（線画ではなく陰影をつけた円柱を描く）
    gluQuadricDrawStyle(quad, GLU_FILL);

    /// スムースシェーディングを行うよう設定する
    gluQuadricNormals(quad, GLU_SMOOTH);

    glPushMatrix();
    glTranslated((GLdouble)0, 0, (GLdouble)(-0.5 * height));
    gluCylinder(quad, (GLdouble)radius, (GLdouble)radius, (GLdouble)height, (GLint)16, (GLint)1);
    gluDisk(quad, (GLdouble)0, (GLdouble)radius, (GLint)16, (GLint)1);
    glTranslated((GLdouble)0, 0, (GLdouble)height);
    gluDisk(quad, (GLdouble)0, (GLdouble)radius, (GLint)16, (GLint)1);
    glPopMatrix();

    /// 生成したquadric object を削除
    gluDeleteQuadric(quad);

    return;
}

/// 球描画
void OpenGLObject::drawSphere(double radius)
{
    /// quadric object を一つ生成する
    GLUquadricObj* quad = gluNewQuadric();

    /// 面の塗り潰しを指定する(線画ではなく陰影をつけた円柱を描く)
    gluQuadricDrawStyle(quad, GLU_FILL);

    /// スムースシェーディングを行うよう設定する
    gluQuadricNormals(quad, GLU_SMOOTH);

    /// 球面
    gluSphere(quad, (GLdouble)radius, (GLint)16, (GLint)16);

    /// 生成したquadric object を削除
    gluDeleteQuadric(quad);

    return;
}

/**
 *		Box描画(線画)
 *			成分入力
 */
void OpenGLObject::drawBox(double startX, double startY, double startZ, double endX, double endY, double endZ)
{
    /// 第1面描画(x-y平面に平行な面)
    //////////////////////////////////
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f((GLfloat)startX, (GLfloat)endY, (GLfloat)endZ);
    glVertex3f((GLfloat)startX, (GLfloat)startY, (GLfloat)endZ);
    glVertex3f((GLfloat)endX, (GLfloat)startY, (GLfloat)endZ);
    glVertex3f((GLfloat)endX, (GLfloat)endY, (GLfloat)endZ);
    glEnd();

    /// 第2面描画(y-z平面に平行な面)
    //////////////////////////////////
    glBegin(GL_POLYGON);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f((GLfloat)endX, (GLfloat)endY, (GLfloat)endZ);
    glVertex3f((GLfloat)endX, (GLfloat)startY, (GLfloat)endZ);
    glVertex3f((GLfloat)endX, (GLfloat)startY, (GLfloat)startZ);
    glVertex3f((GLfloat)endX, (GLfloat)endY, (GLfloat)startZ);
    glEnd();

    /// 第3面描画(x-y平面に平行な面)
    //////////////////////////////////
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f((GLfloat)endX, (GLfloat)endY, (GLfloat)startZ);
    glVertex3f((GLfloat)endX, (GLfloat)startY, (GLfloat)startZ);
    glVertex3f((GLfloat)startX, (GLfloat)startY, (GLfloat)startZ);
    glVertex3f((GLfloat)startX, (GLfloat)endY, (GLfloat)startZ);
    glEnd();

    /// 第4面描画(y-z平面に平行な面)
    //////////////////////////////////
    glBegin(GL_POLYGON);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f((GLfloat)startX, (GLfloat)endY, (GLfloat)startZ);
    glVertex3f((GLfloat)startX, (GLfloat)startY, (GLfloat)startZ);
    glVertex3f((GLfloat)startX, (GLfloat)startY, (GLfloat)endZ);
    glVertex3f((GLfloat)startX, (GLfloat)endY, (GLfloat)endZ);
    glEnd();

    /// 第5面描画(x-z平面に平行な面)
    //////////////////////////////////
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f((GLfloat)startX, (GLfloat)startY, (GLfloat)endZ);
    glVertex3f((GLfloat)startX, (GLfloat)startY, (GLfloat)startZ);
    glVertex3f((GLfloat)endX, (GLfloat)startY, (GLfloat)startZ);
    glVertex3f((GLfloat)endX, (GLfloat)startY, (GLfloat)endZ);
    glEnd();

    /// 第6面描画(x-z平面に平行な面)
    //////////////////////////////////
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f((GLfloat)startX, (GLfloat)endY, (GLfloat)startZ);
    glVertex3f((GLfloat)startX, (GLfloat)endY, (GLfloat)endZ);
    glVertex3f((GLfloat)endX, (GLfloat)endY, (GLfloat)endZ);
    glVertex3f((GLfloat)endX, (GLfloat)endY, (GLfloat)startZ);
    glEnd();

    return;
}

/**
 *		Box描画(線画)
 *			ベクトル入力
 */
void OpenGLObject::drawBox(const Vector& startPoint, const Vector& endPoint)
{
    using designlab_robot_gui::math::THREE_DIMENSION;

    /// 引数チェック
    assert(startPoint.getSize() == THREE_DIMENSION);
    assert(endPoint.getSize() == THREE_DIMENSION);

    drawBox(startPoint(1), startPoint(2), startPoint(3), endPoint(1), endPoint(2), endPoint(3));

    return;
}

/**
 *		フレーム描画（平面と4隅の垂線）
 *			成分入力
 */
void OpenGLObject::drawFrame(double startX, double startY, double startZ, double endX, double endY, double endZ)
{
    /// 色をグレーに設定
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialAmbDiffGray);
    /// 線幅決定
    glLineWidth(4.0f);

    /// 平面描画
    glBegin(GL_LINE_LOOP);
    glVertex3f((GLfloat)endX, (GLfloat)endY, (GLfloat)startZ);
    glVertex3f((GLfloat)endX, (GLfloat)startY, (GLfloat)startZ);
    glVertex3f((GLfloat)startX, (GLfloat)startY, (GLfloat)startZ);
    glVertex3f((GLfloat)startX, (GLfloat)endY, (GLfloat)startZ);
    glEnd();

    /// 以下で垂線描画
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

    return;
}

/**
 *		フレーム描画（平面と4隅の垂線）
 *			ベクトル入力
 */
void OpenGLObject::drawFrame(const Vector& startPoint, const Vector& endPoint)
{
    using designlab_robot_gui::math::THREE_DIMENSION;

    /// 引数チェック
    assert(startPoint.getSize() == THREE_DIMENSION);
    assert(endPoint.getSize() == THREE_DIMENSION);

    drawFrame(startPoint(1), startPoint(2), startPoint(3), endPoint(1), endPoint(2), endPoint(3));

    return;
}

/**
 *		線描画
 */

 /// 線描画（成分入力）
void OpenGLObject::drawLine(double startX, double startY, double startZ,
                    double endX, double endY, double endZ,
                    double width)
{
    /// 線の太さ決定
    glLineWidth((GLfloat)width);

    /// 線の描画
    glBegin(GL_LINES);
    glVertex3f((GLfloat)startX, (GLfloat)startY, (GLfloat)startZ);
    glVertex3f((GLfloat)endX, (GLfloat)endY, (GLfloat)endZ);
    glEnd();

    return;
}

/// 線描画（ベクトル入力）
void OpenGLObject::drawLine(const Vector& startPoint, const Vector& endPoint, double width)
{
    using designlab_robot_gui::math::THREE_DIMENSION;

    /// 引数チェック
    assert(startPoint.getSize() == THREE_DIMENSION);
    assert(endPoint.getSize() == THREE_DIMENSION);

    drawLine(startPoint(1), startPoint(2), startPoint(3), endPoint(1), endPoint(2), endPoint(3), width);
}

/// 三角形描画
void OpenGLObject::drawTriangle(const Vector& point1, const Vector& point2,
                                const Vector& point3, double width)
{
    using designlab_robot_gui::math::THREE_DIMENSION;

    /// 引数チェック
    assert(point1.getSize() == THREE_DIMENSION);
    assert(point2.getSize() == THREE_DIMENSION);
    assert(point3.getSize() == THREE_DIMENSION);

    /// 実際に描画
    drawLine(point1, point2, width);
    drawLine(point2, point3, width);
    drawLine(point3, point1, width);

}

/// 四角形描画
void OpenGLObject::drawQuadrangle(const Vector& point1, const Vector& point2,
                                  const Vector& point3, const Vector& point4,
                                  double width)
{
    using designlab_robot_gui::math::THREE_DIMENSION;

    /// 引数チェック
    assert(point1.getSize() == THREE_DIMENSION);
    assert(point2.getSize() == THREE_DIMENSION);
    assert(point3.getSize() == THREE_DIMENSION);
    assert(point4.getSize() == THREE_DIMENSION);

    drawLine(point1, point2, width);
    drawLine(point2, point3, width);
    drawLine(point3, point1, width);
    drawLine(point1, point2, width);
}

/**
 *	----------------------------------------
 *	2Dモデリング
 *	----------------------------------------
 */
 /**
  *		平面板描画(対角線の頂点と高さ)
  */
  /// 水平面（XY
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

/// 前額面（YZ）
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

/// 矢状面（ZX）
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

/**
 *		メッシュ状の床を描画する
 *			startX, startY：床の対角線の始点
 *			endX, endY：床の対角線の終点
 *			z：床の高さ
 *			dx, dy：メッシュの間隔
 */
void OpenGLObject::drawGridFloor(void)
{
    if (!isGridFloorDrawn)
        return;

    glCallList(gridFloorDisplayListID);

}

/**
 *	------------------------------------------------------------
 *		OpenGLObjectクラスのprotectedなメンバ関数
 *	------------------------------------------------------------
 */
 /**
  *	説明
  *		ディスプレイリストの生成
  */
void OpenGLObject::newGLObjDispList(void)
{
    /// ディスプレイリストIDの作成
    gridFloorDisplayListID = glGenLists(1);

    /// ローカル変数の宣言
    Vector start(3);
    Vector end(3);

    const GLfloat glidColor[4] = { 0.1f, 0.1f, 0.3f, 1.0f };
    const GLfloat planeColor[4] = { 0.2f, 0.4, 0.6f, 1.0f };

    /// ディスプレイリストの作成
    glNewList(gridFloorDisplayListID, GL_COMPILE);

    setMaterialColor(planeColor);
    /**
     *		平面の描画
     */
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f((GLfloat)GRID_START_X, (GLfloat)GRID_START_Y, (GLfloat)GRID_HEIGHT);
    glVertex3f((GLfloat)GRID_START_X, (GLfloat)GRID_END_Y, (GLfloat)GRID_HEIGHT);
    glVertex3f((GLfloat)GRID_END_X, (GLfloat)GRID_END_Y, (GLfloat)GRID_HEIGHT);
    glVertex3f((GLfloat)GRID_END_X, (GLfloat)GRID_START_Y, (GLfloat)GRID_HEIGHT);
    glEnd();

    /**
     *		グリッドの描画
     */
     /// 色の選択
    setMaterialColor(glidColor);

    /// 高さ固定
    start(3) = GRID_HEIGHT + 1.0;	end(3) = GRID_HEIGHT + 1.0;

    // x方向の描画
    start(2) = GRID_START_Y;		end(2) = GRID_END_Y;

    double x;
    for (x = GRID_START_X; x <= GRID_END_X; x += GRID_DX)
    {
        start(1) = x;	end(1) = x;
        drawLine(start, end, 1.5);
    }

    /// y方向の描画
    start(1) = GRID_START_X;		end(1) = GRID_END_X;

    double y;
    for (y = GRID_START_Y; y <= GRID_END_Y; y += GRID_DY)
    {
        start(2) = y;	end(2) = y;
        drawLine(start, end, 1.5);
    }

    /// ディスプレイリストの保存
    glEndList();

    return;
}

/**
 *	説明
 *		ディスプレイリストの消去
 */
void OpenGLObject::deleteObjDispList(void)
{
    /// ディスプレイリストの消去
    glDeleteLists(gridFloorDisplayListID, 1);
}

}	/// end of namespace Graphic