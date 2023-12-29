/**
 *  �t�@�C����
 *		WalkingRobot.h
 *  ����
 *		���s���{�b�g�̏��C�^���w
 *		LegTrack�N���X�̃R���|�W�V����
 *  ���t
 *		�쐬��: 2007/02/05(MON)		�X�V��: 2018/11/05(MON)
 */

//  20200819  �N���[���֘A�R�����g�A�E�g�ETrackLeg�������Leg���Ă�
//  20201015  ��������
//  20201018  ��4�֐�

#ifndef __WalkingRobot_h__
#define __WalkingRobot_h__

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include <cassert>
#include "..\Math\Matrix\Matrix.h"
//#include "Leg\TrackLeg.h"
#include "Leg\Leg.h"  //20200819
#include "..\Utility\Constants.h"
#include "AsuraParameter.h"
#include "..\Plan\PlanParameter.h"  //20201015

namespace Asura
{

/**
 *	----------------------------------------------------------------------
 *		WalkingRobot�N���X
 *	----------------------------------------------------------------------
 */
class WalkingRobot
{
/**
 *	------------------------------------------------------------
 *		�����N���X�̐錾
 *	------------------------------------------------------------
 */
	/**
	 *	BodyData�N���X�F���̂̏��
	 */
	class BodyData
	{
		/**
		 *	------------------------------------------------------------
		 *		�����o�ϐ�
		 *	------------------------------------------------------------
		 */
		public:
			/// �����ϊ��s��: [4x4]
			Math::Matrix transformation;

			/// ���̂̈ʒu�x�N�g���F[3]
			Math::Vector position;

			/// ���̑��x�F[3]
			Math::Vector velocity;

		private:
		/**
		 *	------------------------------------------------------------
		 *		�����o�֐�
		 *	------------------------------------------------------------
		 */
		/**
		 *	----------------------------------------
		 *	�R���X�g���N�^�ƃf�X�g���N�^
		 *	----------------------------------------
		 */
		public:
			/// �R���X�g���N�^
			BodyData();
			/// �f�X�g���N�^
			virtual ~BodyData();

		private:
		    /// �I�u�W�F�N�g�̃������̈���m�ۂ���
			void initBodyData(void);
	};

/**
 *	------------------------------------------------------------
 *		�����o�ϐ�
 *	------------------------------------------------------------
 */
public:
protected:
/**
 *		���s���{�b�g�̃f�[�^
 */
	/// ���s���{�b�g�̃f�[�^
	BodyData bodyData;

	/// �ړ��l��
	LocomotionStyle locomotionStyle;
	
/**
 *		�N���[���ό^�r�@�\�Ɋւ���ϐ�
 */
	/// �r�@�\�̃C���X�^���X
	//TrackLeg** trackLegs;
	Leg** trackLegs;  //  20200819

	/// �r�̉^�����̃C���X�^���X
	LegPhase*	legPhases;

/**
 *		�Ō�ɃG���[�ƂȂ����r�ԍ�
 *		0�͑S��OK�ŃG���[��
 */
	int lastErrorTrackLegNo;

/**
 *		�Ō�̉^���w�G���[
 */
	Kinematics lastKineError;

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
	WalkingRobot();
	explicit WalkingRobot(const Math::Vector& bodyPosition);

	/// �f�X�g���N�^
	virtual ~WalkingRobot();

	/// �������֐�
	void initializeWalkingRobot(void);
/**
 *	----------------------------------------
 *	���{�b�g�{�̂Ɋւ������
 *	----------------------------------------
 */
/**
 *	----------------------------------------
 *	�A�N�Z�X�֐�
 *	----------------------------------------
 */
	/// ���̂̓����ϊ��s��: [4x4]
	const Math::Matrix& getBodyTransformation(void) const {return bodyData.transformation;}
	/// ���̂̈ʒu�x�N�g���F[3]
	const Math::Vector& getBodyPosition(void) const {return bodyData.position;}
	/// ���̂̑��x�x�N�g���F[3]
	const Math::Vector& getBodyVelocity(void) const {return bodyData.velocity;}

	/// �ړ��l��
	const LocomotionStyle getLocomotionStyle(void) const {return locomotionStyle;}
	///�^���w�G���[
	const Kinematics getLastKineError(void) const{ return lastKineError; }

