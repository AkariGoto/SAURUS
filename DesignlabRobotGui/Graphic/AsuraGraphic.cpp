/**
 *  �t�@�C����
 *		AsuraGraphic.cpp
 *  ����
 *		OpenGL��p����TITAN X��`�悷��N���X
 *  ���t
 *		�쐬��: 2007/05/12(SAT)		�X�V��: 2008/07/07(MON)
 */

//  20200819  �N���[���֘A�R�����g�A�E�g
//  20200822  �\������r�C��
//  20201019  �r��̃��[���h���W�Ƌr���W�̐؂�ւ�
//  202311�@ASURA�֘A�R�����g�A�E�g

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
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
 *		AsuraGraphic�N���X
 *	----------------------------------------------------------------------
 */

/**
 *	------------------------------------------------------------
 *		AsuraGraphic�N���X�̃����o�֐���`
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	�R���X�g���N�^�ƃf�X�g���N�^
 *	----------------------------------------
 */
/// �R���X�g���N�^
AsuraGraphic::AsuraGraphic(HWND hWnd, AsuraData* asuraData, ViewType type)
{
	/// ������
	initializeAsuraGraphic(hWnd, asuraData);

	/// ���_������
	setViewType(type);

}

/// �f�X�g���N�^
AsuraGraphic::~AsuraGraphic()
{
	finalizeAsuraGraphic();
}


/**
 *		�������E�I������
 */
/// ������
void AsuraGraphic::initializeAsuraGraphic(HWND hWnd, AsuraData* asuraData)
{
	/// �t���O������
	isAsuraDrawn = true;
	isSupportPolygonDrawn = false;
	isDisplayListReady = false;
	isAnimationRecorded = false;

	legCoordinate = false;  //20201019  �N�����͋r���W�n��\�����Ȃ�

	/// �ŏ���OpenGL�̃f�t�H���g���������s��
	if (!OpenGLObject::createGLObject(hWnd))
		return;

	/// �|�C���^���L����������|�C���^���Z�b�g
	if ( asuraData != NULL )
		asuraDataSrcPtr = asuraData;

	/// �f�B�X�v���C���X�g�쐬
	newAsuraDisplayList();

	/// �e�̏���
	GLfloat floorP1[3] = {1.0f, 0.0f, (GLfloat)GRID_HEIGHT};
	GLfloat floorP2[3] = {0.0f, 1.0f, (GLfloat)GRID_HEIGHT};
	GLfloat floorP3[3] = {-1.0f, -1.0f, (GLfloat)GRID_HEIGHT};

	calcShadowPlaneEquation(planeFactorOfGridFloor, floorP1, floorP2, floorP3);

	RECT rect;
	GetClientRect(hWnd, &rect);

	/// AVI����
	movieRecorder.initialize((rect.right/4)*4, (rect.bottom/4)*4, 24);
}

/// �I������
void AsuraGraphic::finalizeAsuraGraphic(void)
{
	/// �f�B�X�v���C���X�g�j��
	deleteAsuraDisplayList();
}

/**
 *	����
 *		�f�B�X�v���C���X�g�̐����E����
 */
