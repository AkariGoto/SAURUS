
#include "Math/Matrix/matrix_library.h"

#include "Math/math_constant.h"


namespace designlab_robot_gui::math
{

Matrix rotationMatrix(const Vector& axis, double angle)
{
    using designlab_robot_gui::math::THREE_DIMENSION;

    /// ローカル変数
    /// 回転行列
    Matrix rotation(THREE_DIMENSION, THREE_DIMENSION);
    /// 単位方向ベクトル
    Vector unit(THREE_DIMENSION);

    /// 方向ベクトルを代入
    unit = axis;
    /// 正規化
    unit.normalize();

    rotation(1, 1) = cos(angle) + unit(1) * unit(1) * (1 - cos(angle));
    rotation(1, 2) = unit(1) * unit(2) * (1 - cos(angle)) - unit(3) * sin(angle);
    rotation(1, 3) = unit(1) * unit(3) * (1 - cos(angle)) + unit(2) * sin(angle);
    rotation(2, 1) = unit(1) * unit(2) * (1 - cos(angle)) + unit(3) * sin(angle);
    rotation(2, 2) = cos(angle) + unit(2) * unit(2) * (1 - cos(angle));
    rotation(2, 3) = unit(2) * unit(3) * (1 - cos(angle)) - unit(1) * sin(angle);
    rotation(3, 1) = unit(1) * unit(3) * (1 - cos(angle)) - unit(2) * sin(angle);
    rotation(3, 2) = unit(2) * unit(3) * (1 - cos(angle)) + unit(1) * sin(angle);
    rotation(3, 3) = cos(angle) + unit(3) * unit(3) * (1 - cos(angle));

    return (rotation);
}

Vector zeroVector(int dimension)
{
    Vector result(dimension);

    return result;
}

double innerProduct(const Vector& x, const Vector& y)
{
    /// 引数チェック
    if (x.getSize() != y.getSize())
    {
        std::cerr << "Error: [innerProduct] Vector type is unmatched\n" << std::endl;
        abort();
    }

    /// 返り値
    double ip = 0;

    int i;
    for (i = 1; i <= x.getSize(); i++)
        ip += x(i) * y(i);

    return ip;
}

Vector outerProduct(const Vector& x, const Vector& y)
{
    using designlab_robot_gui::math::THREE_DIMENSION;

    if (x.getSize() != THREE_DIMENSION || y.getSize() != THREE_DIMENSION)
    {
        std::cerr << "Error: [outerProduct] Vector is not 3 dimension\n" << std::endl;

        return zeroVector(THREE_DIMENSION);
    }

    Vector op(3);

    op(1) = x(2) * y(3) - x(3) * y(2);
    op(2) = x(3) * y(1) - x(1) * y(3);
    op(3) = x(1) * y(2) - x(2) * y(1);

    return op;
}

void outerProduct(double x1, double y1, double z1,
            double x2, double y2, double z2,
            double* x, double* y, double* z)
{
    *x = (y1 * z2 - z1 * y2);
    *y = (z1 * x2 - x1 * z2);
    *z = (x1 * y2 - y1 * x2);

    return;
}

}  // namespace designlab_robot_gui::math
