/**
 *  �t�@�C����
 *		TrackLeg.h
 *  ����
 *		�N���[���ό^�r�@�\�̏��C�^���w
 *  ���t
 *		�쐬��: 2007/02/05(MON)		�X�V��: 2007/02/19(MON)
 */

#ifndef __TrackLeg_h__
#define __TrackLeg_h__

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include "..\..\Math\Matrix\Matrix.h"
#include "Leg.h"
#include "..\..\Utility\Constants.h"
#include "..\AsuraParameter.h"


namespace Asura
{

/**
 *	----------------------------------------------------------------------
 *		TrackLeg�N���X
 *	----------------------------------------------------------------------
 */
class TrackLeg : public Leg
{
/**
 *	------------------------------------------------------------
 *		�����N���X�̐錾
 *	------------------------------------------------------------
 */
	/**
	 *	TrackData�N���X�F���̂̏��
	 */
	class TrackData
	{
		/**
		 *	------------------------------------------------------------
		 *		�����o�ϐ�
		 *	------------------------------------------------------------
		 */
		public:
			/**
			 *		�����ϊ��s��
			 */	
			/// �N���[������
			Math::Matrix	baseTransformation;
			/// �֐�: [4x4]x2
			Math::Matrix* jointTransformation;
			/// �N���[����[
			Math::Matrix	endTransformation;

			/**
			 *		�ʒu�x�N�g��
			 */
			/// �N���[������
			Math::Vector	basePosition;
			/// �֐�: [3]x2
			Math::Vector* jointPosition;
			/// �N���[����[
			Math::Vector	endPosition;

			/**
			 *		�֐߂̏��
			 */
			/// �֐ߊp�x: [3]
			Math::Vector jointAngle;
			/// �֐ߑ��x: [3]
			Math::Vector jointVelocity;
			/// �֐߃g���N: [3]
			Math::Vector jointTorque;

			double trackSpeed;
	
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
			TrackData();
			/// �f�X�g���N�^
			virtual ~TrackData();

		private:
		    /// �I�u�W�F�N�g�̃������̈���m�ۂ���
			void newTrackData(void);
			/// �I�u�W�F�N�g�̃������̈���������
		    void deleteTrackData(void);
	};

/**
 *	------------------------------------------------------------
 *		�����\���̂̐錾
 *	------------------------------------------------------------
 */
	/**
	 *	�@�\�I�Ȑ���
	 */
	struct TrackLimit
	{
		/// �֐ߊp���E
		double angleMin[TRACK_JOINT_NUM];
		double angleMax[TRACK_JOINT_NUM];

		double trackSpeedMax;

	};

/**
 *	------------------------------------------------------------
 *		�����o�ϐ�
 *	------------------------------------------------------------
 */
public:
private:
/**
 *		�r�̏��
 *		�������̏��Ԃ̂��ߐ�ɐ錾
 */
	TrackData trackData;

/**
 *		�@�\�I�Ȑ���
 */
	TrackLimit trackLimit;

/**
 *		�r�@�\�̋쓮����
 */
	DriveSystem driveSystem;

/**
 *		�Ō�ɃG���[�̌����ƂȂ����֐ߔԍ��i1�C2�C3�j
 *		0�͑S��OK�C�G���[��
 */
	int trackLastErrorJointNo;

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
	/**
	 *		x, y, x: �r�̍����ʒu		
	 *		theta: �r�̐��񎲉��̎p��
	 */
	explicit TrackLeg(int no, double x=0, double y=0, double z=0, double theta=0);

	/// �f�X�g���N�^
	virtual ~TrackLeg();

/**
 *	----------------------------------------
 *	�A�N�Z�X�֐�
 *	�N���[���̃f�[�^�𓾂�
 *	----------------------------------------
 */
	/**
	 *	�����ϊ��s��
	 */
	/// �N���[������
	const Math::Matrix& getTrackBaseTransformation(void) const {return trackData.baseTransformation;}
	/// �֐�
	const Math::Matrix& getTrackJointTransformation(int jointNo) const;
	/// �N���[���O���̐�[
	const Math::Matrix& getTrackEndTransformation(void) const {return trackData.endTransformation;}
	
	/**
	 *	�ʒu�x�N�g��
	 */
	/// �r����
	const Math::Vector& getTrackBasePosition(void) const {return trackData.basePosition;}
	/// �֐�
	const Math::Vector& getTrackJointPosition(int jointNo) const;
	/// �N���[���O���̐�[
	const Math::Vector& getTrackEndPosition(void) const {return trackData.endPosition;}

