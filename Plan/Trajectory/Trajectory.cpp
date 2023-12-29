/**
 *  File: Trajectory.cpp
 *
 *  Description: �O�����������N���X�̒�`��
 *
 *  Created: 2007/03/06(Tue)
 *  Updated: 2007/03/06(Tue)
 *
 *  Copyright (C) Tokyo Institute of Technology Hirose Lab.
 */

#include "Trajectory.h"

using namespace std;
using namespace Math;
using namespace Const;

namespace Plan
{
/**
 *		�R���X�g���N�^�ƃf�X�g���N�^
 */
/// �f�t�H���g�R���X�g���N�^
Trajectory::Trajectory()
{
	initializeTrajectory();
}

/// �R�s�[�R���X�g���N�^
Trajectory::Trajectory(const Trajectory& trajectory)
{
	initializeTrajectory();
	copy(trajectory);
}

/// �f�X�g���N�^
Trajectory::~Trajectory()
{
}

/**
 *		���Z�q
 */
/// ������Z�q
Trajectory& Trajectory::operator=(const Trajectory& trajectory)
{
	/// ���ȑ����h��
	if ( &trajectory != this )
	{
		copy(trajectory);
	}

	return *this;
}

/// �������Z�q
bool Trajectory::operator ==(const Trajectory& trajectory) const
{
	if ( startPosition != trajectory.startPosition ){return false;}
	if ( goalPosition != trajectory.goalPosition ){return false;}
	if ( startTime != trajectory.startTime ){return false;}
	if ( startTime != trajectory.goalTime ) {return false;}

	return true;
}


/// �񓙉����Z�q
bool Trajectory::operator!=(const Trajectory& trajectory) const
{
	if ( *this == trajectory )
		return false;
	else
		return true;
}

/**
 *		������
 */
void Trajectory::initializeTrajectory(void)
{
	/// �X�^�[�g�n�_
	startPosition.setSize(THREE_DIMENSION);
	/// �S�[���n�_
	goalPosition.setSize(THREE_DIMENSION);

	/// �X�^�[�g����
	startTime = 0.0;
	/// �S�[������
	goalTime = 0.0;
}

/**
 *		�O���p�����[�^�̐ݒ�
 */
void Trajectory::setDistance(const Vector& start, const Vector& goal)
{
	startPosition = start;
	goalPosition = goal;
}

void Trajectory::setTime(double start, double goal)
{
	startTime = start;
	goalTime = goal;
}

/**
 *		�O���p�����[�^�̈ڍs�i�X�^�[�g�����炷�j
 */
void Trajectory::shiftStartPosition(const Math::Vector& start)
{
	Vector shift(THREE_DIMENSION);
	shift = goalPosition - startPosition;

	startPosition = start;
	goalPosition = start + shift;
}

void Trajectory::shiftStartTime(double start)
{
	double shift;
	shift = goalTime - startTime;

	startTime = start;
	goalTime = start + shift;

}

/**
 *		�O���𓾂�
 */
Vector Trajectory::getPosition(double splitTime)
{
	/// ���o���r��O��
	Vector trajectory(THREE_DIMENSION);

	if ( splitTime <= startTime )
	{
		trajectory = startPosition;
	}
	else if ( startTime < splitTime && splitTime <= goalTime )
	{
		trajectory = (goalPosition - startPosition)*( (splitTime - startTime) / (goalTime - startTime) );
	}
	else if ( goalTime < splitTime )
	{
		trajectory = goalPosition;
	}
	
	return trajectory;
}

/**
 *		Trajectory�N���X��private�ȃ����o�֐�
 */
void Trajectory::copy(const Trajectory& trajectory)
{
	startPosition = trajectory.startPosition;
	goalPosition = trajectory.goalPosition;

	startTime = trajectory.startTime;
	goalTime = trajectory.goalTime;
}


}	/// end of namespace Plan