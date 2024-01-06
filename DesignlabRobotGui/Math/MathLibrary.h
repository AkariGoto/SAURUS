
#ifndef __MathLibrary_h__
#define __MathLibrary_h__

#include "MathConstant.h"

#include "Matrix\MatrixLibrary.h"
#include "Math/math_geometry.h"


inline double SplineM(double X, double Dx, double a, double b, double c, double d)
{
    return a + b * (-X + Dx) + c * pow(-X + Dx, 2) + d * pow(-X + Dx, 3);
}

#endif	/// __MathLibrary_h__