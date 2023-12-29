/**
 *  File: MathGeometry.h
 *
 *  Description:	���w�̊􉽊w���C�u����
 *
 *  Created: 2007/02/05(Mon)
 *  Updated: 2007/02/05(Mon)
 *
 *  Copyright (C) Tokyo Institute of Technology Hirose Lab.
 */

#ifndef __MathGeometry_h__
#define __MathGeometry_h__

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include <cmath>
#include "Matrix\MatrixLibrary.h"

namespace Math
{

/**
 *		distancePonitToPoint: �_�Ɠ_�Ƃ̋���
 *			point1: �v������_
 *			point2: ������̓_
 */
extern double distancePointToPoint(const Vector& point1, const Vector& point2);

/**
 *		normalVectorTo: �����ȃx�N�g�����쐬
 *			normalVectorToLine: �����ɐ���
 *			normalVectorToSegment: �����ɐ���
 */
extern Vector normalVectorToLine(const Vector& point, const Vector& linePoint, const Vector& lineDirectionVector);
extern Vector normalVectorToSegment(const Vector& point, const Vector& start, const Vector& end);

/**
 *		distancePointAndLine: �_�ƒ����Ƃ̋���
 *			point: �v������_
 *			linePoint: ������̓_
 *			lineDirectionVector: �����̕����x�N�g��
 */
extern double distancePointAndLine(const Vector& point, const Vector& linePoint, const Vector& lineDirectionVector);

/**
 *		DistancePointAndSegment: �_�Ɛ����Ƃ̋���
 *			point: �v������_
 *			start: �����̎n�_
 *			end: �����̏I�_
 */
extern double distancePointAndSegment(const Vector& point, const Vector& start, const Vector& end);


/**
 *		normalVectorOfPlane	: ���ʂɐ����ȃx�N�g�����쐬
 *			����:
 *			p1, p2, p3: ���ʏ�̈꒼����ɂȂ�3�_
 */
extern Vector normalVectorOfPlane(const Vector& p1, const Vector& p2, const Vector& p3);

/**
 *		IntersectPointLineAndPlane			: �����ƕ��ʂƂ̌�_
 *			����:
 *				linePoint: ���ʏ�̈꒼����ɂȂ�3�_
 *				lineDirectionVector: �����̕����x�N�g��
 *				planePoint: ���ʏ�̓_
 *				planeNormalVector: ���ʂ̖@���x�N�g��
 */
extern Vector intersectPointLineAndPlane(const Vector& linePoint, const Vector& lineDirectionVector, 
														const Vector& planePoint, const Vector& planeNormalVector);

/**
 *		isPointInsideTriangle
 *			�O�p�`�̒��ɓ_�����邩�ǂ���
 *				�����ɂ���ꍇ: true
 *				�O���ɂ���ꍇ: false
 *			����
 *				Vector a, b, c: �O�p�`�̊e���_�̈ʒu�x�N�g��
 *				Vector p: ���肷�ׂ��_
 */
extern bool isPointInsideTriangle(const Vector& p, const Vector& a, const Vector& b, const Vector& c);

/**
 *		minDistanceToTriangleEdge
 *			��������_����O�p�`�̊e�ӂ܂ł̋����ōł��ŏ��̒l�����߂�
 */
extern double minDistanceToTriangleEdge(const Vector& p, const Vector& a, const Vector& b, const Vector& c);

/**
 *		isPointInsideTriangle
 *			�ʎl�p�`�̒��ɓ_�����邩�ǂ���
 *				�����ɂ���ꍇ: true
 *				�O���ɂ���ꍇ: false
 *			����
 *				Vector a, b, c, d: �l�p�`�̊e���_�̈ʒu�x�N�g��
 *				Vector p: ���肷�ׂ��_
 */
extern bool isPointInsideQuadrangle(const Vector& p, const Vector& a, const Vector& b, const Vector& c, const Vector& d);

/**
 *		minDistanceToQuadrangleEdge
 *			��������_����l�p�`�̊e�ӂ܂ł̋����ōł��ŏ��̒l�����߂�
 */
extern double minDistanceToQuadrangleEdge(const Vector& p, const Vector& a, const Vector& b, const Vector& c, const Vector& d);

}	/// end of namespace Math
#endif	/// __MathGeometry_h__