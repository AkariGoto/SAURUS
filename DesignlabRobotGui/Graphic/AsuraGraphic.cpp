/**
 *  ファイル名
 *		AsuraGraphic.cpp
 *  説明
 *		OpenGLを用いてTITAN Xを描画するクラス
 *  日付
 *		作成日: 2007/05/12(SAT)		更新日: 2008/07/07(MON)
 */

//  20200819  クローラ関連コメントアウト
//  20200822  表示する脚修正
//  20201019  脚先のワールド座標と脚座標の切り替え
//  202311　ASURA関連コメントアウト

/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */
#include "asuraGraphic.h"

using namespace std;
using namespace Math;
using namespace Const;
using namespace Asura;
using namespace Data;

namespace Graphic
{
/**
 *	----------------------------------------------------------------------
 *		AsuraGraphicクラス
 *	----------------------------------------------------------------------
 */

/**
 *	------------------------------------------------------------
 *		AsuraGraphicクラスのメンバ関数定義
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	コンストラクタとデストラクタ
 *	----------------------------------------
 */
/// コンストラクタ
AsuraGraphic::AsuraGraphic(HWND hWnd, AsuraData* asuraData, ViewType type)
{
	/// 初期化
	initializeAsuraGraphic(hWnd, asuraData);

	/// 視点を決定
	setViewType(type);

}

/// デストラクタ
AsuraGraphic::~AsuraGraphic()
{
	finalizeAsuraGraphic();
}


/**
 *		初期化・終了処理
 */
/// 初期化
void AsuraGraphic::initializeAsuraGraphic(HWND hWnd, AsuraData* asuraData)
{
	/// フラグ初期化
	isAsuraDrawn = true;
	isSupportPolygonDrawn = false;
	isDisplayListReady = false;
	isAnimationRecorded = false;

	legCoordinate = false;  //20201019  起動時は脚座標系を表示しない

	/// 最初にOpenGLのデフォルト初期化を行う
	if (!OpenGLObject::createGLObject(hWnd))
		return;

	/// ポインタが有効だったらポインタをセット
	if ( asuraData != NULL )
		asuraDataSrcPtr = asuraData;

	/// ディスプレイリスト作成
	newAsuraDisplayList();

	/// 影の準備
	GLfloat floorP1[3] = {1.0f, 0.0f, (GLfloat)GRID_HEIGHT};
	GLfloat floorP2[3] = {0.0f, 1.0f, (GLfloat)GRID_HEIGHT};
	GLfloat floorP3[3] = {-1.0f, -1.0f, (GLfloat)GRID_HEIGHT};

	calcShadowPlaneEquation(planeFactorOfGridFloor, floorP1, floorP2, floorP3);

	RECT rect;
	GetClientRect(hWnd, &rect);

	/// AVI準備
	movieRecorder.initialize((rect.right/4)*4, (rect.bottom/4)*4, 24);
}

/// 終了処理
void AsuraGraphic::finalizeAsuraGraphic(void)
{
	/// ディスプレイリスト破棄
	deleteAsuraDisplayList();
}

/**
 *	説明
 *		ディスプレイリストの生成・消去
 */
void AsuraGraphic::newAsuraDisplayList(void)
{
	if ( isDisplayListReady )
		return;

	/// ディスプレイリストの生成
	int bodyList = glGenLists(1);
	int legList = glGenLists(LEG_DISP_LIST_NUM);

	/*  02020819
	int trackList = glGenLists(TRACK_DISP_LIST_NUM);

	// 生成判定
	if (bodyList == 0 || legList == 0 || trackList == 0 )
	{
		cerr << "Error: [OpenGL::newAsuraDisplayList] Display List is not generated\n" << endl;
		return;
	}
	*/

	/// 胴体用のディスプレイリスト
	bodyDisplayListID = bodyList;

	/// 脚用のディスプレイリストの準備
	int i;
	for (i=0; i<LEG_DISP_LIST_NUM; i++)
		legDisplayListID[i] = legList + i;

	/// クローラ用のディスプレイリストの準備
/*	int j;
	for (j=0; j<TRACK_DISP_LIST_NUM; j++)
		trackDisplayListID[j] = trackList + j;*/

	/// 関節色
	//SAURUS
	const GLfloat jointColor[4] = { 0.55f, 0.55f, 0.65f, 1.0f };
	const GLfloat linkColor[4] = {0.156f, 0.40f, 1.0f, 1.0f};

	/*ASURA20231128変更
	 const GLfloat linkColor[4] = {0.55f, 0.55f, 0.65f, 1.0f};
	 const GLfloat jointColor[4] = {0.10f, 0.50f, 0.50f, 1.0f};
	*/
	//const GLfloat joint4Color[4] = {0.0f, 0.0f, 1.0f, 1.0f};
/**
 *		胴体用のディスプレイリストの作成
 */
	/// body
	glNewList(bodyDisplayListID, GL_COMPILE);
			setMaterialColor( linkColor );
			drawBox(	-BODY_LENGTH/2, -BODY_WIDTH/Const::ROOT_2/2+50, -BODY_HEIGHT/2,
							BODY_LENGTH/2, BODY_WIDTH/Const::ROOT_2/2-50, BODY_HEIGHT/2 );
			drawBox(	-BODY_LENGTH/Const::ROOT_2/2+50, -BODY_WIDTH/2, -BODY_HEIGHT/2,
							BODY_LENGTH/Const::ROOT_2/2-50, BODY_WIDTH/2, BODY_HEIGHT/2 );

/*		glTranslated(BODY_LENGTH/Const::ROOT_2/2, 0, 0);	/// 座標変換
			drawBox(	-40, -BODY_WIDTH/Const::ROOT_2/2, -BODY_HEIGHT/2,
							40, BODY_WIDTH/Const::ROOT_2/2, BODY_HEIGHT/2 );

		glTranslated(-BODY_LENGTH, 0, 0);	/// 座標変換
			drawBox(	-40, -BODY_WIDTH*2/3, -BODY_HEIGHT/2,
							40, BODY_WIDTH*2/3, BODY_HEIGHT/2 );
*/	glEndList();

/**
 *		脚用のディスプレイリストの作成
 */
	/// Base
	glNewList(legDisplayListID[0], GL_COMPILE);
			setMaterialColor( jointColor );	///関節	/// 原点
			drawCylinder(30, BODY_HEIGHT+10);
			//drawCylinder(50, BODY_HEIGHT + 50);ASURA_20231128変更
		//glTranslated(0, 0, -LINK1/2);	/// 座標変換		
			//setMaterialColor( linkColor );
			//drawBox(-30, -30, -LINK1/2, 30, 30, LINK1/2);			
	glEndList();

	/// 1.Hip
	glNewList(legDisplayListID[1], GL_COMPILE);
			setMaterialColor( jointColor );	///関節	/// 原点
			drawCylinder(30, BODY_HEIGHT+10);
			//drawCylinder(50, BODY_HEIGHT + 10);ASURA_20231128変更
		glTranslated(-LINK1, 0,0 );	/// 座標変換
			setMaterialColor( linkColor );
			drawBox(0, -35,-35 ,  LINK1, 35,35);
			//drawBox(0, -45, -45, LINK1,45, 45);ASURA_20231128変更
	glEndList();

	/// 2.Thigh
	glNewList(legDisplayListID[2], GL_COMPILE);
			setMaterialColor( jointColor );///関節	
			drawCylinder(30, 60);
			//drawCylinder(40, 70);ASURA_20231128変更
		glTranslated(-LINK2, 0, 0);	/// 座標変換
			setMaterialColor( linkColor );
			drawBox(0, -30, -30, LINK2, 30, 30);
			setMaterialColor( jointColor );///関節	
			drawCylinder(30, 60);
			//drawCylinder(40, 110);ASURA_20231128変更
	glEndList();

	 /// 3.Shank
	glNewList(legDisplayListID[3], GL_COMPILE);
		glTranslated(-LINK3, 0, 0);	/// 座標変換
			setMaterialColor( linkColor );
			drawBox(0,-25,-25, LINK3, 25, 25);
			setMaterialColor( jointColor );///関節	
			drawCylinder(30, 60);
			//drawCylinder(40, 70); ASURA_20231128変更
	glEndList();

	
	 /// 4.Foot
	glNewList(legDisplayListID[4], GL_COMPILE);
		glTranslated(-FOOT, 0, 0);	/// 座標変換
			setMaterialColor( linkColor );
			drawBox(0, -25, -25, FOOT, 25, 25);//drawBox(3*FOOT/5, -40, -35, FOOT, 40, 35);
			setMaterialColor( jointColor );///関節	
			drawCylinder(30, 60);
			//drawCylinder(40, 70); ASURA_20231128変更
	glEndList();
	

/**
 *		クローラ用のディスプレイリストの作成
 */	
/*	/// Base
	glNewList(legDisplayListID[0], GL_COMPILE);
			setMaterialColor( jointColor );	///関節	/// 原点
			drawCylinder(50, BODY_HEIGHT+10);
		//glTranslated(0, 0, -LINK1/2);	/// 座標変換
			//setMaterialColor( linkColor );
			//drawBox(-30, -30, -LINK1/2, 30, 30, LINK1/2);
	glEndList();

	/// Hip
	glNewList(legDisplayListID[1], GL_COMPILE);
			setMaterialColor( jointColor );	///関節	/// 原点
			drawCylinder(50, BODY_HEIGHT+10);
		glTranslated(-LINK1, 0,0 );	/// 座標変換
			setMaterialColor( linkColor );
			drawBox(0, -45,-45 ,  LINK1, 45,45);
	glEndList();

	/// Thigh
	glNewList(legDisplayListID[2], GL_COMPILE);
			setMaterialColor( jointColor );///関節	
			drawCylinder(40, 70);
		glTranslated(-LINK2, 0, 0);	/// 座標変換
			setMaterialColor( linkColor );
			drawBox(0, -30, -30, LINK2, 30, 30);
			setMaterialColor( jointColor );///関節	
			drawCylinder(40, 110);
	glEndList();

	 /// Shank
	glNewList(legDisplayListID[3], GL_COMPILE);
		glTranslated(-LINK3, 0, 0);	/// 座標変換
			setMaterialColor( linkColor );
			drawBox(0,-25,-25, LINK3, 25, 25);
			setMaterialColor( jointColor );///関節	
			drawCylinder(40, 70);
	glEndList();
*/
	/// 準備完了
	isDisplayListReady = true;

}

void AsuraGraphic::deleteAsuraDisplayList(void)
{
	/// ディスプレイリストの消去
	glDeleteLists(bodyDisplayListID, 1);
	glDeleteLists(legDisplayListID[0], LEG_DISP_LIST_NUM);

	/*  20200819
	glDeleteLists(trackDisplayListID[0], TRACK_DISP_LIST_NUM);
	*/
}

/**
 *	----------------------------------------
 *	描画に関係するもの
 *	----------------------------------------
 */
/**
 *	説明
 *		OpenGLのオブジェクト描画
 */
void AsuraGraphic::drawObjects(GLenum renderMode)
{
	Matrix gAb(DH_DIMENSION, DH_DIMENSION);  //20201019

	switch (renderMode)
	{
		case GL_RENDER:
				/// シーンを描画
				glDisable(GL_CULL_FACE);	
					/// 座標系描画
					drawCoordinateAxis(300.0, 5.0, 0.8);
					/// Asura描画
					drawAsura();
				glEnable(GL_CULL_FACE);	

				//20201019
				if (legCoordinate)
				{
					glDisable(GL_CULL_FACE);
					glPushMatrix();
					/// 脚座標系に変換
					gAb = asuraDataSrcPtr->getLegBaseTransformation(2);
					transformOpenGLMatrix(gAb);
					drawCoordinateAxis(300.0, 5.0, 0.8);
					glPopMatrix();
					glEnable(GL_CULL_FACE);
				}

			break;

		case GL_SELECT:
			break;

		default:
			break;
	}
}

/**
 *	説明
 *		OpenGLイメージ描画
 */
void AsuraGraphic::drawScenes(void)
{
	drawObjects(GL_RENDER);
	return;
}

/**
 *	説明
 *		影描画
 */
void AsuraGraphic::drawShadow(void)
{
	/// 影の光源
	float lightPosition[] = {60, 80, 3000, 1};

	/// 投影行列：4x4の同時変換行列
	GLfloat shadowMatrix[16];

	///ステンシルバッファクリア値の設定
	glClearStencil( 0 );

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_POSITION, (float*)lightPosition);
	/// 平面射影行列の算出
	calcShadowMatrix(shadowMatrix, planeFactorOfGridFloor, lightPosition);

	
	/// 床のステンシルを付ける
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, ~0);
	
	/// これから描画するもののステンシル値にすべて１タグをつける
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	/// 床の描画
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
	glStencilOp(GL_KEEP,GL_KEEP ,GL_INCR);
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

	glCullFace( GL_FRONT);
	glEnable(GL_CULL_FACE );
	glEnable(GL_AUTO_NORMAL );


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

	return;
}

