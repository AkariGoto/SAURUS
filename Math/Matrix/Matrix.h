/**
 *  �t�@�C����
 *		Matrix.h
 *  ����
 *		���Z�q�⃁�\�b�h�ȂǍs��̎�ȉ��Z�̒�`
 *  ���t
 *		�쐬��: 2007/01/15(MON)		�X�V��: 2007/03/06(TUE)
 */


#ifndef __Matrix_h__	/// 2�d�C���N���[�h���
#define __Matrix_h__	/// 2�d�C���N���[�h�֎~

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <cassert>

/**
 *	----------------------------------------------------------------------
 *		�萔�A�}�N���錾
 *	----------------------------------------------------------------------
 */
/// �[���Ƃ݂Ȃ���
const double NEAR_ZERO = 1.0E-16;
/// �[���Ƃ̑��Ό덷
const double ZERO_ERROR_MARGIN = 1.0E-8;
/// �}�V���덷
const double MATRIX_TINY = 1.0E-20;

namespace Math
{
/**
 *	----------------------------------------------------------------------
 *		�N���X�̑O���錾
 *	----------------------------------------------------------------------
 */
class Matrix;
class Vector;

/**
 *	----------------------------------------------------------------------
 *		��ʓI�ȎZ�p���Z�q
 *	----------------------------------------------------------------------
 */
	/// �x�N�g���̘a
	const Vector operator+(const Vector& left, const Vector& right);
	/// �x�N�g���̍�
	const Vector operator-(const Vector& left, const Vector& right);
	/// �x�N�g���̃X�J���{
	const Vector operator*(double schalor, const Vector& right);
	/// �x�N�g���̃X�J���{
	const Vector operator*(const Vector& left, double schalor);
	/// �x�N�g���������Ŋ���Z
	const Vector operator/(const Vector& left, double schalor);

	/// �s��̘a
	const Matrix operator+(const Matrix& left, const Matrix& right);
	/// �s��̍�
	const Matrix operator-(const Matrix& left, const Matrix& right);

/**
 *	----------------------------------------------------------------------
 *		Matrix�N���X
 *	----------------------------------------------------------------------
 */
class Matrix
{
/**
 *	------------------------------------------------------------
 *		�����o�ϐ�
 *	------------------------------------------------------------
 */
private:
	/// �s�v�f��
	int rows;
	/// ��v�f��
	int columns;

	/// �s�x�N�g���ւ̃|�C���^�i�z��j
	Vector* elements;

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
	friend std::ostream& operator<<(std::ostream& output, const Matrix& matrix);
	/**
	 *	����
	 *		�N���XMatrix�̑��d��`���ꂽ�X�g���[���}�����Z�q
	 *		�s��̑S�v�f�̒l����͂���
	 *	����
	 *		output: ���͐�X�g���[��
	 *		matrix: �ړI�̍s��
	*/
	friend std::istream& operator>>(std::istream& input, Matrix& matrix);

	/**
	 *	����
	 *		2�̍s�񂪓��������ǂ������`�F�b�N
	 *	����
	 *		true: ������
	 *		false: �������Ȃ�
	 */
	friend bool operator==(const Matrix& left, const Matrix& right);
    
	/**
	 *	����
	 *		2�̍s�񂪈قȂ��Ă��邩�ǂ������`�F�b�N
	 *	����
	 *		true: �قȂ��Ă���
	 *		false: ������
	 */
	friend bool operator!=(const Matrix& left, const Matrix& right);

	/**
	 *		��ʓI�ȎZ�p���Z�q
	 */
	/// �s�񁖍s��
	friend const Matrix operator*(const Matrix& left, const Matrix& right);
    
	/// �s�񁖃x�N�g��
	friend const Vector operator*(const Matrix& matrix, const Vector& vector);
    
