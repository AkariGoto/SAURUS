/**
 *  File: BodyTrajectory.cpp
 *
 *  Description: ���̋O�����������N���X�̐錾��
 *
 *  Created: 2007/03/06(Tue)
 *  Updated: 2007/03/06(Tue)
 *
 *  Copyright (C) Tokyo Institute of Technology Hirose Lab.
 */

#include "BodyTrajectory.h"

using namespace std;
using namespace Math;
using namespace Const;

namespace Plan
{
/**
 *		�R���X�g���N�^�ƃf�X�g���N�^
 */
/// �f�t�H���g�R���X�g���N�^
BodyTrajectory::BodyTrajectory()
{
	initializeBodyTrajectory();
}

/// �R�s�[�R���X�g���N�^
BodyTrajectory::BodyTrajectory(const BodyTrajectory& trajectory)
{
	initializeBodyTrajectory();
	copy(trajectory);
}

/// �f�X�g���N�^
BodyTrajectory::~BodyTrajectory()
{
}

/**
 *		���Z�q
 */
/// ������Z�q
BodyTrajectory& BodyTrajectory::operator=(const BodyTrajectory& trajectory)
{
	/// ���ȑ���h�~
	if ( &trajectory != this )
	{
		copy(trajectory);
	}

	return *this;
}

/// �������Z�q
bool BodyTrajectory::operator==(const BodyTrajectory& trajectory) const
{
	if ( moveStride != trajectory.moveStride ){return false;}
	if ( moveTime != trajectory.moveTime ){return false;}

	return true;
}

/// �񓙉����Z�q
bool BodyTrajectory::operator!=(const BodyTrajectory& trajectory) const
{
	if ( *this == trajectory )
		return false;
	else
		return true;
}

/**
 *		������
 */
void BodyTrajectory::initializeBodyTrajectory(void)
{
	moveStride.setSize(THREE_DIMENSION);
	moveTime = 0.0;
}

/**
 *		�O���p�����[�^�̐ݒ�
 *		goalPoint�����goalTime�������X�V�����
 */
void BodyTrajectory::setBodyTrajectory(const Math::Vector& stride)
{
	moveStride = stride;
	goalPosition = startPosition + stride;
	return;
}

void BodyTrajectory::setBodyTrajectoryTime(double time)
{
	moveTime = time;
	goalTime = startTime + time;
	return;
}

/**
 *		�O���𓾂�
 */
Vector BodyTrajectory::getPosition(double splitTime)
{
	/// ���o���d�S�O��
	Vector trajectory(THREE_DIMENSION);

	return trajectory;
}

/**
 *		BodyTrajectory�N���X��private�ȃ����o�֐�
 */
void BodyTrajectory::copy(const BodyTrajectory &trajectory)
{
	moveStride = trajectory.moveStride;
	moveTime = trajectory.moveTime;

	return;
}

}	/// end of namespace Plan