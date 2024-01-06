
#ifndef DESIGNLABROBOTGUI_MATH_MATRIX_MATRIX_H_
#define DESIGNLABROBOTGUI_MATH_MATRIX_MATRIX_H_

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <cassert>


/// ゼロとみなす数
const double NEAR_ZERO = 1.0E-16;
/// ゼロとの相対誤差
const double ZERO_ERROR_MARGIN = 1.0E-8;
/// マシン誤差
const double MATRIX_TINY = 1.0E-20;


namespace designlab_robot_gui::math
{

class Matrix;
class Vector;

const Vector operator+(const Vector& left, const Vector& right);
const Vector operator-(const Vector& left, const Vector& right);
const Vector operator*(double schalor, const Vector& right);
const Vector operator*(const Vector& left, double schalor);
const Vector operator/(const Vector& left, double schalor);

const Matrix operator+(const Matrix& left, const Matrix& right);
const Matrix operator-(const Matrix& left, const Matrix& right);


class Matrix
{
private:
    int rows;
    int columns;

    Vector* elements;

    friend std::ostream& operator<<(std::ostream& output, const Matrix& matrix);
    friend std::istream& operator>>(std::istream& input, Matrix& matrix);

    friend bool operator==(const Matrix& left, const Matrix& right);
    friend bool operator!=(const Matrix& left, const Matrix& right);

    friend const Matrix operator*(const Matrix& left, const Matrix& right);
    friend const Vector operator*(const Matrix& matrix, const Vector& vector);
    friend const Vector operator*(const Vector& vector, const Matrix& matrix);


public:
    explicit Matrix(int row = 0, int col = 0);

    /// コピーコンストラクタ
    Matrix(const Matrix& matrix);

    /// デストラクタ
    virtual ~Matrix();


    void setSize(int row, int col);

    int getRow() const { return rows; }
    int getRow() { return rows; }
    int getColumn() const { return columns; }
    int getColumn() { return columns; }

    int isSquare() const { return (rows == columns); }
    int isOneColumn() const { return (columns == 1); }

    void cleanUp();

    /**
     *	----------------------------------------
     *	行列演算のための演算子
     *	----------------------------------------
     */
     /// 代入演算子
    virtual Matrix& operator=(const Matrix& right);

    /**
     *	説明
     *		添え字演算子
     *	引数
     *		row: 行列の行番号
     *		col:  行列の列番号
     */
     /// ユーザ用（引数チェック有）
    double& operator()(int row = 1, int col = 1);
    /// 計算用[行ベクトルを取り出す]（引数チェック無，高速計算用）
    Vector& operator[] (int index);

    /**
     *	説明
     *		constオブジェクトに対する添え字演算子
     *	引数
     *		row: 行列の行番号
     *		col:  行列の列番号
     */
     /// ユーザ用（引数チェック有）
    const double& operator()(int row = 1, int col = 1) const;
    /// 計算用[行ベクトルを取り出す]（引数チェック無，高速計算用）
    const Vector& operator[] (int index) const;


    Matrix& operator+=(const Matrix& right);
    Matrix& operator-=(const Matrix& right);
    Matrix& operator*=(const Matrix& right);


    /**
     *	----------------------------------------
     *	行列の部分コピー
     *	----------------------------------------
     */
     /**
      *	説明
      *		読み込んだ行列をコピー先の開始点，コピー元の始点と終点を指定してコピーする
      *	引数
      *		startRow, startCol: コピー先の行列のコピー開始点
      *		source: コピー元の行列
      *		firstRow, firstCol: コピー元の行列の始点
      *		lastRow, lastCol: コピー元の行列の終点
      */
    virtual Matrix& loadMatrix(int startRow, int startCol, const Matrix& source, int firstRow, int firstCol, int lastRow, int lastCol);
    /**
     *	説明
     *		読み込んだ行列をコピー元の始点と終点を指定してコピーする
     *		コピー先の行列に1行1列目からコピーする
     *	引数
     *		source: コピー元の行列
     *		firstRow, firstCol: コピー元の行列の始点
     *		lastRow, lastCol: コピー元の行列の終点
     */
    virtual Matrix& loadMatrix(const Matrix& source, int firstRow, int firstCol, int lastRow, int lastCol);
    /**
     *	説明
     *		読み込んだ行列をコピー先の始点と終点を指定してコピーする
     *		コピー元の行列の1行1列目からコピーする
     *	引数
     *		startRow, startCol: コピー先の行列のコピー開始点
     *		source: コピー元の行列
     */
    virtual Matrix& loadMatrix(int startRow, int startCol, const Matrix& source);
    /**
     *	説明
     *		読み込んだ行列をコピー元の始点を指定してコピーする
     *		コピー先の行列に1行1列目からコピーする
     *	引数
     *		source: コピー元の行列
     *		firstRow, firstCol: コピー元の行列の始点
     */
    virtual Matrix& loadMatrix(const Matrix& source, int firstRow, int firstCol);
    /**
     *	説明
     *		読み込んだ行列をコピーする
     *		コピー先の行列に1行1列目からコピーする
     *		コピー元の行列の1行1列目からコピーする
     *	引数
     *		source: コピー元の行列
     */
    virtual Matrix& loadMatrix(const Matrix& source);


