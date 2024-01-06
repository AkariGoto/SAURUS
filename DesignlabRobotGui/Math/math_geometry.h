
#ifndef __MathGeometry_h__
#define __MathGeometry_h__

#include <cmath>

#include "Matrix/MatrixLibrary.h"


namespace Math
{


//! @brief 点と点との距離
//! @param[in] point1 計測する点
//! @param[in] point2 直線上の点
double distancePointToPoint(const Vector& point1, const Vector& point2);

/**
 *		normalVectorTo: 垂直なベクトルを作成
 *			normalVectorToLine: 直線に垂直
 *			normalVectorToSegment: 線分に垂直
 */
extern Vector normalVectorToLine(const Vector& point, const Vector& linePoint, const Vector& lineDirectionVector);
extern Vector normalVectorToSegment(const Vector& point, const Vector& start, const Vector& end);

/**
 *		distancePointAndLine: 点と直線との距離
 *			point: 計測する点
 *			linePoint: 直線上の点
 *			lineDirectionVector: 直線の方向ベクトル
 */
extern double distancePointAndLine(const Vector& point, const Vector& linePoint, const Vector& lineDirectionVector);

/**
 *		DistancePointAndSegment: 点と線分との距離
 *			point: 計測する点
 *			start: 線分の始点
 *			end: 線分の終点
 */
extern double distancePointAndSegment(const Vector& point, const Vector& start, const Vector& end);


/**
 *		normalVectorOfPlane	: 平面に垂直なベクトルを作成
 *			引数:
 *			p1, p2, p3: 平面上の一直線上にない3点
 */
extern Vector normalVectorOfPlane(const Vector& p1, const Vector& p2, const Vector& p3);

/**
 *		IntersectPointLineAndPlane			: 直線と平面との交点
 *			引数:
 *				linePoint: 平面上の一直線上にない3点
 *				lineDirectionVector: 直線の方向ベクトル
 *				planePoint: 平面上の点
 *				planeNormalVector: 平面の法線ベクトル
 */
extern Vector intersectPointLineAndPlane(const Vector& linePoint, const Vector& lineDirectionVector,
                            const Vector& planePoint, const Vector& planeNormalVector);

/**
 *		isPointInsideTriangle
 *			三角形の中に点があるかどうか
 *				内側にある場合: true
 *				外側にある場合: false
 *			引数
 *				Vector a, b, c: 三角形の各頂点の位置ベクトル
 *				Vector p: 判定すべき点
 */
extern bool isPointInsideTriangle(const Vector& p, const Vector& a, const Vector& b, const Vector& c);

/**
 *		minDistanceToTriangleEdge
 *			ある内部点から三角形の各辺までの距離で最も最小の値を求める
 */
extern double minDistanceToTriangleEdge(const Vector& p, const Vector& a, const Vector& b, const Vector& c);

/**
 *		isPointInsideTriangle
 *			凸四角形の中に点があるかどうか
 *				内側にある場合: true
 *				外側にある場合: false
 *			引数
 *				Vector a, b, c, d: 四角形の各頂点の位置ベクトル
 *				Vector p: 判定すべき点
 */
extern bool isPointInsideQuadrangle(const Vector& p, const Vector& a, const Vector& b, const Vector& c, const Vector& d);

/**
 *		minDistanceToQuadrangleEdge
 *			ある内部点から四角形の各辺までの距離で最も最小の値を求める
 */
extern double minDistanceToQuadrangleEdge(const Vector& p, const Vector& a, const Vector& b, const Vector& c, const Vector& d);

}	/// end of namespace Math
#endif	/// __MathGeometry_h__