/**
 *	説明
 *		OpenGLイメージのレンダリング
 *		OpenGL描画のコールバック関数になる
 */
void AsuraGraphic::renderScenes(void)
{
	/// レンダリングコンテキストをカレントにする
	if (wglMakeCurrent(deviceContextHandle, renderingContextHandle))
	{
		/// バッファをクリア（指定したバッファを特定の色で消去）
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glLoadIdentity();

		/// 視点決定
		setSceneView(sceneWidth, sceneHeight);

		/**
		 *	オブジェクトの描画
		 */
		/// 今の座標系を保存しておく
		glPushMatrix();
		{
			/// シーンを描画
			drawScenes();

			/// 影描画
			drawShadow();

			/// 支持脚多角形描画
			drawSupportPolygon();
		}
		/// 座標系を戻す
		glPopMatrix();

		/// コマンドのフラッシュ
		if (isAnimationRecorded)//
			movieRecorder.recordFrame();
		else
			glFlush();
	}

	/**
	 *		バッファの切替
	 *			バックバッファをフロントバッファに切り替え，新しい画像を見せる
	 */
	SwapBuffers( wglGetCurrentDC() );

	wglMakeCurrent(deviceContextHandle, NULL);

	return;
}


/**
 *		シーンの視点をViewTypeにより設定
 */