    virtual Matrix& loadZero();
    virtual Matrix& loadIdentity();

    Matrix& transpose();
    Matrix transposition();
    const Matrix transposition() const;



    /**
     *	説明
     *		呼出元の逆行列を返す
     */
    Matrix inversion(void);
    const Matrix inversion(void) const;



    /**
     *	説明
     *		行列の要素をコンソールに書き出す
     */
     /// 全ての要素を書き出す
    virtual void show(void) const;
    /// 全ての要素を指数形式で書き出す
    virtual void showExp(void) const;

    /**
     *	説明
     *		行列の体格成分の和を求める
     */
    double trace(void);
    const double trace(void) const;

    /**
     *	説明
     *		行の交換
     *	引数
     *		i: 交換する行
     *		j: 交換される行
     */
    void swapRows(int i, int j);






private:
    /**
     *	説明
     *		行列オブジェクトのメモリ領域を確保する
     */
    void newMatrix(void);
    /**
     *	説明
     *		行列オブジェクトのメモリ領域を解放する
     */
    void deleteMatrix(void);

    /**
     *	----------------------------------------
     *	連立方程式を求めるための補助関数
     *	----------------------------------------
     */
     /**
      *	説明
      *		LU分解を行う
      *	引数
      *		Matrix  a	: LU分解される行列
      *		Matrix  lu	: LU分解後の行列
      *		Matrix  indx: 部分ピボット選択の際の行交換を記録
      *		double* d	: 行交換の回数が偶数=1，行交換の回数が奇数=-1
      *
      *		Reference :	William H. Press, et al, NUMERICAL RECIPES in C,
      *					Cambridge University Press, 1988
      */
    int Matrix::luDecomposition(const Matrix& a, Matrix& lu, Vector& indx, double* d) const;

    /**
     *	説明
     *		後退代入を用いて連立1次方程式を解く(a*x=b)
     *	引数
     *		Matrix  a	: LU分解で求めたAのLU分解（不変）
     *		Vector  indx: LU分解の際の部分ピボット選択の行交換情報
     *		Vector  b	: 既知の右辺ベクトル（解ベクトルxの値を上書き）
     *
     *		Reference :	William H. Press, et al, NUMERICAL RECIPES in C,
     *					Cambridge University Press, 1988
     */
    void luBackSubstitution(const Matrix& a, const Vector& indx, Vector& b) const;

    /**
     *	----------------------------------------
     *	特異値分解を行うための関数群
     *	----------------------------------------
     */
     /**
      *	説明
      *		行列の特異値分解（Singular Value Decomposition）
      *	引数
      *		Matrix  a	: 特異値分解される行列
      *		Matrix  u	: 直行行列
      *		Matrix  v	: 特異値の対角行列
      *		Matrix  w	: 直行行列
      *		double* d	: 行交換の回数が偶数=1，行交換の回数が奇数=-1
      *
      *		Reference :	William H. Press, et al, NUMERICAL RECIPES in C,
      *					Cambridge University Press, 1988
      */
    void singularValueDecomposition(const Matrix& a, Matrix& u, Matrix& w, Matrix& v) const;

    /**
     *	説明
     *		三平方の定理（アンダー/オーバーフローを防ぐ）
     *	引数
     *		（double）: a, b
     *	戻り値
     *		sqrt(a*a + b*b)
     *　
     *		Reference : William H. Press, et al, NUMERICAL RECIPES in C,
     *					Cambridge University Press, 1988
     */
    double pythagorean(double a, double b) const;