	/// �x�N�g�����s��
	friend const Vector operator*(const Vector& vector, const Matrix& matrix);

/**
 *	------------------------------------------------------------
 *		�����o�֐�
 *	------------------------------------------------------------
 */
public:
/**
 *	----------------------------------------
 *	�R���X�g���N�^�ƃf�X�g���N�^
 *	----------------------------------------
 */
	/**
	 *	����
	 *		�v�f���w�肷��R���X�g���N�^
	 *	����
	 *		row: �s��
	 *		col:  ��
	 */
	explicit Matrix(int row = 0, int col = 0);
	
	/// �R�s�[�R���X�g���N�^
	Matrix(const Matrix& matrix);
	
	/// �f�X�g���N�^
	virtual ~Matrix();

/**
 *	----------------------------------------
 *	�s��̗v�f������܂��͒��ׂ�֐��Q
 *	----------------------------------------
 */
	/**
	 *	����
	 *		�s��̌^��ݒ肷��
	 *	����
	 *		row: �s��
	 *		col:  ��
	 */
    void setSize(int row, int col);

	/**
	 *	����
	 *		�s��̗v�f�𒲂ׂ�
	 *		getRow: �s��̍s��
	 *		getCol:  �s��̗�
	 */
	int getRow(void) const { return rows;}
	int getRow(void) { return rows;}
	int getColumn(void) const { return columns;}
	int getColumn(void) { return columns;}
	
	/**
	 *	����
	 *		�s�񂪐����s��ł��邩�ǂ���
	 *	����
	 *		��0: �����s�� 
	 *		0: �����s��łȂ�
	 */
	int isSquare(void) const {return ( rows == columns );}

	/**
	 *	����
	 *		�s�񂪃x�N�g���ł��邩�ǂ���
	 *	����
	 *		��0: �����s�� 
	 *		0: �����s��łȂ�
	 */
	int isOneColumn(void) const {return ( columns == 1 );}

	/**
	 *	����
	 *		�����v�f�̏���
	 */
	void cleanUp(void);

/**
 *	----------------------------------------
 *	�s�񉉎Z�̂��߂̉��Z�q
 *	----------------------------------------
 */
	/// ������Z�q
	virtual Matrix& operator=(const Matrix& right);

	/**
	 *	����
	 *		�Y�������Z�q
	 *	����
	 *		row: �s��̍s�ԍ�
	 *		col:  �s��̗�ԍ�
	 */
	/// ���[�U�p�i�����`�F�b�N�L�j
	double& operator()(int row=1, int col=1);
	/// �v�Z�p[�s�x�N�g�������o��]�i�����`�F�b�N���C�����v�Z�p�j
	Vector& operator[] (int index);

	/**
	 *	����
	 *		const�I�u�W�F�N�g�ɑ΂���Y�������Z�q
	 *	����
	 *		row: �s��̍s�ԍ�
	 *		col:  �s��̗�ԍ�
	 */
	/// ���[�U�p�i�����`�F�b�N�L�j
	const double& operator()(int row=1, int col=1) const;
	/// �v�Z�p[�s�x�N�g�������o��]�i�����`�F�b�N���C�����v�Z�p�j
	const Vector& operator[] (int index) const;


