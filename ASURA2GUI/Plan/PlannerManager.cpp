/**
 *  �t�@�C����
 *		PlannerManager.cpp
 *  ����
 *		����v����Ǘ�����N���X�̐錾��
 *  ���t
 *		�쐬��: 2007/09/01(SAT)		�X�V��: 2008/10/04(SAT)
 */


#include "PlannerManager.h"
#include "iostream"

using namespace std;

namespace Plan
{

/**
 *	----------------------------------------------------------------------
 *		PlannerManager�N���X
 *	----------------------------------------------------------------------
 */

/**
 *	------------------------------------------------------------
 *		PlannerManager�N���X�̃����o�֐���`
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	�R���X�g���N�^�ƃf�X�g���N�^
 *	----------------------------------------
 */
/// �f�t�H���g�R���X�g���N�^
PlannerManager::PlannerManager() : plannerPointer(NULL)
{
}

/// �f�X�g���N�^
PlannerManager::~PlannerManager()
{
	plannerPointer = NULL;
}

/**
 *	�ړI�̓����ؑ�
 */
bool PlannerManager::switchPlan(Planner* planner, Plan::Strategy strategy)
{
	if ( strategy == Plan::NO_STRATEGY )
	{
		/// �v����Ȃ���
		plannerPointer = NULL;

		/// �v��̋L�^
		lastStrategy = currentStrategy;
		currentStrategy = strategy;
		
		return true;
	}

	/// �����̃`�F�b�N
	if ( planner == NULL )
	{
		cerr << "Error: [PlannerManager::switchPlan] Invalid argument" << endl;
		return false;
	}

	/// �v��̐ؑւ�
	plannerPointer = planner;

	/// �v��̋L�^
	lastStrategy = currentStrategy;
	currentStrategy = strategy;

	return true;
}

/*
 *	���s�����ǂ����̔���
 */
bool PlannerManager::isActive(void)
{
	if ( plannerPointer == NULL ) return false;

	return plannerPointer->isActive();
}

/**
 *	�^������̓I�ɐ�������֐��Q
 */
/// �v��J�n�̂��߂̏�����
bool PlannerManager::setup(void)
{
	if ( plannerPointer == NULL ) return false;
	if (currentStrategy == TRIPOD_1_CYCLE)  //20201005
	{
		plannerPointer->standByForStop();
	}
	return plannerPointer->setup();
}

/// �v����J�n����
bool PlannerManager::startMotion(void)
{
	if ( plannerPointer == NULL ) return false;

	return plannerPointer->startPlan();
}
	
/// �v����~����
bool PlannerManager::stopMotion(void)
{
	if ( plannerPointer == NULL ) return false;

	return plannerPointer->stopPlan();
}

/// ��~�̑ҋ@��Ԃɂ���
bool PlannerManager::standByForStop(void)
{
	if ( plannerPointer == NULL ) return false;

	return plannerPointer->standByForStop();
}

/// �^���𐶐�����
PlanStatus PlannerManager::activateRobot(void)
{
	if ( plannerPointer == NULL ) return Plan::INVALID;

	return plannerPointer->activateRobot();
}

/// ���{�b�g�̏u�Ԃ̏�Ԃ𐶐�����
PlanStatus PlannerManager::createSnapshot(void)
{
	if ( plannerPointer == NULL ) return Plan::INVALID;

	return plannerPointer->createPlanSnapshot();
}


}