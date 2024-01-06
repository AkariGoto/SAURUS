
#ifndef DESIGNLABROBOTGUI_MATH_MATRIX_MATRIX_LIBRARY_H_
#define DESIGNLABROBOTGUI_MATH_MATRIX_MATRIX_LIBRARY_H_

#include "Math/Matrix/matrix.h"


namespace designlab_robot_gui::math
{

/**
 *	説明
 *		零ベクトルを求める
 */
Vector zeroVector(int dimension);


/**
 *	説明
 *		3次元の外積を求める
 *		ベクトルのみで計算
 */
Vector outerProduct(const Vector& x, const Vector& y);

/**
 *	説明
 *		3次元の外積を求める
 *		成分のみで計算
 */
void outerProduct(double x1, double y1, double z1,
              double x2, double y2, double z2,
              double* x, double* y, double* z);

} // namespace designlab_robot_gui::math


#endif  // DESIGNLABROBOTGUI_MATH_MATRIX_MATRIX_LIBRARY_H_
