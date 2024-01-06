/**
 *  ファイル名
 *		Matrix.cpp
 *  説明
 *		演算子やメソッドなど行列の主な演算の定義
 *  日付
 *		作成日: 2007/01/15(MON)		更新日: 2007/06/23(SAT)
 */

 /**
  *	----------------------------------------------------------------------
  *		ヘッダファイルインクルード
  *	----------------------------------------------------------------------
  */
#include "Matrix.h"

using namespace std;

namespace Math
{
/**
 *	----------------------------------------------------------------------
 *		Matrixクラス
 *	----------------------------------------------------------------------
 */

 /**
  *	------------------------------------------------------------
  *		フレンド関数
  *	------------------------------------------------------------
  */

  /**
   *	説明
   *		クラスMatrixの多重定義されたストリーム挿入演算子
   *		行列の全要素の値を出力する
   *	引数
   *		output: 出力先ストリーム
   *		matrix: 目的の行列
   */
ostream& operator<<(ostream& output, const Matrix& matrix)
{
    /// 科学表記法
    output.setf(ios::scientific);

    for (int i = 0; i < matrix.rows; i++)
        output << matrix.elements[i];

    output << endl;

    return output;
}

/**
 *	説明
 *		クラスMatrixの多重定義されたストリーム挿入演算子
 *		行列の全要素の値を入力する
 *	引数
 *		output: 入力先ストリーム
 *		matrix: 目的の行列
 */
istream& operator>>(istream& input, Matrix& matrix)
{
    cout << matrix.rows << "-by-" << matrix.columns << " matrix: Input all elements" << endl;

    for (int i = 0; i < matrix.rows; i++)
    {
        cout << (i + 1) << "-th row";
        input >> matrix.elements[i];
    }

    return input;
}


/**
 *	説明
 *		2つの行列が等しいかどうかをチェック
 *	引数
 *		true: 等しい
 *		false: 等しくない
 */
bool operator==(const Matrix& left, const Matrix& right)
{
    if ((left.rows != right.rows) || (left.columns != right.columns))
        return false;	/// サイズが違う

    for (int i = 0; i < left.rows; i++)
        if (left.elements[i] != right.elements[i])
            return false; /// 行列の内容が違う

    return true; /// ２つの行列は等しい
}

/**
 *	説明
 *		2つの行列が異なっているかどうかをチェック
 *	引数
 *		true: 異なっている
 *		false: 等しい
 */
bool operator!=(const Matrix& left, const Matrix& right)
{
    if (left == right)
        return false;	/// 等しい
    else
        return true;		///異なる
}

/**
 *		一般的な算術演算子
 *		多重定義された＊演算子
 */
 /// 行列とベクトルの積を求め，値渡しで返す
const Vector operator*(const Matrix& matrix, const Vector& vector)
{
    /// サイズのチェック
    if (matrix.columns != vector.dimensions)
    {
        cerr << "Error: [operator*] Matrix type is unmatched\n" << endl;
        abort();
    }

    Vector x(matrix.rows);

    for (int i = 0; i < matrix.rows; i++)
    {
        double sum = 0.0;

        for (int j = 0; j < matrix.columns; j++)
            sum += matrix.elements[i].elements[j] * vector.elements[j];

        x[i] = sum;
    }

    x.cleanUp();

    return x;
}

/// ベクトルと行列の積を求め，値渡しで返す
const Vector operator*(const Vector& vector, const Matrix& matrix)
{
    /// サイズのチェック
    if (vector.dimensions != matrix.rows)
    {
        cerr << "Error: [operator*] Matrix type is unmatched\n" << endl;
        abort();
    }

    Vector x(matrix.columns);

    for (int i = 0; i < matrix.columns; i++)
    {
        double sum = 0.0;

        for (int j = 0; j < matrix.rows; j++)
            sum += x.elements[j] * matrix.elements[j].elements[i];

        x.elements[i] = sum;
    }

    x.cleanUp();

    return x;
}

/// ２つの行列の積を求め，値渡しで返す
const Matrix operator*(const Matrix& left, const Matrix& right)
{
    /// サイズのチェック
    if (left.columns != right.rows)
    {
        cerr << "Error: [operator*] Matrix type is unmatched\n" << endl;
        abort();
    }

    /// 一時的なオブジェクトを作る
    Matrix matrix(left.rows, right.columns);

    for (int i = 0; i < left.rows; i++)
    {
        for (int j = 0; j < right.columns; j++)
        {
            double sum = 0.0;

            for (int k = 0; k < left.columns; k++)
                sum += left.elements[i].elements[k] * right.elements[k].elements[j];

            matrix.elements[i].elements[j] = sum;
        }
    }

    matrix.cleanUp();

    return matrix;
}

/**
 *	------------------------------------------------------------
 *		Matrixクラスのメンバ関数定義
 *	------------------------------------------------------------
 */

