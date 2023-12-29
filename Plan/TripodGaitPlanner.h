/**
 *  �t�@�C����
 *		TripodGaitPlanner.h
 *  ����
 *		����e�v��N���X�i�g���b�g���e�ł̕��s�j
 *  ���t
 *		�쐬��: 2008/12/19(FRI)		�X�V��: 2008/12/19(FRI)
 */

 //  20200820  TrotGait��TripodGait�ɒu��
//  20200929  �x���r���̂��߂�z��������ǉ�
//  20201005  1�����ŏI��
//  20201016  ���s�J�n���̎p���ւ̃Z�b�g
//  20201017  ���s�J�n���̎p���ւ̃Z�b�g
//  20201020  �����~��̍ē���
 
#ifndef __TripodGaitPlanner_h__
#define __TripodGaitPlanner_h__

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include "PlanParameter.h"
#include "Planner.h"
#include "..\Kinematics\AsuraX.h"
#include "..\Math\MathLibrary.h"

namespace Plan
{
/**
 *				���W�n�̒�`
 *
 *		 �E��n					
 *									
 *						 y		
 *		x  1			4��		
 *	   ����	|-----------|����	
 *	     ��	|	  x		|   x			 
 *	     y	|	  ��	|		
 *			|  ����		|		
 *			|    y		|y		
 *	    x	|			|��	
 *	   ����	|-----------|����	
 *	     ��2			3   x	
 *	     y				
 */

/**
 *	----------------------------------------------------------------------
 *		TripodGaitPlanner�N���X
 *	----------------------------------------------------------------------
 */
	class TripodGaitPlanner : public Planner
	{
		/**
		 *	------------------------------------------------------------
		 *		�����o�ϐ�
		 *	------------------------------------------------------------
		 */
	public:

	private:
		/**
		 *	�������e�Ɋ֌W�������
		 */

		 /**
		  *	���s����
		  *		�P�ʁF�b
		  */
		double cycleTime;

		/**
		 *	�f���[�e�B��
		 *		1���s�������ɂ����邠��r�̎x���r���Ԃ̔䗦
		 */
		double dutyFactor;

		/**
		 *	�X�g���C�h
		 *		1�������ɓ��̂��ړ����鋗��
		 *		�X�g���[�N�̓f���[�e�B�� x �X�g���C�h
		 */
		double stride;

		/**
		 *	���s���x
		 */
		double walkingSpeed;

		/**
		 *	���s�񐔂̃J�E���^
		 */
		int walkingCounter;

		/**
		 *	���s�����̒P�ʃx�N�g��
		 */
		Math::Vector unitWalkingDirection;

		//20200929  �x���r���Ɏg�p����z�������̒P�ʃx�N�g��
		Math::Vector unitUpDirection;

		/**
		 *	�r��ʒu
		 *		�r�̗V�r�^���O�Ղ̒��_�ʒu
		 */
		Math::Vector* footReferencePosition;


		/**
		 *	���s���s�����߂ɕK�v�ȃp�����[�^�Q
		 */
		 /// ���̂̏����ʒu
		Math::Vector initialBodyPosition;

		/// ���̏����ʒu
		Math::Vector* initialFootPosition;

		/// �V�r�J�n�ʒu
		Math::Vector* swingStartPosition;
		/// �V�r�I���ʒu
		Math::Vector* swingStopPosition;

		/// �V�r�U��グ
		Math::Vector swingUp;
		/// �V�r�U�艺��
		Math::Vector swingDown;

		/// ���s���ԁi0 < t < �����j
		double walkingTime;
		/// ���s�����J�n����
		double cycleStartTime;
		/// ���K���������s���ԁi0 < t < 1�j
		double normalizedWalkingTime;

		/// �V�r�J�n����
		double* swingStartTime;
		double* swingStopTime;

		//20201020  ��~���̕��s�����̌o�ߎ���
		double cycleElapsedTime;