	/// �r�I�u�W�F�N�g
	//const TrackLeg& getTrackLeg(int legNo) const;    20200819
	/// �r�̉^����
	const LegPhase getLegPhase(int legNo) const;
	/// �Ō�ɃG���[���N�������r�ԍ����擾
	const int getLastErrorTrackLegNo(void) const {return lastErrorTrackLegNo;}

/**
 *	----------------------------------------
 *	�Z�b�g�֐�
 *	----------------------------------------
 */
	/// ���̂̈ʒu�����V
	void initializeBodyTransformation(const Math::Matrix& newBodyTransformation);
	/// ���̂̈ʒu�����V
	void initializeBodyPosition(const Math::Vector& newBodyPosition);
	void initializeBodyPosition(double x, double y, double z);

	/// �ړ��l��
	void setLocomotionStyle(LocomotionStyle style);
	/// �r�̉^����
	void setLegPhase(int legNo, LegPhase phase);

/**
 *	----------------------------------------
 *	�^���w
 *	----------------------------------------
 */
	/// ���̂̈ʒu�����肷��
	Kinematics placeBodyPosition(Math::Vector& nextBodyPosition);
	/// ���̂̎p�������肷��
	Kinematics placeBodyPosture(Math::Matrix& nextBodyPosture);
	/// ���̂̈ʒu�p���𓯎��Ɍ��肷��
	Kinematics placeBodyFrame(Math::Matrix& nextBodyFrame);

/**
 *	----------------------------------------
 *	���W�ϊ�
 *	----------------------------------------
 */
	/// �O���[�o�����瓷�̃��[�J����
	Math::Matrix transformationGlobalToLocal(const Math::Matrix& matrix);
	Math::Vector transformationGlobalToLocal(const Math::Vector& vector);
	/// ���̃��[�J������O���[�o����
	Math::Matrix transformationLocalToGlobal(const Math::Matrix& matrix);
	Math::Vector transformationLocalToGlobal(const Math::Vector& vector);