 /**
  *	説明
  *		要素を指定するコンストラクタ
  *	引数
  *		row: 行数
  *		col:  列数
  */
Matrix::Matrix(int row, int col) : rows(row), columns(col)
{
    /// 行列の領域確保
    newMatrix();
}

/// コピーコンストラクタ
Matrix::Matrix(const Matrix& matrix) : rows(matrix.rows), columns(matrix.columns)
{
    /// 行列の領域確保
    newMatrix();

    /// 要素を代入
    for (int i = 0; i < rows; i++)
        this->elements[i] = matrix.elements[i];
}

/// デストラクタ
Matrix::~Matrix()
{
    deleteMatrix();
}

void Matrix::setSize(int row, int col)
{
    /// 行列の領域解放
    deleteMatrix();

    this->rows = row;
    this->columns = col;

    /// 行列の領域確保
    newMatrix();
}

/**
 *	説明
 *		微小要素の消去
 */
void Matrix::cleanUp(void)
{
    int i, j;
    double max = 0.0;

    for (i = 0; i < rows; i++)
        for (j = 0; j < columns; j++)
            if (fabs(elements[i].elements[j]) > max)
                max = fabs(elements[i].elements[j]);

    if (max > NEAR_ZERO)
        for (i = 0; i < rows; i++)
            for (j = 0; j < columns; j++)
                if (fabs(elements[i].elements[j]) / max < ZERO_ERROR_MARGIN)
                    elements[i].elements[j] = 0.0;

    return;
}

/**
 *	----------------------------------------
 *	行列演算のための演算子
 *	----------------------------------------
 */
 /// 代入演算子
Matrix& Matrix::operator=(const Matrix& right)
{
    /// 自己代入をチェック
    if (this != &right)
    {

        /// 行列の型をチェック
        if ((rows != right.rows) || (columns != right.columns))
        {
            cerr << "Error: [Matrix::operator=] Invalid argument (size different)" << endl;
            abort();
        }

        /// 行ベクトルを代入
        for (int i = 0; i < rows; i++)
            this->elements[i] = right.elements[i];
    }

    ///  x = y = zと書けるようにする
    return *this;
}

/**
 *	代表的な組み合わせ演算子
 */
 /// 多重定義された＋＝演算子
Matrix& Matrix::operator+=(const Matrix& right)
{
    /// サイズのチェック
    if ((rows != right.rows) || (columns != right.columns))
    {
        cerr << "Error: [Matrix::operator+=] Matrix type is unmatched\n" << endl;
        abort();
    }

    for (int i = 0; i < rows; i++)
        elements[i] += right.elements[i];

    return *this;
}

/// 多重定義された-＝演算子
Matrix& Matrix::operator-=(const Matrix& right)
{
    /// サイズのチェック
    if ((rows != right.rows) || (columns != right.columns))
    {
        cerr << "Error: [Matrix::operator-=] Matrix type is unmatched\n" << endl;
        abort();
    }

    for (int i = 0; i < rows; i++)
        elements[i] -= right.elements[i];

    return *this;
}

/// 多重定義された＊＝演算子
Matrix& Matrix::operator*=(const Matrix& right)
{
    /// サイズのチェック
    if ((rows != right.rows) || (columns != right.columns))
    {
        cerr << "Error: [Matrix::operator*=] Matrix type is unmatched\n" << endl;
        abort();
    }

    /// 一時的なオブジェクトを作る
    Matrix matrix(rows, right.columns);

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < right.columns; j++)
        {
            double sum = 0.0;

            for (int k = 0; k < columns; k++)
                sum += elements[i].elements[k] * right.elements[k].elements[j];

            matrix.elements[i].elements[j] = sum;
        }
    }

    matrix.cleanUp();

    return *this = matrix;
}

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
Matrix& Matrix::loadMatrix(int startRow, int startCol,
              const Matrix& source,
              int firstRow, int firstCol,
              int lastRow, int lastCol)
{
    /// 引数のチェック
    if (!(1 <= firstRow) || !(1 <= firstCol)
      || !(firstRow <= lastRow) || !(firstCol <= lastCol)
      || !(source.rows <= (lastRow - firstRow))
      || !(source.columns <= (lastCol - firstCol))
      || !((lastRow - firstRow) <= (rows - startRow))
      || !((lastCol - firstCol) <= (columns - startCol)))
    {
        cerr << "Error: [Matrix::loadMatrix] Index is out of range\n" << endl;
        abort();
    }

    /// コピー開始
    for (int i = 0; i < (lastRow - firstRow + 1); i++)
        for (int j = 0; j < (lastCol - firstCol + 1); j++)
            (*this)[startRow + i - 1][startCol + j - 1] = source[firstRow + i - 1][firstCol + j - 1];

    return *this;
}

/**
 *	説明
 *		読み込んだ行列をコピー元の始点と終点を指定してコピーする
 *		コピー先の行列に1行1列目からコピーする
 *	引数
 *		source: コピー元の行列
 *		firstRow, firstCol: コピー元の行列の始点
 *		lastRow, lastCol: コピー元の行列の終点
 */
Matrix& Matrix::loadMatrix(const Matrix& source, int firstRow, int firstCol, int lastRow, int lastCol)
{

    this->loadMatrix(1, 1, source, firstRow, firstCol, lastRow, lastCol);

    return *this;
}

/**
 *	説明
 *		読み込んだ行列をコピー先の始点と終点を指定してコピーする
 *		コピー元の行列の1行1列目からコピーする
 *	引数
 *		startRow, startCol: コピー先の行列のコピー開始点
 *		source: コピー元の行列
 */
Matrix& Matrix::loadMatrix(int startRow, int startCol, const Matrix& source)
{
    this->loadMatrix(startRow, startCol, source, 1, 1, source.rows, source.columns);

    return *this;
}

/**
 *	説明
 *		読み込んだ行列をコピー元の始点を指定してコピーする
 *		コピー先の行列に1行1列目からコピーする
 *	引数
 *		source: コピー元の行列
 *		firstRow, firstCol: コピー元の行列の始点
 */
Matrix& Matrix::loadMatrix(const Matrix& source, int firstRow, int firstCol)
{
    /// 引数チェック
    if (!((source.rows - firstRow + 1) <= rows) || !((source.columns - firstCol + 1) <= columns))
    {
        cerr << "Error: [Matrix::loadMatrix] Failed copy into sub matrix\n" << endl;
        abort();
    }

    /// コピー開始
    for (int i = 0; i < this->rows; i++)
        for (int j = 0; j < this->columns; j++)
            (*this)[i][j] = source[firstRow + i - 1][firstCol + j - 1];
    ///[1+i-1][1+j-1]
    return *this;
}

