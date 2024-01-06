
/// @file      matrix_class.h
/// @author    Hasegawa
/// @copyright © 埼玉大学 設計工学研究室 2024. All right reserved.

#ifndef DESIGNLABROBOTGUI_MATH_MATRIX_CLASS_H_
#define DESIGNLABROBOTGUI_MATH_MATRIX_CLASS_H_

#include <array>
#include <cmath>
#include <type_traits>

#include "Math/vector_class.h"


namespace designlab_robot_gui::math
{

//! @brief 行列クラス．
//! @tparam ROW 行列の行数．
//! @tparam COL 行列の列数．
//! @tparam T 行列の要素の型．デフォルトは double．
template <int ROW, int COL, typename T = double,
    typename = typename std::enable_if<std::is_arithmetic<T>::value>::type>
class Matrix final
{
    static_assert(ROW > 0, "行数は 0 以上である必要があります．");
    static_assert(COL > 0, "列数は 0 以上である必要があります．");

public:
    //! @brief コンストラクタ．全ての値を 0 で初期化する．
    Matrix() : data_{} {}

    Matrix(const Matrix& other) = default;  //!< コピーコンストラクタ．
    Matrix& operator=(const Matrix& other) = default;  //!< コピー代入演算子．

    ~Matrix() = default;  //!< デストラクタ．


    //! @brief 行列の行数(Row)を取得する．
    //! @return 行列の行数．
    [[nodiscard]]
    constexpr int GetRow() const { return ROW; }

    //! @brief 行列の列数(Col)を取得する．
    //! @return 行列の列数．
    [[nodiscard]]
    constexpr int GetCol() const { return COL; }

    //! @brief 正方行列かどうかを判定する．
    //! @retval true 正方行列である．
    //! @retval false 正方行列でない．
    [[nodiscard]]
    constexpr bool IsSquare() const { return ROW == COL; }

    //! @brief 行列の列が1かどうかを判定する．
    //! @retval true 行列の列が1である．
    //! @retval false 行列の列が1でない．
    [[nodiscard]]
    constexpr bool IsOneColumn() const { return COL == 1; }

    //! @brief 行列の行が1かどうかを判定する．
    //! @retval true 行列の行が1である．
    //! @retval false 行列の行が1でない．
    [[nodiscard]]
    constexpr bool IsOneRow() const { return ROW == 1; }

    [[nodiscard]] constexpr bool IsZero() const
    {
        for (const auto& row : data_)
        {
            for (const auto& col : row)
            {
                if (col != 0)
                {
                    return false;
                }
            }
        }

        return true;
    }

    template <int R = ROW, int C = COL, class = std::enable_if_t<(R == C)>>
    [[nodiscard]]
    Matrix GetInverse() const
    {
        return Matrix();
    }

    //! @brief 転置行列を取得する．
    //! @return 転置行列．
    template <int R = ROW, int C = COL, class = std::enable_if_t<(R == C)>>
    [[nodiscard]]
    Matrix GetTranspose() const
    {
        Matrix result{};

        for (int row = 0; row < ROW; ++row)
        {
            for (int col = 0; col < COL; ++col)
            {
                result.SetValue(col, row, GetValue(row, col));
            }
        }

        return result;
    }


    //! @brief 行列の要素を取得する．
    //! @param[in] row 行番号．0 から始まる．
    //! @param[in] col 列番号．0 から始まる．
    //! @return 指定した要素の値．
    [[nodiscard]]
    constexpr T GetValue(const int row, const int col) const
    {
        assert(row >= 0 && row < ROW);
        assert(col >= 0 && col < COL);

        return data_[row][col];
    }

    //! @brief 行列の要素を設定する．
    //! @param[in] row 行番号．0 から始まる．
    //! @param[in] col 列番号．0 から始まる．
    //! @param[in] value 設定する値．
    constexpr void SetValue(const int row, const int col, const T value)
    {
        assert(row >= 0 && row < ROW);
        assert(col >= 0 && col < COL);

        data_[row][col] = value;
    }


    //! @brief 行列の1行1列目の要素を取得する．
    //! @return 行列の1行1列目の要素の値．
    [[nodiscard]]
    constexpr T value_1_1() const
    {
        return data_[0][0];
    }

    //! @brief 行列の1行2列目の要素を取得する．
    //! 列数が 2 未満の場合はコンパイルエラーとなる．
    //! @return 行列の1行2列目の要素の値．
    template <int C = COL, class = std::enable_if_t<(C >= 2)>>
    [[nodiscard]]
    constexpr T value_1_2() const
    {
        return data_[0][1];
    }

    //! @brief 行列の1行3列目の要素を取得する．
    //! 列数が 3 未満の場合はコンパイルエラーとなる．
    //! @return 行列の1行3列目の要素の値．
    template <int C = COL, class = std::enable_if_t<(C >= 3)>>
    [[nodiscard]]
    constexpr T value_1_3() const
    {
        return data_[0][2];
    }

    //! @brief 行列の1行4列目の要素を取得する．
    //! 列数が 4 未満の場合はコンパイルエラーとなる．
    //! @return 行列の1行4列目の要素の値．
    template <int C = COL, class = std::enable_if_t<(C >= 4)>>
    [[nodiscard]]
    constexpr T value_1_4() const
    {
        return data_[0][3];
    }

