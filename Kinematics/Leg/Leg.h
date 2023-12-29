/**
 *  �t�@�C����
 *		Leg.h
 *  ����
 *		1�r�̊֐߂⑫��Ɋւ�����C�^���w�Ȃǂ�����
 *  ���t
 *		�쐬��: 2007/01/31(WED)		�X�V��: 2018/02/12(MON)
 */

//20201015  ��4�����N
//20201018  ��4�֐�
//20221026  �����N4�p��

#ifndef __Leg_h__
#define __Leg_h__

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include <cassert>
#include "..\..\Math\Matrix\Matrix.h"
#include "..\..\Utility\Constants.h"
#include "..\AsuraParameter.h"

namespace Asura
{

/**
 *	----------------------------------------------------------------------
 *		Leg�N���X
 *	----------------------------------------------------------------------
 */
class Leg
{
/**
 *	------------------------------------------------------------
 *		�����N���X�̐錾
 *	------------------------------------------------------------
 */
	/**
	 *		LegData�N���X�F�r�̏��
	 */
	class LegData
	{
		/**
		 *	------------------------------------------------------------
		 *		�����o�ϐ�
		 *	------------------------------------------------------------
		 */
		public:
			/**
			 *	���W�n�͑S�ē��̍��W�n�i�����ł̓O���[�o�����W�j
			 */
			/// �����ϊ��s��
			/// �r����: [4x4]
			Math::Matrix	baseTransformation;
			/// �֐�: [4x4]x3
			Math::Matrix* jointTransformation;
			/// ����]: [4x4]
			Math::Matrix	footTransformation;

			Math::Vector bodyAngle;

			/**
			 *		�ʒu�x�N�g��
			 */
			/// �r����: [3]
			Math::Vector	basePosition;
			/// �e�֐�: [3]x3
			Math::Vector* jointPosition;
			/// ����: [3]
			Math::Vector	footPosition;

			/**
			 *		�֐߂̏��
			 */
			/// �֐ߊp�x: [3]-->[4]
			Math::Vector jointAngle;
			/// �֐ߑ��x: [3]
			Math::Vector jointVelocity;
			/// �֐߃g���N: [3]
			Math::Vector jointTorque;
			/// ����֐ߊp�x: [1]
			//Math::Vector  FootjointAngle;//---Add
			double FootjointAngle;

			/**
			 *		�A�N�`���G�[�^�̏��
			 */
			///�A�N�`���G�[�^���W: [4]
			Math::Vector ActPos;
			/// �ϊ��s��: [4x4]
			//Math::Matrix	ActTransformation;

			double LegTipAngle;  //20221026
			
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
			LegData();
			/// �f�X�g���N�^
			virtual ~LegData();

		private:
		    /// �I�u�W�F�N�g�̃������̈���m�ۂ���
			void newLegData(void);
			/// �I�u�W�F�N�g�̃������̈���������
		    void deleteLegData(void);
	};

/**
 *	------------------------------------------------------------
 *		�����\���̂̐錾
 *	------------------------------------------------------------
 */
	/**
	 *	�@�\�I�Ȑ���
	 */
	struct LegLimit
	{
		/// �֐ߊp���E
		double angleMin[LEG_JOINT_NUM];
		double angleMax[LEG_JOINT_NUM];
		double FootangleMin;
		double FootangleMax;


		/// ���攽�͌��E
		double footReaction[3];

		/// �r���a�������[�`
		double reachRadiusMin;
		double reachRadiusMax;
	
		/// �r�����������[�`
		double reachHeightMin;
		double reachHeightMax;
	};