/**
 *	説明
 *		読み込んだ行列をコピーする
 *		コピー先の行列に1行1列目からコピーする
 *		コピー元の行列の1行1列目からコピーする
 *	引数
 *		source: コピー元の行列
 */
Matrix& Matrix::loadMatrix(const Matrix& source)
{

    if (!(source.rows <= rows) || !(source.columns <= columns))
    {
        cerr << "Error: [Matrix::loadMatrix] Failed copy into sub matrix\n" << endl;
        abort();
    }

    for (int i = 0; i < this->rows; i++)
        for (int j = 0; j < this->columns; j++)
            (*this)[i][j] = source[i][j];
    ///[1+i-1][1+j-1]
    return *this;
}

/**
 *	説明
 *		零行列を読み込む
 */
Matrix& Matrix::loadZero(void)
{
    for (int i = 0; i < this->rows; i++)
        for (int j = 0; j < this->columns; j++)
            (*this)[i][j] = 0.0;

    return *this;
}

/**
 *	説明
 *		単位行列を読み込む
 */
Matrix& Matrix::loadIdentity(void)
{
    if (!this->isSquare())
    {
        cerr << "Error: [Matrix::loadIdentity] Matrix is not square\n" << endl;
        abort();
    }

    this->loadZero();

    for (int i = 0; i < this->rows; i++)
        (*this)[i][i] = 1.0;

    return *this;
}


Matrix& Matrix::transpose(void)
{
    if (!this->isSquare())
    {
        cerr << "Error: [Matrix::transpose] Not square matrix\n" << endl;
        abort();
    }

    double swap;

    for (int i = 0; i < this->rows; i++)
    {
        for (int j = 0; j < (i - 1); j++)
        {
            swap = (*this)[j][i];

            (*this)[j][i] = (*this)[i][j];
            (*this)[i][j] = swap;
        }
    }

    return *this;
}


Matrix Matrix::transposition(void)
{
    if (!this->isSquare())
    {
        cerr << "Error: [Matrix::transposition] Not square matrix\n" << endl;
        abort();
    }

    /// 戻り値の転置行列
    Matrix trans((*this));
    double swap;

    for (int i = 0; i < this->rows; i++)
    {
        for (int j = 0; j < (i - 1); j++)
        {
            swap = trans[j][i];

            trans[j][i] = trans[i][j];
            trans[i][j] = swap;
        }
    }

    /// 値返しにする
    return trans;
}

const Matrix Matrix::transposition(void) const
{
    if (!this->isSquare())
    {
        cerr << "Error: [Matrix::transposition] Not square matrix\n" << endl;
        abort();
    }

    /// 戻り値の転置行列
    Matrix trans((*this));
    double swap;

    for (int i = 0; i < this->rows; i++)
    {
        for (int j = 0; j < (i - 1); j++)
        {
            swap = trans[j][i];

            trans[j][i] = trans[i][j];
            trans[i][j] = swap;
        }
    }

    /// 値返しにする
    return trans;
}



/**
 *	説明
 *		呼出元の逆行列を返す
 */
Matrix Matrix::inversion(void)
{
    /// 正方行列かどうかのチェック
    if (!this->isSquare())
    {
        cerr << "Error: [Matrix::inverse] Not square matrix\n" << endl;
        abort();
    }

    Matrix lu(*this);						/// 係数行列
    Matrix x(this->rows, this->columns);	/// 求める逆行列
    Vector indx(rows);						/// 行交換を記録したベクトル
    Vector b(rows);
    double d;								///　行交換の回数が偶数=1，行交換の回数が奇数=-1

    /// 行列を1回だけLU分解する
    luDecomposition((*this), lu, indx, &d);

    /// 逆行列を列ごとに求める
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < rows; j++)
            b[j] = 0.0;

        b[i] = 1.0;
        luBackSubstitution(lu, indx, b);

        for (int k = 0; k < rows; k++)
            x[k][i] = b[k];
    }

    /// 求めるべき逆行列
    return x;
}

/**
 *	説明
 *		呼出元の逆行列を返す
 */
const Matrix Matrix::inversion(void) const
{
    /// 正方行列かどうかのチェック
    if (!this->isSquare())
    {
        cerr << "Error: [Matrix::inverse] Not square matrix\n" << endl;
        abort();
    }

    Matrix lu(*this);						/// 係数行列
    Matrix x(this->rows, this->columns);	/// 求める逆行列
    Vector indx(rows);						/// 行交換を記録したベクトル
    Vector b(rows);
    double d;								///　行交換の回数が偶数=1，行交換の回数が奇数=-1

    /// 行列を1回だけLU分解する
    luDecomposition((*this), lu, indx, &d);

    /// 逆行列を列ごとに求める
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < rows; j++)
            b[j] = 0.0;

        b[i] = 1.0;
        luBackSubstitution(lu, indx, b);

        for (int k = 0; k < rows; k++)
            x[k][i] = b[k];
    }

    /// 求めるべき逆行列
    return x;
}


/**
 *	説明
 *		行列の要素をコンソールに書き出す
 */
 /// 全ての要素を書き出す
void Matrix::show(void) const
{
    for (int i = 0; i < this->rows; i++)
    {
        for (int j = 0; j < this->columns; j++)
        {
            if ((*this)[i][j] < 0.0)		/// 負の場合
            {
                cout.precision(5);
                cout.setf(ios_base::fixed, ios_base::floatfield);
                cout << "-" << -(*this)[i][j];

            }
            else if ((*this)(i, j) == 0.0)	/// 0の場合
            {
                cout.precision(5);
                cout.setf(ios_base::fixed, ios_base::floatfield);
                cout << 0.0;
            }
            else								/// 正の場合
            {
                cout.precision(5);
                cout.setf(ios_base::fixed, ios_base::floatfield);
                cout << (*this)[i][j];
            }
        }

        cout << endl;
    }

    cout << endl;
}

