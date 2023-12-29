/**
 *  File: Trajectory.h
 *
 *  Description: �O�����������N���X�̐錾��
 *
 *  Created: 2007/03/06(Tue)
 *  Updated: 2007/03/06(Tue)
 *
 *  Copyright (C) Tokyo Institute of Technology Hirose Lab.
 */

#ifndef __Trajectory_h__
#define __Trajectory_h__

#include "..\..\Math\Matrix\Matrix.h"
#include "..\..\Utility\Constants.h"

namespace Plan
{
/**
 *		//////////////////////////////////////////////////
 *				Trajectory�N���X
 *		//////////////////////////////////////////////////
 */
class Trajectory
{
/**
 *		//////////////////////////////////////////////////
 *				�����o�ϐ�
 *		//////////////////////////////////////////////////
 */
protected:
/**
 *		�����Ɋ֌W�������
 */
	/// �X�^�[�g�ʒu
	Math::Vector startPosition;
	/// �S�[���ʒu
	Math::Vector goalPosition;

/**
 *		���ԂɊ֌W�������
 */
	/// �X�^�[�g����
	double startTime;
	/// �S�[������
	double goalTime;

private:

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
	Trajectory();
	/// �R�s�[�R���X�g���N�^
	Trajectory(const Trajectory& trajectory);
	/// �f�X�g���N�^
	virtual ~Trajectory();

/**
 *		���Z�q
 */
	/// ������Z�q
	virtual Trajectory& operator=(const Trajectory& trajectory);
	/// �������Z�q
	bool operator==(const Trajectory& trajectory) const;
	bool operator!=(const Trajectory& trajectory) const;

/**
 *		�A�N�Z�X�֐�
 */
	/// �����Ɋ֌W�������
	const Math::Vector& getStartPosition(void) const {return startPosition;}
			Math::Vector& getStartPosition(void) {return startPosition;}
	const Math::Vector& getGoalPosition(void) const {return goalPosition;}
			Math::Vector& getGoalPosition(void)  {return goalPosition;}

	/// �ړ�����
	const Math::Vector getDistance(void) const{return (goalPosition - startPosition);}
			Math::Vector getDistance(void) {return (goalPosition - startPosition);}

	/// ���ԂɊ֌W�������
	const double getStartTime(void) const {return startTime;}
			double getStartTime(void) {return startTime;}
	const double getGoalTime(void) const {return goalTime;}
			double getGoalTime(void) {return goalTime;}

	/// ���O������
	double getTime(void) const {return (goalTime - startTime);}
	
/**
 *		�Z�b�g�֐�
 */
/**
 *		�O���p�����[�^�̐ݒ�
 */
	void setDistance(const Math::Vector& start, const Math::Vector& goal);
	void setTime(double start, double goal);


/**
 *		������
 */
	void initializeTrajectory(void);

/**
 *		�O���p�����[�^�̈ڍs�i�X�^�[�g�����炷�j
 */
	void shiftStartPosition(const Math::Vector& start);
	void shiftStartTime(double start);


/**
 *		�O���𓾂�
 */
	virtual Math::Vector getPosition(double splitTime);

private:
	/// �R�s�[�R���X�g���N�^�Ƒ�����Z�q�̂��߂̃w���v�֐�
	void copy(const Trajectory& trajectory);

};	/// end of class Trajectory

}	/// end of namespace Plan

#endif	/// __Trajectory_h__