	/**
	 *	DH�p�����[�^
	*/
	struct DHParameter
	{
		double	alpha[LEG_JOINT_NUM];
		double	a[LEG_JOINT_NUM];
		double	d[LEG_JOINT_NUM];
	};


/**
 *	------------------------------------------------------------
 *		�����o�ϐ�
 *	------------------------------------------------------------
 */
public:
protected:
/**
 *	�r�̏��
 *	�������̏��Ԃ̂��ߐ�ɐ錾
 */
	LegData legData;

/**
 *	�@�\�I�Ȑ���
 */
	LegLimit legLimit;

/**
 *	DH�p�����[�^
 */
	DHParameter dhParam;

/**
 *	���R�r�A��
 */
	/// ���R�r�s��: [3x3]
	Math::Matrix jacobian;
	/// �t���R�r�s��: [3x3]
	Math::Matrix inverseJacobian;

/**
 *	�r�̍����̈ʒu�ix y z�j�E�p���i�Ɓj: [4]
 */
	Math::Vector basePose;	

/**
 *	�r�̎p���w�W�i�t�^���w���ɗp����j0 or 1
 */
	int poseIndicator[2];

/**
 *	�r�ԍ�
 */
	int legNo;


/**
 *	�Ō�ɃG���[�̌����ƂȂ����֐ߔԍ��i1�C2�C3�j
 *	0�͑S��OK�C�G���[��
 */
	int legLastErrorJointNo;

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
	/**
	 *	����
	 *		�f�t�H���g�R���X�g���N�^
	 *	����
	 *		x, y, x: �r�̍����ʒu		
	 *		theta: �r�̐��񎲉��̎p��
	 */
	explicit Leg(int no=1, double x=0, double y=0, double z=0, double theta=0);

	/// �f�X�g���N�^
	virtual ~Leg();

/**
 *	----------------------------------------
 *	�A�N�Z�X�֐�
 *	�r�̃f�[�^�𓾂�
 *	----------------------------------------
 */
	/**
	 *	�����ϊ��s��
	 */
	/// �r����
	const Math::Matrix& getLegBaseTransformation(void) const {return legData.baseTransformation;}
	/// �֐�
	const Math::Matrix& getLegJointTransformation(int jointNo) const;
	/// ����
	const Math::Matrix& getLegFootTransformation(void) const {return legData.footTransformation;}
	
	/**
	 *	�ʒu�x�N�g��
	 */
	/// �r����
	const Math::Vector& getLegBasePosition(void) const {return legData.basePosition;}
	/// �֐߈ʒu
	const Math::Vector& getLegJointPosition(int jointNo) const;
	/// �����ʒu: [3]
	const Math::Vector& getLegFootPosition(void) const {return legData.footPosition;}

	/**
	 *	�֐߂̏��
	 */
	/// �֐ߊp�x: [3]
	const Math::Vector& getLegJointAngle(void) const {return legData.jointAngle;}
	/// �֐ߑ��x: [3]
	const Math::Vector& getLegJointVelocity(void) const {return legData.jointVelocity;}
	/// �֐߃g���N: [3]
	const Math::Vector& getLegJointTorque(void) const {return legData.jointTorque;}
	/// ����֐ߊp�x :[1]
	const double getFootJointAngle(void) const {return legData.FootjointAngle;}//------Add

	const double getLegTipAngle(void) const { return legData.LegTipAngle; }  //20221026

	/**
	 *	���R�r�A��
	 */
	/// ���R�r�s��: [3x3]
	const Math::Matrix& getJacobian(void) const {return jacobian;}
	/// �t���R�r�s��: [3x3]
	const Math::Matrix& getInverseJacobian(void) const {return inverseJacobian;}
	
	/// �r�̍����̈ʒu�E�p��: [4]
	const Math::Vector& getBasePose(void) const {return basePose;}	

	/// �r�ԍ��̎擾
	const int getLegNo(void) const {return legNo;}

	/// �֐߂̃G���[�ԍ�
	const int getLegLastErrorJointNo(void) const {return legLastErrorJointNo;}

	/// �{�[���˂��ʒu: [4]�i���C���ψʁj
	const Math::Vector& getLegActuatorPosition(void) const {return legData.ActPos;}//------Add

/**
 *	----------------------------------------
 *	�Z�b�g�֐�
 *	----------------------------------------
 */
	/// �r�ԍ��̐ݒ�
	void setLegNo(int no);

/**
 *	----------------------------------------
 *	�^���w
 *	----------------------------------------
 */
	/**
	 *	�����ϊ��s��
	 */
	/// �r����
	Math::Matrix legBaseHomogeneousTransformation(void);
	/// �֐�
	Math::Matrix legJointHomogeneousTransformation(int jointNo);
	/// ����
	Math::Matrix legFootHomogeneousTransformation(void);

