/**
 *  File: GraphicParameter.h
 *
 *  Description: �O���t�B�b�N�Ɋւ���p�����[�^�̐錾
 *
 *  Created: 2007/05/08(Tue)
 *  Updated: 2007/05/08(Tue)
 *
 *  Copyright (C) Tokyo Institute of Technology Hirose Lab.
 */
/**
 *  �t�@�C����
 *		OpenGLParameter.h
 *  ����
 *		�O���t�B�b�N�Ɋւ���p�����[�^�̐錾
 *  ���t
 *		�쐬��: 2007/05/08(TUE)		�X�V��: 2008/09/14(SUN)
 */

#ifndef __OpenGLParameter_h__
#define __OpenGLParameter_h__

namespace Graphic
{
/**
 *	gluPerspective() �Ŏg�p����f�t�H���g�l
 */
const GLdouble PERS_DEFAULT_FOVY	= 45.0;
const GLdouble PERS_DEFAULT_NEAR	= 0.5;
const GLdouble PERS_DEFAULT_FAR		= 500;

/**
 *	OpenGL�̃}�e���A���F
 */
const GLfloat materialAmbDiffRed[4]		= {1.0f, 0.0f, 0.0f, 1.0f};
const GLfloat materialAmbDiffGreen[4]	= {0.0f, 1.0f, 0.0f, 1.0f};
const GLfloat materialAmbDiffBlue[4]	= {0.0f, 0.0f, 1.0f, 1.0f};
const GLfloat materialAmbDiffYellow[4]	= {1.0f, 0.8f, 0.0f, 1.0f};
const GLfloat materialAmbDiffSky[4]		= {0.0f, 0.7f, 1.0f, 1.0f};
const GLfloat materialAmbDiffOrange[4]	= {1.0f, 0.5f, 0.0f, 1.0f};
const GLfloat materialAmbDiffPurple[4]	= {1.0f, 0.0f, 1.0f, 1.0f};
const GLfloat materialAmbDiffGray[4]	= {0.6f, 0.6f, 0.6f, 1.0f};
const GLfloat materialAmbDiffWhite[4]	= {1.0f, 1.0f, 1.0f, 1.0f};
const GLfloat materialAmbDiffBlack[4]	= {0.0f, 0.0f, 0.0f, 1.0f};

/**
 *	�O���b�h�\�����邽�߂̒萔
 */
const double GRID_START_X		= -20000.0;
const double GRID_START_Y		= -20000.0;
const double GRID_END_X			= 20000.0;
const double GRID_END_Y			= 20000.0;
const double GRID_HEIGHT		= 0.0;
const double GRID_DX			= 500.0;
const double GRID_DY			= 500.0;

/**
 *	�s�b�L���O�Ɏg�p����萔
 */
const int SELECT_BUFF_SIZE = 1024;
const int NAME_ARRAY_SIZE = 10;

/**
 *	���_�p�^�[��
 */
typedef enum
{
	PERSPECTIVE, 
	TOP, 
	SIDE, 
	FRONT
} ViewType;

/**
 *	��ȐF�p�^�[��
 */
typedef enum
{
	RED, 
	GREEN, 
	BLUE, 
	YELLOW, 
	GRAY, 
	BLACK, 
	WHITE, 
	SKY, 
	ORANGE, 
	PURPLE
} COLOR;

}

#endif	/// __OpenGLParameter_h__