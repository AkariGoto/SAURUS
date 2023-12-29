/**
 *  �t�@�C����
 *		Leg.cpp
 *  ����
 *		1�r�̊֐߂⑫��Ɋւ�����C�^���w�Ȃǂ�����
 *  
 *		�쐬��: 2007/01/31(WED)		�X�V��: 2018/02/12(MON)
 */

//20201014  ��4�֐�
//20201015  ��4�����N
//20201018  ��4�֐�
//20221026�@�����N4�p��

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include "Leg.h"
//#include "TrackLeg.h"  20200819
#include "..\..\Utility\Constants.h"
#include "..\..\System\DebugOutput.h"
//#include <winsock2.h>

using namespace std;
using namespace Math;
using namespace Const;

namespace Asura
{
/**
 *	----------------------------------------------------------------------
 *		Leg�N���X
 *	----------------------------------------------------------------------
 */

/**
 *	------------------------------------------------------------
 *		Leg�N���X�̃����o�֐���`
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	�R���X�g���N�^�ƃf�X�g���N�^
 *	----------------------------------------
 */
/// �f�t�H���g�R���X�g���N�^

Leg::Leg(int no, double x, double y, double z, double theta)
{

/**
 *	�����o�ϐ��̏�����
 */
	/// �r�ԍ��ݒ�
	legNo = no;

	/**
	 *	�@�\�I�Ȍ��E
	 */
	/// �֐ߊp���E
	/// �֐ߊp����
	legLimit.angleMin[0]			= LEG_ANGLE1_MIN;
	legLimit.angleMin[1]			= LEG_ANGLE2_MIN;
	legLimit.angleMin[2]			= LEG_ANGLE3_MIN;
	legLimit.FootangleMin			= LEG_ANGLE4_MIN;

	/// �֐ߊp���	
	legLimit.angleMax[0]			= LEG_ANGLE1_MAX;
	legLimit.angleMax[1]			= LEG_ANGLE2_MAX;
	legLimit.angleMax[2]			= LEG_ANGLE3_MAX;
	legLimit.FootangleMax			= LEG_ANGLE4_MAX;
	
	/// ���攽�͌��E
	legLimit.footReaction[0]	= FOOT_REACTION_X;
	legLimit.footReaction[1]	= FOOT_REACTION_Y;
	legLimit.footReaction[2]	= FOOT_REACTION_Z;
	
	/// �r���a�������[�`
	legLimit.reachRadiusMin		= REACH_RADIUS_MIN;
	legLimit.reachRadiusMax		= REACH_RADIUS_MAX;
	/// �r�����������[�`
	legLimit.reachHeightMin		= REACH_HEIGHT_MIN;
	legLimit.reachHeightMax		= REACH_HEIGHT_MAX;

	/**
	 *	DH�p�����[�^
	 */
	dhParam.alpha[0]				= -PI/2;
	dhParam.alpha[1]				= 0;
	dhParam.alpha[2]				= 0;

	dhParam.a[0]					= LINK1;///OFFSET1;
	dhParam.a[1]					= LINK2;
	dhParam.a[2]					= LINK3;
	//dhParam.a[3]					= FOOT;

	/// �r�ԍ��̃I�t�Z�b�g���������E�ŕς���
	switch (no){
		///�i�s��������
		//case 0:
		case 1:
		case 2:
		case 3:
		{
			dhParam.d[0]					= -OFFSET1;
			dhParam.d[1]					= -OFFSET2;
			dhParam.d[2]					= -OFFSET3;
		}
		break;
		///�i�s�����E��
		//case 3:
		case 4:
		case 5:
		case 6:
		{
			dhParam.d[0]					= OFFSET1;
			dhParam.d[1]					= OFFSET2;
			dhParam.d[2]					= OFFSET3;
		}
		break;

		default:
			break;
	}

	//20201014
	legData.FootjointAngle = 0;

	//20221026
	legData.LegTipAngle = 0;

/**
 *	�s��̃T�C�Y����
 */
	/// ���R�r�s��: [3x3]
	jacobian.setSize(THREE_DIMENSION, THREE_DIMENSION);
	/// �t���R�r�s��: [3x3]
	inverseJacobian.setSize(THREE_DIMENSION, THREE_DIMENSION);

	/// �r�̎p���w�W�i�t�^���w���ɗp����j0 or 1
	this->setLegPoseIndicator();

	/// �r�̍�������
	basePose.setSize(4);
	this->setLegBasePose(x, y, z, theta);

	/// �ŏ��̓G���[�֐ߖ�
	legLastErrorJointNo = 0;

	/// �����֐ߒl�ŏ~�^���w�������Ċ֐߈ʒu��ێ�
	solveDirectKinematics();
}

/// �f�X�g���N�^
Leg::~Leg()
{

}

/**
 *	----------------------------------------
 *	�Z�b�g�֐�
 *	----------------------------------------
 */
/// �r�ԍ��̐ݒ�
void Leg::setLegNo(int no)
{
	/// �����`�F�b�N
	if ( !(1 <= legNo && legNo <= LEG_NUM ) )
	{
		cerr << "Error: [Leg::setLegNo] Argument is invalid\n" << endl;//cerr =>�W���G���[�o��
        abort();
	}

	legNo = no;
	
	
}

/**
 *	----------------------------------------
 *	�^���w
 *	----------------------------------------
 */
/**
 *	����
 *		���^���w������
 *		�����ϊ��s���p���ĉ���
 *
 */
Kinematics Leg::solveDirectKinematics(void)
{
	/// �߂�l�p
	Kinematics kine;
	/// �J�E���^
	int i, j;

	/// �܂��֐߂����͈͓��ɂ��邩�ǂ������ׂ�
	kine = checkLegJointMotionRange();

	/// �֐߂����͈͊O
	if ( kine != NO_KINE_ERROR )	
	{
		cerr << "Error: [Leg::solveDirectKinematics] Invalid joint angle" << endl;
	
		return kine;
	}

	/// �r�����܂ł̍��W�ϊ�
	legData.baseTransformation = legBaseHomogeneousTransformation();
	/// �r�����ʒu�x�N�g������
	for (i=1; i<=THREE_DIMENSION; i++)
		legData.basePosition(i) = legData.baseTransformation(i, 4);

	/// �r���������1�֐߂܂ł̍��W�ϊ�
	legData.jointTransformation[0] = legData.baseTransformation*legJointHomogeneousTransformation(1);
	/// ��1�֐߈ʒu�x�N�g������
	for (i=1; i<=THREE_DIMENSION; i++)
		legData.jointPosition[0](i) = legData.jointTransformation[0](i, 4);

	/// �֐߂̓����ϊ��𒀎��s��
	for (i=1; i<LEG_JOINT_NUM; i++)
	{
		/// �����ϊ��s����v�Z
		legData.jointTransformation[i] = legData.jointTransformation[i-1]*legJointHomogeneousTransformation(i+1);

		/// �ʒu�x�N�g�����擾
		for (j=1; j<=THREE_DIMENSION; j++)
			legData.jointPosition[i](j) = legData.jointTransformation[i](j, 4);
	}

	/// ����̓����ϊ��s��X�V
	legData.footTransformation = legData.jointTransformation[2]*legFootHomogeneousTransformation();
	/// ���ʒu�ʒu�x�N�g������
	for (i=1; i<=THREE_DIMENSION; i++)
		legData.footPosition(i) = legData.footTransformation(i ,4);

	/// ���^���w����������ɑ��悪���͈͓��ɂ��邩�ǂ���
	kine = checkLegFootReachRange();

	calculationActatorPosition();

	return kine;
}

/**
 *	����
 *		�t�^���w������
 *		�@�\���P���Ȃ��ߊ􉽊w�I�ɉ���
 */
Kinematics Leg::solveInverseKinematics(void)
{
	/**
	 *	���[�J���ϐ�
	 */
	/// �߂�l
	Kinematics kine = NO_KINE_ERROR;
	/// �r���W�n�ł̋r��̓����ϊ��s��
	Matrix footTransform(DH_DIMENSION, DH_DIMENSION);
	/// �r��̈ʒu
	double x=0.00; double y=0.00; double z=0.00;
	/// �֐ߊp�̌v�Z�Ɏg��
	double sin3=0.00; double cos3=0.00;
	double sinA=0.00; double cosA=0.00;
	/// �v�Z�⏕
	double q1=0.00; double q2=0.00;			
	/// �]���藝�p�̌v�Z�⏕
	double r1=0.00; double r2=0.00;
	/// �ꎞ�ۑ��p
	double formerJoint1;
	///�r��ڒn�p�x
	//double LegTipAngle = Const::PI/2 ;  //+ Const::PI/4;
	//legData.LegTipAngle = 90 / 180 * Const::PI;  //+ Const::PI/4;


	/// �r�̉��͈͂��`�F�b�N
	kine = checkLegFootReachRange();

	/// ���͈͊O��������
	if ( kine != NO_KINE_ERROR )
	{
		return kine;
	}

	/// �O���[�o�����W�n����r���W�n�ւ̕ϊ�
	footTransform = legBaseInverseHomogeneousTransformation()*legData.footTransformation; //legBaseInverseHomogeneousTransformation��footTransformation���m�F����LegTipAngle�����邩�m�F

	/// �ʒu����
	x = footTransform(1, 4);
	y = footTransform(2, 4);
	z = footTransform(3, 4) ;//+ FOOT;	/// ���񍂂������I�t�Z�b�g�i���z�����ɃI�t�Z�b�g�Ɖ���j

	//20221026
	legData.LegTipAngle = (-0.1 * sqrt(x * x + y *y) + 0.02 * z + 151) / 180  * Const::PI;
	/*
	if (legData.LegTipAngle < 67)
	{
		legData.LegTipAngle = 67;
	}
	else if (legData.LegTipAngle > 113)
	{
		legData.LegTipAngle = 113;
	}
	//*/

	/// �v�Z�⏕�ϐ����Z�o
	q1 = dhParam.d[1] + dhParam.d[2];
	q2 = z - dhParam.d[0];

	r1 = sqrt( x*x + y*y - q1*q1 );
	r2 = sqrt( x*x + y*y - q1*q1 + q2*q2 );

	/// �֐�1�̓��ٓ_����
	if ( ( fabs(x) < 0.001 ) && ( fabs(y) < 0.001 ) )
	{
		/// �G���[�֐ߔԍ����L�^
		legLastErrorJointNo = 1;

		return KINE_ERROR_SINGULAR;
	}
	/// �֐�3�̉�͂̂��ߊ֐�1�̒l���ꎞ�ۑ�
	formerJoint1 = legData.jointAngle(1);

	/// �֐�1�̒l���v�Z
	legData.jointAngle(1) = atan2(y, x) - poseIndicator[0]*atan2(q1, r1);

/*

	/// �֐�3�̐����C�]�����v�Z
	cos3 = ( r2*r2 - dhParam.a[1]*dhParam.a[1] - dhParam.a[2]*dhParam.a[2] )/( 2.0*dhParam.a[1]*dhParam.a[2] );
	

	/// �֐�3�̗]�����v�Z�\���ǂ���

	if ( fabs(cos3) > 1.0 )
	{
		/// �t�^���w�s�Ȃ̂Ŋ֐�1�̒l��߂�
		legData.jointAngle(1) = formerJoint1;
		
		/// �G���[�֐ߔԍ����L�^
		legLastErrorJointNo = 3;
		
		return KINE_ERROR_UNREACHABLE;
	}
	else if ( fabs(cos3) > 0.99 )
	{
		/// ���ٓ_
		/// �G���[�֐ߔԍ����L�^
		legLastErrorJointNo = 3;

		return KINE_ERROR_SINGULAR;
	}
	
	/// �֐�3�̐������v�Z
	sin3 = poseIndicator[1]*sqrt( 1.00 - cos3*cos3);
	

	/// �֐�3�̒l����
	legData.jointAngle(3) = atan2(sin3, cos3);

	
	/// �֐�2�̕⏕�p�̗]�����v�Z
	cosA = ( r2*r2 + dhParam.a[1]*dhParam.a[1] - dhParam.a[2]*dhParam.a[2] )/(2.00*r2*dhParam.a[1]);
	sinA = sqrt( 1.00 - cosA*cosA);
	
	/// �֐�2�̒l����
	legData.jointAngle(2) = -atan2(q2, poseIndicator[0]*r1) - poseIndicator[0]*poseIndicator[1]*atan2(sinA, cosA);

	
*/
	//q1 = sqrt(x*x +	y*y) - LINK1 - FOOT * cos(LegTipAngle );
	//q2 = z + FOOT * sin(LegTipAngle );
	//20221026
	q1 = sqrt(x * x + y * y) - LINK1 - FOOT * cos(legData.LegTipAngle);
	q2 = z + FOOT * sin(legData.LegTipAngle);
	
	r1 = (q1*q1 + q2*q2 + LINK2*LINK2 -LINK3*LINK3)/(2*LINK2);
	r2 = (q1*q1 + q2*q2 - LINK2*LINK2 +LINK3*LINK3)/(2*LINK3);

	/// ��2�֐ߌv�Z
	legData.jointAngle(2) = -atan2(q2, q1)						- atan2(sqrt(q1*q1+q2*q2-r1*r1),r1);

	///��3�֐ߌv�Z
	legData.jointAngle(3) = atan2(sqrt(q1*q1+q2*q2-r2*r2),r2)	+ atan2(sqrt(q1*q1+q2*q2-r1*r1),r1);

	///����֐�
	//legData.FootjointAngle = LegTipAngle - legData.jointAngle(2) - legData.jointAngle(3);
	legData.FootjointAngle = legData.LegTipAngle - legData.jointAngle(2) - legData.jointAngle(3);  //20221026

	/// �Ō�Ɋe�֐߂̉��͈͂��m�F
	kine = checkLegJointMotionRange();

	/// �֐߂����͈͊O
	if (kine != NO_KINE_ERROR)
	{
		cerr << "Error: [Leg::solveDirectKinematics] Invalid joint angle" << endl;

		return kine;
	}


	///�{�[���˂����W�v�Z(���C���ψ�)
	calculationActatorPosition();

	return kine;
	
}

/**
 *	����
 *		�p���w�W�̕ύX
 *		�r�̎p�������肷��
 *		���ɍ��C�ځC�G���猈�߂�D�w�W��1 or -1
 */
void Leg::setLegPoseIndicator(int hip, int knee)
{
	if ( abs(hip) != 1 || abs(knee) != 1 )
	{
			cerr << "Error: [Leg::setPoseIndicator] Invalid argument" << endl;
            return;
	}

	poseIndicator[0]		= hip;
	poseIndicator[1]		= knee;

	return;
}

/**
 *	����
 *		�r�̍����̈ʒu�p������
 *		����͈ʒu: x, y, z, �p��: �ƂŌ��肷��
 */
void Leg::setLegBasePose(double x, double y, double z, double theta)
{
	/// �r�����p�����[�^�ɑ��
	basePose(1) = x;
	basePose(2) = y;
	basePose(3) = z;
	basePose(4) = theta;

	return;
}

/**
 *	----------------------------------------
 *	�r�̋@�\�������`�F�b�N
 *	----------------------------------------
 */
/// �֐߂̉��͈͂𒲍�
Kinematics Leg::checkLegJointMotionRange(void)
{
	for (int i=1; i<=LEG_JOINT_NUM; i++)
	{
		if ( legData.jointAngle(i) < legLimit.angleMin[i-1] )
		{
			/// ���͈͂̉����ȉ�
			legLastErrorJointNo = i;

			return KINE_ERROR_JOINT_UNDER_LIMIT;
		}
		else if ( legData.jointAngle(i) > legLimit.angleMax[i-1] )
		{
			/// ���͈͂̏���ȏ�
			legLastErrorJointNo = i;

			return KINE_ERROR_JOINT_OVER_LIMIT;
		}

	}

	if (legData.FootjointAngle < legLimit.FootangleMin)
	{
		/// ���͈͂̉����ȉ�
		legLastErrorJointNo = 4;

		return KINE_ERROR_JOINT_UNDER_LIMIT;
	}
	else if (legData.FootjointAngle > legLimit.FootangleMax)
	{
		/// ���͈͂̏���ȏ�
		legLastErrorJointNo = 4;

		return KINE_ERROR_JOINT_OVER_LIMIT;
	}

	/// �G���[�Ȃ�
	legLastErrorJointNo = 0;

	return NO_KINE_ERROR;
}

/// �r�̃��[�`�𒲍�
Kinematics Leg::checkLegFootReachRange(void)
{
	/// �r���W�n�ł̋r��̓����ϊ��s��
	Matrix transform(DH_DIMENSION, DH_DIMENSION);
	/// �v�Z�p�⏕�ϐ�
	double x, y, z;

	/// �O���[�o�����W�n����r���W�n�ւ̕ϊ�
	transform = legBaseInverseHomogeneousTransformation()*legData.footTransformation;
	/// �v�Z�p�̕ϐ��ɑ��
	x = transform(1, 4);
	y = transform(2, 4);
	//z = transform(3, 4) + FOOT;
	z = transform(3, 4) + FOOT * sin(legData.LegTipAngle);  //20221026

	/// ���a�����̉��͈�	//���͈͓����Ƃ����A�͈͓���NO_KINE_ERROR
	if ( sqrt( x*x + y*y ) < legLimit.reachRadiusMin )
	{
		return KINE_ERROR_REACH_RADIUS_UNDER;
	}
	else if ( sqrt( x*x + y*y ) > legLimit.reachRadiusMax )
	{
		return KINE_ERROR_REACH_RADIUS_OVER;
	}

	/// ���������̉��͈�
	if ( z < legLimit.reachHeightMin)
	{
		return KINE_ERROR_REACH_HEIGHT_UNDER;
	}
	else if ( z > legLimit.reachHeightMax )
	{
		return KINE_ERROR_REACH_HEIGHT_OVER;
	}

	/// ���͈͓�
	return NO_KINE_ERROR;
}

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
 *		�i���s���ɌĂ΂��j20201015
 */
Kinematics Leg::placeLegFootPosition(const Vector& nextFootPosition)
{
	Matrix lastFootTransform(DH_DIMENSION, DH_DIMENSION);

	/// �����`�F�b�N
	if ( nextFootPosition.getSize() != THREE_DIMENSION )
	{
			cerr << "Error: [Leg::placeFootPosition] Invalid argument" << endl;

			return KINE_ERROR_ELSE;
	}

	/// ����̓����ϊ��s����ꎞ�ۑ�
	lastFootTransform = legData.footTransformation;
	/// �ڕW����ʒu����	
	for (int i=1; i<=THREE_DIMENSION; i++)
		legData.footTransformation(i, 4) = nextFootPosition(i);

	/// �t�^���w������
	Kinematics kine = this->solveInverseKinematics();

	/// �t�^���w�̌��ʂőI��
	//*
	switch (kine)
	{
		case NO_KINE_ERROR:
			/// ���^���w�������ēr���̊֐߈ʒu���v�Z
			kine = solveDirectKinematics();
			break;

		case KINE_ERROR_UNREACHABLE:
			/// ����ʒu�����ɖ߂�
			legData.footTransformation = lastFootTransform;
			cerr << "Error1" << endl;
			break;

		case KINE_ERROR_SINGULAR:
			/// ����ʒu�����ɖ߂�
			legData.footTransformation = lastFootTransform;
			//cerr << "Error2" << endl;
			break;

		case KINE_ERROR_ELSE:
			/// ����ʒu�����ɖ߂�
			legData.footTransformation = lastFootTransform;
			cerr << "Error3" << endl;
			break;

		default:
			break;
	}
	//*/

	return kine;
}

/** 
 *	����
 *		�֐ߊp���Z�b�g
 *		�Z�b�g�����֐ߊp�ŏ��^���w������
 *		�iWalkingRobot�̃R���X�g���N�^�ŌĂ΂��j20201015
 */
Kinematics Leg::placeLegJointAngles(const Vector& nextJointAngle, const double& nextFootJointAngle)
{
	/// �ꎞ�ۑ��p
	Vector lastJointAngle(THREE_DIMENSION);
	double lastFootJointAngle;

	/// �����`�F�b�N
	if ( nextJointAngle.getSize() != 3 )
	{
			cerr << "Error: [Leg::placeJointAngles] Invalid argument" << endl;
			return KINE_ERROR_ELSE;
	}

	/// ���݂̊֐ߊp���ꎞ�ۑ�
	lastJointAngle = legData.jointAngle;
	legData.jointAngle = nextJointAngle;  //20201018
	lastFootJointAngle = legData.FootjointAngle;
	legData.FootjointAngle = nextFootJointAngle;  //20201018

	/// ���^���w������
	Kinematics kine = solveDirectKinematics();
	switch (kine)
	{
		case NO_KINE_ERROR:
			break;

		case KINE_ERROR_UNREACHABLE:
			legData.jointAngle = lastJointAngle;
			break;

		case KINE_ERROR_SINGULAR:
			legData.jointAngle = lastJointAngle;
			break;

		case KINE_ERROR_ELSE:
			legData.jointAngle = lastJointAngle;
			break;
	}

	return kine;

	

}

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
void Leg::calculateLegJointVelocity(const Vector& footVelocity)
{
	return;
}

/**
 *	����
 *		�֐߃g���N: [3]
 *		�r��[�ɉ�������׏d����v�Z
 */
void Leg::calculateLegJointTorque(const Vector& footReaction)
{
	return;
}

/**
 *	���R�r�A�����g�̌v�Z
 */
/// ���R�r�s��: [3x3]
void Leg::calculateJacobian(void)
{
	return;
}

/// �t���R�r�s��: [3x3]
void Leg::calculateInverseJacobian(void)
{
	return;
}
/**
 *	����
 *		�{�[���˂����W�v�Z�i���C���ψʁj
 */
void Leg::calculationActatorPosition(void)
{
	/*
	legData.ActPos(1)	= Pulley_Radius *legData.jointAngle(1)		+ Pulley_Radius *legData.jointAngle(2);//123;//
	legData.ActPos(2)	= Pulley_Radius *legData.jointAngle(2)		- Pulley_Radius *legData.jointAngle(1);//456;//
	legData.ActPos(3)	= Pulley_Radius *legData.jointAngle(2)		+ Pulley_Radius *legData.jointAngle(3);//789;//
	legData.ActPos(4)	= Pulley_Radius *legData.jointAngle(3)		+ Pulley_Radius *legData.FootjointAngle;//1011;//+ Pulley_Radius *legData.jointAngle(4);
	*/

	/*
	legData.ActPos(1) = (-Pulley_Radius * legData.jointAngle(1) + 2 * Pulley_Radius * legData.jointAngle(2));//123;//
	legData.ActPos(2) = -(Pulley_Radius * legData.jointAngle(1) + 2 * Pulley_Radius * legData.jointAngle(2));//456;//
	legData.ActPos(3) = (Pulley_Radius * legData.jointAngle(2) + Pulley_Radius * legData.jointAngle(3));//789;//
	legData.ActPos(4) = (Pulley_Radius * legData.jointAngle(3) + Pulley_Radius * legData.FootjointAngle);//1011
	//*/

	//*  �Ȃ����l������Ȃ��̂Œ���
	legData.ActPos(1) = (-Pulley_Radius * legData.jointAngle(1) + 2 * Pulley_Radius * legData.jointAngle(2)) * 0.983818;//123;//
	legData.ActPos(2) = -(Pulley_Radius * legData.jointAngle(1) + 2 * Pulley_Radius * legData.jointAngle(2))  * 0.980921;//456;//
	legData.ActPos(3) = (Pulley_Radius * legData.jointAngle(2) + Pulley_Radius * legData.jointAngle(3)) * 0.982403;//789;//
	legData.ActPos(4) = (Pulley_Radius * legData.jointAngle(3) + Pulley_Radius * legData.FootjointAngle) * 0.983152;//1011
	//*/

	return;
}

/**
 *	----------------------------------------------------------------------
 *		Leg�̓����N���X�@LegData�N���X
 *	----------------------------------------------------------------------
 */
/**
 *	----------------------------------------
 *	�R���X�g���N�^�ƃf�X�g���N�^
 *	----------------------------------------
 */
/// �f�t�H���g�R���X�g���N�^
Leg::LegData::LegData()
{
	newLegData();
	
}

/// �f�X�g���N�^
Leg::LegData::~LegData()
{
	deleteLegData();
}

/**
 *	------------------------------------------------------------
 *		TrackData�N���X��private�ȃ����o�֐�
 *	------------------------------------------------------------
 */
/// �I�u�W�F�N�g�̃������̈���m�ۂ���
void Leg::LegData::newLegData(void)
{
	/// �����o�̃������̈�m��
	jointTransformation	=	new Matrix[LEG_JOINT_NUM];
	jointPosition		=	new Vector[LEG_JOINT_NUM];

	/// �s��̃T�C�Y����
	/// �����ϊ��s��
	baseTransformation.setSize(DH_DIMENSION, DH_DIMENSION);
	baseTransformation.loadIdentity();

	for ( int i=0; i<LEG_JOINT_NUM; i++)
	{
		jointTransformation[i].setSize(DH_DIMENSION, DH_DIMENSION);
		jointTransformation[i].loadIdentity();
	}

	footTransformation.setSize(DH_DIMENSION, DH_DIMENSION);
	footTransformation.loadIdentity();

	/// �ʒu�x�N�g��
	basePosition.setSize(THREE_DIMENSION);

	for (int j=0; j<LEG_JOINT_NUM; j++)
		jointPosition[j].setSize(THREE_DIMENSION);

	footPosition.setSize(THREE_DIMENSION);

	/// �֐ߊp�x
	jointAngle.setSize(LEG_JOINT_NUM);
	/// �֐ߑ��x
	jointVelocity.setSize(LEG_JOINT_NUM);
	/// �֐߃g���N
	jointTorque.setSize(LEG_JOINT_NUM);
	/// ����֐ߊp�x
	//FootjointAngle.setSize(1);
	FootjointAngle;

	///�A�N�`���G�[�^���W
	//ActPos		=	new Vector[LEG_ACT_NUM];
	ActPos.setSize(LEG_ACT_NUM);
	//�ϊ��s��
	//ActTransformation.setSize(LEG_ACT_NUM,4);
	return;
}

/// �I�u�W�F�N�g�̃������̈���������
void Leg::LegData::deleteLegData(void)
{
	delete [] jointTransformation;
	delete [] jointPosition;

	return;
}

} /// end of namespace Asura