	/**
	 *	�֐߂̏��
	 */
	/// �֐ߊp�x: [3]
	const Math::Vector& getTrackJointAngle(void) const {return trackData.jointAngle;}
	/// �֐ߑ��x: [3]
	const Math::Vector& getTrackJointVelocity(void) const {return trackData.jointVelocity;}
	/// �֐߃g���N: [3]
	const Math::Vector& getTrackJointTorque(void) const {return trackData.jointTorque;}

	/// �N���[�����x
	const double getTrackSpeed(void) const {return trackData.trackSpeed;}

	/// �r�쓮����
	const DriveSystem getDriveSystem(void) const {return driveSystem;}

	/// �֐߂̃G���[�ԍ�
	const int getTrackLastErrorJointNo(void) const {return trackLastErrorJointNo;}

/**
 *	----------------------------------------
 *	�^���w
 *	----------------------------------------
 */
	/**
	 *	�����ϊ��s��𓾂�
	 */
	/// �N���[������
	Math::Matrix trackBaseHomogeneousTransformation(void);
	/// �֐�
	Math::Matrix trackJointHomogeneousTransformation(int jointNo);
	/// �N���[���O���̐�[
	Math::Matrix trackEndHomogeneousTransformation(void);

	/**
	 *	�t�����ϊ��s��𓾂�
	 */
	/// �N���[������
	Math::Matrix trackBaseInverseHomogeneousTransformation(void);
	/// �֐�
	Math::Matrix trackJointInverseHomogeneousTransformation(int jointNo);
	/// �N���[���O���̐�[
	Math::Matrix trackEndInverseHomogeneousTransformation(void);

	/// �N���[�����j�b�g�̎p����ς���
	Kinematics changeTrackPosture(const Math::Vector& angle);

/**
 *	----------------------------------------
 *	��Ԍv�Z�֐�
 *	----------------------------------------
 */
	/**
	 *	����
	 *		�֐ߑ��x: [3]
	 *		�N���[����[�̑��x����v�Z
	 */
	void calculateTrackJointVelocity(const Math::Vector& trackVelocity);
	
	/**
	 *	����
	 *		�֐߃g���N: [3]
	 *		�N���[����[�ɉ�������׏d����v�Z
	 */
	void calculateTrackJointTorque(const Math::Vector& trackReaction);

	/**
	 *	����
	 *		�N���[�����x��ύX����
	 */
	void changeTrackSpeed(double trackSpeed);

	/**
	 *	����
	 *		�쓮�����̐ݒ�
	 *		�߂�l�F�ݒ肵���쓮����
	 */
	DriveSystem setDriveSystem(DriveSystem ds)  {driveSystem = ds; return driveSystem;}

	/**
	 *	����
	 *		�N���[���̋@�\�������`�F�b�N����
	 *		�֐߂̉��͈͂𒲍�
	*/
	Kinematics checkTrackJointMotionRange(void);

private:

/**
 *	------------------------------------------------------------
 *		�l�����C���X�^���X���d�����ăI�u�W�F�N�g�����֎~
 *	------------------------------------------------------------
 */
	/// �R�s�[�R���X�g���N�^����
	TrackLeg(const TrackLeg& trackLeg);