	/**
	 *	�t�����ϊ��s��
	 */
	/// �r����
	Math::Matrix legBaseInverseHomogeneousTransformation(void);
	/// �֐�
	Math::Matrix legJointInverseHomogeneousTransformation(int jointNo);
	/// ����
	Math::Matrix legFootInverseHomogeneousTransformation(void);

	/// ���^���w
	Kinematics solveDirectKinematics(void);

	/// �t�^���w
	Kinematics solveInverseKinematics(void);

	/// �p���w�W�̕ύX
	void setLegPoseIndicator(int hip = 1, int knee = 1);

	/// �r�̍����ݒ�
	void setLegBasePose(double x, double y, double z, double theta);
/**
 *	----------------------------------------
 *	�r�̋@�\�������`�F�b�N
 *	----------------------------------------
 */
	/// �֐߂̉��͈͂𒲍�
	Kinematics checkLegJointMotionRange(void);

	/// �r�̃��[�`�𒲍�
	Kinematics checkLegFootReachRange(void);

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
	Kinematics placeLegFootPosition(const Math::Vector& nextFootPosition);

	/** 
	 *	����
	 *		�֐ߊp���Z�b�g
	 *		�Z�b�g�����֐ߊp�ŏ��^���w������
	 */
	Kinematics placeLegJointAngles(const Math::Vector& nextJointAngle, const double& nextFootJointAngle);
	//20201018  const double& nextFootJointAngle�ǉ�

	void setLegTipAngle(const double angle) { legData.LegTipAngle = angle; return; }  //20221026

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
	void calculateLegJointVelocity(const Math::Vector& footVelocity);
	
	/**
	 *	����
	 *		�֐߃g���N: [3]
	 *		�r��[�ɉ�������׏d����v�Z
	 */
	void calculateLegJointTorque(const Math::Vector& footReaction);

	/**
	 *	���R�r�A�����g�̌v�Z
	 */
	/// ���R�r�s��: [3x3]
	void calculateJacobian(void);
	/// �t���R�r�s��: [3x3]
	void calculateInverseJacobian(void);

	/**
	 *	����
	 *		�{�[���˂��ʒu�v�Z�i���C���ψʁj
	 */
	///�w�ߒl�ϊ�
	void calculationActatorPosition(void);

private:
/**
 *	------------------------------------------------------------
 *		�l�����C���X�^���X���d�����ăI�u�W�F�N�g�����֎~
 *	------------------------------------------------------------
 */
	/// �R�s�[�R���X�g���N�^����
	Leg(const Leg& leg);