    /**
     *	説明
     *		クイックソート
     */
    void quickSort(Matrix& w, Vector& no, int low, int high) const;
};	/// end of class Matrix

/**
 *	----------------------------------------------------------------------
 *		Vectorクラス
 *	----------------------------------------------------------------------
 */
class Vector
{
    /**
     *	------------------------------------------------------------
     *		メンバ変数
     *	------------------------------------------------------------
     */
private:
    /// ベクトルの先頭要素へのポインタ
    double* elements;
    /// ベクトルの次元
    int			dimensions;

    /**
     *	------------------------------------------------------------
     *		フレンド関数
     *	------------------------------------------------------------
     */
     /// クラスVectorの多重定義されたストリーム抽出演算子
     /// ベクトルの全要素の値を出力する
    friend std::ostream& operator<<(std::ostream& output, const Vector& vector);
    /// ベクトルの全要素の値を入力する
    friend std::istream& operator>>(std::istream& input, Vector& vector);

    /// ベクトルが等しいかどうか
    friend bool operator==(const Vector& left, const Vector& right);

    /// ベクトルが異なっているかどうか
    friend bool operator!=(const Vector& left, const Vector& right);

    /**
     *		一般的な算術演算子
     */
     /// 内積
    friend double operator*(const Vector& left, const Vector& right);

    /// 行列とベクトルの積
    friend const Vector operator*(const Matrix& matrix, const Vector& vector);

    /// ベクトルと行列の積
    friend const Vector operator*(const Vector& vector, const Matrix& matrix);

    /// 行列同士の積
    friend const Matrix operator*(const Matrix& left, const Matrix& right);

    /**
     *		Matrixクラスのメンバ関数
     */
     /// 微小要素の消去
    friend void Matrix::cleanUp(void);
    /// 行の交換
    friend void Matrix::swapRows(int i, int j);
    /// 組み合わせ演算子
    friend Matrix& Matrix::operator*=(const Matrix& right);

    /**
     *	------------------------------------------------------------
     *		メンバ関数
     *	------------------------------------------------------------
     */
public:
    /**
     *	----------------------------------------
     *	コンストラクタとデストラクタ
     *	----------------------------------------
     */
     /// デフォルトコンストラクタ
       /**
      *		dim: 成分数
      */
    explicit Vector(int dim = 0);

    ///コピーコンストラクタ
    Vector(const Vector& vector);

    /// 配列で初期化するコンストラクタ
      /**
     *		ptrArray: 初期化用の配列
     *		dim: 成分数
     */
    explicit Vector(const double* ptrArray, int dim);
    /// デストラクタ
    virtual ~Vector();

    /// ベクトルのサイズを設定する
      /**
     *		dim: 成分数
     */
    void setSize(int dim);
    /// ベクトルのサイズを得る
    int getSize() const { return dimensions; }

    /**
     *	----------------------------------------
     *	演算子の多重定義
     *----------------------------------------
     */

     /// 代入演算子
       /**
      *		dim: 成分数
      */
    Vector& operator= (const Vector& right);

    /// 添え字演算子
      /**
     *		index: 要素数
     */
    double& operator()(int index);		/// ユーザ用
    double& operator[](int index);		/// 計算用
    /// constオブジェクトに対する添え字演算子
      /**
     *		index: 要素数
     */
    const double& operator()(int index) const;		/// ユーザ用（引数チェック有）
    const double& operator[](int index) const;		/// 計算用（引数チェック無，高速計算用）

    /// (-1)ベクトル（ベクトルの向きを変える）
    const Vector operator-() const { return -1.0 * (*this); }

    /**
     *		代表的な組み合わせ演算子
     */
    Vector& operator*=(double scalar);
    Vector& operator/=(double scalar);
    Vector& operator+=(const Vector& vector);
    Vector& operator-=(const Vector& vector);

    /**
     *	----------------------------------------
     *	代表的な関数
     *	----------------------------------------
     */
     /// ノルムを求める
    double norm(void) const;
    /// ベクトルの規格化（単位ベクトルにする）
    const Vector& normalize(void);
    Vector GetNormalize(void);
    const Vector GetNormalize(void) const;