	/**
	 *	��\�I�ȑg�ݍ��킹���Z�q
	 */
    Matrix& operator+=(const Matrix& right);
    Matrix& operator-=(const Matrix& right);
    Matrix& operator*=(const Matrix& right);

	
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
	virtual Matrix& loadMatrix(int startRow, int startCol, const Matrix& source, int firstRow, int firstCol, int lastRow, int lastCol);
	/**
	 *	����
	 *		�ǂݍ��񂾍s����R�s�[���̎n�_�ƏI�_���w�肵�ăR�s�[����
	 *		�R�s�[��̍s���1�s1��ڂ���R�s�[����
	 *	����
	 *		source: �R�s�[���̍s��
	 *		firstRow, firstCol: �R�s�[���̍s��̎n�_
	 *		lastRow, lastCol: �R�s�[���̍s��̏I�_
	 */
	virtual Matrix& loadMatrix(const Matrix& source, int firstRow, int firstCol, int lastRow, int lastCol);
	/**
	 *	����
	 *		�ǂݍ��񂾍s����R�s�[��̎n�_�ƏI�_���w�肵�ăR�s�[����
	 *		�R�s�[���̍s���1�s1��ڂ���R�s�[����
	 *	����
	 *		startRow, startCol: �R�s�[��̍s��̃R�s�[�J�n�_
	 *		source: �R�s�[���̍s��
	 */
	virtual Matrix& loadMatrix(int startRow, int startCol, const Matrix& source);
	/**
	 *	����
	 *		�ǂݍ��񂾍s����R�s�[���̎n�_���w�肵�ăR�s�[����
	 *		�R�s�[��̍s���1�s1��ڂ���R�s�[���� 
	 *	����
	 *		source: �R�s�[���̍s��
	 *		firstRow, firstCol: �R�s�[���̍s��̎n�_
	 */	
	virtual Matrix& loadMatrix(const Matrix& source, int firstRow, int firstCol);
	/**
	 *	����
	 *		�ǂݍ��񂾍s����R�s�[����
	 *		�R�s�[��̍s���1�s1��ڂ���R�s�[���� 
	 *		�R�s�[���̍s���1�s1��ڂ���R�s�[����
	 *	����
	 *		source: �R�s�[���̍s��
	 */
	virtual Matrix& loadMatrix(const Matrix& source);

/**
 *	----------------------------------------
 *	�s�����ʂȍs��ɂ���
 *	----------------------------------------
 */
	/**
	 *	����
	 *		��s���ǂݍ���
	 */
	virtual Matrix& loadZero(void);
	/**
	 *	����
	 *		�P�ʍs���ǂݍ���
	 */
	virtual Matrix& loadIdentity(void);

/**
 *	----------------------------------------
 *	�s��̎�ȉ��Z
 *	----------------------------------------
 */
/**
 *	�]�u�s��
 */
	/**
	 *	����
	 *		�ďo���̍s���]�u�s��ɂ���
	 */
	Matrix& transpose(void);

	/**
	 *	����
	 *		�Ăяo�����s��̓]�u�s���Ԃ�
	 */
	Matrix transposition(void);
	const Matrix transposition(void) const;

/**
 *	�t�s��
 */
	/**
	 *	����
	 *		�ďo���̍s����t�s��ɂ���
	 */
	Matrix& inverse(void);

	/**
	 *	����
	 *		�ďo���̋t�s���Ԃ�
	 */
	Matrix inversion(void);
	const Matrix inversion(void) const;

	/**
	 *	����
	 *		�s�񎮂�Ԃ�
	 */
	double determinant(void);
	const double determinant(void) const;
	
	/**
	 *	����
	 *		�s��̗v�f���R���\�[���ɏ����o��
	 */
	/// �S�Ă̗v�f�������o��
	virtual void show(void) const;
	/// �S�Ă̗v�f���w���`���ŏ����o��
	virtual void showExp(void) const;

	/**
	 *	����
	 *		�s��̑̊i�����̘a�����߂�
	 */
	double trace(void);
	const double trace(void) const;

/**
 *	����
 *		�s�̌���
 *	����
 *		i: ��������s
 *		j: ���������s
 */
	void swapRows(int i, int j);


/**
 *	----------------------------------------
 *	���ʂȍs�񉉎Z
 *	----------------------------------------
 */
	/**
	 *	����
	 *		�[���t�s������߂�
	 *		threshold�̒l�ɂ͒��ӂ��邱�Ɓi���������Ă��傫�����Ă��댯�j
	 */
	/// �ďo���̋[���t�s������߂�
	Matrix pseudoInversion(double threshold = 1.0E-6);
	const Matrix pseudoInversion(double threshold = 1.0E-6) const;

