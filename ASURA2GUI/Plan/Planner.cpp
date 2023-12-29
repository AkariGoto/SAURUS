/**
 *  �t�@�C����
 *		Planner.cpp
 *  ����
 *		�ΏۂƂ���WalkingRobot�I�u�W�F�N�g�̊�{����𐶐�����N���X
 *  ���t
 *		�쐬��: 2007/03/06(TUE)		�X�V��: 2007/03/12(MON)
 */

//  20200819  ����]�T�֘A�R�����g�A�E�g
//  20201020  �����~��̍ē���
 
 /**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include "Planner.h"
#include "..\Math\MathLibrary.h"
#include "..\Math\Matrix\Matrix.h"

using namespace std;
using namespace Math;
using namespace Asura;
using namespace Const;

namespace Plan
{
/**
 *	----------------------------------------------------------------------
 *		Planner�N���X
 *	----------------------------------------------------------------------
 */

/**
 *	------------------------------------------------------------
 *		Planner�N���X�̃����o�֐���`
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	�R���X�g���N�^�ƃf�X�g���N�^
 *	----------------------------------------
 */
/// �f�t�H���g�R���X�g���N�^
Planner::Planner(AsuraX* asuraPointer_, TimeManager* timeManagerPointer_)
{
	/// �|�C���^�̃Z�b�g
	asuraPointer = asuraPointer_;
	timeManagerPointer = timeManagerPointer_;

	/// �O���v�f�𐶐�
	newTrajectories();

	/// �����o�ϐ��̏�����
	isRunning = false;
	isSuspended = false;

	//stabilityMargin = 0.0;  20200819
	elapsedTime = 0.0;
	planStartTime = 0.0;
}

/// �f�X�g���N�^
Planner::~Planner()
{
	/// �k���|�C���^���Z�b�g
	asuraPointer = NULL;

	/// �O���v�f������
	deleteTrajectories();
}


/**
 *		���䂷����s���{�b�g�I�u�W�F�N�g�ւ̃|�C���^
 */
/// �|�C���^���擾
void Planner::acquireAsura(AsuraX* ptr)
{
	/// ���݂̃|�C���^�����Z�b�g
	releaseAsura();

	asuraPointer = ptr;

	return;
}
/// �|�C���^�����
void Planner::releaseAsura(void)
{
	asuraPointer = NULL;

	return;
}

/**
 *		�g�p���鎞�ԊǗ��I�u�W�F�N�g�ւ̃|�C���^
 */

/// �|�C���^���擾
void Planner::acquireTimeManager(TimeManager* ptr)
{
	/// ���݂̃|�C���^�����Z�b�g
	releaseTimeManager();

	timeManagerPointer = ptr;

	return;
}

/// �|�C���^�����
void Planner::releaseTimeManager(void)
{
	timeManagerPointer = NULL;

	return;
}

/**
 *		�O���p�����[�^�̐ݒ�
 */
/// �V�r�O����ݒ肷��
void Planner::setLegSwingTrajectory(
		int legNo, 
		const Math::Vector& start,
		const Math::Vector& upPhase,
		const Math::Vector& returnPhase, 
		const Math::Vector& downPhase
	)
{
	/// �r�̈����`�F�b�N
	assert( 1 <= legNo && legNo <= LEG_NUM ); 

	/// �O���v�f���Z�b�g
	legTrajectory[legNo-1].setLegTrajectory(start, upPhase, returnPhase, downPhase);

	return;
}

/// �V�r���Ԃ�ݒ肷��
void Planner::setLegSwingTime(
		int legNo, 
		double start,
		double upPhase, 
		double returnPhase, 
		double downPhase
	)
{
	/// �r�̈����`�F�b�N
	assert( 1 <= legNo && legNo <= LEG_NUM ); 

	/// �O���v�f���Z�b�g
	legTrajectory[legNo-1].setLegTrajectoryTime(start, upPhase, returnPhase, downPhase);

	return;
}