void AsuraGraphic::setSceneView(double width, double height)
{
	/// 視点属性により視点を変更
	switch ( getViewType() )
	{
		case PERSPECTIVE:
			setViewPoint(	cameraView.getDistance(),
							cameraView.getAzimuth(), 
							cameraView.getElevation(),
							cameraView.getViewCenterPosition(1),
							cameraView.getViewCenterPosition(2),
							cameraView.getViewCenterPosition(3),
							width, height
						);
			break;

		case TOP:
			setViewPoint(	2500.0, -90.0, 90.0,//1200.0, -90.0, 90.0,
							asuraDataSrcPtr->getBodyPosition()(1),
							asuraDataSrcPtr->getBodyPosition()(2),
							asuraDataSrcPtr->getBodyPosition()(3) - 100,
							width, height
						);
			break;
        
		case SIDE:
			setViewPoint(	1500.0, -90.0, 0.0,//1200.0, -90.0, 0.0,
							asuraDataSrcPtr->getBodyPosition()(1),
							asuraDataSrcPtr->getBodyPosition()(2),
							asuraDataSrcPtr->getBodyPosition()(3) - 100,
							width, height
						);
			break;
    
		case FRONT:
			setViewPoint(	1500.0, 0.0, 0.0,//1200.0, 0.0, 0.0,
							asuraDataSrcPtr->getBodyPosition()(1),
							asuraDataSrcPtr->getBodyPosition()(2),
							asuraDataSrcPtr->getBodyPosition()(3) - 100,
							width, height
						);
			break;

		default:
			break;

    }

	return;
}

