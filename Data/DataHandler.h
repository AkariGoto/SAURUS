/**
 *  �t�@�C����
 *		DataHandler.h
 *  ����
 *		���{�b�g�̃f�[�^�n���h��
 *  ���t
 *		�쐬��: 2007/02/12(Mon)		�X�V��: 2007/02/19(Mon)
 */
//20200819  �N���[���֘A�R�����g�A�E�g


#ifndef __DataHandler_h__
#define __DataHandler_h__

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include "..\Kinematics\AsuraX.h"
#include "..\Plan\Planner.h"
#include "AsuraData.h"
#include "PlanData.h"
#include "..\Kinematics\AsuraParameter.h"
#include "..\Plan\PlanParameter.h"

namespace Data
{

/**
 *	----------------------------------------------------------------------
 *		DataHandler�N���X
 *	----------------------------------------------------------------------
 */
class DataHandler
{
/**
 *	------------------------------------------------------------
 *		�����o�ϐ�
 *	------------------------------------------------------------
 */
public:
private:
/**
 *	�f�[�^���擾���郍�{�b�g�ւ̃|�C���^
 */
	/// �L�l�}�e�B�N�X�f�[�^
	Asura::AsuraX* asuraDataSourcePointer;
	/// �v��f�[�^
	Plan::Planner* planDataSourcePointer;


/**
 *	�f�[�^��ۑ�����f�[�^�ւ̃|�C���^
 */
	Data::AsuraData* asuraDataTargetPointer;
	Data::PlanData* planDataTargetPointer;

/**
 *	�ێ��f�[�^
 */
	/// �L�l�}�e�B�N�X
	Data::AsuraData asuraData;
	/// �v��
	Data::PlanData planData;

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
	/**
	 *	�f�t�H���g�R���X�g���N�^
	 *	����
	 *		asuraPtr: �f�[�^���󂯎��AsuraX�N���X�ւ̃|�C���^
	 *		asuraDataPtr: �f�[�^��n��AsuraData�N���X�ւ̃|�C���^
	 *		planPtr: �f�[�^���󂯎��Planner�N���X�ւ̃|�C���^
	 *		planDataPtr: �f�[�^��n��planDataPtr�N���X�ւ̃|�C���^
	 */
	explicit DataHandler(Asura::AsuraX* asuraPtr, Data::AsuraData* asuraDataPtr, 
								Plan::Planner* planPtr, Data::PlanData* planDataPtr);

	/// �f�X�g���N�^
	virtual ~DataHandler();

/**
 *	----------------------------------------
 *	�f�[�^���̃|�C���^�̎擾�E���
 *	----------------------------------------
 */
	/// �擾
	void acquireAsuraDataSource(Asura::AsuraX* asuraPointer);
	/// ���
	void releaseAsuraDataSource(void);

	/// �擾
	void acquirePlanDataSource(Plan::Planner* planPointer);
	/// ���
	void releasePlanDataSource(void);

/**
 *	----------------------------------------
 *	�f�[�^��̃|�C���^�̎擾�E���
 *	----------------------------------------
 */
	/// �擾
	void acquireAsuraDataTarget(Data::AsuraData* dataPointer);
	/// ���
	void releaseAsuraDataTarget(void);

	/// �擾
	void acquirePlanDataTarget(Data::PlanData* dataPointer);
	/// ���
	void releasePlanDataTarget(void);

/**
 *	----------------------------------------
 *	�f�[�^�̎�荞�݂Ɠ]��
 *	----------------------------------------
 */
	/// �]��
	void exportAsuraData(void);
	/// ��荞��
	void importAsuraData(void);

	/// �]��
	void exportPlanData(void);
	/// ��荞��
	void importPlanData(void);


/**
 *	�f�[�^�n��
 */
	void handleAsuraData(void);
	void handlePlanData(void);

/**
 *	----------------------------------------
 *	�A�N�Z�X�֐�
 *	----------------------------------------
 */
/**
 *	�L�l�}�e�B�N�X�f�[�^
 */
	const Data::AsuraData& getAsuraData(void) const{return asuraData;}

	/**
	 *	����
	 */
	/// �����ϊ��s��
	const Math::Matrix& getBodyTransformation(void) const {return asuraData.getBodyTransformation();}

	/// ���̈ʒu
	const Math::Vector& getBodyPosition(void) const {return asuraData.getBodyPosition();}

	/// ���̑��x
	const Math::Vector& getBodyVelocity(void) const {return asuraData.getBodyVelocity();}

	/**
	 *	�r
	 */
	/// �r�������Ƃ̓����ϊ��s��
	const Math::Matrix& getLegBaseTransformation(int legNo) const;

	/// �r�֐߂��Ƃ̓����ϊ��s��
	const Math::Matrix& getLegJointTransformation(int legNo, int jointNo) const;

	/// ���ʒu���Ƃ̓����ϊ��s��
	const Math::Matrix& getLegFootTransformation(int legNo) const;

	/// �r�̍����ʒu
	const Math::Vector& getLegBasePosition(int legNo) const;

	/// �r�̊֐߈ʒu
	const Math::Vector& getLegJointPosition(int legNo, int jointNo) const;

	/// ���ʒu
	const Math::Vector& getLegFootPosition(int legNo) const;

	/// �r�̊֐ߊp�x
	const Math::Vector& getLegJointAngle(int legNo) const;

	/// �r�̊֐ߑ��x
	const Math::Vector& getLegJointVelocity(int legNo) const;

	/// �r�̊֐߃g���N
	const Math::Vector& getLegJointTorque(int legNo) const;

	/// �r�̉^����
	const Asura::LegPhase getLegPhase(int legNo) const;

	/**
	 *	�N���[��
	 */
	/*  20200819
	/// �N���[���������Ƃ̓����ϊ��s��
	const Math::Matrix& getTrackBaseTransformation(int trackNo) const;

	/// �N���[���֐߂��Ƃ̓����ϊ��s��
	const Math::Matrix& getTrackJointTransformation(int trackNo, int jointNo) const;

	/// �N���[����[���Ƃ̓����ϊ��s��
	const Math::Matrix& getTrackEndTransformation(int trackNo) const;

	/// �N���[���̍����ʒu
	const Math::Vector& getTrackBasePosition(int trackNo) const;

	/// �N���[���̊֐߈ʒu
	const Math::Vector& getTrackJointPosition(int trackNo, int jointNo) const;

	/// �N���[���̍����ʒu
	const Math::Vector& getTrackEndPosition(int trackNo) const;

	/// �N���[���̊֐ߊp�x
	const Math::Vector& getTrackJointAngle(int trackNo) const;

	/// �N���[���̊֐ߑ��x
	const Math::Vector& getTrackJointVelocity(int trackNo) const;

	/// �N���[���̊֐߃g���N
	const Math::Vector& getTrackJointTorque(int trackNo) const;
	*/

/**
 *	�v��f�[�^
 */
	const Data::PlanData& getPlanData(void) const{return planData;}

	//const double getStabilityMargin(void) const{return planData.getStabilityMargin();}  20200819
	const double getElapsedTime(void) const{return planData.getElapsedTime();}

private:
};


}
#endif /// __DataHandler_h__