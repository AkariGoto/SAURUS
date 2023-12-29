/**
 *  �t�@�C����
 *		AsuraData.h
 *  ����
 *		TITAN X�̃f�[�^�i�L�l�}�e�B�N�X�C�N���[���Ȃǁj
 *  ���t
 *		�쐬��: 2007/02/12(Mon)		�X�V��: 2007/02/19(Mon)
 */

#ifndef __AsuraData_h__
#define __AsuraData_h__

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include "..\Math\Matrix\Matrix.h"
#include "..\Kinematics\AsuraParameter.h"
#include "..\Utility\Constants.h"

namespace Data
{

/**
 *	----------------------------------------------------------------------
 *		AsuraData�N���X
 *	----------------------------------------------------------------------
 */
class AsuraData
{
/**
 *	------------------------------------------------------------
 *		�����o�ϐ�
 *	------------------------------------------------------------
 */
private:
/**
 *	----------------------------------------
 *	����
 *	----------------------------------------
 */
	/// �����ϊ��s��
	Math::Matrix bodyTransformation;

	/// ���̈ʒu
	Math::Vector bodyPosition;

	/// ���̑��x
	Math::Vector bodyVelocity;

	/// ���s�����x�N�g��
	Math::Vector walkingDirection;

	/// �ړ��l��
	Asura::LocomotionStyle locomotionStyle;

/**
 *	----------------------------------------
 *	�r
 *	----------------------------------------
 */
	/// �r�����̓����ϊ��s��
	Math::Matrix* legBaseTransformation;
	
	/// �r�֐߂��Ƃ̓����ϊ��s��
	Math::Matrix** legJointTransformation;

	/// ���ʒu�̓����ϊ��s��
	Math::Matrix* legFootTransformation;

	/// �r�����̊֐߈ʒu
	Math::Vector* legBasePosition;

	/// �r�̊֐߈ʒu
	Math::Vector** legJointPosition;

	/// ���ʒu
	Math::Vector* legFootPosition;

	/// �r�̊֐ߊp�x
	Math::Vector* legJointAngle;

	/// �r�̊֐ߑ��x
	Math::Vector* legJointVelocity;

	/// �r�̊֐߃g���N
	Math::Vector* legJointTorque;

	/// �r�̉^����
	Asura::LegPhase* legPhase;
	/// ����֐ߊp�x: [6]
	Math::Vector FootJointAngle;//---Add

	/// �r�̃{�[���˂��ʒu�i���C���ψʁj
	Math::Vector* legActuatorPosition;
/**
 *	----------------------------------------
 *	�N���[��
 *	----------------------------------------
 */
	/// �N���[�������̓����ϊ��s��
	Math::Matrix* trackBaseTransformation;

	/// �N���[���֐߂��Ƃ̓����ϊ��s��
	Math::Matrix** trackJointTransformation;

	/// �N���[����[�̓����ϊ��s��
	Math::Matrix* trackEndTransformation;

	/// �N���[�������ʒu
	Math::Vector* trackBasePosition;

	/// �N���[���̊֐߈ʒu
	Math::Vector** trackJointPosition;

	/// �N���[����[�̈ʒu
	Math::Vector* trackEndPosition;



	/// �N���[���̊֐ߊp�x
	Math::Vector* trackJointAngle;

	/// �N���[���̊֐ߑ��x
	Math::Vector* trackJointVelocity;

	/// �N���[���̊֐߃g���N
	Math::Vector* trackJointTorque;

	/// �N���[�����x
	double* trackSpeed;

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
	AsuraData();

	/// �R�s�[�R���X�g���N�^
	AsuraData(const AsuraData& asuraData);

	/// �f�X�g���N�^
	virtual ~AsuraData();

	/**
 *	�N���X���g�𕡐�����
 */
	AsuraData& clone(void);

/**
 *	----------------------------------------
 *	���Z�q
 *	----------------------------------------
 */
	/// ������Z�q
	virtual AsuraData& operator=(const AsuraData& asuraData);

	/// �������Z�q
	bool operator==(const AsuraData& asuraData);

	/// �񓙉����Z�q
	bool operator!=(const AsuraData& asuraData);



/**
 *	----------------------------------------
 *	�A�N�Z�X�֐�
 *	----------------------------------------
 */
	/**
	 *	����
	 */
	/// �����ϊ��s��
	const Math::Matrix& getBodyTransformation(void) const{return bodyTransformation;}
	/// ���̈ʒu
	const Math::Vector& getBodyPosition(void) const{return bodyPosition;}
	/// ���̑��x
	const Math::Vector& getBodyVelocity(void) const{return bodyVelocity;}
	/// ���s�����x�N�g��
	const Math::Vector& getWalkingDirection(void) const{return walkingDirection;}
	/// �ړ��l��
	const Asura::LocomotionStyle getLocomotionStyle(void) const{return locomotionStyle;}