/**
 *		Asuraの描画
 */
void AsuraGraphic::drawAsura(void)
{
	if (!isAsuraDrawn)
		return;

	/// 胴体描画
	drawBody();

	/// ロコモーション様式により描画を分ける
	if (asuraDataSrcPtr->getLocomotionStyle() ==  LEGGED)
	{
		
		//for (int i=0; i<LEG_NUM; i++)
			//drawLeg(i+1);
		
		int i=2;
		drawLeg(i);
	}
	else
	{
		int j;
		for (j=0; j<LEG_NUM; j++)
			drawTrack(j+1);		
	}

	return;
}

/**
 *		Asuraの本体描画
 */
void AsuraGraphic::drawBody(void)
{
	/// グローバルから胴体座標への変換行列
	Matrix gAb(DH_DIMENSION, DH_DIMENSION);

	glPushMatrix();
		/// 胴体座標系に変換
		gAb = asuraDataSrcPtr->getBodyTransformation();
		transformOpenGLMatrix(gAb);
			glCallList(bodyDisplayListID);
	glPopMatrix();

	return;
}

/**
 *		Asuraの脚描画
 */
void AsuraGraphic::drawLeg(int legNo)
{
	if (!isDisplayListReady)
		return;

	/// 脚の引数チェック
	assert( 1 <= legNo && legNo <= LEG_NUM ); 

	Matrix trans(DH_DIMENSION, DH_DIMENSION);	/// 座標変換用の行列
	Matrix bAl(DH_DIMENSION, DH_DIMENSION);		/// 胴体 -> 脚


	glPushMatrix();
		
		/// 脚のベース座標系を取得して，脚座標に変換
		glPushMatrix();
		bAl = asuraDataSrcPtr->getLegBaseTransformation(legNo);
			transformOpenGLMatrix(bAl);
				glCallList(legDisplayListID[0]);
		glPopMatrix();

		/// hip
		glPushMatrix();
			trans = asuraDataSrcPtr->getLegJointTransformation(legNo, 1);
			transformOpenGLMatrix(trans);
				glCallList(legDisplayListID[1]);
		glPopMatrix();

		/// shank
		glPushMatrix();
			trans = asuraDataSrcPtr->getLegJointTransformation(legNo, 2);
			transformOpenGLMatrix(trans);
				glCallList(legDisplayListID[2]);
		glPopMatrix();

		 /// knee
		glPushMatrix();
			trans = asuraDataSrcPtr->getLegJointTransformation(legNo, 3);
			transformOpenGLMatrix(trans);
				glCallList(legDisplayListID[3]);
		glPopMatrix();

		/// foot
		glPushMatrix();
			trans = asuraDataSrcPtr->getLegFootTransformation(legNo);
			transformOpenGLMatrix(trans);
				glCallList(legDisplayListID[4]);
		glPopMatrix();

	glPopMatrix();

	return;
}