		/**
		 *	���s�̂��߂̃t���O�Q
		 */
		 /// ���s�̏���������K�v�����邩�ǂ����̃t���O
		bool isWalkingToGetSet;

		/// �r�O�������肳��Ă��邩�ǂ����̃t���O
		bool isTrajectoryToGetSet;

		/// ���s�J�n�̃t���O
		bool isWalkingStarted;

		/// ���s��~�ҋ@�t���O
		bool isWaitingToStop;

		//20201016
		//  �p���ڍs�̃t���O
		bool isSetting;

		//20201017
		//  �p���ڍs�ɗp����ϐ�
		Math::Vector* initialJointAngle;  //�ڍs�O�̊֐ߊp�x
		Math::Vector* finalJointAngle;  //�ڍs��̊֐ߊp�x
		Math::Vector* settingJointAngle;  //�ڍs���̊֐ߊp�x
		//[i][j]��(i+1)�Ԗڂ̋r�̑�(j+1)�֐߂̊p�x������
		double* initialFootJointAngle;  //�ڍs�O�̊֐ߊp�x
		double* finalFootJointAngle;  //�ڍs��̊֐ߊp�x
		double* settingFootJointAngle;  //�ڍs���̊֐ߊp�x

		Math::Vector* angularVelosity;  //�p���ڍs���̊p���x

		//���ꂼ��̊֐߂𓮂����I���܂ł̏��v����
		double* t01;
		double* t02;
		double* t03;
		double* t04;
		double* t05;

		double settingStartTime;  //�p���ڍs�J�n����
		double settingTime;  //�p���ڍs�J�n����̌o�ߎ���

/**
 *	���s�ɋ@�\�ǉ����邽�߂̕ϐ�
 */
	/// ���s��~���O�̗V�r
	int swingLegWaitingToStop;

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
	explicit TripodGaitPlanner(Asura::AsuraX* asuraPointer_ = NULL, TimeManager* timeManagerPointer_ = NULL);
	/// �f�X�g���N�^
	virtual ~TripodGaitPlanner();

/**
 *	����e�̏�����
 */
	void initializeTripodGaitPlanner(void);

/**
 *	���s���������邽�߂̊֐��Q
 */
	/// ���e�p�����[�^�̌v�Z
	void calculateGaitParameters(void);

	/// ���s�����p���Ɉڍs
	bool shiftToInitialStandingPosture(void);

/**
 *	------------------------------------------------------------
 *	�I�[�o�[���C�h�֐�
 *		�^������̓I�ɐ�������֐��Q
 *	------------------------------------------------------------
 */
	/// ���s�J�n�̂��߂̏�����
	virtual bool setup(void);

	/// ���s���J�n����
	virtual bool startPlan(void);	
	
	/// ���s�𑦍��ɒ�~����
	virtual bool stopPlan(void);

	/// ���s��1���I�����ɒ�~����
	virtual bool standByForStop(void);

	/// �r�^���𐶐�����
	virtual PlanStatus activateRobot(void);

	/// ���{�b�g�̏u�Ԃ̏�Ԃ𐶐�����
	/// �w�ߒl�����������ƂɌĂяo�����Ƃɂ�胍�{�b�g�̘A���ȓ�����v�悷��
	virtual PlanStatus createPlanSnapshot(void);

	//20201016  �p���̈ڍs
	virtual bool settingPlan(void);

/**
 *	���s���J�n�������ǂ���
 */
	bool isWalking(void) const{return isWalkingStarted;}

private:
	/// �R�s�[�R���X�g���N�^����
	TripodGaitPlanner(const TripodGaitPlanner& tripodtGaitPlanner);
	/// ������Z�q����
	TripodGaitPlanner& operator=(const TripodGaitPlanner& tripodGaitPlanner);

	/// ���s�̂��߂̃I�u�W�F�N�g����
	void newTripodGaitItems(void);
	/// ���s�̂��߂̃I�u�W�F�N�g����
	void deleteTripodGaitItems(void);
};

}

#endif	/// __TripodGaitPlanner_h__ 