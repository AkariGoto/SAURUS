/**
 *  File: LegTrajectory.h
 *
 *  Description: �r�O�����������N���X�̐錾��
 *					 �����`�O��
 *
 *  Created: 2007/03/06(Tue)
 *  Updated: 2007/03/06(Tue)
 *
 *  Copyright (C) Tokyo Institute of Technology Hirose Lab.
 */

#ifndef __LegTrajectory_h__
#define __LegTrajectory_h__

#include "Trajectory.h"
#include "..\..\Math\Matrix\Matrix.h"
#include "..\..\Utility\Constants.h"

namespace Plan
{
/**
 *		//////////////////////////////////////////////////
 *				LegTrajectory�N���X
 *		//////////////////////////////////////////////////
 */
class LegTrajectory : public Trajectory
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
	Math::Vector returnStroke;
	/// �V�r�U��グ����
	Math::Vector upSwing;
	/// �V�r�U�艺������
	Math::Vector downSwing;

	Math::Vector Swing;

/**
 *		���ԂɊ֌W�������
 */
	/// ���A����
	double returnTime;
	/// �U��グ����
	double upTime;
	/// �U�艺������
	double downTime;

/**
 *		//////////////////////////////////////////////////
 *				�����o�֐�
 *		//////////////////////////////////////////////////
 */
/**
 *		�R���X�g���N�^�ƃf�X�g���N�^
 */
public:
	/// �f�t�H���g�R���X�g���N�^
	LegTrajectory();
	/// �R�s�[�R���X�g���N�^
	LegTrajectory(const LegTrajectory& trajectory);
	/// �f�X�g���N�^
	virtual ~LegTrajectory();

/**
 *		���Z�q
 */
	/// ������Z�q
	virtual LegTrajectory& operator=(const LegTrajectory& trajectory);
	/// �������Z�q
	bool operator==(const LegTrajectory& trajectory) const;
	/// �񓙉����Z�q
	bool operator!=(const LegTrajectory& trajectory) const;

/**
 *		�A�N�Z�X�֐�
 */
	/// �V�r�X�g���[�N
	const Math::Vector& getReturnStroke(void) const{return returnStroke;}
	/// �V�r�U��グ����
	const Math::Vector& getUpSwing(void) const{return upSwing;}
	/// �V�r�U�艺������
	const Math::Vector& getDownSwing(void) const{return downSwing;}

	/// ���A����
	double getReturnTime(void) const{return returnTime;}
	/// �U��グ����
	double getUpTime(void) const{return upTime;}
	/// �U�艺������
	double getDownTime(void) const{return downTime;}


/**
 *		�Z�b�g�֐�
 */
/**
 *		�O���p�����[�^�̐ݒ�
 *		goalPoint�����goalTime�������X�V�����
 */
	void setLegTrajectory(	
									const Math::Vector& start, 
									const Math::Vector& upPhase, 
									const Math::Vector& returnPhase, 
									const Math::Vector& downPhase
									);

	void setLegTrajectoryTime(double start, double upPhase, double returnPhase, double downPhase);

/**
 *		������
 */
	void initializeLegTrajectory(void);

/**
 *		�O���𓾂�
 *		time: �O�����̎��ԁistartTime < splitTime < goalTime�j
 */
	Math::Vector getPosition(double splitTime);

private:
	/// �R�s�[�R���X�g���N�^�Ƒ�����Z�q�̂��߂̃w���v�֐�
	void copy(const LegTrajectory& trajectory);

};	/// end of class LegTrajectory

}	/// end of namespace lan
#endif	/// __LegTrajectory_h__