/**
 *		Asuraのトラック描画
 */
void AsuraGraphic::drawTrack(int trackNo)
{
	/*if (!isDisplayListReady)
		return;

	/// 脚の引数チェック
	assert( 1 <= trackNo && trackNo <= LEG_NUM ); 

	Matrix trans(DH_DIMENSION, DH_DIMENSION);	/// 座標変換用の行列
	Matrix bAl(DH_DIMENSION, DH_DIMENSION);		/// 胴体 -> 脚


	glPushMatrix();
		
		/// 脚のベース座標系を取得して，脚座標に変換
		glPushMatrix();
		bAl = asuraDataSrcPtr->getTrackBaseTransformation(trackNo);
			transformOpenGLMatrix(bAl);
				glCallList(trackDisplayListID[0]);
		glPopMatrix();

		/// hip
		glPushMatrix();
		trans = asuraDataSrcPtr->getTrackJointTransformation(trackNo, 1);
			transformOpenGLMatrix(trans);
				glCallList(trackDisplayListID[1]);
		glPopMatrix();

		/// shank
		glPushMatrix();
		trans = asuraDataSrcPtr->getTrackJointTransformation(trackNo, 2);
			transformOpenGLMatrix(trans);
				glCallList(trackDisplayListID[2]);
		glPopMatrix();

		 /// knee
		glPushMatrix();
			trans = asuraDataSrcPtr->getTrackEndTransformation(trackNo);
			transformOpenGLMatrix(trans);
				glCallList(trackDisplayListID[3]);
		glPopMatrix();

	glPopMatrix();

	return;*/
}



/**
 *		支持脚多角形(三角形or四角形)の描画
 */
void AsuraGraphic::drawSupportPolygon(void)
{

	/// 描画フラグ
	if ( !isSupportPolygonDrawn )
		return;

	int i;
	Asura::LegPhase phase[LEG_NUM];

	for (i=0; i<LEG_NUM; i++)
	{
		phase[i] = asuraDataSrcPtr->getLegPhase(i+1);
	}

	/// 支持脚多角形の色決定
	setMaterialColor( selectMaterialColor(Graphic::YELLOW) );

	if ( phase[0] == Asura::SWING )
	{
		drawTriangle(	asuraDataSrcPtr->getLegFootPosition(2), asuraDataSrcPtr->getLegFootPosition(3), 
							asuraDataSrcPtr->getLegFootPosition(4), 2.0 );
	}
	else if ( phase[1] == Asura::SWING )
	{
		drawTriangle(	asuraDataSrcPtr->getLegFootPosition(1), asuraDataSrcPtr->getLegFootPosition(3), 
							asuraDataSrcPtr->getLegFootPosition(4), 2.0 );
	}
	else if ( phase[2] == Asura::SWING )
	{
		drawTriangle(	asuraDataSrcPtr->getLegFootPosition(1), asuraDataSrcPtr->getLegFootPosition(2), 
							asuraDataSrcPtr->getLegFootPosition(4), 2.0 );
	}
	else if ( phase[3] == Asura::SWING )
	{
		drawTriangle(	asuraDataSrcPtr->getLegFootPosition(1), asuraDataSrcPtr->getLegFootPosition(2), 
							asuraDataSrcPtr->getLegFootPosition(3), 2.0 );
	}
	else
	{
		drawQuadrangle(	asuraDataSrcPtr->getLegFootPosition(1), asuraDataSrcPtr->getLegFootPosition(2), 
								asuraDataSrcPtr->getLegFootPosition(4), asuraDataSrcPtr->getLegFootPosition(3),
								2.0 );
	}

	return;
}