	/**
	 *	����
	 *		�������iwj�̂����ő�ƍŏ��̔�j
	 *		ill-conditioned�i�������j�F���������傫������
	 *	�ԋp�l
	 *		������
	 */
	double conditionNumber(void) const;


private:
	/**
	 *	����
	 *		�s��I�u�W�F�N�g�̃������̈���m�ۂ���
	 */
	void newMatrix(void);
	/**
	 *	����
	 *		�s��I�u�W�F�N�g�̃������̈���������
	 */
	void deleteMatrix(void);

/**
 *	----------------------------------------
 *	�A�������������߂邽�߂̕⏕�֐�
 *	----------------------------------------
 */
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
	 *					Cambridge University Press, 1988
	 */
	int Matrix::luDecomposition(const Matrix& a, Matrix& lu, Vector& indx, double* d) const;

	/**
	 *	����
	 *		��ޑ����p���ĘA��1��������������(a*x=b)
	 *	����
	 *		Matrix  a	: LU�����ŋ��߂�A��LU�����i�s�ρj
	 *		Vector  indx: LU�����̍ۂ̕����s�{�b�g�I���̍s�������
	 *		Vector  b	: ���m�̉E�Ӄx�N�g���i���x�N�g��x�̒l���㏑���j
	 *
	 *		Reference :	William H. Press, et al, NUMERICAL RECIPES in C, 
	 *					Cambridge University Press, 1988
	 */
	void luBackSubstitution(const Matrix& a, const Vector& indx, Vector& b) const;

/**
 *	----------------------------------------
 *	���ْl�������s�����߂̊֐��Q
 *	----------------------------------------
 */
	/**
	 *	����
	 *		�s��̓��ْl�����iSingular Value Decomposition�j
	 *	����
	 *		Matrix  a	: ���ْl���������s��
	 *		Matrix  u	: ���s�s��
	 *		Matrix  v	: ���ْl�̑Ίp�s��
	 *		Matrix  w	: ���s�s��
	 *		double* d	: �s�����̉񐔂�����=1�C�s�����̉񐔂��=-1
	 *
	 *		Reference :	William H. Press, et al, NUMERICAL RECIPES in C, 
	 *					Cambridge University Press, 1988
	 */
	void singularValueDecomposition(const Matrix& a, Matrix& u, Matrix& w, Matrix& v) const;

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
	double pythagorean(double a, double b) const;

	/**
	 *	����
	 *		�N�C�b�N�\�[�g
	 */
	void quickSort(Matrix& w, Vector& no, int low, int high) const;
};	/// end of class Matrix

/**
 *	----------------------------------------------------------------------
 *		Vector�N���X
 *	----------------------------------------------------------------------
 */
class Vector
{
/**
 *	------------------------------------------------------------
 *		�����o�ϐ�
 *	------------------------------------------------------------
 */
private:
	/// �x�N�g���̐擪�v�f�ւ̃|�C���^
    double*	elements;
    /// �x�N�g���̎���
	int			dimensions;

/**
 *	------------------------------------------------------------
 *		�t�����h�֐�
 *	------------------------------------------------------------
 */
	/// �N���XVector�̑��d��`���ꂽ�X�g���[�����o���Z�q
	/// �x�N�g���̑S�v�f�̒l���o�͂���
	friend std::ostream& operator<<(std::ostream& output, const Vector& vector);
	/// �x�N�g���̑S�v�f�̒l����͂���
	friend std::istream& operator>>(std::istream& input, Vector& vector);

	/// �x�N�g�������������ǂ���
	friend bool operator==(const Vector& left, const Vector& right);
    
	/// �x�N�g�����قȂ��Ă��邩�ǂ���
	friend bool operator!=(const Vector& left, const Vector& right);

	/**
	 *		��ʓI�ȎZ�p���Z�q
	 */
	/// ����
	friend double operator*(const Vector& left, const Vector& right);

	/// �s��ƃx�N�g���̐�
	friend const Vector operator*(const Matrix& matrix, const Vector& vector);
    
