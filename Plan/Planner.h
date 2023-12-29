/**
 *  �t�@�C����
 *		Planner.h
 *  ����
 *		�ΏۂƂ���WalkingRobot�I�u�W�F�N�g�̊�{����𐶐�����N���X
 *  ���t
 *		�쐬��: 2007/03/06(TUE)		�X�V��: 2007/03/12(MON)
 */

//  20200819  ����]�T�֘A�R�����g�A�E�g
//  20201020  �����~��̍ē���

#ifndef __Planner_h__
#define __Planner_h__

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include<stdio.h>
#include "..\Math\Matrix\Matrix.h"
#include "..\Utility\Constants.h"
#include "..\Kinematics\AsuraX.h"

#include "PlanParameter.h"
#include "TimeManager.h"

#include "Trajectory\LegTrajectory.h"
#include "Trajectory\BodyTrajectory.h"

namespace Plan
{
/**
 *	----------------------------------------------------------------------
 *		Planner�N���X
 *	----------------------------------------------------------------------
 */
class Planner
{
/**
 *	------------------------------------------------------------
 *		�����o�ϐ�
 *	------------------------------------------------------------
 */
protected:
/**
 *		���䂷�郍�{�b�g�I�u�W�F�N�g�ւ̃|�C���^
 */
	Asura::AsuraX* asuraPointer;

/**
 *		���Ԃ��擾���邽�߂̎��ԊǗ��I�u�W�F�N�g�ւ̃|�C���^
 */
	TimeManager*	timeManagerPointer;

/**
 *		�O���I�u�W�F�N�g
 */
	/// �r�O��
	LegTrajectory* legTrajectory;
	/// ���̋O��
	BodyTrajectory* bodyTrajectory;

/**
 *		�v��ɗp����ϐ�
 */
	/// ����]�T
	//double stabilityMargin;  20200819
	/// �s���J�n����̌o�ߎ���
	double elapsedTime;
	/// �v�����j���O�J�n����
	double planStartTime;

	//20201020  ��~���̌o�ߎ���
	double stopElapsedTime;
/**
 *		���쐧��̂��߂̃t���O
 */
	bool isRunning;
	bool isSuspended;


private:
	
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
	explicit Planner(Asura::AsuraX* asuraPointer_ = NULL, TimeManager* timeManagerPointer_ = NULL);

	/// �f�X�g���N�^
	virtual ~Planner();

/**
 *	----------------------------------------
 *	�|�C���^�̑���
 *	----------------------------------------
 */
	/// ���䂷����s���{�b�g�ւ̃|�C���^���擾����
	void acquireAsura(Asura::AsuraX* ptr);
	/// ���䂷����s���{�b�g�ւ̃|�C���^���������
	void releaseAsura(void);

	/// �g�p���鎞�ԊǗ��I�u�W�F�N�g�ւ̃|�C���^���擾����
	void acquireTimeManager(TimeManager* ptr);
	/// �g�p���鎞�ԊǗ��I�u�W�F�N�g�ւ̃|�C���^���������
	void releaseTimeManager(void);

/**
 *	----------------------------------------
 *	�A�N�Z�X�֐�
 *	----------------------------------------
 */
	/**
	 *		�����o�ϐ��̎擾
	 */
	/// ����]�T
	//double getStabilityMargin(void) const{return stabilityMargin;}  20200819
	/// �v��J�n����̌o�ߎ���
	double getElapsedTime(void) const{return elapsedTime;}
	/// �J�n����
	double getPlanStartTime(void) const{return planStartTime;}

	/**
	 *		�r�O�����̂��̂𓾂�
	 */
	LegTrajectory& getLegTrajectory(int legNo)
	{
		assert( 1 <= legNo && legNo <= Asura::LEG_NUM ); 
		
		return legTrajectory[legNo-1];
	}

