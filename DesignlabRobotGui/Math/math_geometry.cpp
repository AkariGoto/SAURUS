
#include "Math/math_geometry.h"

#include "Matrix\MatrixLibrary.h"

using namespace std;
using namespace Const;

namespace Math
{

double distancePointToPoint(const Vector& point1, const Vector& point2)
{
    assert(point1.getSize() == THREE_DIMENSION ||
           point2.getSize() == THREE_DIMENSION);

    Vector temp;
    temp = point2 - point1;

    return temp.norm();

}

/**
 *		normalVectorTo: 垂直なベクトルを作成
 *			normalVectorToLine: 直線に垂直
 *			normalVectorToSegment: 線分に垂直
 */

 /**
  *		point: 計測する点
  *		linePoint					: 直線上の点
  *		lineDirectionVector	: 直線の方向ベクトル
  *
  * →  →  →				→
  *	l = a + tb (直線)		c (点)
  *	   →   →  →                      →
  *	(1)c  -  a のb 方向への正射影ベクトル sを考える
  *                 →          →
  *	(2)上の正射影 sベクトルと c の差分ベクトルを取る
  *	(3)そのベクトルの大きさが垂線の距離, つまり点と直線との距離
  */
Vector normalVectorToLine(const Vector& point, const Vector& linePoint, const Vector& lineDirectionVector)
{
    assert(point.getSize() == THREE_DIMENSION ||
           linePoint.getSize() == THREE_DIMENSION ||
           lineDirectionVector.getSize() == THREE_DIMENSION);

    double ip = 0;	/// 正射影ベクトルのための内積値
    Vector temp(THREE_DIMENSION);	/// 計算のためのベクトル
    temp = lineDirectionVector;

    Vector s(THREE_DIMENSION);	/// 正射影ベクトル
    Vector normal(THREE_DIMENSION);	/// 法線ベクトル

    ip = innerProduct((point - linePoint), lineDirectionVector);

    /// 垂直だったら
    if (fabs(ip) < 1.0E-15)
        return linePoint - point;

    /// → →  →
    /// c - a の b 方向への正射影ベクトルを計算
    s = (ip / temp.norm()) * (lineDirectionVector / temp.norm()); /// 単位方向ベクトル

    // 垂線ベクトルを求める
    normal = s + linePoint - point;

    return normal;

}

Vector normalVectorToSegment(const Vector& point, const Vector& start, const Vector& end)
{
    assert(point.getSize() == THREE_DIMENSION || start.getSize() == THREE_DIMENSION || end.getSize() == THREE_DIMENSION);

    /// 法線ベクトル
    Vector normal(THREE_DIMENSION);

    /// 線分の方程式を用いて法線ベクトルを求める
    normal = normalVectorToLine(point, start, (end - start));

    return normal;
}

/**
 *		distancePointAndLine	: 点と直線との距離(Distance between point and line)
 *			point: 計測する点
 *		line_point: 直線上の点
 *		line_dir					: 直線の方向ベクトル
 * → →   →				→
 *	l = a + tb (直線)		c (直線)
 *	   →  →  →                   →
 *	(1)c - a のb 方向への正射影ベクトルsを考える
 *                         →
 *	(2)上の正射影ベクトルとc の差分ベクトルを取る
 *	(3)そのベクトルの大きさが垂線の距離, つまり点と直線との距離
 */
double distancePointAndLine(const Vector& point, const Vector& linePoint, const Vector& lineDirectionVector)
{
    Vector normal(THREE_DIMENSION); /// 法線ベクトル

    /// 垂線ベクトルの大きさが求める距離
    normal = normalVectorToLine(point, linePoint, lineDirectionVector);

    return normal.norm();
}

/**
 *		distancePointAndSegment: 点と線分との距離
 *			point: 計測する点
 *			start: 線分の始点
 *			end: 線分の終点
 */
double distancePointAndSegment(const Vector& point, const Vector& start, const Vector& end)
{
    Vector normal(THREE_DIMENSION); /// 法線ベクトル

    // 垂線ベクトルの大きさが求める距離
    normal = normalVectorToSegment(point, start, end);

    return normal.norm();
}

/**
 *		normalVectorOfPlane: 平面に垂直なベクトルを作成
 *			引数:
 *				p1, p2, p3: 平面上の一直線上にない3点
 *
 *	→	 →   →   →   →
 *	n = (p2 - p1) X (p3 - p1): 外積で求める. 法線ベクトルnは正規化してある
 */
Vector normalVectorOfPlane(const Vector& p1, const Vector& p2, const Vector& p3)
{
    /// 3次元ベクトル判定
    assert(p1.getSize() == THREE_DIMENSION || p2.getSize() == THREE_DIMENSION || p3.getSize() == THREE_DIMENSION);

    Vector vector1(THREE_DIMENSION);
    Vector vector2(THREE_DIMENSION);
    Vector normal(THREE_DIMENSION);

    vector1 = p2 - p1;
    vector2 = p3 - p1;

    /// 一直線上にないことをチェック
    double ratio1 = 0;
    double ratio2 = 0;
    double ratio3 = 0;

    ratio1 = vector2(1) / vector1(1);
    ratio2 = vector2(2) / vector1(2);
    ratio3 = vector2(3) / vector1(3);

    if ((fabs(ratio1 - ratio2) < 1.0E-15) &&
       (fabs(ratio2 - ratio3) < 1.0E-15) &&
       (fabs(ratio3 - ratio1) < 1.0E-15))
    {
        Vector zero(THREE_DIMENSION);
        return zero;
    }

    /// 法線ベクトルを計算
    normal = outerProduct(vector1, vector2);

    return normal.normalize();
}


/**
 *		intersectPointLineAndPlane	: 直線と平面との交点
 *			引数:
 *				linePoint: 直線上の点
 *				lineDirectionVector: 直線の方向ベクトル
 *				planePoint: 平面上の点
 *				planeNormalVector: 平面の法線ベクトル
 *
 *						   →   →
 *		平面の方程式	n ・ P + d = 0
 *			    →			      →		  →  →  →
 *		交点をP 平面上の点をP0とすると n ・(P - P0) = 0
 *					      →   →					         →  →    →
 *		平面の方程式 n ・ P + d = 0 に直線の方程式 P = a + t・b を代入
 *						     →   →  →	 →  →
 *		これにより, t = -n ・( a - P0 )/(n ・ b) と求まる
 */
Vector intersectPointLineAndPlane(const Vector& linePoint, const Vector& lineDirectionVector,
                        const Vector& planePoint, const Vector& planeNormalVector)
{
    double a, b;
    double t;		/// 直線の媒介変数

    Vector zero(THREE_DIMENSION);
    Vector vector(THREE_DIMENSION);
    Vector point(THREE_DIMENSION);

    vector = linePoint - planePoint;

    a = -innerProduct(planeNormalVector, vector);
    b = innerProduct(planeNormalVector, lineDirectionVector);

    /// 平行なので交点なし
    if (fabs(b) < 1.0E-15) return zero;

    /// 媒介変数tを求める
    t = a / b;

    /// 直線の方程式に代入し交点を求める
    point = linePoint + lineDirectionVector * t;

    return point;
}

/**
 *		isPointInsideTriangle
 *			三角形の中に点があるかどうか
 *				内側にある場合: true
 *				外側にある場合: false
 *			引数
 *				Vector a, b, c: 三角形の各頂点の位置ベクトル
 *				Vector p: 判定すべき点
 */
 /**
  *										/// △ABCの法線をN, 判定点Pとする
  *			A						/// 点Pから三角形の頂点へのベクトルを作成
  *		   ／＼					/// PA =A-P, PB = B-P, PC = C-P
  *		 ／    ＼				/// △PAB, △PBC, △PCAの法線をN1, N2, N3とする
  *	   ／	  　 ＼			/// N1=PA X PB, N2=PB X PC, N3=PC X PA
  *	 ／		 P     ＼			/// 三角形の内部にある場合, NとN1, N2, N3の向きが同じ
  * ／			     ＼		/// 外部にある場合は、どれかがＮとは逆向き
  *B￣￣￣￣￣￣￣￣￣￣ C	/// 向きが同じかどうかを内積で調べる
  */
bool isPointInsideTriangle(const Vector& p, const Vector& a, const Vector& b, const Vector& c)
{

    if (innerProduct(outerProduct(b - a, c - a), outerProduct(a - p, b - p)) < 0) return false;
    if (innerProduct(outerProduct(b - a, c - a), outerProduct(b - p, c - p)) < 0) return false;
    if (innerProduct(outerProduct(b - a, c - a), outerProduct(c - p, a - p)) < 0) return false;

    return true;
}

/**
 *		ある内部点から三角形の各辺までの距離で最も最小の値を求める
 *			返り値
 *				点が内部なら: 距離の値(double)
 *				点が外部なら: 距離の負の値を返す
 *			引数
 *				Vector a, b, c: 三角形の各頂点の位置ベクトル
 *				Vector p: 判定すべき点
 */
double minDistanceToTriangleEdge(const Vector& p, const Vector& a, const Vector& b, const Vector& c)
{
    /// それぞれの辺までの距離
    double dist[3];

    dist[0] = distancePointAndSegment(p, a, b);		/// 辺abまでの距離
    dist[1] = distancePointAndSegment(p, b, c);		/// 辺bcまでの距離
    dist[2] = distancePointAndSegment(p, c, a);		/// 辺caまでの距離

    int i;	/// カウンタ
    double min;	/// 距離の最小値
    double distance;	/// 返り値

    min = dist[0];	/// 最初はdist[0]を最小値に仮定

    /// 最小値を選出
    for (i = 0; i < 3; i++)
    {
        if (min > dist[i])
            min = dist[i];
    }

    // 三角形abcの内部にあるかどうか
    if (!isPointInsideTriangle(p, a, b, c))
        distance = -min;	/// 内部にないなら得られた最小値を負にして返す
    else if (isPointInsideTriangle(p, a, b, c))
        distance = min;	/// 内部にあるなら最小値をそのまま返す

    return distance;
}

/**
 *		isPointInsideTriangle
 *			凸四角形の中に点があるかどうか
 *				内側にある場合: true
 *				外側にある場合: false
 *			引数
 *				Vector a, b, c, d: 四角形の各頂点の位置ベクトル
 *				Vector p: 判定すべき点
 */
 /// 四角形を二つの三角形に分け, IsPointInsideTriangleを2回使う
bool isPointInsideQuadrangle(const Vector& p, const Vector& a, const Vector& b, const Vector& c, const Vector& d)
{
    if (!isPointInsideTriangle(p, a, b, c))
    {
        if (!isPointInsideTriangle(p, a, c, d)) return false;
    }

    return true;
}

/**
 *		ある内部点から四角形の各辺までの距離で最も最小の値を求める
 *			返り値
 *				点が内部なら: 距離の値(double)
 *				点が外部なら: 距離の負の値を返す
 *			引数
 *				Vector a, b, c, d: 三角形の各頂点の位置ベクトル
 *				Vector p: 判定すべき点
 */
double minDistanceToQuadrangleEdge(const Vector& p, const Vector& a, const Vector& b, const Vector& c, const Vector& d)
{
    double dist[4];		/// それぞれの辺までの距離
    double min;			/// 距離の最小値
    int i;					/// カウンタ
    double distance;	/// 返り値

    dist[0] = distancePointAndSegment(p, a, b);		/// 辺abまでの距離
    dist[1] = distancePointAndSegment(p, b, c);		/// 辺bcまでの距離
    dist[2] = distancePointAndSegment(p, c, d);		/// 辺cdまでの距離
    dist[3] = distancePointAndSegment(p, d, a);		/// 辺daまでの距離

    /// 最初はdist[0]を最小値に仮定
    min = dist[0];

    /// 最小値を選出
    for (i = 0; i < 4; i++)
    {
        if (min > dist[i])
            min = dist[i];
    }

    //// 四角形abcdの内部にあるかどうか
    if (!isPointInsideQuadrangle(p, a, b, c, d))
        distance = -min;	/// 内部にないなら得られた最小値を負にして返す
    else
        distance = min;	/// 内部にあるなら最小値をそのまま返す

    return distance;
}

}	/// end of namespace Math