/// 全ての要素を指数形式で書き出す
void Matrix::showExp(void) const
{
    for (int i = 0; i < this->rows; i++)
    {
        for (int j = 0; j < this->columns; j++)
        {
            if ((*this)[i][j] < 0.0)		/// 負の場合
            {
                cout.precision(5);
                cout.setf(ios_base::scientific, ios_base::floatfield);
                cout << "-" << -(*this)[i][j];

            }
            else if ((*this)(i, j) == 0.0)	/// 0の場合
            {
                cout.precision(5);
                cout.setf(ios_base::scientific, ios_base::floatfield);
                cout << 0.0;
            }
            else								/// 正の場合
            {
                cout.precision(5);
                cout.setf(ios_base::scientific, ios_base::floatfield);
                cout << (*this)[i][j];
            }
        }

        cout << endl;
    }

    cout << endl;
}

/**
 *	説明
 *		行列の体格成分の和を求める
 */
double Matrix::trace(void)
{
    /// 正方行列かどうかのチェック
    if (!this->isSquare())
    {
        cerr << "Error: [Matrix::trace] Not square matrix\n" << endl;
        abort();
    }

    double res = 0;
    double tmp = 0;		/// 計算用
    double trace = 0;		/// 対角成分の和

    for (int i = 0; i < this->rows; i++)
    {
        res += (*this)[i][i];
        tmp = trace;
        trace += res;
        tmp = trace - tmp;
        res = res - tmp;
    }

    return trace;
}

/**
 *	説明
 *		行列の体格成分の和を求める
 */
const double Matrix::trace(void) const
{
    /// 正方行列かどうかのチェック
    if (!this->isSquare())
    {
        cerr << "Error: [Matrix::trace] Not square matrix\n" << endl;
        abort();
    }

    double res = 0;
    double tmp = 0;		/// 計算用
    double trace = 0;		/// 対角成分の和

    for (int i = 0; i < this->rows; i++)
    {
        res += (*this)[i][i];
        tmp = trace;
        trace += res;
        tmp = trace - tmp;
        res = res - tmp;
    }

    return trace;
}

/**
 *	説明
 *		行の交換
 *	引数
 *		i: 交換する行
 *		j: 交換される行
 */
void Matrix::swapRows(int i, int j)
{
    /// 引数チェック
    if (!(1 < i && i < rows) || !(1 < j && j < rows))
    {
        cerr << "Error: [Matrix::swapRows] Arguments are invalid\n" << endl;
        abort();
    }

    if (i == j) return;

    /// 行ベクトルの交換
    double* temp = elements[i].elements;
    elements[i].elements = elements[j].elements;
    elements[j].elements = temp;

    return;
}





void Matrix::newMatrix(void)
{
    if (rows == 0 || columns == 0)
    {
        rows = 0;
        columns = 0;
        elements = 0;

        return;
    }

    /// 行ベクトルのメモリ領域確保
    elements = new Vector[rows];

    /// new演算子の成否確認
    if (elements == 0)
    {
        cerr << "Error: [Matrix::newMatrix] Memory is not allocated\n";
        abort();
    }

    for (int i = 0; i < rows; i++)
        elements[i].setSize(columns);
}

/**
 *	説明
 *		行列オブジェクトのメモリ領域を解放する
 */
void Matrix::deleteMatrix(void)
{
    /// 行ベクトルを解放
    delete[] elements;

    /// メンバ変数初期化
    rows = 0;
    columns = 0;
    elements = 0;
}

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
 *						Cambridge University Press, 1988
 */
int Matrix::luDecomposition(const Matrix& a, Matrix& lu, Vector& indx, double* d) const
{
    int		i, imax = 0, j, k;
    double	big, dum, sum, temp;

    /// LU分解される行列で初期化
    lu.loadMatrix(a);
    int n = a.getRow();
    Vector vv(n);		/// 暗黙のスケーリングを記録するためのもの

    *d = 1.0;			/// まだ行交換をしていない

    /// 行についてループし，暗黙のスケーリングの情報取得
    for (i = 1; i <= n; i++)
    {
        big = 0.0;

        for (j = 1; j <= n; j++)
            if ((temp = fabs(lu[i - 1][j - 1])) > big)
                big = temp;

        /// 最大の要素が0なら特異行列
        if (big == 0.0)
        {
            cerr << "Error: [Matrix::luDecomposition] Singular matrix occurs\n" << endl;
            return 0;
        }

        vv[i - 1] = 1.0 / big;
    }

    /// Crout法，列についてのループ
    for (j = 1; j <= n; j++)
    {
        for (i = 1; i < j; i++)
        {
            sum = lu[i - 1][j - 1];

            for (k = 1; k < i; k++)
                sum -= lu[i - 1][k - 1] * lu[k - 1][j - 1];

            lu[i - 1][j - 1] = sum;
        }

        big = 0.0;		/// 最大ピボット要素を探す準備

        for (i = j; i <= n; i++)
        {
            sum = lu[i - 1][j - 1];

            for (k = 1; k < j; k++)
                sum -= lu[i - 1][k - 1] * lu[k - 1][j - 1];

            lu[i - 1][j - 1] = sum;

            /// ピボットの良さは今までの最大値以上か？
            if ((dum = vv[i - 1] * fabs(sum)) >= big)
            {
                big = dum;
                imax = i;
            }
        }

        /// 行交換の必要があるか？
        if (j != imax)
        {
            for (k = 1; k <= n; k++)
            {
                dum = lu[imax - 1][k - 1];
                lu[imax - 1][k - 1] = lu[j - 1][k - 1];
                lu[j - 1][k - 1] = dum;
            }

            *d = -(*d);		/// dの偶奇を変える

            vv[imax - 1] = vv[j - 1];		/// スケール因子を交換する
        }

        indx[j - 1] = imax;

        /**
         *		ピボット要素が0なら行列は特異である
         *		特異行列の利用法によっては0にすることが良いこともある
         */
        if (lu[j - 1][j - 1] == 0.0)
            lu[j - 1][j - 1] = MATRIX_TINY;

        /// ピボットを要素で割る
        if (j != n)
        {
            dum = 1.0 / lu[j - 1][j - 1];

            for (i = j + 1; i <= n; i++)
                lu[i - 1][j - 1] *= dum;
        }
    }	/// Crout法，列についてのループ -> 次の列に移る

    /// 正常終了
    return 1;
}