/**
 *		�V�r�̋O����̈ʒu���擾����
 */
Vector Planner::getLegSwingPosition(int legNo, double splitTime)
{
	/// �r�̈����`�F�b�N
	assert( 1 <= legNo && legNo <= LEG_NUM ); 

	return legTrajectory[legNo-1].getPosition(splitTime);
}

/**
 *	----------------------------------------
 *	���ۂɓ�����s���֐�
 *	----------------------------------------
 */
/**
 *		�V�r����
 *		�\�߃��[�U���ݒ肵���O����V�r����
 */
PlanStatus Planner::swingLeg(int legNo, double splitTime)
{
	/// �����`�F�b�N
	assert( 1 <= legNo && legNo <=LEG_NUM );

/**
 *		���[�J���ϐ��̐錾
 */
	/// �^���w�̌���
	Kinematics kine;
	/// �G���[���N�������֐ߔԍ�
	int errorJointNo = 0;

	/// �����~����������V�r���Ȃ�
	if ( isSuspended )
	{
		return (SUSPEND);
	}

	/// �V�r���ԓ����ǂ����̃`�F�b�N
	if ( ( legTrajectory[legNo-1].getStartTime() < splitTime ) && ( splitTime < legTrajectory[legNo-1].getGoalTime() ) )
	{
		/// �����̎��Ԃł̋O����̈ʒu���擾
		/// PTP����ɂ��C����ڕW�ʒu�ɃZ�b�g
		kine = asuraPointer->placeLegFootPosition( legNo, legTrajectory[legNo-1].getPosition(splitTime) );

		/// �t�^���w�����������ǂ����̃`�F�b�N
		if ( kine != NO_KINE_ERROR )
		{
			printPlanErrorMessage();
			return INVALID;
		}

		/// �r�̏�Ԃ�V�r�ɃZ�b�g
		asuraPointer->setLegPhase(legNo, SWING);
	}
	else
	{
		/// �����̎��Ԃł̋O����̈ʒu���擾
		/// PTP����ɂ��C����ڕW�ʒu�ɃZ�b�g
		kine = asuraPointer->placeLegFootPosition( legNo, legTrajectory[legNo-1].getPosition(splitTime) );

		/// �t�^���w�����������ǂ����̃`�F�b�N
		if ( kine != NO_KINE_ERROR )
		{
			printPlanErrorMessage();
			return INVALID;
		}

		/// �r�̏�Ԃ��x���r�ɃZ�b�g
		asuraPointer->setLegPhase(legNo, SUPPORT);

		/// ����I��
		return END;
	}

	/// ������s��
	return RUN;
}

/**
 *		���̐��i
 */
PlanStatus Planner::moveBody(double splitTime)
{

	/// �����~����������V�r���Ȃ�
	if ( isSuspended )
	{
		return (SUSPEND);
	}

/**
 *		���[�J���ϐ��̐錾
 */
	/// �O����̖ڕW�r��ʒu
	Vector targetPosition(THREE_DIMENSION);
	/// �^���w�̌���
	Kinematics kine;
	/// �G���[���N�������r�ԍ�
	int errorLegNo = 0;
	/// �G���[���N�������֐ߔԍ�
	int errorJointNo = 0;

	/// �����̎��Ԃł̋O����̈ʒu���v�Z
	targetPosition = bodyTrajectory->getPosition(splitTime);

	/// PTP����ɂ��d�S��ڕW�ʒu�ɃZ�b�g
	kine = asuraPointer->placeBodyPosition(targetPosition);

	/// �t�^���������邩�ǂ����̃`�F�b�N
	if ( kine != NO_KINE_ERROR )
	{
		Planner::printPlanErrorMessage();

		return INVALID;
	}

	/// ���̐��i���ԓ��ɂ��邩�ǂ����`�F�b�N
	if ( !( bodyTrajectory->getStartTime() < splitTime ) || !( splitTime < bodyTrajectory->getGoalTime() ) )
	{
		return END;
	}

	return RUN;


}