	Math::Matrix rollY(double angle);

/**
 *	----------------------------------------
 *	�r�Ɋւ������
 *	TrackLeg�N���X������r�N���X�֐��̌Ăяo��  (TrackLeg�N���X����Ȃ��`�ɕύX  20200819)
 *	----------------------------------------
 */
/**
 *	----------------------------------------
 *	�A�N�Z�X�֐�
 *	----------------------------------------
 */
	/// �r�����̓����ϊ��s��
	const Math::Matrix& getLegBaseTransformation(int legNo) const;
	/// �֐߂̓����ϊ��s��
	const Math::Matrix& getLegJointTransformation(int legNo, int jointNo) const;
	/// �����̓����ϊ��s��
	const Math::Matrix& getLegFootTransformation(int legNo) const;
	/// �r�����̈ʒu�x�N�g��
	const Math::Vector& getLegBasePosition(int legNo) const;
	/// �֐߈ʒu�̈ʒu�x�N�g��
	const Math::Vector& getLegJointPosition(int legNo, int jointNo) const;
	/// �����ʒu�̈ʒu�x�N�g��
	const Math::Vector& getLegFootPosition(int legNo) const;
	/// �֐ߊp�x�x�N�g��
	const Math::Vector& getLegJointAngle(int legNo) const;
	/// ����֐ߊp�x
	const double getFootJointAngle(int legNo) const;//-------Add
	/// �֐ߑ��x�x�N�g��
	const Math::Vector& getLegJointVelocity(int legNo) const;
	/// �֐߃g���N�x�N�g��
	const Math::Vector& getLegJointTorque(int legNo) const;
	/// �r�̍����̈ʒu�E�p��
	const Math::Vector& getBasePose(int legNo) const;
	/// �r�ԍ��̎擾
	const int getLegNo(int legNo) const;
	/// �֐߂̃G���[�ԍ�
	const int  getLegLastErrorJointNo(int legNo) const;
	/// �A�N�`���G�[�^�[���W
	const Math::Vector&  getLegActuatorPosition(int legNo) const;

/**
 *	----------------------------------------
 *	�Z�b�g�֐�
 *	----------------------------------------
 */
	/// �r�ԍ��̐ݒ�
	void setLegNo(int legNo, int no);

/**
 *	----------------------------------------
 *	�^���w
 *	----------------------------------------
 */
	/// ���^���w
	Kinematics solveLegDirectKinematics(int legNo);
	/// �t�^���w
	Kinematics solveLegInverseKinematics(int legNo);
	/// �p���w�W�̕ύX
	void setLegPoseIndicator(int legNo, int hip = -1, int knee = -1);
	/// �r�̍����ݒ�
	void setLegBasePose(int legNo, double x, double y, double z, double theta);

/**
 *	----------------------------------------
 *	�r��PTP����p
 *	----------------------------------------
 */
	/**
	 *	����
	 *		���̈ʒu���Z�b�g�i�����͏�ɏd�͕����Ɖ���j
 	 *		���^���w�Ƌt�^���w����v���Ă��邩���m�F����
	 *		�r���̊֐߈ʒu���v�Z����
	 */
	Kinematics placeLegFootPosition(int legNo, const Math::Vector& nextFootPosition);
	/**
	 *	����
	 *		�֐ߊp���Z�b�g
	 *		�Z�b�g�����֐ߊp�ŏ��^���w������
	 */
	Kinematics placeLegJointAngles(int legNo, const Math::Vector& nextJointAngle, const double& nextFootJointAngle);
	// const double& nextFootJointAngle�ǉ�
/**
 *	----------------------------------------
 *	���R�r�A�����g���Ă̏�Ԍv�Z
 *	----------------------------------------
 */
	/**
	 *	����
	 *		�֐ߑ��x: [3]
	 *		�r�摬�x����v�Z
	 */
	void calculateLegJointVelocity(int legNo, const Math::Vector& footVelocity);
	/**
	 *	����
	 *		�֐߃g���N: [3]
	 *		�r��[�ɉ�������׏d����v�Z
	 */
	void calculateLegJointTorque(int legNo, const Math::Vector& footForce);

/**
 *	----------------------------------------
 *	�N���[���Ɋւ������
 *	----------------------------------------
 */

/**
 *	----------------------------------------
 *	�A�N�Z�X�֐�
 *	----------------------------------------
 */
 /*  20200819
	/// �N���[�������̓����ϊ��s��
	const Math::Matrix& getTrackBaseTransformation(int trackNo) const;
	/// �N���[���֐߂̓����ϊ��s��
	const Math::Matrix& getTrackJointTransformation(int trackNo, int jointNo) const;
	/// �N���[����[�̓����ϊ��s��
	const Math::Matrix& getTrackEndTransformation(int trackNo) const;
	/// �N���[�������̈ʒu�x�N�g��
	const Math::Vector& getTrackBasePosition(int trackNo) const;
	/// �N���[���֐߂̈ʒu�x�N�g��
	const Math::Vector& getTrackJointPosition(int trackNo, int jointNo) const;
	///�N���[����[�̈ʒu�x�N�g��
	const Math::Vector& getTrackEndPosition(int trackNo) const;
	/// �֐ߊp�x�x�N�g��
	const Math::Vector& getTrackJointAngle(int trackNo) const;
	/// �֐ߑ��x�x�N�g��
	const Math::Vector& getTrackJointVelocity(int trackNo) const;
	/// �֐߃g���N�x�N�g��
	const Math::Vector& getTrackJointTorque(int trackNo) const;
	/// �N���[���X�s�[�h
	const double getTrackSpeed(int trackNo) const;

	/// �r�쓮����
	const DriveSystem getDriveSystem(int trackNo) const;
	/// �N���[���`�Ԏ��ɍŌ�ɃG���[�ƂȂ����֐ߔԍ�
	const int getTrackLastErrorJointNo(int trackNo) const;
	*/
/**
 *	----------------------------------------
 *	�N���[���̉^���w
 *	----------------------------------------
 */
	