/**
 *	説明
 *		後退代入を用いて連立1次方程式を解く
 *	引数
 *		Matrix  a	: LU分解で求めたAのLU分解（不変）
 *		Matrix  indx: LU分解の際の部分ピボット選択の行交換情報
 *		double* d	: 行交換の回数が偶数=1，行交換の回数が奇数=-1
 *
 *		Reference :	William H. Press, et al, NUMERICAL RECIPES in C,
 *						Cambridge University Press, 1988
 */
void Matrix::luBackSubstitution(const Matrix& a, const Vector& indx, Vector& b) const
{
    int    i, ii = 0, ip, j;
    double sum;
    int n = a.getRow();

    /**
     *		iiが正なら，それはbの最初の0でない要素の添え字である．
     *		行交換を元に戻しながら前進代入を行う
     */
    for (i = 1; i <= n; i++)
    {
        ip = (int)indx[i - 1];
        sum = b[ip - 1];
        b[ip - 1] = b[i - 1];

        if (ii)
        {
            for (j = ii; j <= i - 1; j++)
                sum -= a[i - 1][j - 1] * b[j - 1];
        }
        else if (sum)		/// 非零要素に出会った。これ以降は上のループで和を求めなければならない
            ii = i;

        b[i - 1] = sum;
    }

    /// 後退代入開始
    for (i = n; i >= 1; i--)
    {
        sum = b[i - 1];

        for (j = i + 1; j <= n; j++)
            sum -= a[i - 1][j - 1] * b[j - 1];

        /// 解ベクトルxの成分を上書き保存
        b[i - 1] = sum / a[i - 1][i - 1];
    }	/// 解法終了
}

/**
 *	説明
 *		行列の特異値分解（Singular Value Decomposition）
 *	引数
 *		Matrix  a	: 特異値分解される行列
 *		Matrix  u	: 左基底行列
 *		Matrix  v	: 特異値の対角行列
 *		Matrix  w	: 右基底行列
 *		double* d	: 行交換の回数が偶数=1，行交換の回数が奇数=-1
 *
 *		Reference :	William H. Press, et al, NUMERICAL RECIPES in C,
 *						Cambridge University Press, 1988
 */