/**
 *		�G���[���b�Z�[�W�̏o��
 */
void Planner::printPlanErrorMessage(void)
{
	Kinematics kine = asuraPointer->getLastKineError();
	int legNo = asuraPointer->getLastErrorTrackLegNo();
	int jointNo = asuraPointer->getLegLastErrorJointNo(legNo);
	char error[256] = "No error";

	switch ( kine )
	{
		case KINE_ERROR_UNREACHABLE:
			sprintf_s(error, "Unreachable operation");
			break;
		
		case KINE_ERROR_SINGULAR:
			sprintf_s(error, "Singular posture");
			break;

		case KINE_ERROR_ELSE:
			sprintf_s(error, "Uncertain error");
			break;

		case KINE_ERROR_REACH_RADIUS_UNDER:
			sprintf_s(error, "Under minimum horizontal reach");
			break;

		case KINE_ERROR_REACH_RADIUS_OVER:
			sprintf_s(error, "Over maximum horizontal reach");
			break;

		case KINE_ERROR_REACH_HEIGHT_UNDER:
			sprintf_s(error, "Under minimum vertical reach");
			break;

		case KINE_ERROR_REACH_HEIGHT_OVER:
			sprintf_s(error, "Under maximum vertical reach");
			break;

		case KINE_ERROR_JOINT_UNDER_LIMIT:
			sprintf_s(error, "Under minimum joint angle");
			break;

		case KINE_ERROR_JOINT_OVER_LIMIT:
			sprintf_s(error, "Over maximum joint angle");
			break;

		case KINE_ERROR_JOINT_VELOCITY_LIMIT:
			sprintf_s(error, "Over maximum joint angular velocity");
			break;

		case KINE_ERROR_JOINT_TORQUE_LIMIT:
			sprintf_s(error, "Over maximum joint torque velocity");
			break;
	}

	cout << " Joint-" << jointNo << " of " << "Leg-" << legNo << ":" << error << endl;	
}

/**
 *	----------------------------------------
 *	�^������̓I�ɐ�������֐��Q 
 *	----------------------------------------
 */
/// ���s�J�n�̂��߂̏�����
bool Planner::setup(void)
{
	elapsedTime = 0.0;  //20201020
	stopElapsedTime = elapsedTime;  //20201020

	/// ����J�n
	isRunning = true;

	return true;
}

/// ���s���J�n����
bool Planner::startPlan(void)
{
	/// �v�����j���O�J�n���Ԃ�ݒ�
	//planStartTime = timeManagerPointer->getRealTime();  //20201020
	planStartTime = timeManagerPointer->getRealTime() - stopElapsedTime;  //elapsedTime�͋N������0

	return true;
}
	
/// ���s���~����
bool Planner::stopPlan(void)
{
	//isRunning = false;  //20201020

	stopElapsedTime = elapsedTime;  //20201020

	return true;
}

bool Planner::standByForStop(void)
{
	return true;
}

/**
 *		����]�T�̌v�Z
 *			����: �d�͕�������
 */
