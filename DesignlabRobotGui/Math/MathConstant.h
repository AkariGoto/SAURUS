/**
 *  File: MathConstant.h
 *
 *  Description: 数学で使用頻度の高い定数
 *
 *  Created: 2007/02/06(Mon)
 *  Updated: 2007/02/06(Mon)
 *
 *  Copyright (C) Tokyo Institute of Technology Hirose Lab.
 */

#ifndef __MathConstant_h__
#define __MathConstant_h__

namespace Const
{
/**
 *		数学
 */
/// √2
const double	ROOT_2				= 1.4142135623730950488;
/// √3
const double	ROOT_3				= 1.7320508075688772935;
/// √10
const double	ROOT_10			= 3.1622776601683793320;
/// log2
const double	LOG_2				= 0.3010299956639811952;
/// log3
const double	LOG_3				= 0.4771212547196624373;
/// loge
const double	LOG_E				= 0.4342944819032518277;
/// ln10
const double	LN_10				= 2.3025850929940456840;
/// π
const double	PI	   					= 3.1415926535897932385;
/// e
const double	E						= 2.7182818284590452354;
/// 1/π
const double	ONE_OVER_PI		= 0.3183098861837906715;
/// 1/e
const double	ONE_OVER_E		= 0.3678794411714423216;
/// √π
const double	ROOT_PI				= 1.7724538509055160273;
/// euler数
const double	EULER				= 0.5772156649015328606;

/**
 *		変換
 */
/// 度 -> ラジアン
const double	DEG2RAD			= 0.01745329251994329576;			/// PI/180.0
/// ラジアン -> 度	
const double	RAD2DEG			= 57.2957795130823208767;			/// 180.0/PI

/**
 *		Computer science
 */
const bool		 ON					= true;
const bool		 OFF					= false;
const double	 EPS					= 1.0e-13;
		
}	/// end of namespace Const
#endif  /// __MathConstant_h__