	/**
	 *	�r
	 */
	/// �r�����̓����ϊ��s��
	const Math::Matrix& getLegBaseTransformation(int legNo) const
	{
		return legBaseTransformation[legNo-1];
	}
	/// �r�֐߂��Ƃ̓����ϊ��s��
	const Math::Matrix& getLegJointTransformation(int legNo, int jointNo) const
	{
		return legJointTransformation[legNo-1][jointNo-1];
	}
	/// ���ʒu���Ƃ̓����ϊ��s��
	const Math::Matrix& getLegFootTransformation(int legNo) const
	{
		return legFootTransformation[legNo-1];
	}
	/// �r�����̊֐߈ʒu
	const Math::Vector& getLegBasePosition(int legNo) const
	{
		return legBasePosition[legNo-1];
	}
	/// �r�̊֐߈ʒu
	const Math::Vector& getLegJointPosition(int legNo, int jointNo) const
	{
		return legJointPosition[legNo-1][jointNo-1];
	}
	/// ���ʒu
	const Math::Vector& getLegFootPosition(int legNo) const
	{
		return legFootPosition[legNo-1];
	}
	/// �r�̊֐ߊp�x
	const Math::Vector& getLegJointAngle(int legNo) const
	{
		return legJointAngle[legNo-1];
	}
	/// �r�̊֐ߑ��x
	const Math::Vector& getLegJointVelocity(int legNo) const
	{
		return legJointVelocity[legNo-1];
	}
	/// �r�̊֐߃g���N
	const Math::Vector& getLegJointTorque(int legNo) const
	{
		return legJointTorque[legNo-1];
	}
	/// �r�̉^����
	const Asura::LegPhase getLegPhase(int legNo) const
	{
		return legPhase[legNo-1];
	}
	/// ����֐ߊp�x
	/*
	const Math::Vector& getFootointAngle(int legNo) const//---Add
	{
		return FootJointAngle[legNo-1];
	}
	*/
	/// �r�̃{�[���˂����W�i���C���ψʁj
	const Math::Vector& getLegActuatorPosition(int legNo) const
	{
		return legActuatorPosition[legNo-1];
	}
	/**
	 *	�N���[��
	 */
	/// �N���[�������̓����ϊ��s��
	const Math::Matrix& getTrackBaseTransformation(int trackNo) const
	{
		return trackBaseTransformation[trackNo-1];
	}
	/// �N���[���֐߂��Ƃ̓����ϊ��s��
	const Math::Matrix& getTrackJointTransformation(int trackNo, int jointNo) const
	{
		return trackJointTransformation[trackNo-1][jointNo-1];
	}
	/// �N���[����[�̓����ϊ��s��
	const Math::Matrix& getTrackEndTransformation(int trackNo) const
	{
		return trackEndTransformation[trackNo-1];
	}
	/// �N���[�������ʒu
	const Math::Vector& getTrackBasePosition(int trackNo) const
	{
		return trackBasePosition[trackNo-1];
	}
	/// �N���[���̊֐߈ʒu
	const Math::Vector& getTrackJointPosition(int trackNo, int jointNo) const
	{
		return trackJointPosition[trackNo-1][jointNo-1];
	}
	/// �N���[����[�̈ʒu
	const Math::Vector& getTrackEndPosition(int trackNo) const
	{
		return trackEndPosition[trackNo-1];
	}
	/// �N���[���̊֐ߊp�x
	const Math::Vector& getTrackJointAngle(int trackNo) const
	{
		return trackJointAngle[trackNo-1];
	}
	/// �N���[���̊֐ߑ��x
	const Math::Vector& getTrackJointVelocity(int trackNo) const
	{
		return trackJointVelocity[trackNo-1];
	}
	/// �N���[���̊֐߃g���N
	const Math::Vector& getTrackJointTorque(int trackNo) const
	{
		return trackJointTorque[trackNo-1];
	}
	/// �N���[�����x
	double getTrackSpeed(int trackNo) const
	{
		return trackSpeed[trackNo-1];
	}

/**
 *	----------------------------------------
 *	�Z�b�g�֐�
 *	----------------------------------------
 */
	/**
	 *		����
	 */
	/// �����ϊ��s��
	void setBodyTransformation(const Math::Matrix& transformation)
	{
		bodyTransformation = transformation;
	}
	/// ���̈ʒu
	void setBodyPosition(const Math::Vector& position)
	{
		bodyPosition = position;
	}
	/// ���̑��x
	void setBodyVelocity(const Math::Vector& velocity)
	{
		bodyVelocity = velocity;
	}
	/// ���s�����x�N�g��
	void setWalkingDirection(const Math::Vector& direction)
	{
		walkingDirection = direction;
	}
	/// �ړ��l��
	void setLocomotionStyle(Asura::LocomotionStyle style)
	{
		locomotionStyle = style;
	}