	/// �x�N�g���ƍs��̐�
	friend const Vector operator*(const Vector& vector, const Matrix& matrix);
    
	/// �s�񓯎m�̐�
	friend const Matrix operator*(const Matrix& left, const Matrix& right);

	/**
	 *		Matrix�N���X�̃����o�֐�
	 */
	/// �����v�f�̏���
	friend void Matrix::cleanUp(void);
	/// �s�̌���
	friend void Matrix::swapRows(int i, int j);
	/// �g�ݍ��킹���Z�q
	friend Matrix& Matrix::operator*=(const Matrix& right);

/**
 *	------------------------------------------------------------
 *		�����o�֐�
 *	------------------------------------------------------------
 */
public:
/**
 *	----------------------------------------
 *	�R���X�g���N�^�ƃf�X�g���N�^
 *	----------------------------------------
 */
	/// �f�t�H���g�R���X�g���N�^
    /**
	 *		dim: ������
	 */
	explicit Vector(int dim= 0);

	///�R�s�[�R���X�g���N�^
	Vector(const Vector& vector);

	/// �z��ŏ���������R���X�g���N�^
    /**
	 *		ptrArray: �������p�̔z��
	 *		dim: ������
	 */
	explicit Vector(const double* ptrArray, int dim);
    /// �f�X�g���N�^
	virtual ~Vector();                     
	
	/// �x�N�g���̃T�C�Y��ݒ肷��
    /**
	 *		dim: ������
	 */
	void setSize(int dim);
	/// �x�N�g���̃T�C�Y�𓾂�
    int getSize() const { return dimensions;}

/**
 *	----------------------------------------
 *	���Z�q�̑��d��`
 *----------------------------------------
 */

	/// ������Z�q
    /**
	 *		dim: ������
	 */
    Vector& operator= (const Vector& right);

	/// �Y�������Z�q
    /**
	 *		index: �v�f��
	 */
	double& operator()(int index);		/// ���[�U�p
    double& operator[](int index);		/// �v�Z�p
	/// const�I�u�W�F�N�g�ɑ΂���Y�������Z�q
    /**
	 *		index: �v�f��
	 */
	const double& operator()(int index) const;		/// ���[�U�p�i�����`�F�b�N�L�j
    const double& operator[](int index) const;		/// �v�Z�p�i�����`�F�b�N���C�����v�Z�p�j

	/// (-1)�x�N�g���i�x�N�g���̌�����ς���j
    const Vector operator-() const { return -1.0*(*this);}

