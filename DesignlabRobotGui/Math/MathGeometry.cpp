/**
 *  File: MathGeometry.cpp
 *
 *  Description:	���w�̊􉽊w���C�u����
 *
 *  Created: 2007/02/05(Mon)
 *  Updated: 2007/02/05(Mon)
 *
 *  Copyright (C) Tokyo Institute of Technology Hirose Lab.
 */

#include "MathGeometry.h"
#include "Matrix\MatrixLibrary.h"

using namespace std;
using namespace Const;

namespace Math
{

/**
 *		distancePonitToPoint: �_�Ɠ_�Ƃ̋���
 *			point1: �v������_
 *			point2: ������̓_
 */
double distancePointToPoint(const Vector& point1, const Vector& point2)
{
	assert( point1.getSize() == THREE_DIMENSION || point2.getSize() == THREE_DIMENSION );

	Vector temp;
	temp = point2 - point1;

	return temp.norm();

}

/**
 *		normalVectorTo: �����ȃx�N�g�����쐬
 *			normalVectorToLine: �����ɐ���
 *			normalVectorToSegment: �����ɐ���
 */

/**
 *		point: �v������_
 *		linePoint					: ������̓_
 *		lineDirectionVector	: �����̕����x�N�g��
 *
 * ��  ��  ��				��
 *	l = a + tb (����)		c (�_)
 *	   ��   ��  ��                      ��
 *	(1)c  -  a ��b �����ւ̐��ˉe�x�N�g�� s���l����
 *                 ��          ��
 *	(2)��̐��ˉe s�x�N�g���� c �̍����x�N�g�������
 *	(3)���̃x�N�g���̑傫���������̋���, �܂�_�ƒ����Ƃ̋���
 */
Vector normalVectorToLine(const Vector& point, const Vector& linePoint, const Vector& lineDirectionVector)
{
	assert( point.getSize() == THREE_DIMENSION || linePoint.getSize() == THREE_DIMENSION || lineDirectionVector.getSize() == THREE_DIMENSION );
	
	double ip=0;	/// ���ˉe�x�N�g���̂��߂̓��ϒl
	Vector temp(THREE_DIMENSION);	/// �v�Z�̂��߂̃x�N�g��
	temp = lineDirectionVector;
	
	Vector s(THREE_DIMENSION);	/// ���ˉe�x�N�g��
	Vector normal(THREE_DIMENSION);	/// �@���x�N�g��

	ip = innerProduct( (point - linePoint), lineDirectionVector);
	
	/// ������������
	if ( fabs(ip) < 1.0E-15 )
		return linePoint - point;
	
	/// �� ��  ��
	/// c - a �� b �����ւ̐��ˉe�x�N�g�����v�Z
	s = ( ip / temp.norm() ) * ( lineDirectionVector / temp.norm() ); /// �P�ʕ����x�N�g��

	// �����x�N�g�������߂�
	normal = s + linePoint - point ;

	return normal;

}

Vector normalVectorToSegment(const Vector& point, const Vector& start, const Vector& end)
{
	assert( point.getSize() == THREE_DIMENSION || start.getSize() == THREE_DIMENSION || end.getSize() == THREE_DIMENSION );

	/// �@���x�N�g��
	Vector normal(THREE_DIMENSION);

	/// �����̕�������p���Ė@���x�N�g�������߂�
	normal = normalVectorToLine(point, start, (end - start) );

	return normal;
}

/**
 *		distancePointAndLine	: �_�ƒ����Ƃ̋���(Distance between point and line)
 *			point: �v������_
 *		line_point: ������̓_
 *		line_dir					: �����̕����x�N�g��
 * �� ��   ��				��
 *	l = a + tb (����)		c (����)
 *	   ��  ��  ��                   ��
 *	(1)c - a ��b �����ւ̐��ˉe�x�N�g��s���l����
 *                         ��
 *	(2)��̐��ˉe�x�N�g����c �̍����x�N�g�������
 *	(3)���̃x�N�g���̑傫���������̋���, �܂�_�ƒ����Ƃ̋���
 */
double distancePointAndLine(const Vector& point, const Vector& linePoint, const Vector& lineDirectionVector)
{
	Vector normal(THREE_DIMENSION); /// �@���x�N�g��

	/// �����x�N�g���̑傫�������߂鋗��
	normal = normalVectorToLine(point, linePoint, lineDirectionVector);

	return normal.norm();
}

/**
 *		distancePointAndSegment: �_�Ɛ����Ƃ̋���
 *			point: �v������_
 *			start: �����̎n�_
 *			end: �����̏I�_
 */
double distancePointAndSegment(const Vector& point, const Vector& start, const Vector& end)
{
	Vector normal(THREE_DIMENSION); /// �@���x�N�g��

	// �����x�N�g���̑傫�������߂鋗��
	normal = normalVectorToSegment(point, start, end);

	return normal.norm();
}

/**
 *		normalVectorOfPlane: ���ʂɐ����ȃx�N�g�����쐬
 *			����:
 *				p1, p2, p3: ���ʏ�̈꒼����ɂȂ�3�_
 *
 *	��	 ��   ��   ��   ��
 *	n = (p2 - p1) X (p3 - p1): �O�ςŋ��߂�. �@���x�N�g��n�͐��K�����Ă���
 */
Vector normalVectorOfPlane(const Vector& p1, const Vector& p2, const Vector& p3)
{
	/// 3�����x�N�g������
	assert( p1.getSize() == THREE_DIMENSION || p2.getSize() == THREE_DIMENSION || p3.getSize() == THREE_DIMENSION );

	Vector vector1(THREE_DIMENSION);
	Vector vector2(THREE_DIMENSION);
	Vector normal(THREE_DIMENSION);

	vector1 = p2 - p1;
	vector2 = p3 - p1;

	/// �꒼����ɂȂ����Ƃ��`�F�b�N
	double ratio1 = 0;
	double ratio2 = 0;
	double ratio3 = 0;

	ratio1 = vector2(1) / vector1(1);
	ratio2 = vector2(2) / vector1(2);
	ratio3 = vector2(3) / vector1(3);

	if ( ( fabs(ratio1 - ratio2) < 1.0E-15 ) &&
		 ( fabs(ratio2 - ratio3) < 1.0E-15 ) && 
		 ( fabs(ratio3 - ratio1) < 1.0E-15 ) )
	{
		Vector zero(THREE_DIMENSION);
		return zero;
	}

	/// �@���x�N�g�����v�Z
	normal = outerProduct( vector1, vector2 );

	return normal.normalize();
}


/**
 *		intersectPointLineAndPlane	: �����ƕ��ʂƂ̌�_
 *			����:
 *				linePoint: ������̓_
 *				lineDirectionVector: �����̕����x�N�g��
 *				planePoint: ���ʏ�̓_
 *				planeNormalVector: ���ʂ̖@���x�N�g��
 *
 *						   ��   ��
 *		���ʂ̕�����	n �E P + d = 0
 *			    ��			      ��		  ��  ��  ��
 *		��_��P ���ʏ�̓_��P0�Ƃ���� n �E(P - P0) = 0
 *					      ��   ��					         ��  ��    ��
 *		���ʂ̕����� n �E P + d = 0 �ɒ����̕����� P = a + t�Eb ����
 *						     ��   ��  ��	 ��  ��
 *		����ɂ��, t = -n �E( a - P0 )/(n �E b) �Ƌ��܂� 
 */
Vector intersectPointLineAndPlane(const Vector& linePoint, const Vector& lineDirectionVector, 
												const Vector& planePoint, const Vector& planeNormalVector)
{
	double a, b;
	double t;		/// �����̔}��ϐ�

	Vector zero(THREE_DIMENSION);
	Vector vector(THREE_DIMENSION);
	Vector point(THREE_DIMENSION);

	vector = linePoint - planePoint;

	a = -innerProduct(planeNormalVector, vector);
	b = innerProduct(planeNormalVector, lineDirectionVector);

	/// ���s�Ȃ̂Ō�_�Ȃ�
	if ( fabs(b) < 1.0E-15 ) return zero;

	/// �}��ϐ�t�����߂�
	t = a / b;

	/// �����̕������ɑ������_�����߂�
	point = linePoint + lineDirectionVector * t;

	return point;
}

/**
 *		isPointInsideTriangle
 *			�O�p�`�̒��ɓ_�����邩�ǂ���
 *				�����ɂ���ꍇ: true
 *				�O���ɂ���ꍇ: false
 *			����
 *				Vector a, b, c: �O�p�`�̊e���_�̈ʒu�x�N�g��
 *				Vector p: ���肷�ׂ��_
 */
/**
 *										/// ��ABC�̖@����N, ����_P�Ƃ���
 *			A						/// �_P����O�p�`�̒��_�ւ̃x�N�g�����쐬
 *		   �^�_					/// PA =A-P, PB = B-P, PC = C-P
 *		 �^    �_				/// ��PAB, ��PBC, ��PCA�̖@����N1, N2, N3�Ƃ���
 *	   �^	  �@ �_			/// N1=PA X PB, N2=PB X PC, N3=PC X PA
 *	 �^		 P     �_			/// �O�p�`�̓����ɂ���ꍇ, N��N1, N2, N3�̌���������
 * �^			     �_		/// �O���ɂ���ꍇ�́A�ǂꂩ���m�Ƃ͋t����
 *B�P�P�P�P�P�P�P�P�P�P C	/// �������������ǂ�������ςŒ��ׂ�
 */
bool isPointInsideTriangle(const Vector& p, const Vector& a, const Vector& b, const Vector& c)
{

	if ( innerProduct( outerProduct( b-a, c-a ), outerProduct( a-p, b-p ) ) < 0 ) return false;
	if ( innerProduct( outerProduct( b-a, c-a ), outerProduct( b-p, c-p ) ) < 0 ) return false;
	if ( innerProduct( outerProduct( b-a, c-a ), outerProduct( c-p, a-p ) ) < 0 ) return false;

	return true;
}

/**
 *		��������_����O�p�`�̊e�ӂ܂ł̋����ōł��ŏ��̒l�����߂�
 *			�Ԃ�l
 *				�_�������Ȃ�: �����̒l(double)
 *				�_���O���Ȃ�: �����̕��̒l��Ԃ�
 *			����
 *				Vector a, b, c: �O�p�`�̊e���_�̈ʒu�x�N�g��
 *				Vector p: ���肷�ׂ��_
 */
double minDistanceToTriangleEdge(const Vector& p, const Vector& a, const Vector& b, const Vector& c)
{
	/// ���ꂼ��̕ӂ܂ł̋���
	double dist[3];

	dist[0] = distancePointAndSegment( p, a, b );		/// ��ab�܂ł̋���
	dist[1] = distancePointAndSegment( p, b, c );		/// ��bc�܂ł̋���
	dist[2] = distancePointAndSegment( p, c, a );		/// ��ca�܂ł̋���

	int i;	/// �J�E���^
	double min;	/// �����̍ŏ��l
	double distance;	/// �Ԃ�l
	
	min = dist[0];	/// �ŏ���dist[0]���ŏ��l�ɉ���

	/// �ŏ��l��I�o
	for ( i=0; i<3; i++ )
	{
		if ( min > dist[i] )
			min = dist[i];
	}
	
	// �O�p�`abc�̓����ɂ��邩�ǂ���
	if ( !isPointInsideTriangle( p, a, b, c ) )
		distance = -min;	/// �����ɂȂ��Ȃ瓾��ꂽ�ŏ��l�𕉂ɂ��ĕԂ�
	else if ( isPointInsideTriangle( p, a, b, c ) )
		distance = min;	/// �����ɂ���Ȃ�ŏ��l�����̂܂ܕԂ�

	return distance;
}

/**
 *		isPointInsideTriangle
 *			�ʎl�p�`�̒��ɓ_�����邩�ǂ���
 *				�����ɂ���ꍇ: true
 *				�O���ɂ���ꍇ: false
 *			����
 *				Vector a, b, c, d: �l�p�`�̊e���_�̈ʒu�x�N�g��
 *				Vector p: ���肷�ׂ��_
 */
/// �l�p�`���̎O�p�`�ɕ���, IsPointInsideTriangle��2��g��
bool isPointInsideQuadrangle(const Vector& p, const Vector& a, const Vector& b, const Vector& c, const Vector& d)
{
	if ( !isPointInsideTriangle(p, a, b, c) )
	{
		if ( !isPointInsideTriangle(p, a, c, d) ) return false;
	}

	return true;
}

/**
 *		��������_����l�p�`�̊e�ӂ܂ł̋����ōł��ŏ��̒l�����߂�
 *			�Ԃ�l
 *				�_�������Ȃ�: �����̒l(double)
 *				�_���O���Ȃ�: �����̕��̒l��Ԃ�
 *			����
 *				Vector a, b, c, d: �O�p�`�̊e���_�̈ʒu�x�N�g��
 *				Vector p: ���肷�ׂ��_
 */
double minDistanceToQuadrangleEdge(const Vector& p, const Vector& a, const Vector& b, const Vector& c, const Vector& d)
{
	double dist[4];		/// ���ꂼ��̕ӂ܂ł̋���
	double min;			/// �����̍ŏ��l
	int i;					/// �J�E���^
	double distance;	/// �Ԃ�l

	dist[0] = distancePointAndSegment( p, a, b );		/// ��ab�܂ł̋���
	dist[1] = distancePointAndSegment( p, b, c );		/// ��bc�܂ł̋���
	dist[2] = distancePointAndSegment( p, c, d );		/// ��cd�܂ł̋���
	dist[3] = distancePointAndSegment( p, d, a );		/// ��da�܂ł̋���

	/// �ŏ���dist[0]���ŏ��l�ɉ���
	min = dist[0];

	/// �ŏ��l��I�o
	for ( i=0; i<4; i++ )
	{
		if ( min > dist[i] )
			min = dist[i];
	}
	
	//// �l�p�`abcd�̓����ɂ��邩�ǂ���
	if ( !isPointInsideQuadrangle( p, a, b, c, d ) )
		distance = -min;	/// �����ɂȂ��Ȃ瓾��ꂽ�ŏ��l�𕉂ɂ��ĕԂ�
	else
		distance = min;	/// �����ɂ���Ȃ�ŏ��l�����̂܂ܕԂ�

	return distance;
}

}	/// end of namespace Math