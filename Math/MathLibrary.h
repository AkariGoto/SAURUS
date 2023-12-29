/**
 *  File: MathLibrary.h
 *
 *  Description: 演算子やメソッドなど行列の主な演算が定義されている
 *
 *  Created: 2007/01/15(Mon)
 *  Updated: 2007/03/06(Tue)
 *
 *  Copyright (C) Tokyo Institute of Technology Hirose Lab.
 */

#ifndef __MathLibrary_h__
#define __MathLibrary_h__

#include "MathConstant.h"

#include "Matrix\MatrixLibrary.h"
#include "MathGeometry.h"

	inline double SplineM(double X,double Dx,double a,double b, double c,double d)
	{
		return a+b*(-X+Dx)+c*pow(-X+Dx,2)+d*pow(-X+Dx,3);}

#endif	/// __MathLibrary_h__