	/**
	 *		��\�I�ȑg�ݍ��킹���Z�q
	 */
	Vector& operator*=(double scalar);
    Vector& operator/=(double scalar);
    Vector& operator+=(const Vector& vector);
    Vector& operator-=(const Vector& vector);

/**
 *	----------------------------------------
 *	��\�I�Ȋ֐�
 *	----------------------------------------
 */
	/// �m���������߂�
	double norm(void) const;
	/// �x�N�g���̋K�i���i�P�ʃx�N�g���ɂ���j
    const Vector& normalize(void);
	Vector normalization(void);
	const Vector normalization(void) const;

/**
 *	�����v�f�̏���
 */
	void cleanUp(void);

private:
    /// �I�u�W�F�N�g�̃������̈���m�ۂ���
	void newVector(void);
	/// �I�u�W�F�N�g�̃������̈���������
    void deleteVector(void);

};	/// end of class Vector

/**
 *	------------------------------------------------------------
 *		inline�֐�
 *	------------------------------------------------------------
 */
/**
 *		���d��`���ꂽ�{���Z�q
 *		�Q�̃x�N�g���̘a�����ߒl�n���ŕԂ�
 */
inline const Vector operator+(const Vector& left, const Vector& right)
{
	/// ���Z�q += ���g��
    return Vector(left) += right;
}

/**
 *		���d��`���ꂽ�|���Z�q
 *		�Q�̃x�N�g���̍������ߒl�n���ŕԂ�
 */
inline const Vector operator-(const Vector& left, const Vector& right)
{
	/// ���Z�q -= ���g��
    return Vector(left) -= right;
}

/**
 *		���d��`���ꂽ�����Z�q
 *		�x�N�g���̃X�J���[�{ �i�����~�x�N�g���j
 */
inline const Vector operator*(double schalor, const Vector& right)
{
	/// ���Z�q *= ���g��
    return Vector(right) *= schalor;
}

/**
 *		���d��`���ꂽ�����Z�q
 *		�x�N�g���̃X�J���[�{ �i�x�N�g���~�����j
 *		�����~�x�N�g���p�̊֐����g��
 */
inline const Vector operator*(const Vector& left, double schalor)
{
	/// ���Z�q *= ���g��
    return Vector(left) *= schalor;
}

/**
 *		���d��`���ꂽ/���Z�q
 *		�x�N�g���������Ŋ���Z����
 */
inline const Vector operator/(const Vector& left, double schalor)
{
	/// ���Z�q /= ���g��
    return Vector(left) /= schalor;
}

/**
 *		���d��`���ꂽ�{���Z�q
 *		�Q�̍s��̘a�����߁C�l�n���ŕԂ�
 */
inline const Matrix operator+(const Matrix& left, const Matrix& right)
{
	/// ���Z�q += ���g��
    return Matrix(left) += right;
}

/**
 *		���d��`���ꂽ�|���Z�q
 *		�Q�̍s��̍������߁C�l�n���ŕԂ�
 */
inline const Matrix operator-(const Matrix& left, const Matrix& right)
{
	/// ���Z�q -= ���g��
    return Matrix(left) -= right; 
}

/**
 *	------------------------------------------------------------
 *		Matrix�����o��inline�֐�
 *	------------------------------------------------------------
 */
/// ��const Matrix�I�u�W�F�N�g�̓Y�������Z�q
inline Vector& Matrix::operator[](int index)
{
    return this->elements[index];
}
/// const Matrix�I�u�W�F�N�g�̓Y�������Z�q
inline const Vector& Matrix::operator[](int index) const
{
    return this->elements[index];
}

/**
 *		�Y�������Z�q
 *		row: �s��̍s�ԍ�
 *		col:  �s��̗�ԍ�
 */
inline double& Matrix::operator()(int row, int col)
{
	/// �Y�����̃`�F�b�N
	assert( (row >= 1  && row <= this->rows) && (col >= 1  && col <= this->columns) );

	return this->elements[row-1][col-1];
}

/**
 *		�Y�������Z�q
 *		row: �s��̍s�ԍ�
 *		col:  �s��̗�ԍ�
 */
inline const double& Matrix::operator()(int row, int col) const
{
	/// �Y�����̃`�F�b�N
	assert( (row >= 1  && row <= this->rows) && (col >= 1  && col <= this->columns) );

	return this->elements[row-1][col-1];
}

/**
 *	------------------------------------------------------------
 *		Vector�����o��inline�֐�
 *	------------------------------------------------------------
 */
/// ��const Vector�I�u�W�F�N�g�̓Y�������Z�q
inline double& Vector::operator[](int index)
{
    return this->elements[index];
}

/// const Vector�I�u�W�F�N�g�̓Y�������Z�q
inline const double& Vector::operator[](int index) const
{
    return this->elements[index];
}

/// �Y�������Z�q
/**
 *		index: �v�f��
 */
inline double& Vector::operator()(int index)
{
	/// �����̃`�F�b�N
	assert( index >= 1  && index <= dimensions );

	return this->elements[index-1];
}

/// const�I�u�W�F�N�g�ɑ΂���Y�������Z�q
/**
 *		index: �v�f��
 */
inline const double& Vector::operator()(int index) const
{
	/// �����̃`�F�b�N
	assert( index >= 1  && index <= dimensions );

	return this->elements[index-1];
}

} /// end of namespace Math

#endif /// __Matrix_h__