	/**
	 *	�r
	 */
	/// �r�����̓����ϊ��s��
	void setLegBaseTransformation(int legNo, const Math::Matrix& transformation)
	{
		legBaseTransformation[legNo-1] = transformation;
	}
	/// �r�֐߂��Ƃ̓����ϊ��s��
	void setLegJointTransformation(int legNo, int jointNo, const Math::Matrix& transformation)
	{
		legJointTransformation[legNo-1][jointNo-1] = transformation;
	}
	/// ���ʒu���Ƃ̓����ϊ��s��
	void setLegFootTransformation(int legNo, const Math::Matrix& transformation)
	{
		legFootTransformation[legNo-1] = transformation;
	}
	/// �r�����̊֐߈ʒu
	void setLegBasePosition(int legNo, const Math::Vector& position)
	{
		legBasePosition[legNo-1] = position;
	}
	/// �r�̊֐߈ʒu
	void setLegJointPosition(int legNo, int jointNo, const Math::Vector& position)
	{
		legJointPosition[legNo-1][jointNo-1] = position;
	}
	/// ���̈ʒu
	void setLegFootPosition(int legNo, const Math::Vector& position)
	{
		legFootPosition[legNo-1] = position;
	}
	/// �r�̊֐ߊp�x
	void setLegJointAngle(int legNo, const Math::Vector& angle)
	{
		legJointAngle[legNo-1] = angle;
	}
	/// �r�̊֐ߑ��x
	void setLegJointVelocity(int legNo, const Math::Vector& velocity)
	{
		legJointVelocity[legNo-1] = velocity;
	}
	/// �r�̊֐߃g���N
	void setLegJointTorque(int legNo, const Math::Vector& torque)
	{
		legJointTorque[legNo-1] = torque;
	}
	/// �r�̉^����
	void setLegPhase(int legNo, Asura::LegPhase phase)
	{
		legPhase[legNo-1] = phase;
	}
	///����̊֐ߊp�x
	//*
	void setFootJointAngle(int legNo, const double JointAngle)//-----Add
	{
		//FootJointAngle[legNo-1] = (Math::Vector)JointAngle;
		FootJointAngle[legNo-1] = JointAngle;
	}//*/
	/// �r�̃A�N�`���G�[�^���W
	void setLegActuatorPosition(int legNo, const Math::Vector& ActPos)
	{
		legActuatorPosition[legNo-1] = ActPos;
	}
	/**
	 *	�N���[��
	 */
	/// �N���[�������̓����ϊ��s��
	void setTrackBaseTransformation(int trackNo, const Math::Matrix& transformation)
	{
		trackBaseTransformation[trackNo-1] = transformation;
	}
	/// �N���[���֐߂��Ƃ̓����ϊ��s��
	void setTrackJointTransformation(int trackNo, int jointNo, const Math::Matrix& transformation)
	{
		trackJointTransformation[trackNo-1][jointNo-1] = transformation;
	}
	/// �N���[����[�̓����ϊ��s��
	void setTrackEndTransformation(int trackNo, const Math::Matrix& transformation)
	{
		trackEndTransformation[trackNo-1] = transformation;
	}
	/// �N���[�������ʒu
	void setTrackBasePosition(int trackNo, const Math::Vector& position)
	{
		trackBasePosition[trackNo-1] = position;
	}
	/// �N���[���̊֐߈ʒu
	void setTrackJointPosition(int trackNo, int jointNo, const Math::Vector& position)
	{
		trackJointPosition[trackNo-1][jointNo-1] = position;
	}
	/// �N���[����[�̈ʒu
	void setTrackEndPosition(int trackNo, const Math::Vector& position)
	{
		trackEndPosition[trackNo-1] = position;
	}
	/// �N���[���̊֐ߊp�x
	void setTrackJointAngle(int trackNo, const Math::Vector& angle)
	{
		trackJointAngle[trackNo-1] = angle;
	}
	/// �N���[���̊֐ߑ��x
	void setTrackJointVelocity(int trackNo, const Math::Vector& velocity)
	{
		trackJointVelocity[trackNo-1] = velocity;
	}
	/// �N���[���̊֐߃g���N
	void setTrackJointTorque(int trackNo, const Math::Vector& torque)
	{
		trackJointTorque[trackNo-1] = torque;
	}
	/// �N���[�����x
	void setTrackSpeed(int trackNo, double speed)
	{
		trackSpeed[trackNo-1] = speed;
	}

private:
    /// �I�u�W�F�N�g�̃������̈���m�ۂ���
	void newAsuraData(void);

	/// �I�u�W�F�N�g�̃������̈���������
    void deleteAsuraData(void);

	/// �R�s�[�R���X�g���N�^�Ƒ�����Z�q�̃w���v�֐�
	void copy(const AsuraData& asuraData);

};	/// end of class AsuraData

}	/// end of namespace Data

#endif /// __AsuraData_h__