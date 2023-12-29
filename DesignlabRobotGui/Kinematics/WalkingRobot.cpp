/**
 *  �t�@�C����
 *		WalkingRobot.cpp
 *  ����
 *		���s���{�b�g�̏��C�^���w
 *		LegTrack�N���X�̃R���|�W�V����
 *  ���t
 *		�쐬��: 2007/02/05(MON)		�X�V��: 2007/11/05(MON) SAURUS
 */

//  20200819  �N���[���֘A�R�����g�A�E�g�ETrackLeg�������Leg���Ă�
//  20201015  ��������
//  20201018  ��4�֐�

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include "WalkingRobot.h"

using namespace std;
using namespace Math;
using namespace Const;

/**
 *	----------------------------------------------------------------------
 *		WalkingRobot�N���X
 *	----------------------------------------------------------------------
 */
/**
 *				���W�n�̒�`
 *
 *		 �E��n					
 *									
 *						 y		
 *		x  1			6��		
 *	   ����	|-----------|����	
 *	     ��	|	  x		|   x			 
 *	     y	|	  ��	|		
 *			|  ����		|
  *			|    y		|y		
 *	    x	|			|��	
 *	   ����	|-----------|����	
 *	     ��2|			|5   x	
 *	     y	|			|
 *			|    		|y		
 *	    x	|			|��	
 *	   ����	|-----------|����	
 *	     ��3			4   x	
 *	     y				
 */