void Matrix::singularValueDecomposition(const Matrix& a, Matrix& u, Matrix& w, Matrix& v) const
{
    if (!(a.rows == u.rows && a.columns == u.columns))
    {
        cerr << "Error: [Matrix::singularValueDecomposition] Matrix type is unmatched\n" << endl;
        return;
    }

    /// 対象とする行列をコピー
    u.loadMatrix(a);

    int flag, i, its, j, jj, k, nm, l, n, m;
    double anorm, c, f, g, h, s, scale, x, y, z;
    double maxarg1, maxarg2;
    double iminarg1, iminarg2;

    m = u.rows;
    n = u.columns;

    Vector rv1(n);

    g = scale = anorm = 0.0;

    /// Householder法で2重対角の形に直す
    for (i = 1; i <= n; i++)
    {
        l = i + 1;

        rv1[i - 1] = scale * g;
        g = s = scale = 0.0;

        if (i <= m)
        {
            for (k = i; k <= m; k++)
                scale += fabs(u[k - 1][i - 1]);

            if (scale)
            {
                for (k = i; k <= m; k++)
                {
                    u[k - 1][i - 1] /= scale;
                    s += u[k - 1][i - 1] * u[k - 1][i - 1];
                }

                f = u[i - 1][i - 1];
                g = -(f >= 0.0 ? fabs(sqrt(s)) : -fabs(sqrt(s)));
                h = f * g - s;
                u[i - 1][i - 1] = f - g;

                for (j = l; j <= n; j++)
                {
                    for (s = 0.0, k = i; k <= m; k++)
                        s += u[k - 1][i - 1] * u[k - 1][j - 1];

                    f = s / h;

                    for (k = i; k <= m; k++)
                        u[k - 1][j - 1] += f * u[k - 1][i - 1];
                }

                for (k = i; k <= m; k++)
                    u[k - 1][i - 1] *= scale;
            }
        }

        w[i - 1][i - 1] = scale * g;
        g = s = scale = 0.0;

        if (i <= m && i != n)
        {
            for (k = l; k <= n; k++)
                scale += fabs(u[i - 1][k - 1]);

            if (scale)
            {
                for (k = l; k <= n; k++)
                {
                    u[i - 1][k - 1] /= scale;
                    s += u[i - 1][k - 1] * u[i][k];
                }

                f = u[i - 1][l - 1];
                g = -(f >= 0.0 ? fabs(sqrt(s)) : -fabs(sqrt(s)));
                h = f * g - s;
                u[i - 1][l - 1] = f - g;

                for (k = l; k <= n; k++)
                {
                    rv1[k - 1] = u[i - 1][k - 1] / h;
                }

                for (j = l; j <= m; j++)
                {
                    for (s = 0.0, k = l; k <= n; k++)
                        s += u[j - 1][k - 1] * u[i - 1][k - 1];

                    for (k = l; k <= n; k++)
                        u[j - 1][k - 1] += s * rv1[k - 1];
                }

                for (k = l; k <= n; k++)
                    u[i - 1][k - 1] *= scale;
            }
        }

        maxarg1 = anorm;
        maxarg2 = fabs(w[i - 1][i - 1]) + fabs(rv1[i - 1]);
        anorm = (maxarg1 > maxarg2 ? maxarg1 : maxarg2);
    }	/// ここまでHouseholder法

    /// 右側の変換の累算
    for (i = n; i >= 1; i--)
    {
        if (i < n)
        {
            if (g)
            {
                for (j = l; j <= n; j++)
                    v[j - 1][i - 1] = (u[i - 1][j - 1] / u[i - 1][l - 1]) / g;	/// アンダーフローを避けるため，2度割りする

                for (j = l; j <= n; j++)
                {
                    for (s = 0.0, k = l; k <= n; k++)
                        s += u[i - 1][k - 1] * v[k - 1][j - 1];

                    for (k = l; k <= n; k++)
                        v[k - 1][j - 1] += s * v[k - 1][i - 1];
                }
            }

            for (j = l; j <= n; j++)
                v[i - 1][j - 1] = v[j - 1][i - 1] = 0.0;
        }

        v[i - 1][i - 1] = 1.0;
        g = rv1[i - 1];
        l = i;
    }

    iminarg1 = m;
    iminarg2 = n;

    /// 左側の変換の累算
    for (i = (m < n ? m : n); i >= 1; i--)
    {
        l = i + 1;
        g = w[i - 1][i - 1];

        for (j = l; j <= n; j++)
            u[i - 1][j - 1] = 0.0;

        if (g)
        {
            g = 1.0 / g;

            for (j = l; j <= n; j++)
            {
                for (s = 0.0, k = l; k <= m; k++)
                    s += u[k - 1][i - 1] * u[k - 1][j - 1];

                f = (s / u[i - 1][i - 1]) * g;

                for (k = i; k <= m; k++)
                    u[k - 1][j - 1] += f * u[k - 1][i - 1];
            }

            for (j = i; j <= m; j++)
                u[j - 1][i - 1] *= g;
        }
        else
        {
            for (j = i; j <= m; j++)
                u[j - 1][i - 1] = 0.0;
        }
        u[i - 1][i - 1] += 1.0;
    }

    /// 2重対角行列の対角化：特異値についてのループと反復計算のループ
    for (k = n; k >= 1; k--)
    {
        for (its = 1; its <= 30; its++)
        {
            flag = 1;

            for (l = k; l >= 1; l--)	/// 分割のチェック
            {
                nm = l - 1;			/// rv1(1)は常に0
                if ((fabs(rv1[l - 1]) + anorm) == anorm)
                {
                    flag = 0;
                    break;
                }

                if ((fabs(w[nm - 1][nm - 1]) + anorm) == anorm)
                    break;
            }

            if (flag)		/// l > 1のときrv1(l)を消去
            {
                c = 0.0;
                s = 1.0;

                for (i = l; i <= k; i++)
                {
                    f = s * rv1[i - 1];
                    rv1[i - 1] = c * rv1[i - 1];

                    if ((fabs(f) + anorm) == anorm)
                        break;

                    g = w[i - 1][i - 1];
                    h = pythagorean(f, g);
                    w[i - 1][i - 1] = h;
                    h = 1.0 / h;
                    c = g * h;
                    s = -f * h;

                    for (j = 1; j <= m; j++)
                    {
                        y = u[j - 1][nm - 1];
                        z = u[j - 1][i - 1];
                        u[j - 1][nm - 1] = y * c + z * s;
                        u[j - 1][i - 1] = z * c - y * s;
                    }
                }
            }

            z = w[k - 1][k - 1];

            if (l == k)			/// 収束した
            {
                if (z < 0.0)		/// 特異値を非負にする
                {
                    w[k - 1][k - 1] = -z;

                    for (j = 1; j <= n; j++)
                        v[j - 1][k - 1] = -v[j - 1][k - 1];
                }
                break;
            }

            /// ↓30回繰り返しても収束しなかった
            if (its == 30)
            {
                cerr << "Error: [Matrix::singularValueDecomposition] No convergence in 30 svdcmp iterations\n" << endl;
                return;
            }

            x = w[l - 1][l - 1];		/// 最下部の2行2列の小行列式からのシフト
            nm = k - 1;
            y = w[nm - 1][nm - 1];
            g = rv1[nm - 1];
            h = rv1[k - 1];
            f = ((y - z) * (y + z) + (g - h) * (g + h)) / (2.0 * h * y);
            g = pythagorean(f, 1.0);
            f = ((x - z) * (x + z) + h * ((y / (f + (f >= 0.0 ? fabs(g) : -fabs(g)))) - h)) / x;
            c = s = 1.0;			/// 次のQR変換

            for (j = l; j <= nm; j++)
            {
                i = j + 1;
                g = rv1[i - 1];
                y = w[i - 1][i - 1];
                h = s * g;
                g = c * g;
                z = pythagorean(f, h);
                rv1[j - 1] = z;
                c = f / z;
                s = h / z;
                f = x * c + g * s;
                g = g * c - x * s;
                h = y * s;
                y *= c;

                for (jj = 1; jj <= n; jj++)
                {
                    x = v[jj - 1][j - 1];
                    z = v[jj - 1][i - 1];
                    v[jj - 1][j - 1] = x * c + z * s;
                    v[jj - 1][i - 1] = z * c - x * s;
                }

                z = pythagorean(f, h);
                w[j - 1][j - 1] = z;			/// z=0なら回転は任意

                if (z)
                {
                    z = 1.0 / z;
                    c = f * z;
                    s = h * z;
                }

                f = c * g + s * y;
                x = c * y - s * g;

                for (jj = 1; jj <= m; jj++)
                {
                    y = u[jj - 1][j - 1];
                    z = u[jj - 1][i - 1];

                    u[jj - 1][j - 1] = y * c + z * s;
                    u[jj - 1][i - 1] = z * c - y * s;
                }
            }

            rv1[l - 1] = 0.0;
            rv1[k - 1] = f;
            w[k - 1][k - 1] = x;
        }
    }

    /// 計算用の行列
    Matrix uu(u);
    Matrix vv(v);
    Vector no(a.columns);

    for (int num = 1; num <= w.columns; num++)
        no[num - 1] = (double)num;

    quickSort(w, no, 1, w.columns);

    for (int num = 1; num <= w.columns; num++)
    {
        for (int i = 1; i <= uu.rows; i++)
            uu[i - 1][num - 1] = u[i - 1][(int)no[num - 1] - 1];

        for (int j = 1; j <= vv.rows; j++)
            vv[j - 1][num - 1] = v[j - 1][(int)no[num - 1] - 1];
    }

    return;
}

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
double Matrix::pythagorean(double a, double b) const
{
    double absa = fabs(a);
    double absb = fabs(b);
    double sqrarg;

    if (absa > absb)
    {
        sqrarg = absb / absa;
        /// 戻り値
        return absa * sqrt(1.0 + (sqrarg == 0.0 ? 0.0 : sqrarg * sqrarg));
    }
    else
    {
        sqrarg = absa / absb;
        /// 戻り値
        return (absb == 0.0 ? 0.0 : absb * sqrt(1.0 + (sqrarg == 0.0 ? 0.0 : sqrarg * sqrarg)));
    }
}