	/// ������Z�q����
	Leg& operator=(const Leg& leg)=delete;
};

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
/// �����ϊ��s���Ԃ�
/// jointNo: �֐ߔԍ�
inline const Math::Matrix& Leg::getLegJointTransformation(int jointNo) const
{
	/// �����`�F�b�N
	assert( 1 <= jointNo && jointNo <= LEG_JOINT_NUM );

	return ( legData.jointTransformation[jointNo-1] );
}

/// �֐߈ʒu��Ԃ�
/// jointNo: �֐ߔԍ�
inline const Math::Vector& Leg::getLegJointPosition(int jointNo) const
{
	/// �����`�F�b�N
	assert( 1 <= jointNo && jointNo <= LEG_JOINT_NUM );

	return ( legData.jointPosition[jointNo-1] );
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
 *//// �r����
inline Math::Matrix Leg::legBaseHomogeneousTransformation(void)
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
inline Math::Matrix Leg::legJointHomogeneousTransformation(int jointNo)
{
	/// �����`�F�b�N
	assert( 1 <= jointNo && jointNo <= LEG_JOINT_NUM );

	/// �߂�l�̍s��
	Math::Matrix A(Const::DH_DIMENSION, Const::DH_DIMENSION);

	A(1,1) = cos( legData.jointAngle(jointNo) );	
	A(2,1) = sin( legData.jointAngle(jointNo) );	
	A(3,1) = 0;											
	A(4,1) = 0;											

	A(1,2) = -cos( dhParam.alpha[jointNo-1] )*sin( legData.jointAngle(jointNo) );	
	A(2,2) = cos( dhParam.alpha[jointNo-1] )*cos( legData.jointAngle(jointNo) );	
	A(3,2) = sin( dhParam.alpha[jointNo-1] );
	A(4,2) = 0;

	A(1,3) = sin( dhParam.alpha[jointNo-1] )*sin( legData.jointAngle(jointNo) );	
	A(2,3) = -sin( dhParam.alpha[jointNo-1] )*cos( legData.jointAngle(jointNo) );
	A(3,3) = cos( dhParam.alpha[jointNo-1] );
	A(4,3) = 0;

	A(1,4) = dhParam.a[jointNo-1]*cos( legData.jointAngle(jointNo) );
	A(2,4) = dhParam.a[jointNo-1]*sin( legData.jointAngle(jointNo) );
	A(3,4) = dhParam.d[jointNo-1];
	A(4,4) = 1;

	return A;

}

/// ����
/// ���̂Ƒ����͏�ɕ��s�Ɖ��肵�ĉ�������p����p���̈ڍs���\�ɂ��邽�߂ɁC�e�֐߂̒l��������i20201018�j
inline Math::Matrix Leg::legFootHomogeneousTransformation(void)
{
	/// �߂�l�̍s��
	Math::Matrix A(Const::DH_DIMENSION, Const::DH_DIMENSION);
	/// ����֐ߊp�x
	//double angle = Const::PI / 2 - legData.jointAngle(2) - legData.jointAngle(3);//�ύX

	//20201015
	/*
	double angle;
	if (StandardPos)  //StandardPos��true�̎��̂ݑ�4�����N���n�ʂƕ��s�Ƃ��ď��^���w������
	{
		angle = 0;
	}
	else  //����ȊO�̏ꍇ�͑�4�����N�͒n�ʂƐ���
	{
		angle = Const::PI / 2 - legData.jointAngle(2) - legData.jointAngle(3);//�ύX
	}
	*/
	//20201018
	double angle = legData.FootjointAngle;

	A(1,1) = cos( angle );	
	A(2,1) = sin( angle );	
	A(3,1) = 0;											
	A(4,1) = 0;											

	A(1,2) = -sin( angle );	
	A(2,2) = cos( angle );	
	A(3,2) = 0;
	A(4,2) = 0;

	A(1,3) = 0;	
	A(2,3) = 0;
	A(3,3) = 1;
	A(4,3) = 0;

	A(1,4) = FOOT*cos( angle );
	A(2,4) = FOOT*sin( angle );
	A(3,4) = 0;
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
inline Math::Matrix Leg::legBaseInverseHomogeneousTransformation(void)
{
	/// �߂�l�̍s��
	Math::Matrix A(Const::DH_DIMENSION, Const::DH_DIMENSION);

	A(1,1) = cos( basePose(4) );
	A(2,1) = -sin( basePose(4) );//-�ǉ�
	A(3,1) = 0;
	A(4,1) = 0;

	A(1,2) = sin( basePose(4) );//-�ǉ�
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
inline Math::Matrix Leg::legJointInverseHomogeneousTransformation(int jointNo)
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

/// ����
inline Math::Matrix Leg::legFootInverseHomogeneousTransformation(void)
{
	/// �߂�l�̍s��
	Math::Matrix A(Const::DH_DIMENSION, Const::DH_DIMENSION);
	/// ����֐ߊp�x<-----�ϐ��Œu��������
	//double angle = Const::PI/2 - legData.jointAngle(2) - legData.jointAngle(3);
	double angle = legData.LegTipAngle - legData.jointAngle(2) - legData.jointAngle(3);  //20221026

	A(1,1) = cos( angle );
	A(2,1) = -sin( angle );
	A(3,1) = 0;
	A(4,1) = 0;

	A(1,2) = sin( angle );
	A(2,2) = cos( angle );
	A(3,2) = 0;
	A(4,2) = 0;

	A(1,3) = 0;
	A(2,3) = 0;
	A(3,3) = 1;
	A(4,3) = 0;

	A(1,4) = -FOOT;
	A(2,4) = 0;
	A(3,4) = 0;
	A(4,4) = 1;

	return A;
}


}	/// end of namespace Asura
 

#endif	/// __Leg_h__

