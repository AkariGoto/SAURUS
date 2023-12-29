/**
 *  �t�@�C����
 *		PlanData.cpp
 *  ����
 *		�ړ��v��̃f�[�^�i���ԁA����]�T�Ȃǁj
 *  ���t
 *		�쐬��: 2007/02/12(MON)		�X�V��: 2007/02/19(MON)
 */

 //20200819  ����]�T�֘A�R�����g�A�E�g

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include "PlanData.h"
#include "..\Utility\EngConstant.h"
 

using namespace std;
using namespace Math;
using namespace Const;
using namespace Asura;
using namespace Plan;

namespace Data
{
/**
 *	----------------------------------------------------------------------
 *		PlanData�N���X
 *	----------------------------------------------------------------------
 */

/**
 *	------------------------------------------------------------
 *		PlanData�N���X�̃����o�֐���`
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	�R���X�g���N�^�ƃf�X�g���N�^
 *	----------------------------------------
 */
/// �f�t�H���g�R���X�g���N�^
PlanData::PlanData()
{
	//stabilityMargin = 0.0;  20200819
	elapsedTime = 0.0;
	crabAngle = 0.0;
}

/// �R�s�[�R���X�g���N�^
PlanData::PlanData(const PlanData& planData)
{
	copy(planData);
}

/// �f�X�g���N�^
PlanData::~PlanData()
{
}

/**
 *	----------------------------------------
 *	���Z�q
 *	----------------------------------------
 */
/// ������Z�q
PlanData& PlanData::operator=(const PlanData& planData)
{
	/// ���ȑ���̃`�F�b�N
	if ( &planData != this )
	{
		copy(planData);
	}

	return *this;
}

/// �������Z�q
bool PlanData::operator==(const PlanData& planData)
{
	//if ( stabilityMargin != planData.stabilityMargin ){return false;}  20200819
	if ( elapsedTime != planData.elapsedTime ){return false;}

	return true;
}

/// �񓙉����Z�q
bool PlanData::operator!=(const PlanData& planData)
{
	if ( *this == planData )
		return false;
	else
		return true;
}

/**
 *	------------------------------------------------------------
 *		PlanData�N���X��private�ȃ����o�֐�
 *	------------------------------------------------------------
 */
/**
 *	����
 *		�R�s�[�R���X�g���N�^�Ƒ�����Z�q�̃w���v�֐�
 */
void PlanData::copy(const PlanData& planData)
{

	//stabilityMargin = planData.stabilityMargin;  20200819
	elapsedTime = planData.elapsedTime;

	return;
}

}	/// end of namespace Data