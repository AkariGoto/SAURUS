
#ifndef DESIGNLABROBOTGUI_MATH_VECTOR_CLASS_H_
#define DESIGNLABROBOTGUI_MATH_VECTOR_CLASS_H_

#include <array>
#include <cmath>
#include <type_traits>


namespace designlab_robot_gui::math
{

//! @class Vector
//! @brief D 次元のベクトルを表すクラス．
//! @tparam D ベクトルの次元数．
//! @tparam T ベクトルの要素の型．浮動小数点数型を指定する．
template <int D, class T = double, class = std::enable_if_t<std::is_floating_point_v<T>>>
class Vector final
{
public:
    static_assert(0 < D, "次元は正の整数である必要があります．");

    //! @brief 要素を全て 0 で初期化する．
    constexpr Vector() :
        data_(InitByZero<D, T>())
    {
    }

    //! @brief 受け取った配列の値をコピーして初期化する．
    //! @param data 初期化に使用する配列．
    constexpr explicit Vector(const std::array<T, D>& data) :
        data_(data)
    {
    }

    constexpr Vector(const Vector& other) = default;  //!< コピーコンストラクタは default(なにもしない)．
    constexpr Vector(Vector&& other) noexcept = default;  //!< ムーブコンストラクタ default(なにもしない)．
    constexpr Vector& operator=(const Vector& other) = default;  //!< コピー代入演算子 default(なにもしない)．

    ~Vector() = default;  //!< デストラクタは default(なにもしない)．


    constexpr Vector operator+() const { return *this; }  //!< 単項+演算子．なにもしない．
    constexpr Vector operator-() const { return (*this) * -1; }  //!< 単項-演算子．各要素を-1倍する．


    constexpr Vector operator+(const Vector& rhs) const
    {
        Vector ret;

        for (int i = 0; i < D; i++)
        {
            ret.data_[i] = data_[i] + rhs.data_[i];
        }

        return ret;
    }

    constexpr Vector operator-(const Vector& rhs) const
    {
        Vector ret;

        for (int i = 0; i < D; i++)
        {
            ret.data_[i] = data_[i] - rhs.data_[i];
        }

        return ret;
    }

    constexpr Vector operator*(T rhs) const
    {
        Vector ret;

        for (int i = 0; i < D; i++)
        {
            ret.data_[i] = data_[i] * rhs;
        }

        return ret;
    }

    constexpr Vector operator/(T rhs) const
    {
        Vector ret;

        for (int i = 0; i < D; i++)
        {
            ret.data_[i] = data_[i] / rhs;
        }

        return ret;
    }

    constexpr Vector& operator+=(const Vector& rhs)
    {
        for (int i = 0; i < D; i++)
        {
            data_[i] += rhs.data_[i];
        }

        return *this;
    }

    constexpr Vector& operator-=(const Vector& rhs)
    {
        for (int i = 0; i < D; i++)
        {
            data_[i] -= rhs.data_[i];
        }

        return *this;
    }

    constexpr Vector& operator*=(T rhs)
    {
        for (int i = 0; i < D; i++)
        {
            data_[i] *= rhs;
        }

        return *this;
    }

    constexpr Vector& operator/=(T rhs)
    {
        for (int i = 0; i < D; i++)
        {
            data_[i] /= rhs;
        }

        return *this;
    }


    //! @brief ベクトルの次元数を返す．
    [[nodiscard]]
    constexpr int GetDimension() const
    {
        return D;
    }

    //! @brief ベクトルの長さの2乗を返す．
    //! @return ベクトルの長さの2乗．
    [[nodiscard]]
    constexpr T GetLengthSquared() const
    {
        T ret = 0;

        for (int i = 0; i < D; i++)
        {
            ret += data_[i] * data_[i];
        }

        return ret;
    }

    //! @brief ベクトルの長さを返す．
    //! @return ベクトルの長さ．
    [[nodiscard]]
    inline T GetLength() const
    {
        return std::sqrt(GetLengthSquared());
    }

    //! @brief このベクトルを正規化したベクトルを返す．
    //! @return 正規化したベクトル．
    [[nodiscard]]
    inline Vector GetNormalized() const
    {
        return (*this) / GetLength();
    }

    //! @brief ベクトルの最初の値( x )を返す．
    //! @return ベクトルの最初の値．x の値．
    [[nodiscard]]
    constexpr T x_value() const
    {
        return data_[0];
    }

    //! @brief ベクトルの2番目の値( y )を返す．
    //! ベクトル2次元以上である必要がある．そうでない場合はコンパイルエラーとなる．
    //! @return ベクトルの2番目の値．y の値．
    template <int D_ = D, class = std::enable_if_t<(2 <= D_)>>
    [[nodiscard]]
    constexpr T y_value() const
    {
        return data_[1];
    }

    //! @brief ベクトルの3番目の値( z )を返す．
    //! ベクトル3次元以上である必要がある．そうでない場合はコンパイルエラーとなる．
    //! @return ベクトルの3番目の値．z の値．
    template <int D_ = D, class = std::enable_if_t<(3 <= D_)>>
    [[nodiscard]]
    constexpr T z_value() const
    {
        return data_[2];
    }


    //! @brief このベクトルを正規化する．
    inline void NormalizeSelf()
    {
        (*this) /= GetLength();
    }

private:
    //! D 次元のベクトルを 0 で初期化する．
    template <int D_, class T_>
    constexpr std::array<T_, D_> InitByZero() const
    {
        std::array<T_, D_> ret;

        for (int i = 0; i < D_; i++)
        {
            ret[i] = 0;
        }

        return ret;
    }

    //! 型名が T で，D 次元のベクトル．
    //! T は デフォルトで double 型となる．
    std::array<T, D> data_;
};


// 以下，よく使用するベクトルの型の別名を定義する．

using Vector2 = Vector<2>;
using Vector3 = Vector<3>;
using Vector4 = Vector<4>;
using Vector5 = Vector<5>;


}  // namespace designlab_robot_gui::math


#endif  // DESIGNLABROBOTGUI_MATH_VECTOR_CLASS_H_