namespace Asura
{
/**
 *	------------------------------------------------------------
 *		WalkingRobot�N���X�̃����o�֐���`
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	�R���X�g���N�^�ƃf�X�g���N�^
 *	----------------------------------------
 */
/// �f�t�H���g�R���X�g���N�^
WalkingRobot::WalkingRobot()
{
	/// �r�I�u�W�F�N�g�̐���
	newTrackLegs();

	/// ������
	initializeWalkingRobot();

}
/// �f�X�g���N�^
WalkingRobot::~WalkingRobot()
{
	/// �I�u�W�F�N�g����
	deleteTrackLegs();
}

/// �������֐�
void WalkingRobot::initializeWalkingRobot(void)
{
	/// ���̈ʒu�̏�����
	//initializeBodyPosition(0.0, 0.0, 500.0);
	//20201015
	initializeBodyPosition(0.0, 0.0, Plan::TRIPODGAIT_INITIAL_BODY_POSITION[2]);

	Vector* initialJointAngle;
	initialJointAngle = new Vector[LEG_NUM];

	double* initialFootJointAngle;  //20201018
	initialFootJointAngle = new double[LEG_NUM];  //20201018

	int i, j;
	for (i=0; i<LEG_NUM; i++)
		initialJointAngle[i].setSize(LEG_JOINT_NUM);

	initialJointAngle[0] = Vector(LEG_INITIAL_ANGLE1, LEG_JOINT_NUM);
	initialJointAngle[1] = Vector(LEG_INITIAL_ANGLE2, LEG_JOINT_NUM);
	initialJointAngle[2] = Vector(LEG_INITIAL_ANGLE3, LEG_JOINT_NUM);
	initialJointAngle[3] = Vector(LEG_INITIAL_ANGLE4, LEG_JOINT_NUM);
	initialJointAngle[4] = Vector(LEG_INITIAL_ANGLE5, LEG_JOINT_NUM);
	initialJointAngle[5] = Vector(LEG_INITIAL_ANGLE6, LEG_JOINT_NUM);

	//20201018
	initialFootJointAngle[0] = LEG_INITIAL_ANGLE1[3];
	initialFootJointAngle[1] = LEG_INITIAL_ANGLE2[3];
	initialFootJointAngle[2] = LEG_INITIAL_ANGLE3[3];
	initialFootJointAngle[3] = LEG_INITIAL_ANGLE4[3];
	initialFootJointAngle[4] = LEG_INITIAL_ANGLE5[3];
	initialFootJointAngle[5] = LEG_INITIAL_ANGLE6[3];
	

	/// �r�֐ߊp�̏�����
	for (j=0; j<LEG_NUM; j++)
		placeLegJointAngles( j+1, initialJointAngle[j], initialFootJointAngle[j] );

	delete [] initialJointAngle;
	delete[] initialFootJointAngle;
}

/**
 *	----------------------------------------
 *	���{�b�g�{�̂Ɋւ������
 *	----------------------------------------
 */
/**
 *	----------------------------------------
 *	�Z�b�g�֐�
 *	----------------------------------------
 */
/// ���̂̈ʒu�����V
void WalkingRobot::initializeBodyTransformation(const Math::Matrix& newBodyTransformation)
{
	bodyData.transformation = newBodyTransformation;

	int i;
	for (i=0; i<THREE_DIMENSION; i++)
		bodyData.position(i+1) = bodyData.transformation(i+1, 4);

	return;
}

/// ���̂̈ʒu�����V
void WalkingRobot::initializeBodyPosition(const Math::Vector& newBodyPosition)
{
	bodyData.position = newBodyPosition;

	int i;
	for (i=0; i<THREE_DIMENSION; i++)
		bodyData.transformation(i+1, 4) = bodyData.position(i+1);

	return;
}
void WalkingRobot::initializeBodyPosition(double x, double y, double z)
{
	bodyData.position(1) = x;
	bodyData.position(2) = y;
	bodyData.position(3) = z;

	int i;
	for (i=0; i<THREE_DIMENSION; i++)
		bodyData.transformation(i+1, 4) = bodyData.position(i+1);

	return;
}

/// �ړ��l��
void WalkingRobot::setLocomotionStyle(LocomotionStyle style)
{
	locomotionStyle = style;
}

/// �r�̉^����
void WalkingRobot::setLegPhase(int legNo, LegPhase phase)
{
	/// �����`�F�b�N
	assert( 1 <= legNo && legNo <= LEG_NUM );

	legPhases[legNo-1] = phase;

	return;
}

/**
 *	----------------------------------------
 *	�^���w
 *	----------------------------------------
 */
/// ���̂̈ʒu�����肷��
Kinematics WalkingRobot::placeBodyPosition(Math::Vector& nextBodyPosition)
{
	/// �^���w����
	Kinematics kine;
	/// �ꎞ�ۑ��̂��߂̓��̓����ϊ��s��
	Matrix lastBodyTransform(DH_DIMENSION, DH_DIMENSION);
	Vector presentGlobalFootPosition[LEG_NUM];
	Vector nextLocalFootPosition[LEG_NUM];

	/// �J�E���^
	int i, j;

	/// �ꎞ�ۑ�
	lastBodyTransform = bodyData.transformation;

	/// ���̈ʒu���X�V����O�̋r�̃O���[�o�����W���擾
	for (i=0; i<LEG_NUM; i++)
	{
		/// �x�N�g���T�C�Y����
		presentGlobalFootPosition[i].setSize(THREE_DIMENSION);
		/// ���݂̋r�ʒu�𓷑̍��W�n�ŕۑ�
		presentGlobalFootPosition[i] = transformationLocalToGlobal( trackLegs[i]->getLegFootPosition() );
	}

	/// ���̈ʒu���X�V
	for (i=1; i<=THREE_DIMENSION; i++)
		bodyData.transformation(i, 4) = nextBodyPosition(i);

	/// �X�V�������̍��W�n�ł̐V�������[�J���r�ʒu���v�Z
	for (j=0; j<LEG_NUM; j++)
	{
		/// �x�N�g���T�C�Y����
		nextLocalFootPosition[j].setSize(THREE_DIMENSION);
		/// ���̍��W�n�ł̎��̋r�ʒu���v�Z
		nextLocalFootPosition[j] = transformationGlobalToLocal( presentGlobalFootPosition[j] );
	}

	/// �x���r�݂̂œ��̈ʒu��ύX
	for (j=0; j<LEG_NUM; j++)
	{
		if (legPhases[j] == SUPPORT )
		{
			/// �V��������ڕW�l�ɒu��
			kine = trackLegs[j]->placeLegFootPosition( nextLocalFootPosition[j] );
			
			/// �r�悪�V�����w�ߒl�ɓ��B�ł��Ȃ�������
			if ( kine != NO_KINE_ERROR )
			{
				/// ���̂̏�Ԃ��ŏ��ɖ߂�
				bodyData.transformation = lastBodyTransform;

				/// �G���[�R�[�h���L�^
				lastKineError = kine;
				lastErrorTrackLegNo = i+1;
				
				return kine;
			}
		}
	}

	/// �S�Đ��������̂œ��̈ʒu�X�V
	bodyData.position = nextBodyPosition;

	return NO_KINE_ERROR;
}

/// ���̂̈ʒu�����肷��
Kinematics WalkingRobot::placeBodyPosture(Math::Matrix& nextBodyPosture)
{
	/// �^���w����
	Kinematics kine;

	return NO_KINE_ERROR;
}

/// ���̂̈ʒu�p���𓯎��Ɍ��肷��
Kinematics WalkingRobot::placeBodyFrame(Math::Matrix& nextBodyFrame)
{
	/// �^���w����
	Kinematics kine;

	return NO_KINE_ERROR;
}

/**
 *	----------------------------------------
 *	�r�Ɋւ������
 *	TrackLeg�N���X������r�N���X�֐��̌Ăяo��
 *	----------------------------------------
 */
/**
 *	----------------------------------------
 *	�A�N�Z�X�֐�
 *	----------------------------------------
 */
/// �r�����̓����ϊ��s��
const Matrix& WalkingRobot::getLegBaseTransformation(int legNo) const
{
	/// �����`�F�b�N
	assert( 1 <= legNo && legNo <= LEG_NUM );
	return trackLegs[legNo-1]->getLegBaseTransformation();
}
/// �֐߂̓����ϊ��s��
const Matrix& WalkingRobot::getLegJointTransformation(int legNo, int jointNo) const
{
	/// �����`�F�b�N
	assert( 1 <= legNo && legNo <= LEG_NUM );
	assert( 1 <= jointNo && jointNo <= LEG_JOINT_NUM );
	return trackLegs[legNo-1]->getLegJointTransformation( jointNo );
}
/// �����̓����ϊ��s��
const Matrix& WalkingRobot::getLegFootTransformation(int legNo) const
{
	/// �����`�F�b�N
	assert( 1 <= legNo && legNo <= LEG_NUM );
	return trackLegs[legNo-1]->getLegFootTransformation();
}
/// �r�����̈ʒu�x�N�g��
const Vector& WalkingRobot::getLegBasePosition(int legNo) const
{
	/// �����`�F�b�N
	assert( 1 <= legNo && legNo <= LEG_NUM );
	return trackLegs[legNo-1]->getLegBasePosition();
}
/// �֐߈ʒu�̈ʒu�x�N�g��
const Vector& WalkingRobot::getLegJointPosition(int legNo, int jointNo) const
{
	/// �����`�F�b�N
	assert( 1 <= legNo && legNo <= LEG_NUM );
	assert( 1 <= jointNo && jointNo <= LEG_JOINT_NUM );
	return trackLegs[legNo-1]->getLegJointPosition( jointNo );
}
/// �����ʒu�̈ʒu�x�N�g��
const Vector& WalkingRobot::getLegFootPosition(int legNo) const
{
	/// �����`�F�b�N
	assert( 1 <= legNo && legNo <= LEG_NUM );
	return trackLegs[legNo-1]->getLegFootPosition();
}
/// �֐ߊp�x�x�N�g��
const Vector& WalkingRobot::getLegJointAngle(int legNo) const
{
	/// �����`�F�b�N
	assert( 1 <= legNo && legNo <= LEG_NUM );
	return trackLegs[legNo-1]->getLegJointAngle();
}
/// ����֐ߊp�x

const  double WalkingRobot::getFootJointAngle(int legNo) const//------Add
{
	/// �����`�F�b�N
	assert( 1 <= legNo && legNo <= LEG_NUM );
	return trackLegs[legNo-1]->getFootJointAngle();
	//trackLegs[legNo-1]->getFootJointAngle();
}
// �֐ߑ��x�x�N�g��
const Vector& WalkingRobot::getLegJointVelocity(int legNo) const
{
	/// �����`�F�b�N
	assert( 1 <= legNo && legNo <= LEG_NUM );
	return trackLegs[legNo-1]->getLegJointVelocity();
}
/// �֐߃g���N�x�N�g��
const Vector& WalkingRobot::getLegJointTorque(int legNo) const
{
	/// �����`�F�b�N
	assert( 1 <= legNo && legNo <= LEG_NUM );
	return trackLegs[legNo-1]->getLegJointTorque();
}
/// �r�̍����̈ʒu�E�p��
const Vector& WalkingRobot::getBasePose(int legNo) const
{
	/// �����`�F�b�N
	assert( 1 <= legNo && legNo <= LEG_NUM );
	return trackLegs[legNo-1]->getBasePose();
}
/// �r�ԍ��̎擾
const int WalkingRobot::getLegNo(int legNo) const
{
	/// �����`�F�b�N
	assert( 1 <= legNo && legNo <= LEG_NUM );
	return trackLegs[legNo-1]->getLegNo();
}
/// �֐߂̃G���[�ԍ�
const int WalkingRobot::getLegLastErrorJointNo(int legNo) const
{
	/// �����`�F�b�N
	assert( 1 <= legNo && legNo <= LEG_NUM );
	return trackLegs[legNo-1]->getLegLastErrorJointNo();
}
/// �{�[���˂����W�i���C���ψʁj
const Vector& WalkingRobot:: getLegActuatorPosition(int legNo) const//<----add
{
	/// �����`�F�b�N
	assert( 1 <= legNo && legNo <= LEG_NUM );
	return trackLegs[legNo-1]->getLegActuatorPosition();
}
/// �r�ԍ��̐ݒ�
void WalkingRobot::setLegNo(int legNo, int no)
{
	/// �����`�F�b�N
	assert( 1 <= legNo && legNo <= LEG_NUM );
	assert( 1 <= no && no <= LEG_NUM );
	trackLegs[legNo-1]->setLegNo(no);
	return;
}
/// ���^���w
Kinematics WalkingRobot::solveLegDirectKinematics(int legNo)
{
	/// �����`�F�b�N
	assert( 1 <= legNo && legNo <= LEG_NUM );
	Kinematics kine = trackLegs[legNo-1]->solveDirectKinematics();
	return kine;
}
/// �t�^���w
Kinematics WalkingRobot::solveLegInverseKinematics(int legNo)
{
	/// �����`�F�b�N
	assert( 1 <= legNo && legNo <= LEG_NUM );
	Kinematics kine = trackLegs[legNo-1]->solveInverseKinematics();
	//trackLegs[legNo-1]->calculationActatorPosition();//<----add
	return kine;
}
/// �p���w�W�̕ύX
void WalkingRobot::setLegPoseIndicator(int legNo, int hip, int knee)
{
	/// �����`�F�b�N
	assert( 1 <= legNo && legNo <= LEG_NUM );
	trackLegs[legNo-1]->setLegPoseIndicator(hip, knee);
	return;
}
// �r�̍����ݒ�
void WalkingRobot::setLegBasePose(int legNo, double x, double y, double z, double theta)
{
	/// �����`�F�b�N
	assert( 1 <= legNo && legNo <= LEG_NUM );
	trackLegs[legNo-1]->setLegBasePose(x, y, z, theta);
	return;
}


/**
 *	----------------------------------------
 *	�r�̉^���w
 *	----------------------------------------
 */
/**
 *	����
 *		���̈ʒu���Z�b�g�i�����͏�ɏd�͕����Ɖ���j
 *		���^���w�Ƌt�^���w����v���Ă��邩���m�F����
 *		�r���̊֐߈ʒu���v�Z����
 */
Kinematics WalkingRobot::placeLegFootPosition(int legNo, const Math::Vector& nextFootPosition)
{
	/// �����`�F�b�N
	assert( 1 <= legNo && legNo <= LEG_NUM );
	Kinematics kine = trackLegs[legNo-1]->placeLegFootPosition( nextFootPosition );

	if ( kine != NO_KINE_ERROR )
	{
		/// �G���[�R�[�h���L�^
		lastKineError = kine;
		lastErrorTrackLegNo = legNo;
	}

	return kine;
}
/**
 *	����
 *		�֐ߊp���Z�b�g
 *		�Z�b�g�����֐ߊp�ŏ��^���w������
 */
Kinematics WalkingRobot::placeLegJointAngles(int legNo, const Math::Vector& nextJointAngle, const double& nextFootJointAngle)
{
	/// �����`�F�b�N
	assert( 1 <= legNo && legNo <= LEG_NUM );
	Kinematics kine = trackLegs[legNo-1]->placeLegJointAngles( nextJointAngle, nextFootJointAngle );

	if ( kine != NO_KINE_ERROR )
	{
		/// �G���[�R�[�h���L�^
		lastKineError = kine;
		lastErrorTrackLegNo = legNo;
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
void WalkingRobot::calculateLegJointVelocity(int legNo, const Math::Vector& footVelocity)
{
	/// �����`�F�b�N
	assert( 1 <= legNo && legNo <= LEG_NUM );
	trackLegs[legNo-1]->calculateLegJointVelocity(footVelocity);
	return;
}
/**
 *	����
 *		�֐߃g���N: [3]
 *		�r��[�ɉ�������׏d����v�Z
 */
void WalkingRobot::calculateLegJointTorque(int legNo, const Math::Vector& footReaction)
{
	/// �����`�F�b�N
	assert( 1 <= legNo && legNo <= LEG_NUM );
	trackLegs[legNo-1]->calculateLegJointTorque(footReaction);
	return;
}

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
/// �N���[�������̓����ϊ��s��

/*  20200819
const Matrix& WalkingRobot::getTrackBaseTransformation(int trackNo) const
{
	/// �����`�F�b�N
	assert( 1 <= trackNo && trackNo <= TRACK_NUM );
	return trackLegs[trackNo-1]->getTrackBaseTransformation();
}
/// �֐߂̓����ϊ��s��
const Matrix& WalkingRobot::getTrackJointTransformation(int trackNo, int jointNo) const
{
	/// �����`�F�b�N
	assert( 1 <= trackNo && trackNo <= TRACK_NUM );
	assert( 1 <= jointNo && jointNo <= LEG_JOINT_NUM );
	return trackLegs[trackNo-1]->getTrackJointTransformation( jointNo );
}
/// �N���[����[�̓����ϊ��s��
const Matrix& WalkingRobot::getTrackEndTransformation(int trackNo) const
{
	/// �����`�F�b�N
	assert( 1 <= trackNo && trackNo <= TRACK_NUM );
	return trackLegs[trackNo-1]->getTrackEndTransformation();
}
/// �N���[�������̈ʒu�x�N�g��
const Vector& WalkingRobot::getTrackBasePosition(int trackNo) const
{
	/// �����`�F�b�N
	assert( 1 <= trackNo && trackNo <= TRACK_NUM );
	return trackLegs[trackNo-1]->getTrackBasePosition();
}
/// �N���[���֐߂̈ʒu�x�N�g��
const Vector& WalkingRobot::getTrackJointPosition(int trackNo, int jointNo) const
{
	/// �����`�F�b�N
	assert( 1 <= trackNo && trackNo <= TRACK_NUM );
	return trackLegs[trackNo-1]->getTrackJointPosition( jointNo );
}
/// �N���[����[�̈ʒu�x�N�g��
const Vector& WalkingRobot::getTrackEndPosition(int trackNo) const
{
	/// �����`�F�b�N
	assert( 1 <= trackNo && trackNo <= TRACK_NUM );
	return trackLegs[trackNo-1]->getTrackEndPosition();
}
/// �֐ߊp�x�x�N�g��
const Vector& WalkingRobot::getTrackJointAngle(int trackNo) const
{
	/// �����`�F�b�N
	assert( 1 <= trackNo && trackNo <= TRACK_NUM );
	return trackLegs[trackNo-1]->getTrackJointAngle();
}
/// �֐ߑ��x�x�N�g��
const Vector& WalkingRobot::getTrackJointVelocity(int trackNo) const
{
	/// �����`�F�b�N
	assert( 1 <= trackNo && trackNo <= TRACK_NUM );
	return trackLegs[trackNo-1]->getTrackJointVelocity();
}
/// �֐߃g���N�x�N�g��
const Vector& WalkingRobot::getTrackJointTorque(int trackNo) const
{
	/// �����`�F�b�N
	assert( 1 <= trackNo && trackNo <= TRACK_NUM );
	return trackLegs[trackNo-1]->getTrackJointTorque();
}
/// �N���[���X�s�[�h
const double WalkingRobot::getTrackSpeed(int trackNo) const
{
	/// �����`�F�b�N
	assert( 1 <= trackNo && trackNo <= TRACK_NUM );
	return trackLegs[trackNo-1]->getTrackSpeed();
}
/// �r�쓮����
const DriveSystem WalkingRobot::getDriveSystem(int trackNo) const
{
	/// �����`�F�b�N
	assert( 1 <= trackNo && trackNo <= TRACK_NUM );
	return trackLegs[trackNo-1]->getDriveSystem();
}
/// �N���[���`�Ԏ��ɍŌ�ɃG���[�ƂȂ����֐ߔԍ�
const int WalkingRobot::getTrackLastErrorJointNo(int trackNo) const
{
	/// �����`�F�b�N
	assert( 1 <= trackNo && trackNo <= TRACK_NUM );
	return trackLegs[trackNo-1]->getTrackLastErrorJointNo();
}
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

/*  20200819
Kinematics WalkingRobot::changeTrackPosture(int trackNo, const Math::Vector& angle)
{
	/// �����`�F�b�N
	assert( 1 <= trackNo && trackNo <= TRACK_NUM );
	Kinematics kine = trackLegs[trackNo-1]->changeTrackPosture(angle);
	return kine;
}
*/

/**
 *	����
 *		�֐ߑ��x: [3]
 *		�N���[����[�̑��x����v�Z
 */
/*  20200819
void WalkingRobot::calculateTrackJointVelocity(int trackNo, const Math::Vector& trackVelocity)
{
	/// �����`�F�b�N
	assert( 1 <= trackNo && trackNo <= TRACK_NUM );
	trackLegs[trackNo-1]->calculateTrackJointVelocity( trackVelocity );
	return;
}
*/

/**
 *	����
 *		�֐߃g���N: [3]
 *		�N���[����[�ɉ�������׏d����v�Z
 */
 /*  20200819
void WalkingRobot::calculateTrackJointTorque(int trackNo, const Math::Vector& trackReaction)
{
	/// �����`�F�b�N
	assert( 1 <= trackNo && trackNo <= TRACK_NUM );
	trackLegs[trackNo-1]->calculateTrackJointTorque( trackReaction );
	return;
}
*/

/**
 *	����
 *		�N���[�����x��ύX����
 */
 /*  20200819
void WalkingRobot::changeTrackSpeed(int trackNo, double trackSpeed)
{
	/// �����`�F�b�N
	assert( 1 <= trackNo && trackNo <= TRACK_NUM );
	trackLegs[trackNo-1]->changeTrackSpeed(trackSpeed);
	return;
}
*/

/**
 *	����
 *		�쓮�����̐ݒ�
 *		�߂�l�F�ݒ肵���쓮����
 */
/*  20200819
DriveSystem WalkingRobot::setDriveSystem(int trackLegNo, DriveSystem ds)
{
	/// �����`�F�b�N
	assert( 1 <= trackLegNo && trackLegNo <= TRACK_NUM );
	return trackLegs[trackLegNo-1]->setDriveSystem( ds );
}
*/

/**
 *	------------------------------------------------------------
 *		WalkingRobot�N���X��private�ȃ����o�֐�
 *	------------------------------------------------------------
 */
/// �I�u�W�F�N�g�̃������̈���m�ۂ���
void WalkingRobot::newTrackLegs(void)
{
	/// �r�̉^�����̔z����m��
	legPhases = new LegPhase[LEG_NUM];

	/// �N���X�|�C���^�̔z��m��
	//trackLegs = new TrackLeg*[LEG_NUM];
	trackLegs = new Leg * [LEG_NUM];

	/// �N���X�̃C���X�^���X�𐶐�
	for (int i=0; i<LEG_NUM; i++)
	{
		//trackLegs[i] = new TrackLeg( (i+1), LEG_ROOT_POSX[i], LEG_ROOT_POSY[i], LEG_ROOT_POSZ[i], LEG_ROOT_ANGLE[i] );
		trackLegs[i] = new Leg((i + 1), LEG_ROOT_POSX[i], LEG_ROOT_POSY[i], LEG_ROOT_POSZ[i], LEG_ROOT_ANGLE[i]);
	}

	return;
}

/// �I�u�W�F�N�g�̃������̈���������
void WalkingRobot::deleteTrackLegs(void)
{
	delete [] legPhases;

	/// �C���X�^���X�̔j��
	for (int i=0; i<LEG_NUM; i++)
		delete trackLegs[i];

	/// �N���X�|�C���^�̔j��
	delete [] trackLegs;

	return;
}

/**
 *	----------------------------------------------------------------------
 *		WalkingRobot�̓����N���X�@BodyData�N���X
 *	----------------------------------------------------------------------
 */
/**
 *	----------------------------------------
 *	�R���X�g���N�^�ƃf�X�g���N�^
 *	----------------------------------------
 */
/// �f�t�H���g�R���X�g���N�^
WalkingRobot::BodyData::BodyData()
{
	initBodyData();
}

/// �f�X�g���N�^
WalkingRobot::BodyData::~BodyData()
{
}

/**
 *	------------------------------------------------------------
 *		BodyData�N���X��private�ȃ����o�֐�
 *	------------------------------------------------------------
 */
/// �I�u�W�F�N�g�̃������̈���m�ۂ���
void WalkingRobot::BodyData::initBodyData(void)
{
	/// �s��̃T�C�Y����
	transformation.setSize(DH_DIMENSION, DH_DIMENSION);
	transformation.loadIdentity();

	/// �ʒu�x�N�g���̃T�C�Y����
	position.setSize(THREE_DIMENSION);

	/// ���x�x�N�g���̃T�C�Y����
	velocity.setSize(THREE_DIMENSION);

	return;
}

} /// end of namespace Asura