    /**
     *	微小要素の消去
     */
    void cleanUp(void);

private:
    /// オブジェクトのメモリ領域を確保する
    void newVector(void);
    /// オブジェクトのメモリ領域を解放する
    void deleteVector(void);

};	/// end of class Vector

/**
 *	------------------------------------------------------------
 *		inline関数
 *	------------------------------------------------------------
 */
 /**
  *		多重定義された＋演算子
  *		２つのベクトルの和を求め値渡しで返す
  */
inline const Vector operator+(const Vector& left, const Vector& right)
{
    /// 演算子 += を使う
    return Vector(left) += right;
}

/**
 *		多重定義された－演算子
 *		２つのベクトルの差を求め値渡しで返す
 */
inline const Vector operator-(const Vector& left, const Vector& right)
{
    /// 演算子 -= を使う
    return Vector(left) -= right;
}

/**
 *		多重定義された＊演算子
 *		ベクトルのスカラー倍 （実数×ベクトル）
 */
inline const Vector operator*(double schalor, const Vector& right)
{
    /// 演算子 *= を使う
    return Vector(right) *= schalor;
}

/**
 *		多重定義された＊演算子
 *		ベクトルのスカラー倍 （ベクトル×実数）
 *		実数×ベクトル用の関数を使う
 */
inline const Vector operator*(const Vector& left, double schalor)
{
    /// 演算子 *= を使う
    return Vector(left) *= schalor;
}

/**
 *		多重定義された/演算子
 *		ベクトルを実数で割り算する
 */
inline const Vector operator/(const Vector& left, double schalor)
{
    /// 演算子 /= を使う
    return Vector(left) /= schalor;
}

/**
 *		多重定義された＋演算子
 *		２つの行列の和を求め，値渡しで返す
 */
inline const Matrix operator+(const Matrix& left, const Matrix& right)
{
    /// 演算子 += を使う
    return Matrix(left) += right;
}

/**
 *		多重定義された－演算子
 *		２つの行列の差を求め，値渡しで返す
 */
inline const Matrix operator-(const Matrix& left, const Matrix& right)
{
    /// 演算子 -= を使う
    return Matrix(left) -= right;
}

/**
 *	------------------------------------------------------------
 *		Matrixメンバのinline関数
 *	------------------------------------------------------------
 */
 /// 非const Matrixオブジェクトの添え字演算子
inline Vector& Matrix::operator[](int index)
{
    return this->elements[index];
}
/// const Matrixオブジェクトの添え字演算子
inline const Vector& Matrix::operator[](int index) const
{
    return this->elements[index];
}

/**
 *		添え字演算子
 *		row: 行列の行番号
 *		col:  行列の列番号
 */
inline double& Matrix::operator()(int row, int col)
{
    /// 添え字のチェック
    assert((row >= 1 && row <= this->rows) && (col >= 1 && col <= this->columns));

    return this->elements[row - 1][col - 1];
}

/**
 *		添え字演算子
 *		row: 行列の行番号
 *		col:  行列の列番号
 */
inline const double& Matrix::operator()(int row, int col) const
{
    /// 添え字のチェック
    assert((row >= 1 && row <= this->rows) && (col >= 1 && col <= this->columns));

    return this->elements[row - 1][col - 1];
}

/**
 *	------------------------------------------------------------
 *		Vectorメンバのinline関数
 *	------------------------------------------------------------
 */
 /// 非const Vectorオブジェクトの添え字演算子
inline double& Vector::operator[](int index)
{
    return this->elements[index];
}

/// const Vectorオブジェクトの添え字演算子
inline const double& Vector::operator[](int index) const
{
    return this->elements[index];
}

/// 添え字演算子
/**
 *		index: 要素数
 */
inline double& Vector::operator()(int index)
{
    /// 引数のチェック
    assert(index >= 1 && index <= dimensions);

    return this->elements[index - 1];
}

/// constオブジェクトに対する添え字演算子
/**
 *		index: 要素数
 */
inline const double& Vector::operator()(int index) const
{
    /// 引数のチェック
    assert(index >= 1 && index <= dimensions);

    return this->elements[index - 1];
}

}  // namespace designlab_robot_gui::math

#endif  // DESIGNLABROBOTGUI_MATH_MATRIX_MATRIX_H_
