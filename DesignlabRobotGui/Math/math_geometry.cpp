
#include "Math/math_geometry.h"

#include "Math/math_constant.h"


namespace designlab_robot_gui::math
{

double distancePointToPoint(const Vector& point1, const Vector& point2)
{
    using designlab_robot_gui::math::THREE_DIMENSION;

    assert(point1.getSize() == THREE_DIMENSION ||
           point2.getSize() == THREE_DIMENSION);

    Vector temp;
    temp = point2 - point1;

    return temp.norm();

}

Vector normalVectorToLine(const Vector& point, const Vector& linePoint, const Vector& lineDirectionVector)
{
    using designlab_robot_gui::math::THREE_DIMENSION;

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
    using designlab_robot_gui::math::THREE_DIMENSION;

    assert(point.getSize() == THREE_DIMENSION || start.getSize() == THREE_DIMENSION || end.getSize() == THREE_DIMENSION);

    /// 法線ベクトル
    Vector normal(THREE_DIMENSION);

    /// 線分の方程式を用いて法線ベクトルを求める
    normal = normalVectorToLine(point, start, (end - start));

    return normal;
}

double distancePointAndLine(const Vector& point, const Vector& linePoint, const Vector& lineDirectionVector)
{
    Vector normal(designlab_robot_gui::math::THREE_DIMENSION); /// 法線ベクトル

    /// 垂線ベクトルの大きさが求める距離
    normal = normalVectorToLine(point, linePoint, lineDirectionVector);

    return normal.norm();
}

double distancePointAndSegment(const Vector& point, const Vector& start, const Vector& end)
{
    Vector normal(designlab_robot_gui::math::THREE_DIMENSION); /// 法線ベクトル

    // 垂線ベクトルの大きさが求める距離
    normal = normalVectorToSegment(point, start, end);

    return normal.norm();
}

Vector normalVectorOfPlane(const Vector& p1, const Vector& p2, const Vector& p3)
{
    using designlab_robot_gui::math::THREE_DIMENSION;

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

Vector intersectPointLineAndPlane(const Vector& linePoint, const Vector& lineDirectionVector,
                        const Vector& planePoint, const Vector& planeNormalVector)
{
    using designlab_robot_gui::math::THREE_DIMENSION;

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

bool isPointInsideTriangle(const Vector& p, const Vector& a, const Vector& b, const Vector& c)
{

    if (innerProduct(outerProduct(b - a, c - a), outerProduct(a - p, b - p)) < 0) return false;
    if (innerProduct(outerProduct(b - a, c - a), outerProduct(b - p, c - p)) < 0) return false;
    if (innerProduct(outerProduct(b - a, c - a), outerProduct(c - p, a - p)) < 0) return false;

    return true;
}

double minDistanceToTriangleEdge(const Vector& p, const Vector& a, const Vector& b, const Vector& c)
{
    /// それぞれの辺までの距離
    double dist[3];

    dist[0] = distancePointAndSegment(p, a, b);		/// 辺abまでの距離
    dist[1] = distancePointAndSegment(p, b, c);		/// 辺bcまでの距離
    dist[2] = distancePointAndSegment(p, c, a);		/// 辺caまでの距離

    int i;	/// カウンタ
    double min;	/// 距離の最小値
    double distance{ 0.0 };	/// 返り値

    min = dist[0];	/// 最初は dist[0]を最小値に仮定

    /// 最小値を選出
    for (i = 0; i < 3; i++)
    {
        if (min > dist[i])
            min = dist[i];
    }

    // 三角形 abc の内部にあるかどうか
    if (!isPointInsideTriangle(p, a, b, c))
        distance = -min;	/// 内部にないなら得られた最小値を負にして返す
    else if (isPointInsideTriangle(p, a, b, c))
        distance = min;	/// 内部にあるなら最小値をそのまま返す

    return distance;
}

bool isPointInsideQuadrangle(const Vector& p, const Vector& a, const Vector& b, const Vector& c, const Vector& d)
{
    if (!isPointInsideTriangle(p, a, b, c))
    {
        if (!isPointInsideTriangle(p, a, c, d)) return false;
    }

    return true;
}

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

    /// 最初は dist [0]を最小値に仮定
    min = dist[0];

    /// 最小値を選出
    for (i = 0; i < 4; i++)
    {
        if (min > dist[i])
            min = dist[i];
    }

    //// 四角形 abcd の内部にあるかどうか
    if (!isPointInsideQuadrangle(p, a, b, c, d))
        distance = -min;	/// 内部にないなら得られた最小値を負にして返す
    else
        distance = min;	/// 内部にあるなら最小値をそのまま返す

    return distance;
}

}  // namespace designlab_robot_gui::math