/**
 *		AsuraGraphicクラスのprivateなメンバ関数
 *
 */
/**
 *		OpenGL座標変換のためのヘルプ関数
 *			OpenGL用の4x4座標変換
 */
void AsuraGraphic::transformOpenGLMatrix(const Matrix& matrix)
{
	/// 変換行列要素
	double T[16];

	int i;
	int j;
	int k=0;
	
	/// 行列要素を配列に格納
	for (i=1; i<=DH_DIMENSION; i++)
	{
		for (j=1; j<=DH_DIMENSION; j++)
		{
			T[k] = matrix(j, i);
			k++;
		}
	}

	/// 現在のマトリクスと掛ける
	glMultMatrixd(T);

	return;
}

/**
 *	説明
 *		影を投影する平面の係数を計算
 *	引数
 *		planeEq: 平面の方程式の係数配列
 *		p0: 平面上の頂点
 *		p1: 平面上の頂点
 *		p2: 平面上の頂点
 */
void AsuraGraphic::calcShadowPlaneEquation(GLfloat planeEq[], GLfloat p0[], GLfloat p1[], GLfloat p2[])
{
	GLfloat vector0[3], vector1[3];

	/// 平面の法線ベクトルを求めるため、外積計算に用いる2つのベクトルを求める
	vector0[0] = p1[0] - p0[0];
	vector0[1] = p1[1] - p0[1];
	vector0[2] = p1[2] - p0[2];

	vector1[0] = p2[0] - p0[0];
	vector1[1] = p2[1] - p0[1];
	vector1[2] = p2[2] - p0[2];

	/// 平面の方程式 ax+ by + cz = dのa, b, c, dを求める
	planeEq[0] = vector0[1]*vector1[2] - vector0[2]*vector1[1];
	planeEq[1] = -(vector0[0]*vector1[2] - vector0[2]*vector1[0]);
	planeEq[2] = vector0[0]*vector1[1] - vector0[1]*vector1[0];

	planeEq[3] = -(planeEq[0]*p0[0] + planeEq[1]*p0[1] + planeEq[2]*p0[2]);
	return;
}

/**
 *	説明
 *		目的の影を投影する行列
 *	引数
 *		matrix: 投影した行列
 *		plnaeEq: 平面の方程式の係数配列（要素数4）
 *		lightPos: 光源の同時座標位置（要素数4）
 */
void AsuraGraphic::calcShadowMatrix(GLfloat matrix[], GLfloat planeEq[], GLfloat lightPos[])
{
	GLfloat dot;

	/// 平面と光源の内積を計算
	dot = planeEq[0]*lightPos[0] + planeEq[1]*lightPos[1] +	planeEq[2]*lightPos[2] + planeEq[3]*lightPos[3];

	matrix[0]	= dot - lightPos[0] * planeEq[0];
	matrix[4]	= 0.f - lightPos[0] * planeEq[1];
	matrix[8]	= 0.f - lightPos[0] * planeEq[2];
	matrix[12]	= 0.f - lightPos[0] * planeEq[3];

	matrix[1]	= 0.f - lightPos[1] * planeEq[0];
	matrix[5]	= dot - lightPos[1] * planeEq[1];
	matrix[9]	= 0.f - lightPos[1] * planeEq[2];
	matrix[13]	= 0.f - lightPos[1] * planeEq[3];

	matrix[2]	= 0.f - lightPos[2] * planeEq[0];
	matrix[6]	= 0.f - lightPos[2] * planeEq[1];
	matrix[10]	= dot - lightPos[2] * planeEq[2];
	matrix[14]	= 0.f - lightPos[2] * planeEq[3];

	matrix[3]	= 0.f - lightPos[3] * planeEq[0];
	matrix[7]	= 0.f - lightPos[3] * planeEq[1];
	matrix[11]	= 0.f - lightPos[3] * planeEq[2];
	matrix[15]	= dot - lightPos[3] * planeEq[3];

	return;
}

}