/*  20200819
double Planner::calculateStabilityMargin(double gx, double gy, double gz)
{
	/// ���ʒu
	Vector foot[LEG_NUM];
	/// �d�S�ʒu
	Vector cog;
	/// �d�͕���
	Vector gravity( positionVector(gx, gy, gz) );
	/// �r�̑�
	LegPhase phase[LEG_NUM];
	/// �d�S�ʒu
	Vector body(THREE_DIMENSION);

	/// ���ʒu�̎擾
	int i;
	for (i=0; i<LEG_NUM; i++)
	{
		foot[i].setSize(THREE_DIMENSION);

		foot[i] = asuraPointer->transformationLocalToGlobal( 
									asuraPointer->getLegFootPosition(i+1) 
									);
	}

	/// �d�S�ʒu�̎擾
	body = asuraPointer->getBodyPosition();

	/// �r�̉^�����̎擾
	int j;
	for (j=0; j<LEG_NUM; j++ )
		phase[j] = asuraPointer->getLegPhase( j+1 );


	/// �r�����Ɉ���]�T���v�Z
	if ( phase[0] == SWING )
		stabilityMargin = stabilityMarginForTriangle( foot[1], foot[2], foot[3],  body, gravity );
	else if ( phase[1] == SWING )
		stabilityMargin = stabilityMarginForTriangle( foot[0], foot[2], foot[3], body, gravity );
	else if ( phase[2] == SWING )
		stabilityMargin = stabilityMarginForTriangle( foot[0], foot[1], foot[3], body, gravity );
	else if ( phase[3] == SWING )
		stabilityMargin = stabilityMarginForTriangle( foot[0], foot[1], foot[2], body, gravity );
	else
		stabilityMargin = stabilityMarginForQuadrangle( foot[0], foot[2], foot[3], foot[3], body, gravity );


	return stabilityMargin;
}
*/


/**
 *		Planner�N���X��private�ȃ����o�֐�
 *
 */
void Planner::newTrajectories(void)
{
	legTrajectory = new LegTrajectory[LEG_NUM];
	bodyTrajectory = new BodyTrajectory;

	return;
}

void Planner::deleteTrajectories(void)
{
	delete [] legTrajectory;
	delete bodyTrajectory;

	return;
}

/**
 *		����]�T�̌v�Z�̂��߂̃w���v�֐�
 */

/*  20200819
/// �x���r�O�p�`���̈���]�T
double Planner::stabilityMarginForTriangle(	const Vector& foot1, const Vector& foot2, const Vector& foot3, 
															const Vector& cog, const Vector& gravity)
{
	/// ����]�T
	double stability;

	/// �x���r�O�p�`�̖@���x�N�g��
	Vector triangleNormal(THREE_DIMENSION);
	/// �d�S�̎x���r�O�p�`�ւ̓��e�_
	Vector cogProjection(THREE_DIMENSION);

	/// �x���r�O�p�`�̖@���x�N�g�����v�Z
	triangleNormal = normalVectorOfPlane( foot1, foot2, foot3 );

	/// �d�S�̏d�͕����ւ̎x���r�O�p�`�Ƃ̌�_���v�Z
	cogProjection = intersectPointLineAndPlane(cog, gravity, foot1, triangleNormal);
	
	/// ����]�T���v�Z
	stability = minDistanceToTriangleEdge(cogProjection, foot1, foot2, foot3);

	return stability;
}

/// �x���r�l�p�`���̈���]�T
double Planner::stabilityMarginForQuadrangle(	const Vector& foot1, const Vector& foot2, 
																const Vector& foot3, const Vector& foot4, 
																const Vector& cog, const Vector& gravity)
{
	/// ����]�T
	double stability;
	
	/// �x���r�O�p�`�̖@���x�N�g��
	Vector quadrangleNormal(THREE_DIMENSION);
	/// �d�S�̎x���r�O�p�`�ւ̓��e�_
	Vector cogProjection(THREE_DIMENSION);

	/// �x���r�O�p�`�̖@���x�N�g�����v�Z
	quadrangleNormal = normalVectorOfPlane(foot1, foot2, foot3);
	/// �d�S�̏d�͕����ւ̎x���r�O�p�`�Ƃ̌�_���v�Z
	cogProjection = intersectPointLineAndPlane(cog, gravity, foot1, quadrangleNormal);
	
	/// ����]�T���v�Z
	stability = minDistanceToQuadrangleEdge(cogProjection, foot1, foot2, foot3, foot4);

	return stability;
}
*/

}	/// end of namespace Plan


