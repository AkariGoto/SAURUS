/**
 *  �t�@�C����
 *		Matrix.cpp
 *  ����
 *		���Z�q�⃁�\�b�h�ȂǍs��̎�ȉ��Z�̒�`
 *  ���t
 *		�쐬��: 2007/01/15(MON)		�X�V��: 2007/06/23(SAT)
 */

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include "Matrix.h"

using namespace std;

namespace Math
{
/**
 *	----------------------------------------------------------------------
 *		Matrix�N���X
 *	----------------------------------------------------------------------
 */

/**
 *	------------------------------------------------------------
 *		�t�����h�֐�
 *	------------------------------------------------------------
 */

/**
 *	����
 *		�N���XMatrix�̑��d��`���ꂽ�X�g���[���}�����Z�q
 *		�s��̑S�v�f�̒l���o�͂���
 *	����
 *		output: �o�͐�X�g���[��
 *		matrix: �ړI�̍s��
 */
ostream& operator<<(ostream& output, const Matrix& matrix)
{
	/// �Ȋw�\�L�@
    output.setf(ios::scientific); 

    for (int i = 0; i < matrix.rows; i++)
        output << matrix.elements[i];

	output << endl;

    return output;
}

/**
 *	����
 *		�N���XMatrix�̑��d��`���ꂽ�X�g���[���}�����Z�q
 *		�s��̑S�v�f�̒l����͂���
 *	����
 *		output: ���͐�X�g���[��
 *		matrix: �ړI�̍s��
 */
istream& operator>>(istream& input, Matrix& matrix)
{
	cout << matrix.rows << "-by-" << matrix.columns << " matrix: Input all elements" << endl;
	
	for (int i = 0; i < matrix.rows; i++)
	{
        cout << (i+1) << "-th row";
        input >> matrix.elements[i];
    }

    return input;
}


/**
 *	����
 *		2�̍s�񂪓��������ǂ������`�F�b�N
 *	����
 *		true: ������
 *		false: �������Ȃ�
 */
bool operator==(const Matrix& left, const Matrix& right)
{
    if ( (left.rows != right.rows) || (left.columns != right.columns) )
        return false;	/// �T�C�Y���Ⴄ

    for (int i = 0; i < left.rows; i++)
        if ( left.elements[i] != right.elements[i])
            return false; /// �s��̓��e���Ⴄ

    return true; /// �Q�̍s��͓�����
}

/**
 *	����
 *		2�̍s�񂪈قȂ��Ă��邩�ǂ������`�F�b�N
 *	����
 *		true: �قȂ��Ă���
 *		false: ������
 */
bool operator!=(const Matrix& left, const Matrix& right)
{
    if (left == right)
        return false;	/// ������
    else
        return true;		///�قȂ�
}

/**
 *		��ʓI�ȎZ�p���Z�q
 *		���d��`���ꂽ�����Z�q
 */
/// �s��ƃx�N�g���̐ς����߁C�l�n���ŕԂ�
const Vector operator*(const Matrix& matrix, const Vector& vector)
{
	/// �T�C�Y�̃`�F�b�N
    if (matrix.columns != vector.dimensions)
	{
		cerr << "Error: [operator*] Matrix type is unmatched\n" << endl;
        abort();
    }

    Vector x( matrix.rows );
    
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

/// �x�N�g���ƍs��̐ς����߁C�l�n���ŕԂ�
const Vector operator*(const Vector& vector, const Matrix& matrix)
{
	/// �T�C�Y�̃`�F�b�N
    if ( vector.dimensions != matrix.rows )
	{
		cerr << "Error: [operator*] Matrix type is unmatched\n" << endl;
        abort();
    }

    Vector x( matrix.columns );

    for (int i = 0; i < matrix.columns; i++)
	{
        double sum = 0.0;
		
		for (int j = 0; j < matrix.rows; j++)
            sum +=  x.elements[j] * matrix.elements[j].elements[i];
        
		x.elements[i] = sum;
    }

	x.cleanUp();

    return x;
}

/// �Q�̍s��̐ς����߁C�l�n���ŕԂ�
const Matrix operator*(const Matrix& left, const Matrix& right)
{
	/// �T�C�Y�̃`�F�b�N
    if (left.columns != right.rows)
	{
		cerr << "Error: [operator*] Matrix type is unmatched\n" << endl;
        abort();
    }

	/// �ꎞ�I�ȃI�u�W�F�N�g�����
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
 *		Matrix�N���X�̃����o�֐���`
 *	------------------------------------------------------------
 */

/**
 *	����
 *		�v�f���w�肷��R���X�g���N�^
 *	����
 *		row: �s��
 *		col:  ��
 */
Matrix::Matrix(int row, int col) : rows(row), columns(col)
{
	/// �s��̗̈�m��
    newMatrix(); 
}

/// �R�s�[�R���X�g���N�^
Matrix::Matrix(const Matrix& matrix) : rows(matrix.rows), columns(matrix.columns)
{
	/// �s��̗̈�m��
    newMatrix();

	/// �v�f����
    for (int i = 0; i < rows; i++)
        this->elements[i] = matrix.elements[i];
}

/// �f�X�g���N�^
Matrix::~Matrix()
{
    deleteMatrix();
}

/**
 *	����
 *		�s��̌^��ݒ肷��
 *	����
 *		row: �s��
 *		col:  ��
 */
void Matrix::setSize(int row, int col)
{
	/// �s��̗̈���
    deleteMatrix();

	this->rows = row;
    this->columns = col;
    
	/// �s��̗̈�m��
	newMatrix(); 
}

/**
 *	����
 *		�����v�f�̏���
 */
void Matrix::cleanUp(void)
{
    int i, j;
    double max = 0.0;

	for (i=0;i<rows;i++)
		for (j=0;j<columns;j++)
		   if (fabs(elements[i].elements[j]) > max) 
			   max = fabs(elements[i].elements[j]);

    if (max > NEAR_ZERO)
		for (i=0;i<rows;i++)
			for (j=0;j<columns;j++)
				if (fabs(elements[i].elements[j]) / max < ZERO_ERROR_MARGIN)
					elements[i].elements[j] = 0.0;

	return;
}

/**
 *	----------------------------------------
 *	�s�񉉎Z�̂��߂̉��Z�q
 *	----------------------------------------
 */
/// ������Z�q
Matrix& Matrix::operator=(const Matrix& right)
{
	/// ���ȑ�����`�F�b�N
    if (this != &right)
	{

		/// �s��̌^���`�F�b�N
        if ((rows != right.rows) || (columns != right.columns))
		{
			cerr << "Error: [Matrix::operator=] Invalid argument (size different)" << endl;
            abort();
        }

		/// �s�x�N�g������
        for (int i=0;i<rows;i++)
			this->elements[i] = right.elements[i];
    }

	///  x = y = z�Ə�����悤�ɂ���
    return *this; 
}

/**
 *	��\�I�ȑg�ݍ��킹���Z�q
 */
/// ���d��`���ꂽ�{�����Z�q
Matrix& Matrix::operator+=(const Matrix& right)
{
	/// �T�C�Y�̃`�F�b�N
	if ((rows != right.rows) || (columns != right.columns))
	{
		cerr << "Error: [Matrix::operator+=] Matrix type is unmatched\n" << endl;
        abort();
	}

    for (int i=0;i<rows;i++)
		elements[i] += right.elements[i];

    return *this;
}

/// ���d��`���ꂽ-�����Z�q
Matrix& Matrix::operator-=(const Matrix& right)
{
	/// �T�C�Y�̃`�F�b�N
    if ((rows != right.rows) || (columns != right.columns))
	{
		cerr << "Error: [Matrix::operator-=] Matrix type is unmatched\n" << endl;
        abort();
    }

    for (int i=0;i<rows;i++)
        elements[i] -= right.elements[i];

    return *this;
}

/// ���d��`���ꂽ�������Z�q
Matrix& Matrix::operator*=(const Matrix& right)
{
	/// �T�C�Y�̃`�F�b�N
    if ((rows != right.rows) || (columns != right.columns))
	{
		cerr << "Error: [Matrix::operator*=] Matrix type is unmatched\n" << endl;
        abort();
    }

	/// �ꎞ�I�ȃI�u�W�F�N�g�����
    Matrix matrix(rows, right.columns);
    
	for (int i=0;i<rows;i++)
	{
        for(int j=0;j<right.columns;j++)
		{
            double sum = 0.0;

            for (int k=0;k<columns;k++)
				sum += elements[i].elements[k] * right.elements[k].elements[j];
            
			matrix.elements[i].elements[j] = sum;
        }
	}

	matrix.cleanUp();

    return *this = matrix;
}

/**
 *	----------------------------------------
 *	�s��̕����R�s�[
 *	----------------------------------------
 */
/**
 *	����
 *		�ǂݍ��񂾍s����R�s�[��̊J�n�_�C�R�s�[���̎n�_�ƏI�_���w�肵�ăR�s�[����
 *	����
 *		startRow, startCol: �R�s�[��̍s��̃R�s�[�J�n�_
 *		source: �R�s�[���̍s��
 *		firstRow, firstCol: �R�s�[���̍s��̎n�_
 *		lastRow, lastCol: �R�s�[���̍s��̏I�_
 */
Matrix& Matrix::loadMatrix(	int startRow, int startCol, 
							const Matrix& source, 
							int firstRow, int firstCol, 
							int lastRow, int lastCol)
{
	/// �����̃`�F�b�N
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

	/// �R�s�[�J�n
	for (int i=0;i<(lastRow - firstRow + 1);i++)
		for (int j=0;j<(lastCol - firstCol + 1);j++)
			(*this)[startRow+i - 1][startCol + j - 1] = source[firstRow + i - 1][firstCol + j - 1];

	return *this;
}

/**
 *	����
 *		�ǂݍ��񂾍s����R�s�[���̎n�_�ƏI�_���w�肵�ăR�s�[����
 *		�R�s�[��̍s���1�s1��ڂ���R�s�[����
 *	����
 *		source: �R�s�[���̍s��
 *		firstRow, firstCol: �R�s�[���̍s��̎n�_
 *		lastRow, lastCol: �R�s�[���̍s��̏I�_
 */
Matrix& Matrix::loadMatrix(const Matrix& source, int firstRow, int firstCol, int lastRow, int lastCol)
{

	this->loadMatrix(1, 1, source, firstRow, firstCol, lastRow, lastCol);

	return *this;
}

/**
 *	����
 *		�ǂݍ��񂾍s����R�s�[��̎n�_�ƏI�_���w�肵�ăR�s�[����
 *		�R�s�[���̍s���1�s1��ڂ���R�s�[����
 *	����
 *		startRow, startCol: �R�s�[��̍s��̃R�s�[�J�n�_
 *		source: �R�s�[���̍s��
 */
Matrix& Matrix::loadMatrix(int startRow, int startCol, const Matrix& source)
{
	this->loadMatrix( startRow, startCol, source, 1, 1, source.rows, source.columns );

	return *this;
}

/**
 *	����
 *		�ǂݍ��񂾍s����R�s�[���̎n�_���w�肵�ăR�s�[����
 *		�R�s�[��̍s���1�s1��ڂ���R�s�[���� 
 *	����
 *		source: �R�s�[���̍s��
 *		firstRow, firstCol: �R�s�[���̍s��̎n�_
 */
Matrix& Matrix::loadMatrix(const Matrix& source, int firstRow, int firstCol)
{
	/// �����`�F�b�N
	if (!((source.rows - firstRow+1) <= rows) || !((source.columns - firstCol+1) <= columns))
	{
		cerr << "Error: [Matrix::loadMatrix] Failed copy into sub matrix\n" << endl;
		abort();
	}

	/// �R�s�[�J�n
	for (int i=0;i<this->rows;i++)
		for (int j=0;j<this->columns;j++)
			(*this)[i][j] = source[firstRow + i - 1][firstCol + j - 1];
				///[1+i-1][1+j-1]
	return *this;
}

/**
 *	����
 *		�ǂݍ��񂾍s����R�s�[����
 *		�R�s�[��̍s���1�s1��ڂ���R�s�[���� 
 *		�R�s�[���̍s���1�s1��ڂ���R�s�[����
 *	����
 *		source: �R�s�[���̍s��
 */
Matrix& Matrix::loadMatrix(const Matrix& source)
{
	
	if (!(source.rows <= rows) || !(source.columns <= columns))
	{
		cerr << "Error: [Matrix::loadMatrix] Failed copy into sub matrix\n" << endl;
		abort();
	}

	for (int i=0;i<this->rows;i++)
		for (int j=0;j<this->columns;j++)
			(*this)[i][j] = source[i][j];
				///[1+i-1][1+j-1]
	return *this;
}

/**
 *	����
 *		��s���ǂݍ���
 */
Matrix& Matrix::loadZero(void)
{
	for (int i=0;i<this->rows;i++)
		for (int j=0;j<this->columns;j++)
			(*this)[i][j] = 0.0;

	return *this;
}

/**
 *	����
 *		�P�ʍs���ǂݍ���
 */
Matrix& Matrix::loadIdentity(void)
{
	if (!this->isSquare())
	{
		cerr << "Error: [Matrix::loadIdentity] Matrix is not square\n" << endl;
		abort();
	}
	
	this->loadZero();

	for (int i=0; i<this->rows;i++)
		(*this)[i][i] = 1.0;

	return *this;
}

/**
 *		�]�u�s��
 */
/**
 *	����
 *		�ďo���̍s���]�u�s��ɂ���
 */
Matrix& Matrix::transpose(void)
{
	if (!this->isSquare())
	{
		cerr << "Error: [Matrix::transpose] Not square matrix\n" << endl;
		abort();
	}

	double swap;
  
	for (int i=0;i<this->rows;i++)
	{
		for (int j=0;j<(i-1);j++)
		{
			swap = (*this)[j][i];

			(*this)[j][i] = (*this)[i][j];
			(*this)[i][j] = swap;
		}
	}

	return *this;
}

/**
 *	����
 *		�Ăяo�����s��̓]�u�s���Ԃ�
 */
Matrix Matrix::transposition(void)
{
	if (!this->isSquare())
	{
		cerr << "Error: [Matrix::transposition] Not square matrix\n" << endl;
		abort();
	}

	/// �߂�l�̓]�u�s��
	Matrix trans((*this));
	double swap;
  
	for (int i=0;i<this->rows;i++)
	{
		for (int j=0;j<(i-1);j++)
		{
			swap = trans[j][i];

			trans[j][i] = trans[i][j];
			trans[i][j] = swap;
		}
	}

	/// �l�Ԃ��ɂ���
	return trans;
}
/**
 *	����
 *		�Ăяo�����s��̓]�u�s���Ԃ�
 */
const Matrix Matrix::transposition(void) const
{
	if (!this->isSquare())
	{
		cerr << "Error: [Matrix::transposition] Not square matrix\n" << endl;
		abort();
	}

	/// �߂�l�̓]�u�s��
	Matrix trans((*this));
	double swap;
  
	for (int i=0;i<this->rows;i++)
	{
		for (int j=0;j<(i-1);j++)
		{
			swap = trans[j][i];

			trans[j][i] = trans[i][j];
			trans[i][j] = swap;
		}
	}

	/// �l�Ԃ��ɂ���
	return trans;
}

/**
 *	�t�s��
 */
/**
 *	����
 *		�ďo���̍s����t�s��ɂ���
 */
Matrix& Matrix::inverse(void)
{
	/// �����s�񂩂ǂ����̃`�F�b�N
	if (!this->isSquare())
	{
		cerr << "Error: [Matrix::inverse] Not square matrix\n" << endl;
		abort();
	}

	Matrix lu(*this);		/// �W���s��
	Vector indx(rows);		/// �s�������L�^�����x�N�g��
	Vector b(rows);
	double d;				/// �s�����̉񐔂�����=1�C�s�����̉񐔂��=-1

	/// �s���1�񂾂�LU��������
	luDecomposition(lu, (*this), indx, &d);
	
	/// �t�s���񂲂Ƃɋ��߂�
	for (int i=0;i<rows;i++)
	{
		for (int j=0;j<rows;j++)
			b[j] = 0.0;
		
		b[i] = 1.0;
		luBackSubstitution(lu, indx, b);
		
		for (int k=0;k<rows;k++)
			(*this)[k][i] = b[k];
	}

	/// ���߂�ׂ��t�s��
	return (*this);
}

/**
 *	����
 *		�ďo���̋t�s���Ԃ�
 */
Matrix Matrix::inversion(void)
{
	/// �����s�񂩂ǂ����̃`�F�b�N
	if (!this->isSquare())
	{
		cerr << "Error: [Matrix::inverse] Not square matrix\n" << endl;
		abort();
	}

	Matrix lu(*this);						/// �W���s��
	Matrix x(this->rows, this->columns);	/// ���߂�t�s��
	Vector indx(rows);						/// �s�������L�^�����x�N�g��
	Vector b(rows);
	double d;								///�@�s�����̉񐔂�����=1�C�s�����̉񐔂��=-1

	/// �s���1�񂾂�LU��������
	luDecomposition( (*this), lu, indx, &d);
	
	/// �t�s���񂲂Ƃɋ��߂�
	for (int i=0;i<rows;i++)
	{
		for (int j=0;j<rows;j++)
			b[j] = 0.0;
		
		b[i] = 1.0;
		luBackSubstitution(lu, indx, b);
		
		for (int k=0;k<rows;k++)
			x[k][i] = b[k];
	}

	/// ���߂�ׂ��t�s��
	return x;
}

/**
 *	����
 *		�ďo���̋t�s���Ԃ�
 */
const Matrix Matrix::inversion(void) const
{
	/// �����s�񂩂ǂ����̃`�F�b�N
	if (!this->isSquare())
	{
		cerr << "Error: [Matrix::inverse] Not square matrix\n" << endl;
		abort();
	}

	Matrix lu(*this);						/// �W���s��
	Matrix x(this->rows, this->columns);	/// ���߂�t�s��
	Vector indx(rows);						/// �s�������L�^�����x�N�g��
	Vector b(rows);
	double d;								///�@�s�����̉񐔂�����=1�C�s�����̉񐔂��=-1

	/// �s���1�񂾂�LU��������
	luDecomposition( (*this), lu, indx, &d );
	
	/// �t�s���񂲂Ƃɋ��߂�
	for (int i=0;i<rows;i++)
	{
		for (int j=0;j<rows;j++)
			b[j] = 0.0;
		
		b[i] = 1.0;
		luBackSubstitution(lu, indx, b);
		
		for (int k=0;k<rows;k++)
			x[k][i] = b[k];
	}

	/// ���߂�ׂ��t�s��
	return x;
}

/**
 *	����
 *		�s�񎮂�Ԃ�
 */
double Matrix::determinant(void)
{
	/// �����s�񂩂ǂ����̃`�F�b�N
	if (!this->isSquare())
	{
		cerr << "Error: [Matrix::determinant] Not square matrix\n" << endl;
		abort();
	}

	Matrix lu(this->rows, this->columns);
	Vector indx(this->rows);

	double det;

	if (luDecomposition(lu, (*this), indx, &det) == 0)
		return 0.0;

	for (int i=0;i<this->rows;i++)
		det *= lu[i][i];

  return det;
}

/**
 *	����
 *		�s�񎮂�Ԃ�
 */
const double Matrix::determinant(void) const
{
	/// �����s�񂩂ǂ����̃`�F�b�N
	if (!this->isSquare())
	{
		cerr << "Error: [Matrix::determinant] Not square matrix\n" << endl;
		abort();
	}

	Matrix lu(this->rows, this->columns);
	Vector indx(this->rows);

	double det;

	if (luDecomposition((*this), lu, indx, &det) == 0)
		return 0.0;

	for (int i=0;i<this->rows;i++)
		det *= lu[i][i];

  return det;
}

/**
 *	����
 *		�s��̗v�f���R���\�[���ɏ����o��
 */
/// �S�Ă̗v�f�������o��
void Matrix::show(void) const
{
	for (int i=0;i<this->rows;i++)
	{
		for (int j=0;j<this->columns;j++)
		{
			if ((*this)[i][j] < 0.0)		/// ���̏ꍇ
			{
				cout.precision(5);
				cout.setf(ios_base::fixed, ios_base::floatfield);
				cout << "-" << -(*this)[i][j];
		
			}
			else if ((*this)(i,j) == 0.0)	/// 0�̏ꍇ
			{
				cout.precision(5);
				cout.setf(ios_base::fixed, ios_base::floatfield);
				cout << 0.0;
			}
			else								/// ���̏ꍇ
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

/// �S�Ă̗v�f���w���`���ŏ����o��
void Matrix::showExp(void) const
{
	for (int i=0;i<this->rows;i++)
	{
		for (int j=0;j<this->columns;j++)
		{
			if ((*this)[i][j] < 0.0)		/// ���̏ꍇ
			{
				cout.precision(5);
				cout.setf(ios_base::scientific, ios_base::floatfield);
				cout << "-" << -(*this)[i][j];
		
			}
			else if ((*this)(i,j) == 0.0)	/// 0�̏ꍇ
			{
				cout.precision(5);
				cout.setf(ios_base::scientific, ios_base::floatfield);
				cout << 0.0;
			}
			else								/// ���̏ꍇ
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
 *	����
 *		�s��̑̊i�����̘a�����߂�
 */
double Matrix::trace(void)
{
	/// �����s�񂩂ǂ����̃`�F�b�N
	if (!this->isSquare())
	{
		cerr << "Error: [Matrix::trace] Not square matrix\n" << endl;
		abort();
	}
    
	double res		= 0;
	double tmp		= 0;		/// �v�Z�p
	double trace	= 0;		/// �Ίp�����̘a
	
	for (int i=0;i<this->rows;i++)
	{
		res		+= (*this)[i][i];
		tmp		= trace;
		trace	+= res;
		tmp		= trace - tmp;
		res		= res - tmp;
	}

	return trace;
}

/**
 *	����
 *		�s��̑̊i�����̘a�����߂�
 */
const double Matrix::trace(void) const
{
	/// �����s�񂩂ǂ����̃`�F�b�N
	if (!this->isSquare())
	{
		cerr << "Error: [Matrix::trace] Not square matrix\n" << endl;
		abort();
	}
    
	double res		= 0;
	double tmp		= 0;		/// �v�Z�p
	double trace	= 0;		/// �Ίp�����̘a
	
	for (int i=0;i<this->rows;i++)
	{
		res		+= (*this)[i][i];
		tmp		= trace;
		trace	+= res;
		tmp		= trace - tmp;
		res		= res - tmp;
	}

	return trace;
}

/**
 *	����
 *		�s�̌���
 *	����
 *		i: ��������s
 *		j: ���������s
 */
void Matrix::swapRows(int i, int j)
{
	/// �����`�F�b�N
	if (!(1 < i && i < rows) || !(1 < j && j < rows))
	{
		cerr << "Error: [Matrix::swapRows] Arguments are invalid\n" << endl;
		abort();
    }

	if (i == j) return;

    /// �s�x�N�g���̌���
	double *temp = elements[i].elements;
	elements[i].elements = elements[j].elements;
	elements[j].elements = temp;

	return;
}

/**
 *	����
 *		�[���t�s������߂�
 *		threshold�̒l�ɂ͒��ӂ��邱�Ɓi���������Ă��傫�����Ă��댯�j
 */
Matrix Matrix::pseudoInversion(double threshold)
{
	/// ���ْl�����̂��߂̍s��
	Matrix u;
	Matrix v;
	Matrix w;
	/// �[���t�s��
	Matrix pseudoInv;

	/**
	 *	�ȉ��œ��ْl�����ɗp����s��̃T�C�Y������
	 */
	/// �s�Ɨ�̐��ɂ���ďꍇ����
	if ( this->rows >= this->columns)		/// m>=n�̏ꍇ
	{
		u.setSize(this->rows, this->columns);
		u.loadMatrix(*this);
	}
	else		/// m<n�̏ꍇ
	{
		u.setSize(this->columns, this->rows);
		u.loadMatrix( this->transposition() );
	}
 
	v.setSize(u.columns, u.columns);
	w.setSize(this->columns, this->columns);
 
	pseudoInv.setSize(this->columns, this->rows);

	/// ���ْl�������s��
	this->singularValueDecomposition( (*this), u, w, v );
 
	for (int r = 0; r < w.rows; r++)
	{
		if ( w[r][r] < threshold)
			w(r, r) = 0.0;
		else
			w(r, r) = 1.0 / w[r][r];
  }

	/// �[���t�s������߂�
	pseudoInv = v * w * u.transpose();
	
	if ( this->rows > this->columns )
		return pseudoInv.transpose();

	return pseudoInv;
}

/**
 *	����
 *		�[���t�s������߂�
 *		threshold�̒l�ɂ͒��ӂ��邱�Ɓi���������Ă��傫�����Ă��댯�j
 */
const Matrix Matrix::pseudoInversion(double threshold) const
{
	/// ���ْl�����̂��߂̍s��
	Matrix u;
	Matrix v;
	Matrix w;
	/// �[���t�s��
	Matrix pseudoInv;

	/**
	 *	�ȉ��œ��ْl�����ɗp����s��̃T�C�Y������
	 */
	/// �s�Ɨ�̐��ɂ���ďꍇ����
	if ( this->rows >= this->columns)		/// m>=n�̏ꍇ
	{
		u.setSize(this->rows, this->columns);
		u.loadMatrix(*this);
	}
	else		/// m<n�̏ꍇ
	{
		u.setSize(this->columns, this->rows);
		u.loadMatrix( this->transposition() );
	}
 
	v.setSize(u.columns, u.columns);
	w.setSize(this->columns, this->columns);
 
	pseudoInv.setSize(this->columns, this->rows);

	/// ���ْl�������s��
	this->singularValueDecomposition( (*this), u, w, v );
 
	for (int r = 0; r < w.rows; r++)
	{
		if ( w[r][r] < threshold)
			w(r, r) = 0.0;
		else
			w(r, r) = 1.0 / w[r][r];
  }

	/// �[���t�s������߂�
	pseudoInv = v * w * u.transpose();
	
	if ( this->rows > this->columns )
		return pseudoInv.transpose();

	return pseudoInv;
}

/**
 *	����
 *		�������iwj�̂����ő�ƍŏ��̔�j
 *		ill-conditioned�i�������j�F���������傫������
 *	�ԋp�l
 *		������
 */
double Matrix::conditionNumber(void) const
{
	/// �����s�񂩂ǂ����̃`�F�b�N
	if (!this->isSquare())
	{
		cerr << "Error: [Matrix::conditionNumber] Not square matrix\n" << endl;
		return -1.0;
	}

	int m = this->rows; 
	int n = this->columns;
	
	/// ���ْl�����̂��߂̍s��
	Matrix u(m, n);
	Matrix v(n, n);
	Matrix w(n, n);

	/// ���ْl�������s��
	this->singularValueDecomposition((*this), u, w, v);

	double wmax,wmin;
	int i;
	
	for (wmin = wmax = fabs( w[0][0] ), i=2; i<=this->columns; i++)
	{
		if ( fabs( w[i-1][i-1] ) > wmax )
			wmax = fabs( w[i-1][i-1] );
		
		if ( fabs( w[i-1][i-1] ) < wmin )
			wmin = fabs( w[i-1][i-1] );
	}

	if (wmin <= 0.0)
		return -1.0;

	return log10(wmax/wmin); 
}

/**
 *	------------------------------------------------------------
 *		Matrix�N���X��private�ȃ����o�֐�
 *	------------------------------------------------------------
 */
/**
 *	����
 *		�s��I�u�W�F�N�g�̃������̈���m�ۂ���
 */
void Matrix::newMatrix(void)
{
    if (rows == 0 || columns == 0)
	{
        rows = 0;
        columns = 0;
        elements = 0;

        return;
    }

	/// �s�x�N�g���̃������̈�m��
    elements = new Vector[rows];
	
	/// new���Z�q�̐��ۊm�F
	if (elements == 0)
	{
		cerr << "Error: [Matrix::newMatrix] Memory is not allocated\n";
        abort();
    }

    for (int i=0;i<rows;i++)
        elements[i].setSize(columns);
}

/**
 *	����
 *		�s��I�u�W�F�N�g�̃������̈���������
 */
void Matrix::deleteMatrix(void)
{
	/// �s�x�N�g�������
    delete [] elements;

	/// �����o�ϐ�������
	rows = 0;
	columns = 0;
	elements = 0;
}

/**
 *	����
 *		LU�������s��
 *	����
 *		Matrix  a	: LU���������s�� 
 *		Matrix  lu	: LU������̍s��
 *		Matrix  indx: �����s�{�b�g�I���̍ۂ̍s�������L�^
 *		double* d	: �s�����̉񐔂�����=1�C�s�����̉񐔂��=-1
 *
 *		Reference :	William H. Press, et al, NUMERICAL RECIPES in C, 
 *						Cambridge University Press, 1988
 */
int Matrix::luDecomposition(const Matrix& a, Matrix& lu, Vector& indx, double* d) const
{
	int		i, imax=0, j, k ;
	double	big, dum, sum, temp;
	
	/// LU���������s��ŏ�����
	lu.loadMatrix(a);
	int n = a.getRow();
	Vector vv(n);		/// �Öق̃X�P�[�����O���L�^���邽�߂̂���

	*d = 1.0 ;			/// �܂��s���������Ă��Ȃ�
	
	/// �s�ɂ��ă��[�v���C�Öق̃X�P�[�����O�̏��擾
	for (i=1;i<=n;i++)
	{
		big = 0.0 ;
		
		for (j=1;j<=n;j++)
			if ((temp = fabs(lu[i-1][j-1])) > big)
				big = temp;

		/// �ő�̗v�f��0�Ȃ���ٍs��
		if (big == 0.0)
		{
			cerr <<  "Error: [Matrix::luDecomposition] Singular matrix occurs\n" << endl;
			return 0;
		}
		
		vv[i-1] = 1.0 / big ;
	}

	/// Crout�@�C��ɂ��Ẵ��[�v
	for (j=1;j<=n;j++)
	{
		for ( i=1; i<j; i++ )
		{
			sum = lu[i-1][j-1];
			
			for (k=1;k<i;k++)
				sum -= lu[i-1][k-1] * lu[k-1][j-1];
		  
			lu[i-1][j-1] = sum;
		}

		big = 0.0;		/// �ő�s�{�b�g�v�f��T������

		for (i=j;i<=n;i++)
		{
			sum = lu[i-1][j-1];
      
			for (k=1;k<j;k++)
				sum -= lu[i-1][k-1] * lu[k-1][j-1];
		
			lu[i-1][j-1] = sum;
      
			/// �s�{�b�g�̗ǂ��͍��܂ł̍ő�l�ȏォ�H
			if ((dum = vv[i-1] * fabs(sum)) >= big)
			{
				big  = dum;
				imax = i;
			}
		}

		/// �s�����̕K�v�����邩�H
		if (j != imax)
		{
			for (k=1;k<=n;k++)
			{
				dum = lu[imax-1][k-1];
				lu[imax-1][k-1] = lu[j-1][k-1];
				lu[j-1][k-1] = dum;
			}
      
			*d = -(*d);		/// d�̋���ς���
      
			vv[imax-1] = vv[j-1];		/// �X�P�[�����q����������
		}
    
		indx[j-1] = imax;

		/**
		 *		�s�{�b�g�v�f��0�Ȃ�s��͓��قł���
		 *		���ٍs��̗��p�@�ɂ���Ă�0�ɂ��邱�Ƃ��ǂ����Ƃ�����
		 */
		if (lu[j-1][j-1] == 0.0)
			lu[j-1][j-1] = MATRIX_TINY;
		
		/// �s�{�b�g��v�f�Ŋ���
		if (j != n)
		{
			dum = 1.0 / lu[j-1][j-1];
		
			for (i=j+1;i<=n;i++)
				lu[i-1][j-1] *= dum;
		}
	}	/// Crout�@�C��ɂ��Ẵ��[�v -> ���̗�Ɉڂ�

	/// ����I��
	return 1;
}

/**
 *	����
 *		��ޑ����p���ĘA��1��������������
 *	����
 *		Matrix  a	: LU�����ŋ��߂�A��LU�����i�s�ρj
 *		Matrix  indx: LU�����̍ۂ̕����s�{�b�g�I���̍s�������
 *		double* d	: �s�����̉񐔂�����=1�C�s�����̉񐔂��=-1
 *
 *		Reference :	William H. Press, et al, NUMERICAL RECIPES in C, 
 *						Cambridge University Press, 1988
 */
void Matrix::luBackSubstitution(const Matrix& a, const Vector& indx, Vector& b) const
{
	int    i, ii=0, ip, j;
	double sum;
	int n = a.getRow();

	/**
	 *		ii�����Ȃ�C�����b�̍ŏ���0�łȂ��v�f�̓Y�����ł���D
	 *		�s���������ɖ߂��Ȃ���O�i������s��
	 */
	for (i=1;i<=n;i++)
	{
		ip    = (int)indx[i-1];
		sum   = b[ip-1];
		b[ip-1] = b[i-1];
		
		if (ii)
		{
			for (j=ii;j<=i-1;j++)
				sum -= a[i-1][j-1] * b[j-1];
		}
		else if (sum)		/// ���v�f�ɏo������B����ȍ~�͏�̃��[�v�Řa�����߂Ȃ���΂Ȃ�Ȃ�
			ii = i;
		
		b[i-1] = sum;
	}
  
	/// ��ޑ���J�n
	for (i=n;i>=1;i--)
	{
		sum = b[i-1];
    
		for (j=i+1;j<=n;j++) 
			sum -= a[i-1][j-1] * b[j-1];

		/// ���x�N�g��x�̐������㏑���ۑ�
		b[i-1] = sum / a[i-1][i-1];
	}	/// ��@�I��
}

/**
 *	����
 *		�s��̓��ْl�����iSingular Value Decomposition�j
 *	����
 *		Matrix  a	: ���ْl���������s��
 *		Matrix  u	: �����s��
 *		Matrix  v	: ���ْl�̑Ίp�s��
 *		Matrix  w	: �E���s��
 *		double* d	: �s�����̉񐔂�����=1�C�s�����̉񐔂��=-1
 *
 *		Reference :	William H. Press, et al, NUMERICAL RECIPES in C, 
 *						Cambridge University Press, 1988
 */
void Matrix::singularValueDecomposition(const Matrix& a, Matrix& u, Matrix& w, Matrix& v) const
{
	if ( !( a.rows == u.rows && a.columns == u.columns) )
	{
		cerr << "Error: [Matrix::singularValueDecomposition] Matrix type is unmatched\n" << endl;
		return;
	}

	/// �ΏۂƂ���s����R�s�[
	u.loadMatrix(a);

	int flag, i, its, j, jj, k, nm, l, n, m;
	double anorm,c,f,g,h,s,scale,x,y,z;
	double maxarg1, maxarg2;
	double iminarg1, iminarg2;

	m	= u.rows;
	n	= u.columns;
	
	Vector rv1(n);

	g = scale = anorm = 0.0;

	/// Householder�@��2�d�Ίp�̌`�ɒ���
  	for (i=1; i<=n; i++)
	{
		l=i+1;
		
		rv1[i-1] = scale*g;
		g = s = scale = 0.0;
    
		if (i <= m)
		{
			for (k=i; k<=m; k++)
				scale += fabs( u[k-1][i-1] );
      
			if (scale)
			{
				for (k=i; k<=m; k++)
				{
					u[k-1][i-1] /= scale;
					s += u[k-1][i-1] * u[k-1][i-1];
				}
				
				f = u[i-1][i-1];
				g = - ( f>=0.0 ? fabs( sqrt(s) ) : -fabs( sqrt(s) ) );
				h = f*g - s;
				u[i-1][i-1] = f-g;
				
				for (j=l; j<=n; j++)
				{
					for (s=0.0, k=i; k<=m; k++)
						s += u[k-1][i-1] * u[k-1][j-1];
					
					f=s/h;
	  
					for (k=i; k<=m; k++)
						u[k-1][j-1] += f * u[k-1][i-1];
				}

				for (k=i; k<=m; k++)
					u[k-1][i-1] *= scale;
			}
		}
    
		w[i-1][i-1] = scale*g;
		g=s=scale=0.0;
    
		if (i<=m && i!=n)
		{
			for (k=l; k<=n; k++)
				scale += fabs( u[i-1][k-1] );
      
			if (scale)
			{
				for (k=l; k<=n; k++)
				{
					u[i-1][k-1] /= scale;
					s += u[i-1][k-1] * u[i][k];
				}
	
				f = u[i-1][l-1];
				g = - ( f>=0.0 ? fabs( sqrt(s) ) : -fabs( sqrt(s) ) );
				h = f*g-s;
				u[i-1][l-1] = f - g;
				
				for (k=l; k<=n; k++)
				{
					rv1[k-1] = u[i-1][k-1] / h;
				}
				
				for (j=l; j<=m; j++)
				{
					for (s=0.0, k=l; k<=n; k++)
						s += u[j-1][k-1] * u[i-1][k-1];
					
					for (k=l; k<=n; k++)
						u[j-1][k-1] += s * rv1[k-1];
				}

				for (k=l; k<=n; k++)
					u[i-1][k-1] *= scale;
			}
		}
		
		maxarg1 = anorm;
		maxarg2 = fabs( w[i-1][i-1] )+fabs( rv1[i-1] );
		anorm = ( maxarg1>maxarg2 ? maxarg1 : maxarg2 );
	}	/// �����܂�Householder�@

	/// �E���̕ϊ��̗ݎZ
	for (i=n; i>=1; i--)
	{
		if (i<n)
		{
			if (g)
			{
				for (j=l; j<=n; j++)
					v[j-1][i-1] = ( u[i-1][j-1] / u[i-1][l-1] ) / g;	/// �A���_�[�t���[������邽�߁C2�x���肷��
				
				for (j=l; j<=n; j++)
				{
					for (s=0.0, k=l; k<=n; k++)
						s += u[i-1][k-1] * v[k-1][j-1];
					
					for (k=l; k<=n; k++)
						v[k-1][j-1] += s*v[k-1][i-1];
				}
			}

			for (j=l; j<=n; j++)
				v[i-1][j-1] = v[j-1][i-1] = 0.0;
		}

		v[i-1][i-1] = 1.0;
		g = rv1[i-1];
		l = i;
	}

	iminarg1 = m;
	iminarg2 = n;

	/// �����̕ϊ��̗ݎZ
	for (i=(m<n ? m:n); i>=1; i--)
	{
		l=i+1;
		g=w[i-1][i-1];

		for (j=l; j<=n; j++)
			u[i-1][j-1] = 0.0;
		
		if (g)
		{
			g=1.0/g;

			for (j=l; j<=n; j++)
			{
				for (s=0.0,k=l; k<=m; k++)
					s += u[k-1][i-1] * u[k-1][j-1];
				
				f = ( s / u[i-1][i-1] ) * g;
				
				for (k=i; k<=m; k++)
					u[k-1][j-1] += f * u[k-1][i-1];
			}
			
			for (j=i; j<=m; j++)
				u[j-1][i-1] *= g;
		}
		else
		{
			for (j=i; j<=m; j++)
				u[j-1][i-1] = 0.0;
		}
		u[i-1][i-1] += 1.0;
	}
	
	/// 2�d�Ίp�s��̑Ίp���F���ْl�ɂ��Ẵ��[�v�Ɣ����v�Z�̃��[�v
	for (k=n; k>=1; k--)
	{
		for (its=1; its<=30; its++)
		{
			flag = 1;
			
			for (l=k; l>=1; l--)	/// �����̃`�F�b�N
			{
				nm = l - 1;			/// rv1(1)�͏��0
				if ( ( fabs( rv1[l-1] ) + anorm ) == anorm )
				{
					flag = 0;
					break;
				}
				
				if ( (fabs( w[nm-1][nm-1] ) + anorm ) == anorm)
					break;
			}
			
			if (flag)		/// l > 1�̂Ƃ�rv1(l)������
			{
				c = 0.0;
				s = 1.0;

				for (i=l; i<=k; i++)
				{
					f = s * rv1[i-1];
					rv1[i-1] = c*rv1[i-1];
					
					if ( ( fabs( f )+anorm ) == anorm )
						break;

					g = w[i-1][i-1];
					h = pythagorean(f, g);
					w[i-1][i-1] = h;
					h = 1.0 / h;	
					c = g*h;
					s = -f*h;
					
					for (j=1; j<=m; j++)
					{
						y = u[j-1][nm-1];
						z = u[j-1][i-1];
						u[j-1][nm-1] = y*c + z*s;
						u[j-1][i-1]  = z*c - y*s;
					}
				}
			}

			z = w[k-1][k-1];
			
			if (l == k)			/// ��������
			{
				if (z < 0.0)		/// ���ْl��񕉂ɂ���
				{
					w[k-1][k-1] = -z;
					
					for (j=1; j<=n; j++)
						v[j-1][k-1] = -v[j-1][k-1];
				}
				break;
			}

			/// ��30��J��Ԃ��Ă��������Ȃ�����
			if (its == 30)
			{
				cerr << "Error: [Matrix::singularValueDecomposition] No convergence in 30 svdcmp iterations\n" << endl;
				return;
			}
			
			x = w[l-1][l-1];		/// �ŉ�����2�s2��̏��s�񎮂���̃V�t�g
			nm = k - 1;
			y = w[nm-1][nm-1];
			g = rv1[nm-1];
			h = rv1[k-1];
			f = ( (y-z)*(y+z)+(g-h)*(g+h) ) / (2.0*h*y);
			g = pythagorean(f, 1.0);
			f = ( (x-z)*(x+z)+h*( ( y / ( f + ( f>=0.0 ? fabs(g) : -fabs(g) ) ) ) - h ) ) / x;
			c = s = 1.0;			/// ����QR�ϊ�
			
			for (j=l; j<=nm; j++)
			{
				i = j + 1;
				g = rv1[i-1];
				y = w[i-1][i-1];
				h = s*g;
				g = c*g;
				z = pythagorean(f, h);
				rv1[j-1] = z;
				c = f/z;
				s = h/z;
				f = x*c + g*s;
				g = g*c - x*s;
				h = y*s;
				y *= c;

				for (jj=1; jj<=n; jj++)
				{
					x = v[jj-1][j-1];
					z = v[jj-1][i-1];
					v[jj-1][j-1] = x*c + z*s;
					v[jj-1][i-1] = z*c - x*s;
				}
				
				z = pythagorean(f, h);
				w[j-1][j-1] = z;			/// z=0�Ȃ��]�͔C��
				
				if (z)
				{
					z = 1.0/z;
					c = f*z;
					s = h*z;
				}
				
				f = c*g+s*y;
				x = c*y - s*g;

				for (jj=1; jj<=m; jj++)
				{
					y = u[jj-1][j-1];
					z = u[jj-1][i-1];
					
					u[jj-1][j-1] = y*c + z*s;
					u[jj-1][i-1] = z*c - y*s;
				}
			}
			
			rv1[l-1]	= 0.0;
			rv1[k-1]	= f;
			w[k-1][k-1]	= x;
	}
}

	/// �v�Z�p�̍s��
	Matrix uu(u);
	Matrix vv(v);
	Vector no(a.columns);
	
	for (int num=1; num<=w.columns; num++)
		no[num-1] = (double)num;

	quickSort( w, no, 1, w.columns );

	for (int num=1; num<=w.columns; num++)
	{
		for (int i=1; i<=uu.rows; i++)
			uu[i-1][num-1] = u[i-1][(int)no[num-1]-1];
		
		for(int j=1; j<=vv.rows; j++)
			vv[j-1][num-1] = v[j-1][(int)no[num-1]-1];
	}

	return;
}

/**
 *	����
 *		�O�����̒藝�i�A���_�[/�I�[�o�[�t���[��h���j
 *	����
 *		�idouble�j: a, b
 *	�߂�l
 *		sqrt(a*a + b*b)
 *�@
 *		Reference : William H. Press, et al, NUMERICAL RECIPES in C, 
 *					Cambridge University Press, 1988
 */
double Matrix::pythagorean(double a, double b) const
{
	double absa=fabs(a);
	double absb=fabs(b);
	double sqrarg;

	if ( absa > absb )
	{
		sqrarg = absb/absa;
		/// �߂�l
		return absa*sqrt( 1.0+(sqrarg==0.0?0.0:sqrarg*sqrarg) );
	}
	else
	{
		sqrarg = absa/absb;
		/// �߂�l
		return ( absb==0.0 ? 0.0 : absb*sqrt( 1.0+(sqrarg==0.0?0.0:sqrarg*sqrarg) ) );
	}
}

/**
 *	����
 *		�N�C�b�N�\�[�g
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
			if ( w[lp-1][lp-1] < w[hp-1][hp-1] )
			{
				w_tmp = w[lp-1][lp-1];
				no_tmp = no[lp-1];
				w[lp-1][lp-1] = w[hp-1][hp-1];
				no[lp-1] = no[hp-1];
				w[hp-1][hp-1] = w_tmp; 
				no[hp-1] = no_tmp;
				
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

		quickSort(w, no, low, mid-1);

		quickSort(w, no, mid+1, high);
	}
}

/**
 *	----------------------------------------------------------------------
 *		Vector�N���X
 *	----------------------------------------------------------------------
 */

/**
 *	------------------------------------------------------------
 *		�t�����h�֐�
 *	------------------------------------------------------------
 */
/**
 *	����
 *		�N���XVector�̑��d��`���ꂽ�X�g���[���}�����Z�q
 *		�x�N�g���̑S�v�f�̒l���o�͂���
 *	����
 *		output: �o�͐�X�g���[��
 *		vector: �ړI�̍s��
 */
ostream& operator<<(ostream& output, const Vector& vector)
{
	/// �Ȋw�\�L�@
    output.setf(ios::scientific); 
   
	for (int i = 0; i < vector.dimensions; i++)
	{
        output << setw(15) << vector.elements[i];
        
		if ( !( (i+1) % 5 ) )
			output << endl;
    }

    if ( vector.dimensions % 5 )
		output << endl;

	/// cout << x << y�Ə�����悤�ɂ���
    return output; 
}

/**
 *	����
 *		�N���XVector�̑��d��`���ꂽ�X�g���[���}�����Z�q
 *		�x�N�g���̑S�v�f�̒l����͂���
 *	����
 *		input: ���͐�X�g���[��
 *		vector: �ړI�̍s��
 */
istream& operator>>(istream& input, Vector& vector)
{
	cout << "Vector (" << vector.dimensions
			<< "columns ): Input all elements separated by space" << endl;

    for (int i = 0; i < vector.dimensions; i++)
        input >> vector.elements[i];

	/// cin >> x >> y�Ə�����悤�ɂ���
    return input;
}

/**
 *	����
 *		2�̃x�N�g�������������ǂ������`�F�b�N
 *	����
 *		true: ������
 *		false: �������Ȃ�
 */
bool operator==(const Vector& left, const Vector& right)
{
    if ( left.dimensions != right.dimensions )
        return false;	/// �x�N�g���̃T�C�Y���قȂ�

    for (int i = 0; i < left.dimensions; i++)
	{
        if( fabs( left.elements[i] - right.elements[i] ) > NEAR_ZERO )
            return false;	/// �x�N�g���̓��e���قȂ�
	}

    return true; //�Q�̃x�N�g���͓�����
}

/**
 *	����
 *		2�̃x�N�g�����قȂ��Ă��邩�ǂ������`�F�b�N
 *	����
 *		true: �قȂ��Ă���
 *		false: ������
 */
bool operator!=(const Vector& left, const Vector& right)
{
    if(left == right)
        return false;	///�Q�̃x�N�g���͓�����
    else
        return true;		///�Q�̃x�N�g���͈قȂ�
}

/// ���d��`���ꂽ���Z�q�����g��������
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
 *		Vector�N���X�̃����o�֐���`
 *	------------------------------------------------------------
 */
/**
 *	----------------------------------------
 *	�R���X�g���N�^�ƃf�X�g���N�^
 *	----------------------------------------
 */
/// �N���XVector�̃f�t�H���g�R���X�g���N�^
Vector::Vector(int dim) : dimensions(dim)
{
	/// �x�N�g���I�u�W�F�N�g�̃������̈�m��
    newVector();	

    for (int i = 0; i < dimensions; i++)
		elements[i] = 0.0;
}

/// �N���XVector�̃R�s�[�R���X�g���N�^
Vector::Vector(const Vector& vector) : dimensions(vector.dimensions)
{
	/// �x�N�g���I�u�W�F�N�g�̃������̈�m��
    newVector();

    for(int i = 0; i < dimensions; i++)
        elements[i] = vector.elements[i];	/// �x�N�g���v�f���R�s�[����
}

/// �N���XVector�̃R���X�g���N�^�i�z��ŏ������j
Vector::Vector(const double *ptrArray, int dim) : dimensions(dim)
{
	/// �x�N�g���I�u�W�F�N�g�̃������̈�m��
    newVector();

    for(int i = 0; i < dimensions; i++)
        elements[i] = ptrArray[i];
}

/// �N���XVector�̃f�X�g���N�^
Vector::~Vector()
{
    deleteVector();
}

/// �x�N�g���̃T�C�Y���Đݒ肷��
void Vector::setSize(int dim)
{
	/// �x�N�g���I�u�W�F�N�g�̃������̈���
    deleteVector();

	dimensions = dim;
	/// �x�N�g���I�u�W�F�N�g�̃������̈�m��
    newVector();
    
	for (int i = 0; i < dimensions; i++)
		elements[i] = 0.0; /// �v�f��0.0�ɂ���
}

/**
 *	----------------------------------------
 *	���Z�q�̑��d��`
 *	----------------------------------------
 */
/// ������Z�q
/**
 *		dim: ������
 */
Vector& Vector::operator=(const Vector& right)
{
	/// ���ȑ�����`�F�b�N����
    if (this != &right)
	{

        if( dimensions != right.dimensions)
		{
			cerr << "Error: [Vector::operator=] Invalid argument (size different)" << endl;
            abort();
        }

		/// �x�N�g���̗v�f���R�s�[����
        for(int i = 0; i < dimensions; i++)
			elements[i] = right.elements[i];
    }

	///  x = y = z�Ə�����悤�ɂ���
    return *this; 
}

/**
 *		��\�I�ȑg�ݍ��킹���Z�q
 */
/// ���d��`���ꂽ�������Z�q
Vector& Vector::operator*=(double schalor)
{
    for (int i = 0; i < dimensions; i++) 
		elements[i] *= schalor;

    return *this;
}

/// ���d��`���ꂽ/�����Z�q
Vector& Vector::operator/=(double schalor)
{
	/// 0���h�~
    if ( fabs(schalor) < NEAR_ZERO)
	{
		std::cerr << "Error: [Vector::operator/=] Division by zero\n" << std::endl;
        abort();
    }

    for (int i = 0; i < dimensions; i++)
		elements[i] /= schalor;

    return *this;
}

/// ���d��`���ꂽ�{�����Z�q
Vector& Vector::operator+=(const Vector& right)
{
	/// �T�C�Y�̃`�F�b�N
    if (dimensions != right.dimensions)
	{	
		std::cerr << "Error: [Vector::operator+=] Unmatched vector size\n" << std::endl;
        abort();
    }

    for (int i = 0; i < dimensions; i++)
        elements[i] += right.elements[i];

    return *this;
}

/// ���d��`���ꂽ-�����Z�q
Vector& Vector::operator-=(const Vector& right)
{
	/// �T�C�Y�̃`�F�b�N
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
 *		��\�I�Ȋ֐�
 */
/// �m�����̌v�Z
double Vector::norm(void) const
{
    double a = 0.0;

	for (int i = 0; i < dimensions; i++)
        a += elements[i]*elements[i];

    return sqrt( a );
}

/// �x�N�g�����K�i���i�I�u�W�F�N�g���g���K�i�������j
const Vector& Vector::normalize(void)
{
    double a = norm();
    
	if (a < NEAR_ZERO) 
		return *this; /// �m�����[���̃x�N�g��
    
	for (int i = 0; i < dimensions; i++)
        elements[i] /= a;

    return *this;
}

/// �Ăяo�����x�N�g���̒P�ʃx�N�g����Ԃ�
Vector Vector::normalization(void)
{
	Vector unit(*this);

	unit.normalize();
	return unit;
}

/// �Ăяo�����x�N�g���̒P�ʃx�N�g����Ԃ�
const Vector Vector::normalization(void) const
{
	Vector unit(*this);

	unit.normalize();
	return unit;
}

/// �����v�f������
void Vector::cleanUp(void)
{
	int i;
	double max = 0.0;

	for (i = 0; i < dimensions; i++)
		if ( fabs(elements[i] ) > max )
			max = fabs(elements[i]);

	if ( max > NEAR_ZERO )
		for (i = 0; i < dimensions; i++)
			if ( fabs(elements[i]) / max < ZERO_ERROR_MARGIN )
                elements[i] = 0.0;

	return;
}

/**
 *	------------------------------------------------------------
 *		Vector�N���X��private�ȃ����o�֐�
 *	------------------------------------------------------------
 */
/// �x�N�g���̗̈�m��
void Vector::newVector(void)
{
	/// ���������O�̏ꍇ�|�C���^���k���ɃZ�b�g
    if (dimensions == 0)
	{ 
        elements = 0;
        return;
    }

	/// �x�N�g���̃������[�̈���m�ۂ���
	elements = new double[dimensions];
    
	if (dimensions == 0)
	{
		cerr << "Error: [Vector::newVector] Memory is not allocated\n" << endl;
        abort();
    }
}

/// �x�N�g���̗̈���
void Vector::deleteVector(void)
{
	///�x�N�g���̃������[�̈���J������
    delete [] elements;
}

} /// end of namespace mathematics