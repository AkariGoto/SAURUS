/**
 *  File: GraphicParameter.h
 *
 *  Description: グラフィックに関するパラメータの宣言
 *
 *  Created: 2007/05/08(Tue)
 *  Updated: 2007/05/08(Tue)
 *
 *  Copyright (C) Tokyo Institute of Technology Hirose Lab.
 */
/**
 *  ファイル名
 *		OpenGLParameter.h
 *  説明
 *		グラフィックに関するパラメータの宣言
 *  日付
 *		作成日: 2007/05/08(TUE)		更新日: 2008/09/14(SUN)
 */

#ifndef __OpenGLParameter_h__
#define __OpenGLParameter_h__

namespace Graphic
{
/**
 *	gluPerspective() で使用するデフォルト値
 */
const GLdouble PERS_DEFAULT_FOVY	= 45.0;
const GLdouble PERS_DEFAULT_NEAR	= 0.5;
const GLdouble PERS_DEFAULT_FAR		= 500;

/**
 *	OpenGLのマテリアル色
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
 *	グリッド表示するための定数
 */
const double GRID_START_X		= -20000.0;
const double GRID_START_Y		= -20000.0;
const double GRID_END_X			= 20000.0;
const double GRID_END_Y			= 20000.0;
const double GRID_HEIGHT		= 0.0;
const double GRID_DX			= 500.0;
const double GRID_DY			= 500.0;

/**
 *	ピッキングに使用する定数
 */
const int SELECT_BUFF_SIZE = 1024;
const int NAME_ARRAY_SIZE = 10;

/**
 *	視点パターン
 */
typedef enum
{
	PERSPECTIVE, 
	TOP, 
	SIDE, 
	FRONT
} ViewType;

/**
 *	主な色パターン
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