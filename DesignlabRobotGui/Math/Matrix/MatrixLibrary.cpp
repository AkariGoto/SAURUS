/**
 *  File: MatrixLibrary.cpp
 *
 *  Description: �s��v�Z���C�u����
 *
 *  Created: 2007/05/01(Tue)
 *  Updated: 2007/05/01(Tue)
 *
 *  Copyright (C) Tokyo Institute of Technology Hirose Lab.
 */

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include "MatrixLibrary.h"

using namespace std;
using namespace Math;
using namespace Const;


namespace Math
{

/**
 *	----------------------------------------------------------------------
 *		�s��v�Z
 *	----------------------------------------------------------------------
 */
/**
 *	����
 *		��s��𓾂�
 */
Matrix zeroMatrix(int rowNum, int columnNum)
{
	Matrix result(rowNum, columnNum);

	return result;

}

/**
 *	����
 *		�P�ʍs��𓾂�
 */
Matrix identityMartix(int rowNum, int columnNum)
{
	Matrix result(rowNum, columnNum);

	result.loadIdentity();

	return result;
}

/**
 *	����
 *		��]���Ɖ�]�p����s��𓾂�
*	����
 *		axis�F��]�����x�N�g���i���x�N�g���j
 *		angle�F��]�p
 */
Matrix rotationMatrix(const Vector& axis, double angle)
{
	/// ���[�J���ϐ�
	/// ��]�s��
	Matrix rotation(THREE_DIMENSION, THREE_DIMENSION);
	/// �P�ʕ����x�N�g��
	Vector unit(THREE_DIMENSION);

	/// �����x�N�g������
	unit = axis;
	/// ���K��
	unit.normalize();

	rotation(1,1) = cos(angle) + unit(1)*unit(1)*( 1-cos(angle) );
	rotation(1,2) = unit(1)*unit(2)*( 1-cos(angle) ) - unit(3)*sin(angle);
	rotation(1,3) = unit(1)*unit(3)*( 1-cos(angle) ) + unit(2)*sin(angle);
	rotation(2,1) = unit(1)*unit(2)*( 1-cos(angle) ) + unit(3)*sin(angle);
	rotation(2,2) = cos(angle) + unit(2)*unit(2)*( 1-cos(angle) );
	rotation(2,3) = unit(2)*unit(3)*( 1-cos(angle) ) - unit(1)*sin(angle);	
	rotation(3,1) = unit(1)*unit(3)*( 1-cos(angle) ) - unit(2)*sin(angle);
	rotation(3,2) = unit(2)*unit(3)*( 1-cos(angle) ) + unit(1)*sin(angle);
	rotation(3,3) = cos(angle) + unit(3)*unit(3)*( 1-cos(angle) );

	return (rotation);
}

/**
 *	----------------------------------------------------------------------
 *		�x�N�g���v�Z
 *	----------------------------------------------------------------------
 */
/**
 *	����
 *		��x�N�g�������߂�
 */
Vector zeroVector(int dimension)
{
	Vector result(dimension);

	return result;
}

/**
 *	����
 *		�ʒu�x�N�g�������߂�
 */
Vector positionVector(double x, double y, double z)
{
	Vector result(3);

	result(1) = x;
	result(2) = y;
	result(3) = z;

	return result;
}

/**
 *	����
 *		���ς����߂�
 *		�x�N�g���݂̂Ōv�Z
 */
double innerProduct(const Vector& x, const Vector& y)
{
	/// �����`�F�b�N
	if (x.getSize() != y.getSize())
	{
		cerr << "Error: [innerProduct] Vector type is unmatched\n" << endl;
        abort();
	}

	/// �Ԃ�l
	double ip=0;

	int i;
	for (i=1; i<=x.getSize(); i++)
        ip += x(i)*y(i);

	return ip;
}

/**
 *	����
 *		���ς����߂�
 *		�����݂̂Ōv�Z
 */
double innerProduct(double x1, double y1, double z1, double x2, double y2, double z2)
{
	double ip;

	ip = x1*x2 + y1*y2 + z1*z2;

	return ip;
}

/**
 *	����
 *		3�����̊O�ς����߂�
 *		�x�N�g���݂̂Ōv�Z
 */
Vector outerProduct(const Vector& x, const Vector& y)
{
	if ( x.getSize() != THREE_DIMENSION || y.getSize() != THREE_DIMENSION )
	{
		cerr << "Error: [outerProduct] Vector is not 3 dimension\n" << endl;
		return zeroVector( THREE_DIMENSION );
	}

	Vector op(3);

	op(1) = x(2)*y(3) - x(3)*y(2);
	op(2) = x(3)*y(1) - x(1)*y(3);
	op(3) = x(1)*y(2) - x(2)*y(1);

	return op;
}

/**
 *	����
 *		3�����̊O�ς����߂�
 *		�����݂̂Ōv�Z
 */
void outerProduct(double x1, double y1, double z1, 
						double x2, double y2, double z2,
						double* x, double* y, double* z)
{
	*x = (y1*z2 - z1*y2);
	*y = (z1*x2 - x1*z2);
	*z = (x1*y2 - y1*x2);

	return;
}


}	/// end of namespace Math