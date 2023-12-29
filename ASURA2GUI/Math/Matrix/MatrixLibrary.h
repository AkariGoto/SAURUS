/**
 *  �t�@�C����
 *		File.h
 *  ����
 *		�s��v�Z���C�u����
 *  ���t
 *		�쐬��: 2007/05/01(Tue)		�X�V��: 2007/05/08(Tue)
 */

#ifndef __MatrixLibrary_h__
#define __MatrixLibrary_h__

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */

#include "Matrix.h"
#include "..\..\Utility\EngConstant.h"

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
extern Matrix zeroMatrix(int rowNum, int columnNum);

/**
 *	����
 *		�P�ʍs��𓾂�
 */
extern Matrix identityMartix(int rowNum, int columnNum);

/**
 *	����
 *		��]���Ɖ�]�p����s��𓾂�
*	����
 *		axis�F��]�����x�N�g���i���x�N�g���j
 *		angle�F��]�p
 */
extern Matrix rotationVector(const Vector& axis, double angle);

/**
 *	----------------------------------------------------------------------
 *		�x�N�g���v�Z
 *	----------------------------------------------------------------------
 */
/**
 *	����
 *		��x�N�g�������߂�
 */
extern Vector zeroVector(int dimension);

/**
 *	����
 *		�ʒu�x�N�g�������߂�
 */
extern Vector positionVector(double x, double y, double z);

/**
 *	����
 *		���ς����߂�
 *		�x�N�g���݂̂Ōv�Z
 */
extern double innerProduct(const Vector& x, const Vector& y);

/**
 *	����
 *		���ς����߂�
 *		�����݂̂Ōv�Z
 */
extern double innerProduct(double x1, double y1, double z1, double x2, double y2, double z2);

/**
 *	����
 *		3�����̊O�ς����߂�
 *		�x�N�g���݂̂Ōv�Z
 */
extern Vector outerProduct(const Vector& x, const Vector& y);

/**
 *	����
 *		3�����̊O�ς����߂�
 *		�����݂̂Ōv�Z
 */
extern void outerProduct(	double x1, double y1, double z1, 
							double x2, double y2, double z2,
							double* x, double* y, double* z);
}
#endif	/// __MatrixLibrary_h__