void AsuraGraphic::newAsuraDisplayList(void)
{
	if ( isDisplayListReady )
		return;

	/// �f�B�X�v���C���X�g�̐���
	int bodyList = glGenLists(1);
	int legList = glGenLists(LEG_DISP_LIST_NUM);

	/*  02020819
	int trackList = glGenLists(TRACK_DISP_LIST_NUM);

	// ��������
	if (bodyList == 0 || legList == 0 || trackList == 0 )
	{
		cerr << "Error: [OpenGL::newAsuraDisplayList] Display List is not generated\n" << endl;
		return;
	}
	*/

	/// ���̗p�̃f�B�X�v���C���X�g
	bodyDisplayListID = bodyList;

	/// �r�p�̃f�B�X�v���C���X�g�̏���
	int i;
	for (i=0; i<LEG_DISP_LIST_NUM; i++)
		legDisplayListID[i] = legList + i;

	/// �N���[���p�̃f�B�X�v���C���X�g�̏���
/*	int j;
	for (j=0; j<TRACK_DISP_LIST_NUM; j++)
		trackDisplayListID[j] = trackList + j;*/

	/// �֐ߐF
	//SAURUS
	const GLfloat jointColor[4] = { 0.55f, 0.55f, 0.65f, 1.0f };
	const GLfloat linkColor[4] = {0.156f, 0.40f, 1.0f, 1.0f};

	/*ASURA20231128�ύX
	 const GLfloat linkColor[4] = {0.55f, 0.55f, 0.65f, 1.0f};
	 const GLfloat jointColor[4] = {0.10f, 0.50f, 0.50f, 1.0f};
	*/
	//const GLfloat joint4Color[4] = {0.0f, 0.0f, 1.0f, 1.0f};
/**
 *		���̗p�̃f�B�X�v���C���X�g�̍쐬
 */
	/// body
	glNewList(bodyDisplayListID, GL_COMPILE);
			setMaterialColor( linkColor );
			drawBox(	-BODY_LENGTH/2, -BODY_WIDTH/Const::ROOT_2/2+50, -BODY_HEIGHT/2,
							BODY_LENGTH/2, BODY_WIDTH/Const::ROOT_2/2-50, BODY_HEIGHT/2 );
			drawBox(	-BODY_LENGTH/Const::ROOT_2/2+50, -BODY_WIDTH/2, -BODY_HEIGHT/2,
							BODY_LENGTH/Const::ROOT_2/2-50, BODY_WIDTH/2, BODY_HEIGHT/2 );

/*		glTranslated(BODY_LENGTH/Const::ROOT_2/2, 0, 0);	/// ���W�ϊ�
			drawBox(	-40, -BODY_WIDTH/Const::ROOT_2/2, -BODY_HEIGHT/2,
							40, BODY_WIDTH/Const::ROOT_2/2, BODY_HEIGHT/2 );

		glTranslated(-BODY_LENGTH, 0, 0);	/// ���W�ϊ�
			drawBox(	-40, -BODY_WIDTH*2/3, -BODY_HEIGHT/2,
							40, BODY_WIDTH*2/3, BODY_HEIGHT/2 );
*/	glEndList();

/**
 *		�r�p�̃f�B�X�v���C���X�g�̍쐬
 */
	/// Base
	glNewList(legDisplayListID[0], GL_COMPILE);
			setMaterialColor( jointColor );	///�֐�	/// ���_
			drawCylinder(30, BODY_HEIGHT+10);
			//drawCylinder(50, BODY_HEIGHT + 50);ASURA_20231128�ύX
		//glTranslated(0, 0, -LINK1/2);	/// ���W�ϊ�		
			//setMaterialColor( linkColor );
			//drawBox(-30, -30, -LINK1/2, 30, 30, LINK1/2);			
	glEndList();

	/// 1.Hip
	glNewList(legDisplayListID[1], GL_COMPILE);
			setMaterialColor( jointColor );	///�֐�	/// ���_
			drawCylinder(30, BODY_HEIGHT+10);
			//drawCylinder(50, BODY_HEIGHT + 10);ASURA_20231128�ύX
		glTranslated(-LINK1, 0,0 );	/// ���W�ϊ�
			setMaterialColor( linkColor );
			drawBox(0, -35,-35 ,  LINK1, 35,35);
			//drawBox(0, -45, -45, LINK1,45, 45);ASURA_20231128�ύX
	glEndList();

	/// 2.Thigh
	glNewList(legDisplayListID[2], GL_COMPILE);
			setMaterialColor( jointColor );///�֐�	
			drawCylinder(30, 60);
			//drawCylinder(40, 70);ASURA_20231128�ύX
		glTranslated(-LINK2, 0, 0);	/// ���W�ϊ�
			setMaterialColor( linkColor );
			drawBox(0, -30, -30, LINK2, 30, 30);
			setMaterialColor( jointColor );///�֐�	
			drawCylinder(30, 60);
			//drawCylinder(40, 110);ASURA_20231128�ύX
	glEndList();

	 /// 3.Shank
	glNewList(legDisplayListID[3], GL_COMPILE);
		glTranslated(-LINK3, 0, 0);	/// ���W�ϊ�
			setMaterialColor( linkColor );
			drawBox(0,-25,-25, LINK3, 25, 25);
			setMaterialColor( jointColor );///�֐�	
			drawCylinder(30, 60);
			//drawCylinder(40, 70); ASURA_20231128�ύX
	glEndList();

	
	 /// 4.Foot
	glNewList(legDisplayListID[4], GL_COMPILE);
		glTranslated(-FOOT, 0, 0);	/// ���W�ϊ�
			setMaterialColor( linkColor );
			drawBox(0, -25, -25, FOOT, 25, 25);//drawBox(3*FOOT/5, -40, -35, FOOT, 40, 35);
			setMaterialColor( jointColor );///�֐�	
			drawCylinder(30, 60);
			//drawCylinder(40, 70); ASURA_20231128�ύX
	glEndList();
	

/**
 *		�N���[���p�̃f�B�X�v���C���X�g�̍쐬
 */	
/*	/// Base
	glNewList(legDisplayListID[0], GL_COMPILE);
			setMaterialColor( jointColor );	///�֐�	/// ���_
			drawCylinder(50, BODY_HEIGHT+10);
		//glTranslated(0, 0, -LINK1/2);	/// ���W�ϊ�
			//setMaterialColor( linkColor );
			//drawBox(-30, -30, -LINK1/2, 30, 30, LINK1/2);
	glEndList();

	/// Hip
	glNewList(legDisplayListID[1], GL_COMPILE);
			setMaterialColor( jointColor );	///�֐�	/// ���_
			drawCylinder(50, BODY_HEIGHT+10);
		glTranslated(-LINK1, 0,0 );	/// ���W�ϊ�
			setMaterialColor( linkColor );
			drawBox(0, -45,-45 ,  LINK1, 45,45);
	glEndList();

	/// Thigh
	glNewList(legDisplayListID[2], GL_COMPILE);
			setMaterialColor( jointColor );///�֐�	
			drawCylinder(40, 70);
		glTranslated(-LINK2, 0, 0);	/// ���W�ϊ�
			setMaterialColor( linkColor );
			drawBox(0, -30, -30, LINK2, 30, 30);
			setMaterialColor( jointColor );///�֐�	
			drawCylinder(40, 110);
	glEndList();

	 /// Shank
	glNewList(legDisplayListID[3], GL_COMPILE);
		glTranslated(-LINK3, 0, 0);	/// ���W�ϊ�
			setMaterialColor( linkColor );
			drawBox(0,-25,-25, LINK3, 25, 25);
			setMaterialColor( jointColor );///�֐�	
			drawCylinder(40, 70);
	glEndList();
*/
	/// ��������
	isDisplayListReady = true;

}