/**
 *	説明
 *		クイックソート
 */
void Matrix::quickSort(Matrix& w, Vector& no, int low, int high) const
{
    if (low < high)
    {
        int mid;
        int upsw = 1;
        double w_tmp, no_tmp;
        int lp = low;
        int hp = high;

        while (lp < hp)
        {
            if (w[lp - 1][lp - 1] < w[hp - 1][hp - 1])
            {
                w_tmp = w[lp - 1][lp - 1];
                no_tmp = no[lp - 1];
                w[lp - 1][lp - 1] = w[hp - 1][hp - 1];
                no[lp - 1] = no[hp - 1];
                w[hp - 1][hp - 1] = w_tmp;
                no[hp - 1] = no_tmp;

                if (upsw)
                    upsw = 0;
                else
                    upsw = 1;
            }

            if (upsw)
                hp--;
            else
                lp++;
        }

        mid = lp;

        quickSort(w, no, low, mid - 1);

        quickSort(w, no, mid + 1, high);
    }
}

/**
 *	----------------------------------------------------------------------
 *		Vectorクラス
 *	----------------------------------------------------------------------
 */

 /**
  *	------------------------------------------------------------
  *		フレンド関数
  *	------------------------------------------------------------
  */
  /**
   *	説明
   *		クラスVectorの多重定義されたストリーム挿入演算子
   *		ベクトルの全要素の値を出力する
   *	引数
   *		output: 出力先ストリーム
   *		vector: 目的の行列
   */
ostream& operator<<(ostream& output, const Vector& vector)
{
    /// 科学表記法
    output.setf(ios::scientific);

    for (int i = 0; i < vector.dimensions; i++)
    {
        output << setw(15) << vector.elements[i];

        if (!((i + 1) % 5))
            output << endl;
    }

    if (vector.dimensions % 5)
        output << endl;

    /// cout << x << yと書けるようにする
    return output;
}

/**
 *	説明
 *		クラスVectorの多重定義されたストリーム挿入演算子
 *		ベクトルの全要素の値を入力する
 *	引数
 *		input: 入力先ストリーム
 *		vector: 目的の行列
 */
istream& operator>>(istream& input, Vector& vector)
{
    cout << "Vector (" << vector.dimensions
        << "columns ): Input all elements separated by space" << endl;

    for (int i = 0; i < vector.dimensions; i++)
        input >> vector.elements[i];

    /// cin >> x >> yと書けるようにする
    return input;
}

/**
 *	説明
 *		2つのベクトルが等しいかどうかをチェック
 *	引数
 *		true: 等しい
 *		false: 等しくない
 */
bool operator==(const Vector& left, const Vector& right)
{
    if (left.dimensions != right.dimensions)
        return false;	/// ベクトルのサイズが異なる

    for (int i = 0; i < left.dimensions; i++)
    {
        if (fabs(left.elements[i] - right.elements[i]) > NEAR_ZERO)
            return false;	/// ベクトルの内容が異なる
    }

    return true; //２つのベクトルは等しい
}

/**
 *	説明
 *		2つのベクトルが異なっているかどうかをチェック
 *	引数
 *		true: 異なっている
 *		false: 等しい
 */
bool operator!=(const Vector& left, const Vector& right)
{
    if (left == right)
        return false;	///２つのベクトルは等しい
    else
        return true;		///２つのベクトルは異なる
}

/// 多重定義された演算子＊を使った内積
double operator*(const Vector& left, const Vector& right)
{
    if (left.dimensions != right.dimensions)
    {
        cerr << "Error: [operator*] Element number is not matched\n" << endl;
        abort();
    }

    double a = 0.0;

    for (int i = 0; i < left.dimensions; i++)
        a += left.elements[i] * right.elements[i];

    return a;
}


