
#ifndef DESIGNLABROBOTGUI_GRAPHIC_OPENGL_OPEN_GL_PARAMETER_H_
#define DESIGNLABROBOTGUI_GRAPHIC_OPENGL_OPEN_GL_PARAMETER_H_

#include <gl/GL.h>


namespace designlab_robot_gui::graphic
{

// gluPerspective() で使用するデフォルト値．

const GLdouble PERS_DEFAULT_FOVY = 45.0;
const GLdouble PERS_DEFAULT_NEAR = 0.5;
const GLdouble PERS_DEFAULT_FAR = 500;


// AbstractOpenGLBase のマテリアル色．

const GLfloat materialAmbDiffRed[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
const GLfloat materialAmbDiffGreen[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
const GLfloat materialAmbDiffBlue[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
const GLfloat materialAmbDiffYellow[4] = { 1.0f, 0.8f, 0.0f, 1.0f };
const GLfloat materialAmbDiffSky[4] = { 0.0f, 0.7f, 1.0f, 1.0f };
const GLfloat materialAmbDiffOrange[4] = { 1.0f, 0.5f, 0.0f, 1.0f };
const GLfloat materialAmbDiffPurple[4] = { 1.0f, 0.0f, 1.0f, 1.0f };
const GLfloat materialAmbDiffGray[4] = { 0.6f, 0.6f, 0.6f, 1.0f };
const GLfloat materialAmbDiffWhite[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat materialAmbDiffBlack[4] = { 0.0f, 0.0f, 0.0f, 1.0f };


// グリッド表示するための定数．

const double GRID_START_X = -20000.0;
const double GRID_START_Y = -20000.0;
const double GRID_END_X = 20000.0;
const double GRID_END_Y = 20000.0;
const double GRID_HEIGHT = 0.0;
const double GRID_DX = 500.0;
const double GRID_DY = 500.0;


// ピッキングに使用する定数．

const int SELECT_BUFF_SIZE = 1024;
const int NAME_ARRAY_SIZE = 10;


//! @enum ViewType
//! @brief 視点パターン．
enum class ViewType
{
    PERSPECTIVE,
    TOP,
    SIDE,
    FRONT
};

//! @enum MaterialColor
//! @brief 主な色パターン．
enum class  MaterialColor
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
};

}  // namespace designlab_robot_gui::graphic

#endif  // DESIGNLABROBOTGUI_GRAPHIC_OPENGL_OPEN_GL_PARAMETER_H_