	/**
	 *	����
	 *		�N���[�����j�b�g�̎p����ς���
	 */
	//Kinematics changeTrackPosture(int trackNo, const Math::Vector& angle);  20200819
	/**
	 *	����
	 *		�֐ߑ��x: [3]
	 *		�N���[����[�̑��x����v�Z
	 */
	//void calculateTrackJointVelocity(int trackNo, const Math::Vector& trackVelocity);  20200819
	/**
	 *	����
	 *		�֐߃g���N: [3]
	 *		�N���[����[�ɉ�������׏d����v�Z
	 */
	//void calculateTrackJointTorque(int trackNo, const Math::Vector& trackForce);  20200819
	/**
	 *	����
	 *		�N���[�����x��ύX����
	 */
	//void changeTrackSpeed(int trackNo, double trackSpeed);  20200819
	/**
	 *	����
	 *		�쓮�����̐ݒ�
	 *		�߂�l�F�ݒ肵���쓮����
	 */
	//DriveSystem setDriveSystem(int trackLegNo, DriveSystem ds);  20200819

private:
/**
 *	����
 *		�r�̃C���X�^���X����
 */
	void newTrackLegs(void);
/**
 *	����
 *		�r�̃C���X�^���X�j��
 */
	void deleteTrackLegs(void);

};	/// end of class WalkingRobot

/**
 *	------------------------------------------------------------
 *		inline�֐�
 *	------------------------------------------------------------
 */
/**
 *	----------------------------------------
 *	�A�N�Z�X�֐�
 *	----------------------------------------
 */
 /// �r�̃C���X�^���X
 /*  20200819
 inline const TrackLeg& WalkingRobot::getTrackLeg(int legNo) const
{
	/// �����`�F�b�N
	assert( 1 <= legNo && legNo <= LEG_NUM );

	return *trackLegs[legNo-1];
}
*/

/// �r�̉^����
inline const LegPhase WalkingRobot::getLegPhase(int legNo) const
{
	/// �����`�F�b�N
	assert( 1 <= legNo && legNo <= LEG_NUM );

	return legPhases[legNo-1];
}

/**
 *	----------------------------------------
 *	���W�ϊ�
 *	----------------------------------------
 */
/// �O���[�o�����瓷�̃��[�J����
/// �������W�ϊ��s���ϊ�����
inline Math::Matrix WalkingRobot::transformationGlobalToLocal(const Math::Matrix& matrix)
{
	assert( matrix.getRow() == Const::DH_DIMENSION && matrix.getColumn() == Const::DH_DIMENSION );

	Math::Matrix result(Const::DH_DIMENSION, Const::DH_DIMENSION);
	Math::Matrix transform(Const::DH_DIMENSION, Const::DH_DIMENSION);

	transform(1,1) = bodyData.transformation(1,1);				
	transform(2,1) = bodyData.transformation(1,2);				
	transform(3,1) = bodyData.transformation(1,3);				
	transform(4,1) = 0;													

	transform(1,2) = bodyData.transformation(2,1);
	transform(2,2) = bodyData.transformation(2,2);
	transform(3,2) = bodyData.transformation(2,3);
	transform(4,2) = 0;

	transform(1,3) = bodyData.transformation(3,1);
	transform(2,3) = bodyData.transformation(3,2);
	transform(3,3) = bodyData.transformation(3,3);
	transform(4,3) = 0;


	transform(1,4) = -( bodyData.transformation(1,1)*bodyData.transformation(1,4) + 
							bodyData.transformation(2,1)*bodyData.transformation(2,4) + 
							bodyData.transformation(3,1)*bodyData.transformation(3,4) );
	transform(2,4) = -( bodyData.transformation(1,2)*bodyData.transformation(1,4) + 
							bodyData.transformation(2,2)*bodyData.transformation(2,4) + 
							bodyData.transformation(3,2)*bodyData.transformation(3,4) );
	transform(3,4) = -( bodyData.transformation(1,3)*bodyData.transformation(1,4) + 
							bodyData.transformation(2,3)*bodyData.transformation(2,4) + 
							bodyData.transformation(3,3)*bodyData.transformation(3,4) );
	transform(4,4) = 1;

	result = transform*matrix;

	return result;

}

/// �O���[�o�����瓷�̃��[�J����
/// 3�����ʒu�x�N�g����ϊ�����
inline Math::Vector WalkingRobot::transformationGlobalToLocal(const Math::Vector& vector)
{
	assert( vector.getSize() == Const::THREE_DIMENSION );
	
	Math::Vector result(Const::THREE_DIMENSION);
	Math::Vector left(Const::DH_DIMENSION);
	Math::Vector right(Const::DH_DIMENSION);
	Math::Matrix transform(Const::DH_DIMENSION, Const::DH_DIMENSION);

	/// �O���[�o�����瓷�̃��[�J���ւ̓����ϊ��s��
	transform(1,1) = bodyData.transformation(1,1);				
	transform(2,1) = bodyData.transformation(1,2);				
	transform(3,1) = bodyData.transformation(1,3);				
	transform(4,1) = 0;													

	transform(1,2) = bodyData.transformation(2,1);
	transform(2,2) = bodyData.transformation(2,2);
	transform(3,2) = bodyData.transformation(2,3);
	transform(4,2) = 0;

	transform(1,3) = bodyData.transformation(3,1);
	transform(2,3) = bodyData.transformation(3,2);
	transform(3,3) = bodyData.transformation(3,3);
	transform(4,3) = 0;


	transform(1,4) = -( bodyData.transformation(1,1)*bodyData.transformation(1,4) + 
							bodyData.transformation(2,1)*bodyData.transformation(2,4) + 
							bodyData.transformation(3,1)*bodyData.transformation(3,4) );
	transform(2,4) = -( bodyData.transformation(1,2)*bodyData.transformation(1,4) + 
							bodyData.transformation(2,2)*bodyData.transformation(2,4) + 
							bodyData.transformation(3,2)*bodyData.transformation(3,4) );
	transform(3,4) = -( bodyData.transformation(1,3)*bodyData.transformation(1,4) + 
							bodyData.transformation(2,3)*bodyData.transformation(2,4) + 
							bodyData.transformation(3,3)*bodyData.transformation(3,4) );
	transform(4,4) = 1;

	/// �ꎞ���
	right(1) = vector(1);
	right(2) = vector(2);
	right(3) = vector(3);
	right(4) = 1;

	/// �ϊ�
	left = transform*right;

	/// ���������
	result(1) = left(1);
	result(2) = left(2);
	result(3) = left(3);

	return result;

}

/// ���̃��[�J������O���[�o����
/// �������W�ϊ��s���ϊ�����
inline Math::Matrix WalkingRobot::transformationLocalToGlobal(const Math::Matrix& matrix)
{
	assert( matrix.getRow() == Const::DH_DIMENSION && matrix.getColumn() == Const::DH_DIMENSION );

	Math::Matrix result(Const::DH_DIMENSION, Const::DH_DIMENSION);

	result = bodyData.transformation*matrix;

	return result;
}

/// ���̃��[�J������O���[�o����
/// 3�����ʒu�x�N�g����ϊ�����
inline Math::Vector WalkingRobot::transformationLocalToGlobal(const Math::Vector& vector)
{
	assert( vector.getSize() == Const::THREE_DIMENSION );

	Math::Vector result(Const::THREE_DIMENSION);
	Math::Vector left(Const::DH_DIMENSION);
	Math::Vector right(Const::DH_DIMENSION);

	/// �ꎞ���
	right(1) = vector(1);
	right(2) = vector(2);
	right(3) = vector(3);
	right(4) = 1;

	/// �ϊ�
	left = bodyData.transformation*right;

	/// ���������
	result(1) = left(1);
	result(2) = left(2);
	result(3) = left(3);

	return result;
}

inline Math::Matrix WalkingRobot::rollY(double angle)
{
	/// �߂�l�̍s��
	Math::Matrix A(Const::DH_DIMENSION, Const::DH_DIMENSION);

	A(1,1) = cos( angle );
	A(2,1) = 0;
	A(3,1) = -sin( angle );
	A(4,1) = 0;

	A(1,2) = 0;
	A(2,2) = 1;
	A(3,2) = 0;
	A(4,2) = 0;

	A(1,3) = sin( angle );
	A(2,3) = 0;
	A(3,3) = cos( angle );
	A(4,3) = 0;

	A(1,4) = 0;
	A(2,4) = 0;
	A(3,4) = HG;
	A(4,4) = 1;

	return A;
}

}	/// emd of namespace Asura
#endif /// __WalkingRobot_h__