/**
 *  �t�@�C����
 *		PlannerManager.h
 *  ����
 *		����v����Ǘ�����N���X�̐錾��
 *  ���t
 *		�쐬��: 2007/09/01(SAT)		�X�V��: 2008/10/04(SAT)
 */

#ifndef __PlannerManager_h__
#define __PlannerManager_h__

#include "PlanParameter.h"
#include "Planner.h"

/**
 *	State�f�U�C���p�^�[���ō\��
 */

namespace Plan
{

/**
 *	----------------------------------------------------------------------
 *		PlannerManager�N���X
 *	----------------------------------------------------------------------
 */
class PlannerManager
{
/**
 *	------------------------------------------------------------
 *		�����o�ϐ�
 *	------------------------------------------------------------
 */
public:

private:
/**
 *		�ړI�̓���v��ւ̃|�C���^
 */
	Planner*	plannerPointer;

/**
 *		����v��
 */
	/// �O��̌v��
	Plan::Strategy lastStrategy;

	/// �I�������v��
	Plan::Strategy currentStrategy;

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
	explicit PlannerManager();
	/// �f�X�g���N�^
	virtual ~PlannerManager();

/**
 *	�ړI�̓����ؑ�
 */
	bool switchPlan(Planner* planner, Plan::Strategy strategy);

/**
 *	----------------------------------------
 *	�A�N�Z�X�֐�
 *	----------------------------------------
 */
	Plan::Strategy getCurrentStrategy(void) const{return currentStrategy;}
	Plan::Strategy getLastStrategy(void) const{return lastStrategy;}

/**
 *	���s�����ǂ���
 */
	bool isActive(void);

/**
 *	�^������̓I�ɐ�������֐��Q
 */
	/// �v��J�n�̂��߂̏�����
	bool setup(void);

	/// �v����J�n����
	bool startMotion(void);
	
	/// �v����~����
	bool stopMotion(void);

	/// ��~�̑ҋ@��Ԃɂ���
	bool standByForStop(void);

	/// �^���𐶐�����
	PlanStatus activateRobot(void);

	/// ���{�b�g�̏u�Ԃ̏�Ԃ𐶐�����
	PlanStatus createSnapshot(void);
};

}


#endif	/// __PlannerManager_h__