	/// const�I�u�W�F�N�g�ɑ΂���Y�������Z�q
	LegTrajectory& getLegTrajectory(int legNo) const
	{
		assert( 1 <= legNo && legNo <= Asura::LEG_NUM ); 
			
			return legTrajectory[legNo-1];
	}

/**
 *	----------------------------------------
 *	�Z�b�g�֐�
 *	----------------------------------------
 */
	/// ����]�T
	//void setStabilityMargin(double margin) {stabilityMargin = margin; return;}  20200819
	/// �o�ߎ���
	void setElapsedTime(double time) {elapsedTime = time; return;}
	/// �J�n����
	void setPlanStartTime(double time){planStartTime = time; return;}

/**
 *	----------------------------------------
 *	�O���p�����[�^�̐ݒ�
 *	----------------------------------------
 */ 
	/**
	 *		�V�r�O����ݒ肷��
	 *			legNo�F�V�r
	 *			upPhase�FUP��
	 *			returnPhase�F���A��
	 *			downPhase�FDOWN��
	 */
	void setLegSwingTrajectory(	int legNo, 
											const Math::Vector& start,
											const Math::Vector& upPhase,
											const Math::Vector& returnPhase, 
											const Math::Vector& downPhase
											);

	/**
	 *		�V�r���Ԃ�ݒ肷��
	 *			legNo�F�V�r
	 *			upPhase�FUP��
	 *			returnPhase�F���A��
	 *			downPhase�FDOWN��
	 */
	void setLegSwingTime(	int legNo, 
									double start,
									double upPhase, 
									double returnPhase, 
									double downPhase
									);
	/**
	 *		�V�r�̋O����̈ʒu���擾����
	 */
	Math::Vector getLegSwingPosition(int legNo, double splitTime);

/**
 *	----------------------------------------
 *	���ۂɓ�����s���֐�
 *	----------------------------------------
 */
	/**
	 *		�V�r����
	 */
	PlanStatus swingLeg(int legNo, double splitTime);

	/**
	 *		���̐��i
	*/
	PlanStatus moveBody(double splitTime);

	/**
	 *		�����~
	 */
	void suspendPlan(void) { isSuspended = true; return; }

	/**
	 *		����ĊJ
	 */
	void resumePlan(void) { isSuspended = false; return; }

/**
 *		�t���O�̔���
 */
	bool isActive(void) const{return isRunning;}

/**
 *		�G���[���b�Z�[�W�̏o��
 */
	void printPlanErrorMessage(void);

/**
 *	----------------------------------------
 *	�^������̓I�ɐ�������֐��Q
 *	----------------------------------------
 */
	/// ���s�J�n�̂��߂̏�����
	virtual bool setup(void);

	/// ���s���J�n����
	virtual bool startPlan(void);
	
	/// ���s���~����
	virtual bool stopPlan(void);

	/// ���s��~�̑ҋ@��Ԃɂ���
	virtual bool standByForStop(void);

	/// ���{�b�g���쓮����
	virtual PlanStatus activateRobot(void)=0;

	/// ���{�b�g�̏u�Ԃ̏�Ԃ𐶐�����
	/// �w�ߒl�����������ƂɌĂяo�����Ƃɂ�胍�{�b�g�̘A���ȓ�����v�悷��
	virtual PlanStatus createPlanSnapshot(void)=0;

	/**
	 *		����]�T�̌v�Z
	 *			����: �d�͕�������
	 */
	//virtual double calculateStabilityMargin(double gx=0.0, double gy=0.0, double gz=1.0);  20200819

private:
	/// �R�s�[�R���X�g���N�^����
	Planner(const Planner& planner);
	/// ������Z�q����
	Planner& operator=(const Planner& planner)=delete;

/**
 *		�O���I�u�W�F�N�g
 */
	/// �O���I�u�W�F�N�g�̐���
	void newTrajectories(void);
	/// �O���I�u�W�F�N�g�̔j��
	void deleteTrajectories(void);

/**
 *		����]�T�̌v�Z�̂��߂̃w���v�֐�
 */
	/*  20200819
	/// �x���r�O�p�`���̈���]�T
	double stabilityMarginForTriangle(const Math::Vector& foot1, const Math::Vector& foot2, const Math::Vector& foot3, 
												const Math::Vector& cog, const Math::Vector& gravity);
	/// �x���r�l�p�`���̈���]�T
	double stabilityMarginForQuadrangle(const Math::Vector& foot1, const Math::Vector& foot2, 
													const Math::Vector& foot3, const Math::Vector& foot4, 
													const Math::Vector& cog, const Math::Vector& gravity);
	*/

};	/// end of class Planner

}	/// end of namespace Plan

#endif	/// __Planner_h__

