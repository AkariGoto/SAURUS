/**
 *  File: BodyTrajectory.h
 *
 *  Description: ���̋O�����������N���X�̐錾��
 *					 �����O��
 *
 *  Created: 2007/03/06(Tue)
 *  Updated: 2007/03/06(Tue)
 *
 *  Copyright (C) Tokyo Institute of Technology Hirose Lab.
 */

#ifndef __BodyTrajectory_h__
#define __BodyTrajectory_h__

#include "Trajectory.h"
#include "..\..\Math\Matrix\Matrix.h"
#include "..\..\Utility\Constants.h"

namespace Plan
{
/**
 *		//////////////////////////////////////////////////
 *				BodyTrajectory�N���X
 *		//////////////////////////////////////////////////
 */
class BodyTrajectory : public Trajectory
{
/**
 *		//////////////////////////////////////////////////
 *				�����o�ϐ�
 *		//////////////////////////////////////////////////
 */
private:
/**
 *		�����Ɋ֌W�������
 */
	/// �V�r�X�g���[�N
	Math::Vector moveStride;

/**
 *		���ԂɊ֌W�������
 */
	/// ���A����
	double moveTime;

/**
 *		//////////////////////////////////////////////////
 *				�����o�֐�
 *		//////////////////////////////////////////////////
 */
public:
/**
 *		�R���X�g���N�^�ƃf�X�g���N�^
 */
	/// �f�t�H���g�R���X�g���N�^
	BodyTrajectory();
	/// �R�s�[�R���X�g���N�^
	BodyTrajectory(const BodyTrajectory& trajectory);
	/// �f�X�g���N�^
	virtual ~BodyTrajectory();

/**
 *		���Z�q
 */
	/// ������Z�q
	BodyTrajectory& operator=(const BodyTrajectory& trajectory);
	/// �������Z�q
	bool operator==(const BodyTrajectory& trajectory) const;
	/// �񓙉����Z�q
	bool operator!=(const BodyTrajectory& trajectory) const;

/**
 *		�A�N�Z�X�֐�
 */
	/// ���̈ړ�����
	const Math::Vector& getMoveStride(void) const{return moveStride;}
	/// ���̈ړ�����
	const double getMoveTime(void) const{return moveTime;}

/**
 *		�Z�b�g�֐�
 */
	/// ���̈ړ�����
	void setMoveTime(double time) {moveTime = time; return;}

/**
 *		������
 */
	void initializeBodyTrajectory(void);

/**
 *		�O���p�����[�^�̐ݒ�
 *		goalPoint�����goalTime�������X�V�����
 */
	void setBodyTrajectory(const Math::Vector& stride);
	void setBodyTrajectoryTime(double time);

/**
 *		�O���𓾂�
 */
	Math::Vector getPosition(double splitTime);


private:
	/// �R�s�[�R���X�g���N�^�Ƒ�����Z�q�̂��߂̃w���v�֐�
	void copy(const BodyTrajectory& trajectory);

};	/// end of class BodyTrajectory

}	/// end of namespace Plan

#endif	/// __BodyTrajectory_h__