    //! @brief 行列の2行1列目の要素を取得する．
    //! 行数が 2 未満の場合はコンパイルエラーとなる．
    //! @return 行列の2行1列目の要素の値．
    template <int R = ROW, class = std::enable_if_t<(R >= 2)>>
    [[nodiscard]]
    constexpr T value_2_1() const
    {
        return data_[1][0];
    }

    //! @brief 行列の2行2列目の要素を取得する．
    //! 行数が 2 未満，または列数が 2 未満の場合はコンパイルエラーとなる．
    //! @return 行列の2行2列目の要素の値．
    template <int R = ROW, class = std::enable_if_t<(R >= 2)>, int C = COL, class = std::enable_if_t<(C >= 2)>>
    [[nodiscard]]
    constexpr T value_2_2() const
    {
        return data_[1][1];
    }

    //! @brief 行列の2行3列目の要素を取得する．
    //! 行数が 2 未満，または列数が 3 未満の場合はコンパイルエラーとなる．
    //! @return 行列の2行3列目の要素の値．
    template <int R = ROW, class = std::enable_if_t<(R >= 2)>, int C = COL, class = std::enable_if_t<(C >= 3)>>
    [[nodiscard]]
    constexpr T value_2_3() const
    {
        return data_[1][2];
    }

    //! @brief 行列の2行4列目の要素を取得する．
    //! 行数が 2 未満，または列数が 4 未満の場合はコンパイルエラーとなる．
    //! @return 行列の2行4列目の要素の値．
    template <int R = ROW, class = std::enable_if_t<(R >= 2)>, int C = COL, class = std::enable_if_t<(C >= 4)>>
    [[nodiscard]]
    constexpr T value_2_4() const
    {
        return data_[1][3];
    }

    //! @brief 行列の3行1列目の要素を取得する．
    //! 行数が 3 未満の場合はコンパイルエラーとなる．
    //! @return 行列の3行1列目の要素の値．
    template <int R = ROW, class = std::enable_if_t<(R >= 3)>>
    [[nodiscard]]
    constexpr T value_3_1() const
    {
        return data_[2][0];
    }

    //! @brief 行列の3行2列目の要素を取得する．
    //! 行数が 3 未満，または列数が 2 未満の場合はコンパイルエラーとなる．
    //! @return 行列の3行2列目の要素の値．
    template <int R = ROW, class = std::enable_if_t<(R >= 3)>, int C = COL, class = std::enable_if_t<(C >= 2)>>
    [[nodiscard]]
    constexpr T value_3_2() const
    {
        return data_[2][1];
    }

    //! @brief 行列の3行3列目の要素を取得する．
    //! 行数が 3 未満，または列数が 3 未満の場合はコンパイルエラーとなる．
    //! @return 行列の3行3列目の要素の値．
    template <int R = ROW, class = std::enable_if_t<(R >= 3)>, int C = COL, class = std::enable_if_t<(C >= 3)>>
    [[nodiscard]]
    constexpr T value_3_3() const
    {
        return data_[2][2];
    }

    //! @brief 行列の3行4列目の要素を取得する．
    //! 行数が 3 未満，または列数が 4 未満の場合はコンパイルエラーとなる．
    //! @return 行列の3行4列目の要素の値．
    template <int R = ROW, class = std::enable_if_t<(R >= 3)>, int C = COL, class = std::enable_if_t<(C >= 4)>>
    [[nodiscard]]
    constexpr T value_3_4() const
    {
        return data_[2][3];
    }

    template <int R = ROW, class = std::enable_if_t<(4 <= R)>>
    [[nodiscard]]
    constexpr T value_4_1() const
    {
        return data_[3][0];
    }

    template <int R = ROW, class = std::enable_if_t<(R >= 4)>, int C = COL, class = std::enable_if_t<(C >= 2)>>
    [[nodiscard]]
    constexpr T value_4_2() const
    {
        return data_[3][1];
    }

    template <int R = ROW, class = std::enable_if_t<(R >= 4)>, int C = COL, class = std::enable_if_t<(C >= 3)>>
    [[nodiscard]]
    constexpr T value_4_3() const
    {
        return data_[3][2];
    }

    template <int R = ROW, class = std::enable_if_t<(R >= 4)>, int C = COL, class = std::enable_if_t<(C >= 4)>>
    [[nodiscard]]
    constexpr T value_4_4() const
    {
        return data_[3][3];
    }


private:
    //! ROW 行 COL 列の行列．
    std::array<std::array<T, COL>, ROW> data_;
};


//! @brief 単位行列を生成する．
//! この行列が正方行列であるときのみ使用可能．
//! @tparam ROW 行列の行数．
//! @tparam COL 行列の列数．
//! @tparam T 行列の要素の型．デフォルトは double．
template <int ROW, int COL, typename T = double,
    typename = typename std::enable_if<std::is_arithmetic<T>::value>::type,
    class = std::enable_if_t<(ROW == COL) >
>
[[nodiscard]] Matrix<ROW, COL, T> GetIdentity()
{
    Matrix<ROW, COL, T> result{};

    for (int i = 0; i < ROW; ++i)
    {
        result.SetValue(i, i, 1);
    }

    return result;
}


// 以下，よく使用するベクトルの型の別名を定義する．

using Matrix2x2 = Matrix<2, 2>;
using Matrix3x3 = Matrix<3, 3>;
using Matrix4x4 = Matrix<4, 4>;
using Matrix5x5 = Matrix<5, 5>;

}  // namespace designlab_robot_gui::math


#endif  // DESIGNLABROBOTGUI_MATH_MATRIX_CLASS_H_