/**
 *	------------------------------------------------------------
 *		Vectorクラスのメンバ関数定義
 *	------------------------------------------------------------
 */
 /**
  *	----------------------------------------
  *	コンストラクタとデストラクタ
  *	----------------------------------------
  */
  /// クラスVectorのデフォルトコンストラクタ
Vector::Vector(int dim) : dimensions(dim)
{
    /// ベクトルオブジェクトのメモリ領域確保
    newVector();

    for (int i = 0; i < dimensions; i++)
        elements[i] = 0.0;
}

/// クラスVectorのコピーコンストラクタ
Vector::Vector(const Vector& vector) : dimensions(vector.dimensions)
{
    /// ベクトルオブジェクトのメモリ領域確保
    newVector();

    for (int i = 0; i < dimensions; i++)
        elements[i] = vector.elements[i];	/// ベクトル要素をコピーする
}

/// クラスVectorのコンストラクタ（配列で初期化）
Vector::Vector(const double* ptrArray, int dim) : dimensions(dim)
{
    /// ベクトルオブジェクトのメモリ領域確保
    newVector();

    for (int i = 0; i < dimensions; i++)
        elements[i] = ptrArray[i];
}

/// クラスVectorのデストラクタ
Vector::~Vector()
{
    deleteVector();
}

/// ベクトルのサイズを再設定する
void Vector::setSize(int dim)
{
    /// ベクトルオブジェクトのメモリ領域解放
    deleteVector();

    dimensions = dim;
    /// ベクトルオブジェクトのメモリ領域確保
    newVector();

    for (int i = 0; i < dimensions; i++)
        elements[i] = 0.0; /// 要素を0.0にする
}

/**
 *	----------------------------------------
 *	演算子の多重定義
 *	----------------------------------------
 */
 /// 代入演算子
 /**
  *		dim: 成分数
  */
Vector& Vector::operator=(const Vector& right)
{
    /// 自己代入をチェックする
    if (this != &right)
    {

        if (dimensions != right.dimensions)
        {
            cerr << "Error: [Vector::operator=] Invalid argument (size different)" << endl;
            abort();
        }

        /// ベクトルの要素をコピーする
        for (int i = 0; i < dimensions; i++)
            elements[i] = right.elements[i];
    }

    ///  x = y = zと書けるようにする
    return *this;
}

/**
 *		代表的な組み合わせ演算子
 */
 /// 多重定義された＊＝演算子
Vector& Vector::operator*=(double schalor)
{
    for (int i = 0; i < dimensions; i++)
        elements[i] *= schalor;

    return *this;
}

/// 多重定義された/＝演算子
Vector& Vector::operator/=(double schalor)
{
    /// 0割防止
    if (fabs(schalor) < NEAR_ZERO)
    {
        std::cerr << "Error: [Vector::operator/=] Division by zero\n" << std::endl;
        abort();
    }

    for (int i = 0; i < dimensions; i++)
        elements[i] /= schalor;

    return *this;
}

/// 多重定義された＋＝演算子
Vector& Vector::operator+=(const Vector& right)
{
    /// サイズのチェック
    if (dimensions != right.dimensions)
    {
        std::cerr << "Error: [Vector::operator+=] Unmatched vector size\n" << std::endl;
        abort();
    }

    for (int i = 0; i < dimensions; i++)
        elements[i] += right.elements[i];

    return *this;
}

/// 多重定義された-＝演算子
Vector& Vector::operator-=(const Vector& right)
{
    /// サイズのチェック
    if (dimensions != right.dimensions)
    {
        std::cerr << "Error: [Vector::operator-=] Unmatched vector size\n" << std::endl;
        abort();
    }

    for (int i = 0; i < dimensions; i++)
        elements[i] -= right.elements[i];

    return *this;
}

/**
 *		代表的な関数
 */
 /// ノルムの計算
double Vector::norm(void) const
{
    double a = 0.0;

    for (int i = 0; i < dimensions; i++)
        a += elements[i] * elements[i];

    return sqrt(a);
}

/// ベクトルを規格化（オブジェクト自身が規格化される）
const Vector& Vector::normalize(void)
{
    double a = norm();

    if (a < NEAR_ZERO)
        return *this; /// ノルムゼロのベクトル

    for (int i = 0; i < dimensions; i++)
        elements[i] /= a;

    return *this;
}

/// 呼び出したベクトルの単位ベクトルを返す
Vector Vector::GetNormalize(void)
{
    Vector unit(*this);

    unit.normalize();
    return unit;
}

/// 呼び出したベクトルの単位ベクトルを返す
const Vector Vector::GetNormalize(void) const
{
    Vector unit(*this);

    unit.normalize();
    return unit;
}

/// 微小要素を消去
void Vector::cleanUp(void)
{
    int i;
    double max = 0.0;

    for (i = 0; i < dimensions; i++)
        if (fabs(elements[i]) > max)
            max = fabs(elements[i]);

    if (max > NEAR_ZERO)
        for (i = 0; i < dimensions; i++)
            if (fabs(elements[i]) / max < ZERO_ERROR_MARGIN)
                elements[i] = 0.0;

    return;
}

/**
 *	------------------------------------------------------------
 *		Vectorクラスのprivateなメンバ関数
 *	------------------------------------------------------------
 */
 /// ベクトルの領域確保
void Vector::newVector(void)
{
    /// 次元数が０の場合ポインタをヌルにセット
    if (dimensions == 0)
    {
        elements = 0;
        return;
    }

    /// ベクトルのメモリー領域を確保する
    elements = new double[dimensions];

    if (dimensions == 0)
    {
        cerr << "Error: [Vector::newVector] Memory is not allocated\n" << endl;
        abort();
    }
}

/// ベクトルの領域解放
void Vector::deleteVector(void)
{
    ///ベクトルのメモリー領域を開放する
    delete[] elements;
}

} /// end of namespace mathematics