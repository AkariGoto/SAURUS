
#ifndef DESIGNLABROBOTGUI_MATH_MATH_CONSTANT_H_
#define DESIGNLABROBOTGUI_MATH_MATH_CONSTANT_H_


namespace designlab_robot_gui::math
{

//! √2．
constexpr double ROOT_2 = 1.4142135623730950488;

//! √3．
constexpr double ROOT_3 = 1.7320508075688772935;

//! √10．
constexpr double ROOT_10 = 3.1622776601683793320;

//! log2．10を底とする対数．
constexpr double LOG_2 = 0.3010299956639811952;

//! log3．10を底とする対数．
constexpr double LOG_3 = 0.4771212547196624373;

//! log e．10を底とする対数．
constexpr double LOG_E = 0.4342944819032518277;

//! ln10．eを底とする対数．
constexpr double LN_10 = 2.3025850929940456840;

//! π
constexpr double PI = 3.1415926535897932385;

//! e．自然対数の底．
constexpr double E = 2.7182818284590452354;

//! 1 / π．円周率の逆数．
constexpr double ONE_OVER_PI = 0.3183098861837906715;

//! 1 / e．自然対数の底の逆数．
constexpr double ONE_OVER_E = 0.3678794411714423216;

//! √π．円周率の平方根．
constexpr double ROOT_PI = 1.7724538509055160273;

//! euler数．
constexpr double EULER = 0.5772156649015328606;


// 変換

//! 度 -> ラジアン．PI / 180.0
constexpr double DEG2RAD = 0.01745329251994329576;

//! ラジアン -> 度．180.0 / PI
constexpr double RAD2DEG = 57.2957795130823208767;


// Computer science

constexpr bool ON = true;
constexpr bool OFF = false;
constexpr double EPS = 1.0e-13;


// ロボット工学

//! D-Hの行列要素数．
constexpr int DH_DIMENSION = 4;
//! 3次元数．
constexpr int THREE_DIMENSION = 3;

}  // namespace designlab_robot_gui::math


#endif  // DESIGNLABROBOTGUI_MATH_MATH_CONSTANT_H_