void AsuraGraphic::deleteAsuraDisplayList(void)
{
	/// �f�B�X�v���C���X�g�̏���
	glDeleteLists(bodyDisplayListID, 1);
	glDeleteLists(legDisplayListID[0], LEG_DISP_LIST_NUM);

	/*  20200819
	glDeleteLists(trackDisplayListID[0], TRACK_DISP_LIST_NUM);
	*/
}

/**
 *	----------------------------------------
 *	�`��Ɋ֌W�������
 *	----------------------------------------
 */
/**
 *	����
 *		OpenGL�̃I�u�W�F�N�g�`��
 */
void AsuraGraphic::drawObjects(GLenum renderMode)
{
	Matrix gAb(DH_DIMENSION, DH_DIMENSION);  //20201019

	switch (renderMode)
	{
		case GL_RENDER:
				/// �V�[����`��
				glDisable(GL_CULL_FACE);	
					/// ���W�n�`��
					drawCoordinateAxis(300.0, 5.0, 0.8);
					/// Asura�`��
					drawAsura();
				glEnable(GL_CULL_FACE);	

				//20201019
				if (legCoordinate)
				{
					glDisable(GL_CULL_FACE);
					glPushMatrix();
					/// �r���W�n�ɕϊ�
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
 *	����
 *		OpenGL�C���[�W�`��
 */
void AsuraGraphic::drawScenes(void)
{
	drawObjects(GL_RENDER);
	return;
}

/**
 *	����
 *		�e�`��
 */
void AsuraGraphic::drawShadow(void)
{
	/// �e�̌���
	float lightPosition[] = {60, 80, 3000, 1};

	/// ���e�s��F4x4�̓����ϊ��s��
	GLfloat shadowMatrix[16];

	///�X�e���V���o�b�t�@�N���A�l�̐ݒ�
	glClearStencil( 0 );

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_POSITION, (float*)lightPosition);
	/// ���ʎˉe�s��̎Z�o
	calcShadowMatrix(shadowMatrix, planeFactorOfGridFloor, lightPosition);

	
	/// ���̃X�e���V����t����
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, ~0);
	
	/// ���ꂩ��`�悷����̂̃X�e���V���l�ɂ��ׂĂP�^�O������
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	/// ���̕`��
	drawGridFloor();

	/**
	 *		�J���[�E�f�v�X�o�b�t�@�}�X�N���Z�b�g����
	 *		����ňȉ��̓��e�̃s�N�Z���̐F�̒l�́A�������܂�Ȃ��B
	 */
	glColorMask(0, 0, 0, 0);
	glDepthMask(0);

	/**
	 *		���ɃI�u�W�F�N�g�̉e�̃X�e���V����t����
	 */
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_EQUAL, 1, ~0);

	/**
	 *		���ꂩ��`�悷����̂̃X�e���V���l�ɂ��ׂĂP�^�O������
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
	 *		�r�b�g�}�X�N������	
	 */
	glColorMask(1, 1, 1, 1);
	glDepthMask(1);

	glCullFace( GL_FRONT);
	glEnable(GL_CULL_FACE );
	glEnable(GL_AUTO_NORMAL );


	/**
	 *		�e������
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
 *	����
 *		OpenGL�C���[�W�̃����_�����O
 *		OpenGL�`��̃R�[���o�b�N�֐��ɂȂ�
 */
void AsuraGraphic::renderScenes(void)
{
	/// �����_�����O�R���e�L�X�g���J�����g�ɂ���
	if (wglMakeCurrent(deviceContextHandle, renderingContextHandle))
	{
		/// �o�b�t�@���N���A�i�w�肵���o�b�t�@�����̐F�ŏ����j
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glLoadIdentity();

		/// ���_����
		setSceneView(sceneWidth, sceneHeight);

		/**
		 *	�I�u�W�F�N�g�̕`��
		 */
		/// ���̍��W�n��ۑ����Ă���
		glPushMatrix();
		{
			/// �V�[����`��
			drawScenes();

			/// �e�`��
			drawShadow();

			/// �x���r���p�`�`��
			drawSupportPolygon();
		}
		/// ���W�n��߂�
		glPopMatrix();

		/// �R�}���h�̃t���b�V��
		if (isAnimationRecorded)//
			movieRecorder.recordFrame();
		else
			glFlush();
	}

	/**
	 *		�o�b�t�@�̐ؑ�
	 *			�o�b�N�o�b�t�@���t�����g�o�b�t�@�ɐ؂�ւ��C�V�����摜��������
	 */
	SwapBuffers( wglGetCurrentDC() );

	wglMakeCurrent(deviceContextHandle, NULL);

	return;
}


/**
 *		�V�[���̎��_��ViewType�ɂ��ݒ�
 */
void AsuraGraphic::setSceneView(double width, double height)
{
	/// ���_�����ɂ�莋�_��ύX
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
 *		Asura�̕`��
 */
void AsuraGraphic::drawAsura(void)
{
	if (!isAsuraDrawn)
		return;

	/// ���̕`��
	drawBody();

	/// ���R���[�V�����l���ɂ��`��𕪂���
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
 *		Asura�̖{�̕`��
 */
void AsuraGraphic::drawBody(void)
{
	/// �O���[�o�����瓷�̍��W�ւ̕ϊ��s��
	Matrix gAb(DH_DIMENSION, DH_DIMENSION);

	glPushMatrix();
		/// ���̍��W�n�ɕϊ�
		gAb = asuraDataSrcPtr->getBodyTransformation();
		transformOpenGLMatrix(gAb);
			glCallList(bodyDisplayListID);
	glPopMatrix();

	return;
}

/**
 *		Asura�̋r�`��
 */
void AsuraGraphic::drawLeg(int legNo)
{
	if (!isDisplayListReady)
		return;

	/// �r�̈����`�F�b�N
	assert( 1 <= legNo && legNo <= LEG_NUM ); 

	Matrix trans(DH_DIMENSION, DH_DIMENSION);	/// ���W�ϊ��p�̍s��
	Matrix bAl(DH_DIMENSION, DH_DIMENSION);		/// ���� -> �r


	glPushMatrix();
		
		/// �r�̃x�[�X���W�n���擾���āC�r���W�ɕϊ�
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
 *		Asura�̃g���b�N�`��
 */
void AsuraGraphic::drawTrack(int trackNo)
{
	/*if (!isDisplayListReady)
		return;

	/// �r�̈����`�F�b�N
	assert( 1 <= trackNo && trackNo <= LEG_NUM ); 

	Matrix trans(DH_DIMENSION, DH_DIMENSION);	/// ���W�ϊ��p�̍s��
	Matrix bAl(DH_DIMENSION, DH_DIMENSION);		/// ���� -> �r


	glPushMatrix();
		
		/// �r�̃x�[�X���W�n���擾���āC�r���W�ɕϊ�
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
 *		�x���r���p�`(�O�p�`or�l�p�`)�̕`��
 */
void AsuraGraphic::drawSupportPolygon(void)
{

	/// �`��t���O
	if ( !isSupportPolygonDrawn )
		return;

	int i;
	Asura::LegPhase phase[LEG_NUM];

	for (i=0; i<LEG_NUM; i++)
	{
		phase[i] = asuraDataSrcPtr->getLegPhase(i+1);
	}

	/// �x���r���p�`�̐F����
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
 *		AsuraGraphic�N���X��private�ȃ����o�֐�
 *
 */
/**
 *		OpenGL���W�ϊ��̂��߂̃w���v�֐�
 *			OpenGL�p��4x4���W�ϊ�
 */
void AsuraGraphic::transformOpenGLMatrix(const Matrix& matrix)
{
	/// �ϊ��s��v�f
	double T[16];

	int i;
	int j;
	int k=0;
	
	/// �s��v�f��z��Ɋi�[
	for (i=1; i<=DH_DIMENSION; i++)
	{
		for (j=1; j<=DH_DIMENSION; j++)
		{
			T[k] = matrix(j, i);
			k++;
		}
	}

	/// ���݂̃}�g���N�X�Ɗ|����
	glMultMatrixd(T);

	return;
}

/**
 *	����
 *		�e�𓊉e���镽�ʂ̌W�����v�Z
 *	����
 *		planeEq: ���ʂ̕������̌W���z��
 *		p0: ���ʏ�̒��_
 *		p1: ���ʏ�̒��_
 *		p2: ���ʏ�̒��_
 */
void AsuraGraphic::calcShadowPlaneEquation(GLfloat planeEq[], GLfloat p0[], GLfloat p1[], GLfloat p2[])
{
	GLfloat vector0[3], vector1[3];

	/// ���ʂ̖@���x�N�g�������߂邽�߁A�O�όv�Z�ɗp����2�̃x�N�g�������߂�
	vector0[0] = p1[0] - p0[0];
	vector0[1] = p1[1] - p0[1];
	vector0[2] = p1[2] - p0[2];

	vector1[0] = p2[0] - p0[0];
	vector1[1] = p2[1] - p0[1];
	vector1[2] = p2[2] - p0[2];

	/// ���ʂ̕����� ax+ by + cz = d��a, b, c, d�����߂�
	planeEq[0] = vector0[1]*vector1[2] - vector0[2]*vector1[1];
	planeEq[1] = -(vector0[0]*vector1[2] - vector0[2]*vector1[0]);
	planeEq[2] = vector0[0]*vector1[1] - vector0[1]*vector1[0];

	planeEq[3] = -(planeEq[0]*p0[0] + planeEq[1]*p0[1] + planeEq[2]*p0[2]);
	return;
}

/**
 *	����
 *		�ړI�̉e�𓊉e����s��
 *	����
 *		matrix: ���e�����s��
 *		plnaeEq: ���ʂ̕������̌W���z��i�v�f��4�j
 *		lightPos: �����̓������W�ʒu�i�v�f��4�j
 */
void AsuraGraphic::calcShadowMatrix(GLfloat matrix[], GLfloat planeEq[], GLfloat lightPos[])
{
	GLfloat dot;

	/// ���ʂƌ����̓��ς��v�Z
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