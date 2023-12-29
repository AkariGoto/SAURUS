/**
 *  �t�@�C����
 *		PlanData.h
 *  ����
 *		�ړ��v��̃f�[�^�i���ԁA����]�T�Ȃǁj
 *  ���t
 *		�쐬��: 2007/02/12(MON)		�X�V��: 2007/02/19(MON)
 */

//20200819  ����]�T�֘A�R�����g�A�E�g

#ifndef __PlanData_h__
#define __PlanData_h__

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include "..\Math\Matrix\Matrix.h"
#include "..\Kinematics\AsuraParameter.h"
#include "..\Plan\PlanParameter.h"
#include "..\Utility\Constants.h"

namespace Data
{

/**
 *	----------------------------------------------------------------------
 *		PlanData�N���X
 *	----------------------------------------------------------------------
 */
class PlanData
{
/**
 *	------------------------------------------------------------
 *		�����o�ϐ�
 *	------------------------------------------------------------
 */
private:

	/// ����]�T
	//double stabilityMargin;  20200819

	/// �o�ߎ���
	double elapsedTime;

	/// ���s�p
	double crabAngle;


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
	PlanData();

	/// �R�s�[�R���X�g���N�^
	PlanData(const PlanData& PlanData);

	/// �f�X�g���N�^
	virtual ~PlanData();

/**
 *	----------------------------------------
 *	���Z�q
 *	----------------------------------------
 */
	/// ������Z�q
	virtual PlanData& operator=(const PlanData& planData);

	/// �������Z�q
	bool operator==(const PlanData& planData);

	/// �񓙉����Z�q
	bool operator!=(const PlanData& planData);

/**
 *	�N���X���g�𕡐�����
 */
	//PlanData& clone(void); 20210622�֐���`�Ȃ��B�R�����g�A�E�g

/**
 *	----------------------------------------
 *	�A�N�Z�X�֐�
 *	----------------------------------------
 */
	/// ����]�T
	//double getStabilityMargin(void) const{return stabilityMargin;}  20200819
	/// ����
	double getElapsedTime(void) const{return elapsedTime;}

/**
 *	----------------------------------------
 *	�Z�b�g�֐�
 *	----------------------------------------
 */
	/// ����]�T
	//void setStabilityMargin(double margin) { stabilityMargin = margin; return; }  20200819
	/// ����
	void setElapsedTime(double time) { elapsedTime = time; return; }

private:
	/// �R�s�[�R���X�g���N�^�Ƒ�����Z�q�̃w���v�֐�
	void copy(const PlanData& planData);

};	/// end of class PlanData

}	/// end of namespace Data

#endif /// __PlanData_h__