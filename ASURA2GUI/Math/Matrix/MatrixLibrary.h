/**
 *  ファイル名
 *		File.h
 *  説明
 *		行列計算ライブラリ
 *  日付
 *		作成日: 2007/05/01(Tue)		更新日: 2007/05/08(Tue)
 */

#ifndef __MatrixLibrary_h__
#define __MatrixLibrary_h__

/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */

#include "Matrix.h"
#include "..\..\Utility\EngConstant.h"

namespace Math
{

/**
 *	----------------------------------------------------------------------
 *		行列計算
 *	----------------------------------------------------------------------
 */

/**
 *	説明
 *		零行列を得る
 */
extern Matrix zeroMatrix(int rowNum, int columnNum);

/**
 *	説明
 *		単位行列を得る
 */
extern Matrix identityMartix(int rowNum, int columnNum);

/**
 *	説明
 *		回転軸と回転角から行列を得る
*	引数
 *		axis：回転方向ベクトル（軸ベクトル）
 *		angle：回転角
 */
extern Matrix rotationVector(const Vector& axis, double angle);

/**
 *	----------------------------------------------------------------------
 *		ベクトル計算
 *	----------------------------------------------------------------------
 */
/**
 *	説明
 *		零ベクトルを求める
 */
extern Vector zeroVector(int dimension);

/**
 *	説明
 *		位置ベクトルを求める
 */
extern Vector positionVector(double x, double y, double z);

/**
 *	説明
 *		内積を求める
 *		ベクトルのみで計算
 */
extern double innerProduct(const Vector& x, const Vector& y);

/**
 *	説明
 *		内積を求める
 *		成分のみで計算
 */
extern double innerProduct(double x1, double y1, double z1, double x2, double y2, double z2);

/**
 *	説明
 *		3次元の外積を求める
 *		ベクトルのみで計算
 */
extern Vector outerProduct(const Vector& x, const Vector& y);

/**
 *	説明
 *		3次元の外積を求める
 *		成分のみで計算
 */
extern void outerProduct(	double x1, double y1, double z1, 
							double x2, double y2, double z2,
							double* x, double* y, double* z);
}
#endif	/// __MatrixLibrary_h__