	/// ������Z�q����
	TrackLeg& operator=(const TrackLeg& trackLeg);

};	/// end of class TrackLeg

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
/// �֐߂̓����ϊ��s���Ԃ�
/// jointNo: �֐ߔԍ�
inline const Math::Matrix& TrackLeg::getTrackJointTransformation(int jointNo) const
{
	/// �����`�F�b�N
	assert( 1 <= jointNo && jointNo <= TRACK_JOINT_NUM  );

	return trackData.jointTransformation[jointNo-1];

}

/// �֐߈ʒu��Ԃ�
/// jointNo: �֐ߔԍ�
inline const Math::Vector& TrackLeg::getTrackJointPosition(int jointNo) const
{
	/// �����`�F�b�N
	assert( 1 <= jointNo && jointNo <= TRACK_JOINT_NUM  );

	return trackData.jointPosition[jointNo-1];
}

/**
 *	����
 *		���W�ϊ��ɕK�v�ȓ����ϊ��s��𓱏o����
 *		jointNo -> jointNo+1 �ւ̍��W�ϊ�
 *
 *		A(1,1) = cos(theta);	A(1,2) = -cos(alpha)*sin(theta);		A(1,3) = sin(alpha)*sin(theta);			A(1,4) = a*cos(theta);
 *		A(2,1) = sin(theta);	A(2,2) = cos(alpha)*cos(theta);			A(2,3) = -sin(alpha)*cos(theta);		A(2,4) = a*sin(theta);
 *		A(3,1) = 0;				A(3,2) = sin(alpha);					A(3,3) = cos(alpha);					A(3,4) = d;
 *		A(4,1) = 0;				A(4,2) = 0;								A(4,3) = 0;								A(4,4) = 1;
 */
/// �r����
inline Math::Matrix TrackLeg::trackBaseHomogeneousTransformation(void)
{
	/// �߂�l�̍s��
	Math::Matrix A(Const::DH_DIMENSION, Const::DH_DIMENSION);

	A(1,1) = cos( basePose(4) );
	A(2,1) = sin( basePose(4) );
	A(3,1) = 0;
	A(4,1) = 0;

	A(1,2) = -sin( basePose(4) );
	A(2,2) = cos( basePose(4) );
	A(3,2) = 0;
	A(4,2) = 0;

	A(1,3) = 0;
	A(2,3) = 0;
	A(3,3) = 1;
	A(4,3) = 0;

	A(1,4) = basePose(1);
	A(2,4) = basePose(2);
	A(3,4) = basePose(3);
	A(4,4) = 1;

	return A;

}

/// �֐�
inline Math::Matrix TrackLeg::trackJointHomogeneousTransformation(int jointNo)
{
	/// �����`�F�b�N
	assert( 1 <= jointNo && jointNo <= LEG_JOINT_NUM );

	/// �߂�l�̍s��
	Math::Matrix A(Const::DH_DIMENSION, Const::DH_DIMENSION);

	A(1,1) = cos( trackData.jointAngle(jointNo) );	
	A(2,1) = sin( trackData.jointAngle(jointNo) );	
	A(3,1) = 0;											
	A(4,1) = 0;											

	A(1,2) = -cos( dhParam.alpha[jointNo-1] )*sin( trackData.jointAngle(jointNo) );	
	A(2,2) = cos( dhParam.alpha[jointNo-1] )*cos( trackData.jointAngle(jointNo) );	
	A(3,2) = sin( dhParam.alpha[jointNo-1] );
	A(4,2) = 0;

	A(1,3) = sin( dhParam.alpha[jointNo-1] )*sin( trackData.jointAngle(jointNo) );	
	A(2,3) = -sin( dhParam.alpha[jointNo-1] )*cos( trackData.jointAngle(jointNo) );
	A(3,3) = cos( dhParam.alpha[jointNo-1] );
	A(4,3) = 0;

	A(1,4) = dhParam.a[jointNo-1]*cos( trackData.jointAngle(jointNo) );
	A(2,4) = dhParam.a[jointNo-1]*sin( trackData.jointAngle(jointNo) );
	A(3,4) = dhParam.d[jointNo-1];
	A(4,4) = 1;

	return A;
}

/// �N���[���O����[
inline Math::Matrix TrackLeg::trackEndHomogeneousTransformation(void)
{
	/// �߂�l�̍s��
	Math::Matrix A(Const::DH_DIMENSION, Const::DH_DIMENSION);

	double angle = -Const::PI;

	A(1,1) = cos( angle );	
	A(2,1) = sin( angle );	
	A(3,1) = 0;											
	A(4,1) = 0;											

	A(1,2) = -cos( dhParam.alpha[2] )*sin( angle );	
	A(2,2) = cos( dhParam.alpha[2] )*cos( angle );	
	A(3,2) = sin( dhParam.alpha[2] );
	A(4,2) = 0;

	A(1,3) = sin( dhParam.alpha[2] )*sin( angle );	
	A(2,3) = -sin( dhParam.alpha[2] )*cos( angle );
	A(3,3) = cos( dhParam.alpha[2] );
	A(4,3) = 0;

	A(1,4) = dhParam.a[2]*cos( angle );
	A(2,4) = dhParam.a[2]*sin( angle );
	A(3,4) = dhParam.d[2];
	A(4,4) = 1;

	return A;
}

/**
 *	����
 *		�����ϊ��s��̋t�s��𓱏o����
 *		jointNo+1 -> jointNo �ւ̍��W�ϊ�
 *
 *		A(1,1) = cos(theta);					A(1,2) = sin(theta);					A(1,3) = 0;				A(1,4) = -a;
 *		A(2,1) = -cos(alpha)*sin(theta);		A(2,2) = cos(alpha)*cos(theta);			A(2,3) = sin(alpha);	A(2,4) = -d*sin(alpha);
 *		A(3,1) = sin(alpha)*sin(theta);			A(3,2) = -sin(alpha)*cos(theta);		A(3,3) = cos(alpha);	A(3,4) = -d*cos(alpha);
 *		A(4,1) = 0;								A(4,2) = 0;								A(3,4) = 0;				A(4,4) = 1;
 */
/// �r����
inline Math::Matrix TrackLeg::trackBaseInverseHomogeneousTransformation(void)
{
	/// �߂�l�̍s��
	Math::Matrix A(Const::DH_DIMENSION, Const::DH_DIMENSION);

	A(1,1) = cos( basePose(4) );
	A(2,1) = -sin( basePose(4) );
	A(3,1) = 0;
	A(4,1) = 0;

	A(1,2) = sin( basePose(4) );
	A(2,2) = cos( basePose(4) );
	A(3,2) = 0;
	A(4,2) = 0;

	A(1,3) = 0;
	A(2,3) = 0;
	A(3,3) = 1;
	A(4,3) = 0;

	A(1,4) = -( cos( basePose(4) )*basePose(1) + sin( basePose(4) )*basePose(2) );
	A(2,4) = -( -sin( basePose(4) )*basePose(1) + cos( basePose(4) )*basePose(2) );
	A(3,4) = -basePose(3);
	A(4,4) = 1;

	return A;
}

/// �֐�
inline Math::Matrix TrackLeg::trackJointInverseHomogeneousTransformation(int jointNo)
{
	/// �����`�F�b�N
	assert( 1 <= jointNo && jointNo <= LEG_JOINT_NUM );

	/// �߂�l�̍s��
	Math::Matrix A(Const::DH_DIMENSION, Const::DH_DIMENSION);

	A(1,1) = cos( legData.jointAngle(jointNo) );	
	A(2,1) = -cos( dhParam.alpha[jointNo-1] )*sin( legData.jointAngle(jointNo) );		
	A(3,1) = sin( dhParam.alpha[jointNo-1] )*sin( legData.jointAngle(jointNo) );												
	A(4,1) = 0;											

	A(1,2) = sin( legData.jointAngle(jointNo) );
	A(2,2) = cos( dhParam.alpha[jointNo-1] )*cos( legData.jointAngle(jointNo) );	
	A(3,2) = -sin( dhParam.alpha[jointNo-1] )*cos( legData.jointAngle(jointNo) );
	A(4,2) = 0;

	A(1,3) = 0;
	A(2,3) = sin( dhParam.alpha[jointNo-1] );
	A(3,3) = cos( dhParam.alpha[jointNo-1] );
	A(4,3) = 0;

	A(1,4) = -dhParam.a[jointNo-1];
	A(2,4) = -dhParam.d[jointNo-1]*sin(  dhParam.alpha[jointNo-1] );
	A(3,4) = -dhParam.d[jointNo-1]*cos(  dhParam.alpha[jointNo-1] );
	A(4,4) = 1;

	return A;
}

/// �N���[���O����[
inline Math::Matrix TrackLeg::trackEndInverseHomogeneousTransformation(void)
{
	/// �߂�l�̍s��
	Math::Matrix A(Const::DH_DIMENSION, Const::DH_DIMENSION);

	double angle = -Const::PI;

	A(1,1) = cos( angle );	
	A(2,1) = -cos( dhParam.alpha[2] )*sin( angle );		
	A(3,1) = sin( dhParam.alpha[2] )*sin( angle );												
	A(4,1) = 0;											

	A(1,2) = sin( angle );
	A(2,2) = cos( dhParam.alpha[2] )*cos( angle );	
	A(3,2) = -sin( dhParam.alpha[2] )*cos( angle );
	A(4,2) = 0;

	A(1,3) = 0;
	A(2,3) = sin( dhParam.alpha[2] );
	A(3,3) = cos( dhParam.alpha[2] );
	A(4,3) = 0;

	A(1,4) = -dhParam.a[2];
	A(2,4) = -dhParam.d[2]*sin(  dhParam.alpha[2] );
	A(3,4) = -dhParam.d[2]*cos(  dhParam.alpha[2] );
	A(4,4) = 1;

	return A;
}

}
#endif /// __TrackLeg_h__