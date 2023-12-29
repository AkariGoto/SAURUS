/**
 *  �t�@�C����
 *		TripodGaitPlanner.cpp
 *  ����
 *		����e�v��N���X�i�g���b�g���e�ł̕��s�j
 *  ���t
 *		�쐬��: 2008/12/19(FRI)		�X�V��: 2018/12/19(FRI)
 */

//  20200819  ����]�T�֘A�R�����g�A�E�g
//  20200820  TROTGAIT��TRIPODGAIT�ɒu���ETrotGait��TripodGait�ɒu��
//  20200821  ���{�̈ʒu���Œ�
//  20200929  �x���r����z��������ǉ�
//  20200930  �V�r���̊J�n�ʒu
//  20201005
//  20201016  ���s�J�n���̎p���ւ̃Z�b�g
//  20201017  ���s�J�n���̎p���ւ̃Z�b�g
//  20201018  ���s�J�n���̎p���ւ̃Z�b�g
//  20201020  �����~��̍ē���
//  20220713  �֐߂̉���`�F�b�N

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include "TripodGaitPlanner.h"
//#include <winsock2.h>
using namespace std;
using namespace Math;
using namespace Asura;
using namespace Const;

namespace Plan
{
/**
 *	----------------------------------------------------------------------
 *		TripodGaitPlanner�N���X
 *	----------------------------------------------------------------------
 */

/**
 *	------------------------------------------------------------
 *		TripodGaitPlanner�N���X�̃����o�֐���`
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	�R���X�g���N�^�ƃf�X�g���N�^
 *	----------------------------------------
 */
/// �f�t�H���g�R���X�g���N�^
TripodGaitPlanner::TripodGaitPlanner(AsuraX* asuraPointer_, TimeManager* timeManagerPointer_)
: Planner( asuraPointer_, timeManagerPointer_ )
{
	/// �I�u�W�F�N�g�̐���
	newTripodGaitItems();

	/// ������
	initializeTripodGaitPlanner();

}

/// �f�X�g���N�^
TripodGaitPlanner::~TripodGaitPlanner()
{
	/// �I�u�W�F�N�g�̔j��
	deleteTripodGaitItems();

}

/**
 *	����e�̏�����
 */
void TripodGaitPlanner::initializeTripodGaitPlanner(void)
{
	/// �����o�ϐ��̏�����
	//stabilityMargin = 0.0;  20200819
	walkingCounter = 0;

	swingLegWaitingToStop = 0;

	/// �t���O�ނ̏�����
	isWalkingToGetSet = true;
	isTrajectoryToGetSet = true;
	isWalkingStarted = false;
	isWaitingToStop = false;
	isSetting = false;  //20201016

	//20201017  �p���ڍs�Ɏg�p����ϐ��̏�����
	settingStartTime = 0;
	settingTime = 0;

	/// ����
	walkingTime = 0.0;
	cycleStartTime = 0.0;
	normalizedWalkingTime = 0.0;
	cycleElapsedTime = 0.0;  //20201020

	/// �f���[�e�B��
	dutyFactor = TRIPODGAIT_DUTY_FACTOR;

	/// ���s���x���v�Z
	walkingSpeed = TRIPODGAIT_WALKING_SPEED;

	/// �X�g���C�h�̃Z�b�g
	stride= TRIPODGAIT_STRIDE ;
	//changeflag=0x00;
	
	/// ���s�����̃Z�b�g
	unitWalkingDirection = Vector(DEFAULT_LOCOMOTION_DIRECTION, THREE_DIMENSION);//<----

	//20200929  �x���r���Ɏg�p����z�������̃Z�b�g
	unitUpDirection = Vector(UP_DIRECTION, THREE_DIMENSION);

	/// �ʒu�x�N�g���̏�����
	footReferencePosition[0] = Vector(TRIPODGAIT_FOOT_REF_POSITION_1, THREE_DIMENSION);
	footReferencePosition[1] = Vector(TRIPODGAIT_FOOT_REF_POSITION_2, THREE_DIMENSION);
	footReferencePosition[2] = Vector(TRIPODGAIT_FOOT_REF_POSITION_3, THREE_DIMENSION);
	footReferencePosition[3] = Vector(TRIPODGAIT_FOOT_REF_POSITION_4, THREE_DIMENSION);
	footReferencePosition[4] = Vector(TRIPODGAIT_FOOT_REF_POSITION_5, THREE_DIMENSION);
	footReferencePosition[5] = Vector(TRIPODGAIT_FOOT_REF_POSITION_6, THREE_DIMENSION);
	/// �V�r�^��
	swingUp = Vector(TRIPODGAIT_SWING_UP, THREE_DIMENSION);
	swingDown = Vector(TRIPODGAIT_SWING_DOWN, THREE_DIMENSION);

	/// �f�t�H���g�l�ɂ����e�p�����[�^�̌v�Z
	calculateGaitParameters();

	return;
	
}

/**
 *	���e�p�����[�^�̌v�Z
 */
void TripodGaitPlanner::calculateGaitParameters(void)
{
	/// �v�Z�ς݂Ȃ̂ŕK�v�Ȃ�
	if ( !isTrajectoryToGetSet )
		return;

	/// ���s�������v�Z
	cycleTime = (stride)/ walkingSpeed / dutyFactor;//7/8;//<------!(stride-100)

	/// ���[�J���ϐ��̐錾
	double swingTime = 0;
	double swingUpTime = 0;
	double swingReturnTime = 0;
	double swingDownTime = 0;

	/// �V�r���Ԃ̌v�Z
	swingTime=1.0-dutyFactor;//swingTime = (1+TRIPODGAIT_SWING_DOWN[2]/TRIPODGAIT_SWING_UP[2])/2;//SwingTime=0.5(1 - dutyFactor);//
	/// �i���[�U�ɂ���Ĕz���͕ύX�j
	swingUpTime = swingTime *1/4;
	swingReturnTime = swingTime *2/4;
	swingDownTime = swingTime *1/4;

	/// �e�r�̗V�r�J�n���Ԃ̐ݒ�
	//swingStartTime[0] = dutyFactor - 0.5;
	//swingStartTime[1] = 2*dutyFactor - 0.5;
	//swingStartTime[2] = 2*dutyFactor - 1.0;
	//swingStartTime[3] = dutyFactor;
	swingStartTime[0] = 0;
	swingStartTime[1] = dutyFactor;
	swingStartTime[2] = 0;
	swingStartTime[3] = dutyFactor;
	swingStartTime[4] = 0;
	swingStartTime[5] = dutyFactor;


	/// �V�r���Ԃ��Z�b�g����
	int i;
	for (i=0; i<LEG_NUM; i++)
	{
		setLegSwingTime(i+1, swingStartTime[i], swingUpTime, swingReturnTime, swingDownTime);
		swingStopTime[i] = swingStartTime[i] + 1.0 - dutyFactor;
	}

	/// �V�r�J�n�ʒu�ƏI���ʒu�̌v�Z����ё������ʒu�v�Z
	double a[3];
	a[0]= TRIPODGAIT_SWING_UP[0] + TRIPODGAIT_SWING_DOWN[0];
	a[1]= TRIPODGAIT_SWING_UP[1] + TRIPODGAIT_SWING_DOWN[1];
	a[2]= TRIPODGAIT_SWING_UP[2] + TRIPODGAIT_SWING_DOWN[2];

	int j;
	for (j=0; j<LEG_NUM; j++)
	{
		swingStartPosition[j] = footReferencePosition[j]- (stride)/2*unitWalkingDirection;//<------!(stride-100) 20200930  �V�r�J�n�ʒu
		//swingStopPosition[j] = footReferencePosition[j] + (stride)/2*unitWalkingDirection+Vector(a, THREE_DIMENSION);//<------!(stride-100)//�����Ȃ���

		if (TRIPODGAIT_SWING_UP[2] + TRIPODGAIT_SWING_DOWN[2] != 0)  //20200930  ��艺�莞�̗V�r�I���ʒu
		{
			swingStopPosition[j] = footReferencePosition[j] + (stride) / 2 * unitWalkingDirection + (TRIPODGAIT_SWING_UP[2] + TRIPODGAIT_SWING_DOWN[2]) * unitUpDirection;
		}
		else  //20200930  ���n�̗V�r�I���ʒu
		{
			swingStopPosition[j] = footReferencePosition[j] + (stride) / 2 * unitWalkingDirection;
		}


		
	}

	/// �V�r�O�����Z�b�g����
	int k;
	for (k=0; k<LEG_NUM; k++)
	{
		setLegSwingTrajectory(	k+1, 
								swingStartPosition[k], 
								swingUp, 
								(stride)*unitWalkingDirection, //<------!(stride-100)
								swingDown 
							);
	}

	/// �����ʒu�̃Z�b�g  
	/*  20200930
	initialFootPosition[0] = footReferencePosition[0] - (stride)/2*unitWalkingDirection;
	initialFootPosition[1] = footReferencePosition[1] + (stride)/2*unitWalkingDirection;//<------!(stride-100)
	initialFootPosition[2] = footReferencePosition[2] - (stride)/2*unitWalkingDirection;
	initialFootPosition[3] = footReferencePosition[3] + (stride)/2*unitWalkingDirection;//<------!(stride-100)
	initialFootPosition[4] = footReferencePosition[4] - (stride)/2*unitWalkingDirection;
	initialFootPosition[5] = footReferencePosition[5] + (stride)/2*unitWalkingDirection;//<------!(stride-100)
	*/

	if (TRIPODGAIT_SWING_UP[2] + TRIPODGAIT_SWING_DOWN[2] > 0)
	{
		initialFootPosition[0] = footReferencePosition[0] - (stride) / 2 * unitWalkingDirection;
		initialFootPosition[1] = footReferencePosition[1] + (stride) / 2 * unitWalkingDirection + (TRIPODGAIT_SWING_UP[2] + TRIPODGAIT_SWING_DOWN[2]) * unitUpDirection;
		initialFootPosition[2] = footReferencePosition[2] - (stride) / 2 * unitWalkingDirection;
		initialFootPosition[3] = footReferencePosition[3] + (stride) / 2 * unitWalkingDirection + (TRIPODGAIT_SWING_UP[2] + TRIPODGAIT_SWING_DOWN[2]) * unitUpDirection;
		//initialFootPosition[4] = footReferencePosition[4] - (stride) / 2 * unitWalkingDirection;
		//initialFootPosition[5] = footReferencePosition[5] + (stride) / 2 * unitWalkingDirection + (TRIPODGAIT_SWING_UP[2] + TRIPODGAIT_SWING_DOWN[2]) * unitUpDirection;
	}
	else if (TRIPODGAIT_SWING_UP[2] + TRIPODGAIT_SWING_DOWN[2] < 0)
	{
		initialFootPosition[0] = footReferencePosition[0] - (stride) / 2 * unitWalkingDirection;
		initialFootPosition[1] = footReferencePosition[1] + (stride) / 2 * unitWalkingDirection + (TRIPODGAIT_SWING_UP[2] + TRIPODGAIT_SWING_DOWN[2]) * unitUpDirection;
		initialFootPosition[2] = footReferencePosition[2] - (stride) / 2 * unitWalkingDirection;
		initialFootPosition[3] = footReferencePosition[3] + (stride) / 2 * unitWalkingDirection + (TRIPODGAIT_SWING_UP[2] + TRIPODGAIT_SWING_DOWN[2]) * unitUpDirection;
		//initialFootPosition[4] = footReferencePosition[4] - (stride) / 2 * unitWalkingDirection;
		//initialFootPosition[5] = footReferencePosition[5] + (stride) / 2 * unitWalkingDirection + (TRIPODGAIT_SWING_UP[2] + TRIPODGAIT_SWING_DOWN[2]) * unitUpDirection;
	}
	else
	{
		initialFootPosition[0] = footReferencePosition[0] - (stride) / 2 * unitWalkingDirection;
		initialFootPosition[1] = footReferencePosition[1] + (stride) / 2 * unitWalkingDirection;
		initialFootPosition[2] = footReferencePosition[2] - (stride) / 2 * unitWalkingDirection;
		initialFootPosition[3] = footReferencePosition[3] + (stride) / 2 * unitWalkingDirection;
		//initialFootPosition[4] = footReferencePosition[4] - (stride) / 2 * unitWalkingDirection;
		//initialFootPosition[5] = footReferencePosition[5] + (stride) / 2 * unitWalkingDirection;
	}


	/// ���̂̏����ʒu
	int l;
	for (l=0; l<THREE_DIMENSION; l++)
		initialBodyPosition(l+1) = TRIPODGAIT_INITIAL_BODY_POSITION[l];

	isTrajectoryToGetSet = false;
/*
	WSAData wsaData;
	WSAStartup(MAKEWORD(2,0), &wsaData);
*/
	return;
}

/**
 *	���s�����p���Ɉڍs
 */
bool TripodGaitPlanner::shiftToInitialStandingPosture(void)
{
	Kinematics kine = NO_KINE_ERROR;

	/// �d�S�������ʒu��
	asuraPointer->initializeBodyPosition(initialBodyPosition);

	/// ���ʒu�������p����
	/*  20201016
	int i;
	for (i=0; i<LEG_NUM; i++)
	{
		kine = asuraPointer->placeLegFootPosition(i+1, initialFootPosition[i]);

		if (kine != NO_KINE_ERROR)
		{
			cerr << "[TripodGaitPlanner::shiftToInitialStandingPosture] Cannot shift to initial posture" << endl;
			Planner::printPlanErrorMessage();
		}
	}
	*/
	isSetting = true;  //�p���ڍs�̃t���O

	//20201017
	//�e�֐ߊp�x�擾
	for (int i = 0; i < LEG_NUM; i++)
	{
		for (int j = 0; j < LEG_JOINT_NUM; j++)
		{
			initialJointAngle[i][j] = asuraPointer->getLegJointAngle(i + 1)(j + 1);
		}
		initialFootJointAngle[i] = asuraPointer->getFootJointAngle(i + 1);
	}
	for (int i = 0; i < LEG_NUM; i++)
	{
		kine = asuraPointer->placeLegFootPosition(i + 1, initialFootPosition[i]);
		for (int j = 0; j < LEG_JOINT_NUM; j++)
		{
			finalJointAngle[i][j] = asuraPointer->getLegJointAngle(i + 1)(j + 1);
		}
		finalFootJointAngle[i] = asuraPointer->getFootJointAngle(i + 1);
	}

	//���v���ԎZ�o
	for (int i = 0; i < LEG_NUM; i++)
	{
		for (int j = 0; j < LEG_JOINT_NUM; j++)
		{
			if (finalJointAngle[i][j] >= initialJointAngle[i][j])
			{
				angularVelosity[i][j] = ANGULAR_VELOCITY;
			}
			else
			{
				angularVelosity[i][j] = -1 * ANGULAR_VELOCITY;
			}
			if (finalFootJointAngle[i] >= initialFootJointAngle[i])
			{
				angularVelosity[i][LEG_JOINT_NUM] = ANGULAR_VELOCITY;
			}
			else
			{
				angularVelosity[i][LEG_JOINT_NUM] = -1 * ANGULAR_VELOCITY;
			}
		}
		
		t01[i] = (finalJointAngle[i][0] - initialJointAngle[i][0]) / angularVelosity[i][0];
		t02[i] = t01[i] + (finalJointAngle[i][1] - initialJointAngle[i][1]) / angularVelosity[i][1];
		t03[i] = t02[i] + (finalJointAngle[i][2] - initialJointAngle[i][2]) / angularVelosity[i][2];
		t04[i] = t03[i] + (finalFootJointAngle[i] - initialFootJointAngle[i]) / angularVelosity[i][3];
		/*
		t01[i] = (finalJointAngle[i][0] - initialJointAngle[i][0]) / angularVelosity[i][0];
		t02[i] = (finalJointAngle[i][1] - initialJointAngle[i][1]) / angularVelosity[i][1];
		t03[i] = (finalJointAngle[i][2] - initialJointAngle[i][2]) / angularVelosity[i][2];
		t04[i] = (finalFootJointAngle[i] - initialFootJointAngle[i]) / angularVelosity[i][3];
		*/
		if (t01[i] > t02[i] && t01[i] > t03[i] && t01[i] > t04[i])
		{
			t05[i] = t01[i];
		}
		if (t02[i] > t01[i] && t02[i] > t03[i] && t02[i] > t04[i])
		{
			t05[i] = t02[i];
		}
		if (t03[i] > t01[i] && t03[i] > t02[i] && t03[i] > t04[i])
		{
			t05[i] = t03[i];
		}
		else
		{
			t05[i] = t04[i];
		}
	}
	
	//�p���ڍs�J�n�����擾
	settingStartTime = timeManagerPointer->getRealTime();


	return true;
}

/**
 *	------------------------------------------------------------
 *	�I�[�o�[���C�h�֐�
 *		�^������̓I�ɐ�������֐��Q
 *	------------------------------------------------------------
 */
/**
 *	���s�J�n�̂��߂̏�����
 */

bool TripodGaitPlanner::setup(void)
{
	/// �|�C���^���Z�b�g����Ă��邩�̊m�F
	if (asuraPointer == NULL || timeManagerPointer == NULL)
	{
		cerr << "[TripodGaitPlanner::setup] No control object\n" << endl;

		return false;
	}


	if (isTrajectoryToGetSet)
	{
		cerr << "[TripodGaitPlanner::setup] No trajectories are ready\n" << endl;

		return false;
	}

	if (!shiftToInitialStandingPosture())
	{
		cerr << "[TripodGaitPlanner::setup] Cannot shift to initial posture\n" << endl;

		return false;
	}

	/// ���s��������
	isWalkingToGetSet = false;

	cycleElapsedTime = 0.0;  //20201020

	//changeflag=0x00;		//�ǉ�

	return Planner::setup();
}


/// ���s���J�n����
bool TripodGaitPlanner::startPlan(void)
{
	isRunning = true;  //20201020

	if (isWalkingToGetSet || !isRunning)
	{
		cerr << "[TripodGaitPlanner::startPlan] Not stand-by for walking\n" << endl;
		
		return false;
	}

	/// ���s�����J�n���Ԃ��X�V
	//cycleStartTime = timeManagerPointer->getRealTime();  //20201020
	cycleStartTime = timeManagerPointer->getRealTime() - cycleElapsedTime;  //20201020

	isWalkingStarted = true;

	walkingCounter = 0;  //20201020

	return Planner::startPlan();
}

/// ���s���~����
bool TripodGaitPlanner::stopPlan(void)
{
	isWalkingStarted = false;
	//isWalkingToGetSet = true;  //20201020
	//walkingCounter = 0;  //20201020

	cycleElapsedTime = timeManagerPointer->getRealTime() - cycleStartTime;  //20201020

	isRunning = false;  //20201020

	return Planner::stopPlan();
}

/**
 *	���s��1���I�����ɒ�~����
 */
bool TripodGaitPlanner::standByForStop(void)
{
	isWaitingToStop = true;
	
	//20201005
	/*
	if (0 < normalizedWalkingTime && normalizedWalkingTime<= swingStopTime[1])
	{
		swingLegWaitingToStop = 1;
	}
	else if (swingStopTime[1] < normalizedWalkingTime && normalizedWalkingTime <= swingStopTime[0])
	{
		swingLegWaitingToStop = 2;
	}
	else if (swingStopTime[0] < normalizedWalkingTime && normalizedWalkingTime <= swingStopTime[2])
	{
		swingLegWaitingToStop = 3;
	}
	else if (swingStopTime[2] < normalizedWalkingTime && normalizedWalkingTime <= swingStopTime[3])
	{
		swingLegWaitingToStop = 4;
	}*/
	

/*	if (0 < normalizedWalkingTime && normalizedWalkingTime<= swingStopTime[1])
	{
		swingLegWaitingToStop = 1;
	}
	else if (swingStopTime[1] < normalizedWalkingTime && normalizedWalkingTime <= swingStopTime[0])
	{
		swingLegWaitingToStop = 2;
	}
	else if (swingStopTime[0] < normalizedWalkingTime && normalizedWalkingTime <= swingStopTime[2])
	{
		swingLegWaitingToStop = 3;
	}
	else if (swingStopTime[2] < normalizedWalkingTime && normalizedWalkingTime <= swingStopTime[3])
	{
		swingLegWaitingToStop = 4;
	}
	else if (swingStopTime[3] < normalizedWalkingTime && normalizedWalkingTime <= swingStopTime[4])
	{
		swingLegWaitingToStop = 5;
	}
	else if(swingStopTime[4] < normalizedWalkingTime && normalizedWalkingTime <= swingStopTime[5])
	{
		swingLegWaitingToStop = 6;
	}
	*/
	return isWaitingToStop;
}

/**
 *	�r�^���𐶐�����
 */
PlanStatus TripodGaitPlanner::activateRobot(void)
{
/**
 *		���K���������s���Ԃɂ��r�̉^�������肷��
 */
	/// ������
	PlanStatus plan = Plan::WAIT;
	/// �t�^���w������������
	Kinematics kine = NO_KINE_ERROR;
	/// �J�E���^
	int i;

	double swingTime = (1+ TRIPODGAIT_SWING_DOWN[2]/TRIPODGAIT_SWING_UP[2])/2;

	/*a[3] no using*/
	/*
	double a[3];
	a[0]= TRIPODGAIT_SWING_UP[0] + TRIPODGAIT_SWING_DOWN[0];
	a[1]= TRIPODGAIT_SWING_UP[1] + TRIPODGAIT_SWING_DOWN[1];
	a[2]= TRIPODGAIT_SWING_UP[2] + TRIPODGAIT_SWING_DOWN[2];
	*/

	///1,3,5�V�r�@2,4,6�x��
	if (0.00 <= normalizedWalkingTime && normalizedWalkingTime < TRIPODGAIT_DUTY_FACTOR)
	{
		for (i=0; i<LEG_NUM; i++)
		{
			switch (i)
			{
				///�� 2, 4, 6�r�̎x���r�^��
				case 1://2
				case 3://4
				case 5://6
				{
					if (TRIPODGAIT_SWING_UP[2] == -TRIPODGAIT_SWING_DOWN[2])  //20200929  ���ʎ��̎x���r�^��
					{
						kine = asuraPointer->placeLegFootPosition(
							i + 1,
							initialFootPosition[i] - normalizedWalkingTime * cycleTime * walkingSpeed * unitWalkingDirection
						);
					}
					else  //��艺��ł̎x���r�^��
					{
						kine = asuraPointer->placeLegFootPosition(
							i + 1,
							initialFootPosition[i] - normalizedWalkingTime * cycleTime * walkingSpeed * unitWalkingDirection
							+ normalizedWalkingTime * cycleTime * -(TRIPODGAIT_SWING_UP[2] + TRIPODGAIT_SWING_DOWN[2]) / (cycleTime * dutyFactor) * unitUpDirection
						);
					}

					/// �x���r���ɃZ�b�g
					asuraPointer->setLegPhase(i+1, SUPPORT);

					//if (kine != NO_KINE_ERROR)
					if (kine != NO_KINE_ERROR && i == 1)  //�r2�ɂ��Ă̂݉���ȂǊm�F
					{
						cerr << "[TripodGaitPlanner::activateRobot]" << endl;
						Planner::printPlanErrorMessage();

						suspendPlan();
						return SUSPEND;
					}
				}
				break;
				///��1,3,5 �r�̗V�r�^��
				case 0:
				case 2:
				case 4:
				{
					/// �O��������̑����̂��ߗV�r�^�������͎��ԕ␳��1.0��������
					plan = swingLeg(i+1, normalizedWalkingTime );
					/// �V�r���ɃZ�b�g
					asuraPointer->setLegPhase(i+1, SWING);

					//if (plan == INVALID)
					if (plan == INVALID && i == 1)  //�r2�ɂ��Ă̂݉���ȂǊm�F
					{
						Planner::printPlanErrorMessage();

						suspendPlan();
						return SUSPEND;
					}

					break;
				}
				default:
					break;
			}	/// end of switch (i)
		}	/// end of for (i) loop222
		asuraPointer->initializeBodyPosition(initialBodyPosition + unitWalkingDirection*normalizedWalkingTime*(stride));
	}
	///1,3,5�x���@2,4,6�V�r
	else if (TRIPODGAIT_DUTY_FACTOR <= normalizedWalkingTime && normalizedWalkingTime < 1.00)//TRIPODGAITDUTY_FACTOR
	{
		for ( i = 0; i < LEG_NUM; i++)
		{
			switch (i)
			{
				/// 1, 3, 5�r�̎x���r�^��
				case 0:
				case 2:
				case 4:
				{//swingStopPosition[j] = footReferencePosition[j] + (stride)/2*unitWalkingDirection+Vector(a, THREE_DIMENSION);
					/*kine = asuraPointer->placeLegFootPosition(
								i+1,
							 swingStopPosition[i]- (normalizedWalkingTime - swingStopTime[i])*cycleTime*walkingSpeed*unitWalkingDirection 
								);*/

					if (TRIPODGAIT_SWING_UP[2] == -TRIPODGAIT_SWING_DOWN[2])  //20200929  ���ʎ��̎x���r�^��
					{
						kine = asuraPointer->placeLegFootPosition(
							i + 1,
							swingStopPosition[i] - (normalizedWalkingTime - swingStopTime[i]) * cycleTime * walkingSpeed * unitWalkingDirection
						);
					}
					else  //��艺��ł̎x���r�^��
					{
						kine = asuraPointer->placeLegFootPosition(
							i + 1,
							swingStopPosition[i] - (normalizedWalkingTime - swingStopTime[i]) * cycleTime * walkingSpeed * unitWalkingDirection
							+ (normalizedWalkingTime - swingStopTime[i]) * cycleTime * -(TRIPODGAIT_SWING_UP[2] + TRIPODGAIT_SWING_DOWN[2]) / (cycleTime * dutyFactor) * unitUpDirection
						);
					}


					/// �x���r���ɃZ�b�g
					asuraPointer->setLegPhase( i+1, SUPPORT );

					//if (kine != NO_KINE_ERROR
					if ( kine != NO_KINE_ERROR && i == 1)  //�r2�ɂ��Ă̂݉���ȂǊm�F
					{
						cerr << "[TripodGaitPlanner::activateRobot]" << endl;
						Planner::printPlanErrorMessage();

						suspendPlan();
						return SUSPEND;
					}
				}
				break;
				/// 2, 4, 6�r�̗V�r�^��
				case 1:
				case 3:
				case 5:
				{
					plan = swingLeg(i+1, normalizedWalkingTime );
					/// �V�r���ɃZ�b�g
					asuraPointer->setLegPhase(i+1, SWING);

					//if (plan == INVALID)
					if ( plan == INVALID && i == 1)  //�r2�ɂ��Ă̂݉���ȂǊm�F
					{
						Planner::printPlanErrorMessage();
	
						suspendPlan();
						return SUSPEND;
					}
				}
				//isWaitingToStop=true;
				break;

				default: break;
			}	/// end of switch (i)
		}	/// end of for (i) loop
		asuraPointer->initializeBodyPosition(initialBodyPosition + unitWalkingDirection*normalizedWalkingTime*(stride));
	}/// end of if ( normalizedWalkingTime )

	/*else if ( 1.0<normalizedWalkingTime&&normalizedWalkingTime <= 1.5 )	
	{
		for ( i = 0; i < LEG_NUM; i++)
		{
		kine = asuraPointer->placeLegFootPosition(
								i+1,
							 swingStopPosition[i]- (1.0 - swingStopTime[i])*cycleTime*walkingSpeed*unitWalkingDirection -Vector(a, THREE_DIMENSION)*2*(normalizedWalkingTime-1.0 )
								);

					/// �x���r���ɃZ�b�g
					asuraPointer->setLegPhase( i+1, SUPPORT );
		asuraPointer->initializeBodyPosition(initialBodyPosition + unitWalkingDirection*(stride) + Vector(a, THREE_DIMENSION)*2*(normalizedWalkingTime-1.0 ));//<------!(stride-100)
		//printf("aa\n");
		}
	
	}*/
/// ���{�b�g�̓��̈ʒu���X�V	old position + direction * Time + Stride
//asuraPointer->initializeBodyPosition(initialBodyPosition + unitWalkingDirection*normalizedWalkingTime*(stride) + Vector(a, THREE_DIMENSION) );//<------!(stride-100)

//�{�̂��Œ肷��ꍇ  20200821
asuraPointer->initializeBodyPosition(initialBodyPosition);

//�{�̂��Œ肵�Ȃ��ꍇ  20200821
//asuraPointer->initializeBodyPosition(initialBodyPosition + unitWalkingDirection * normalizedWalkingTime * (stride * 2) + Vector(a, THREE_DIMENSION));//<------!(stride-100)


	return RUN;
}

/**
 *	���{�b�g�̏u�Ԃ̏�Ԃ𐶐�����
 *		�w�ߒl�����������ƂɌĂяo�����Ƃɂ�胍�{�b�g�̘A���ȓ�����v�悷��
 */
PlanStatus TripodGaitPlanner::createPlanSnapshot(void)
{
	/// ���[�J���ϐ��̐錾
	PlanStatus plan = RUN;

	/// ���s���J�n����ĂȂ�������I��
	if ( !isWalkingStarted )
	{
		/// ����𒆒f���Ă��鎞�͕��s�����f
		if ( isSuspended )
		{
			return SUSPEND;
		}

		//20201016
		if (isSetting)
		{
			settingPlan();
		}

		return WAIT;

	}

	/// �v��o�ߎ��Ԃ��v�Z
	Planner::elapsedTime = timeManagerPointer->getRealTime() - Planner::planStartTime;

	/// ���s�����J�n���猻�ݎ����܂ł̕��s�o�ߎ��Ԃ��v�Z
	walkingTime = timeManagerPointer->getRealTime() - cycleStartTime;
	
	/// ���K�����s���Ԃ��v�Z
	normalizedWalkingTime = walkingTime/cycleTime;

	/// �r����~����Ƃ��i���s��~�t���O�������Ă�����j
	/*  20201005
	if ( isWaitingToStop )
	{
		/// ���s��~���O�̗V�r�ŏꍇ����
		switch ( swingLegWaitingToStop )
		{
			case 1:
				if ( swingStopTime[1] < normalizedWalkingTime )
					stopPlan();
				break;

			case 2:
				if ( swingStopTime[0] < normalizedWalkingTime )
					stopPlan();
				break;

			case 3:
				if ( swingStopTime[2] < normalizedWalkingTime )
					stopPlan();

				break;

			case 4:
				if ( swingStopTime[3] < normalizedWalkingTime )
					stopPlan();

				break;

			default:
				break;
		}

		isWaitingToStop = false;
		swingLegWaitingToStop = 0;
	}*/
	
	/// 1�����I�����̎��Ԍv�Z
	if ( normalizedWalkingTime > 1.0 )
	{
		/// ���s�J�E���^�𑝂₷
		walkingCounter++;

		/// ���s�����J�n���Ԃ��X�V
		cycleStartTime = timeManagerPointer->getRealTime();

		//20201020

		/// ���̂̐�΍��W���X�V
		initialBodyPosition = asuraPointer->getBodyPosition();

		/// ���s���ԂƐ��K�����s���Ԃ����Z�b�g
		walkingTime = 0.0;
		normalizedWalkingTime = 0.0;

		//20201005
		//stopPlan();  //1�����ł�߂�Ƃ��̓R�����g�A�E�g����������
		if (isWaitingToStop)
		{
			stopPlan();
			isWaitingToStop = false;
			isWalkingToGetSet = true;
		}
	}

	/// ���{�b�g�̕��s�����ɍ��킹���r�^���𐶐�����
	plan = activateRobot();

	if (plan == SUSPEND)
	{
		stopPlan();
	}

	/// ����]�T���v�Z
	//calculateStabilityMargin();  20200819

	return plan;
}

//20201016
bool TripodGaitPlanner::settingPlan(void)
{
	//20201017
	//�o�ߎ��Ԃ𓾂�
	settingTime = timeManagerPointer->getRealTime() - settingStartTime;

	Kinematics kine = NO_KINE_ERROR;

	//�֐ߊp�x���Z�o����
	for (int i = 0; i < LEG_NUM; i++)
	{//�P�֐߂���
		if (settingTime < 0)  //�ǂ̊֐߂������O
		{
			settingJointAngle[i][0] = initialJointAngle[i][0];
			settingJointAngle[i][1] = initialJointAngle[i][1];
			settingJointAngle[i][2] = initialJointAngle[i][2];
			settingFootJointAngle[i] = initialFootJointAngle[i];
		}
		else if (0 <= settingTime && settingTime <= t01[i])  //��1�֐߂��쓮��
		{
			settingJointAngle[i][0] = angularVelosity[i][0] * settingTime + initialJointAngle[i][0];
			settingJointAngle[i][1] = initialJointAngle[i][1];
			settingJointAngle[i][2] = initialJointAngle[i][2];
			settingFootJointAngle[i] = initialFootJointAngle[i];
		}
		else if (t01[i] <= settingTime && settingTime <= t02[i])  //��2�֐߂��쓮��
		{
			settingJointAngle[i][0] = finalJointAngle[i][0];
			settingJointAngle[i][1] = angularVelosity[i][1] * (settingTime - t01[i]) + initialJointAngle[i][1];
			settingJointAngle[i][2] = initialJointAngle[i][2];
			settingFootJointAngle[i] = initialFootJointAngle[i];
		}
		else if (t02[i] <= settingTime && settingTime <= t03[i])  //��3�֐߂��쓮��
		{
			settingJointAngle[i][0] = finalJointAngle[i][0];
			settingJointAngle[i][1] = finalJointAngle[i][1];
			settingJointAngle[i][2] = angularVelosity[i][2] * (settingTime - t02[i]) + initialJointAngle[i][2];
			settingFootJointAngle[i] = initialFootJointAngle[i];
		}
		else if (t03[i] <= settingTime && settingTime <= t04[i])  //��4�֐߂��쓮��
		{
			settingJointAngle[i][0] = finalJointAngle[i][0];
			settingJointAngle[i][1] = finalJointAngle[i][1];
			settingJointAngle[i][2] = finalJointAngle[i][2];
			settingFootJointAngle[i] = angularVelosity[i][3] * (settingTime - t03[i]) + initialFootJointAngle[i];
		}
		else if (t04[i] <= settingTime)
		{
			settingJointAngle[i][0] = finalJointAngle[i][0];
			settingJointAngle[i][1] = finalJointAngle[i][1];
			settingJointAngle[i][2] = finalJointAngle[i][2];
			settingFootJointAngle[i] = finalFootJointAngle[i];
		}
		

		//�S��
		/*
		if (settingTime < 0)  //�ǂ̊֐߂������O
		{
			settingJointAngle[i][0] = initialJointAngle[i][0];
			settingJointAngle[i][1] = initialJointAngle[i][1];
			settingJointAngle[i][2] = initialJointAngle[i][2];
			settingFootJointAngle[i] = initialFootJointAngle[i];
		}
		else if (0 <= settingTime)
		{
			
			if (t01[i] <= settingTime)
			{
				settingJointAngle[i][0] = finalJointAngle[i][0];
			}
			else
			{
				settingJointAngle[i][0] = angularVelosity[i][0] * settingTime + initialJointAngle[i][0];
			}

			if (t02[i] <= settingTime)
			{
				settingJointAngle[i][1] = finalJointAngle[i][1];
			}
			else
			{
				settingJointAngle[i][1] = angularVelosity[i][1] * settingTime + initialJointAngle[i][1];
			}

			if (t03[i] <= settingTime)
			{
				settingJointAngle[i][2] = finalJointAngle[i][2];
			}
			else
			{
				settingJointAngle[i][2] = angularVelosity[i][2] * settingTime + initialJointAngle[i][2];
			}

			if (t04[i] <= settingTime)
			{
				settingFootJointAngle[i] = finalFootJointAngle[i];
			}
			else
			{
				settingFootJointAngle[i] = angularVelosity[i][3] * settingTime + initialFootJointAngle[i];
			}
		}
		*/
	}

		//���ׂĂ̊֐߂��ڍs���I�������isSetting��false�ɂ���
		/*
		if (t04[0] < settingTime && t04[1] < settingTime && t04[2] < settingTime && t04[3] < settingTime && t04[4] < settingTime && t04[5] < settingTime)
		{
			isSetting = false;
			settingOver();  //20201022
		}
		*/

		if (t05[0] < settingTime && t05[1] < settingTime && t05[2] < settingTime && t05[3] < settingTime && t05[4] < settingTime && t05[5] < settingTime)
		{
			isSetting = false;
			//settingOver();  //20201022
		}

	//�֐ߊp�x�₻������Z�o�����r��ʒu�Ȃǂ��L�^����
	//
	/*
	for (int i = 0; i < LEG_NUM; i++)
	{
		kine = asuraPointer->placeLegJointAngles(i+1, settingJointAngle[i], settingFootJointAngle[i]);
	
	}
	*/

	//�r2�̂�
	kine = asuraPointer->placeLegJointAngles(2, settingJointAngle[1], settingFootJointAngle[1]);
	if (kine != NO_KINE_ERROR)
	{
		isSetting = false;
	}
	return true;
}


/**
 *	------------------------------------------------------------
 *		TripodGaitPlanner�N���X��private�ȃ����o�֐�
 *	------------------------------------------------------------
 */
/// ���s�̂��߂̃I�u�W�F�N�g����
void TripodGaitPlanner::newTripodGaitItems(void)
{
	/// �r��ʒu
	footReferencePosition = new Vector[LEG_NUM];

	/// ���̏����ʒu
	initialFootPosition = new Vector[LEG_NUM];

	/// �V�r�J�n�ʒu
	swingStartPosition = new Vector[LEG_NUM];

	/// �V�r�I���ʒu
	swingStopPosition = new Vector[LEG_NUM];

	/// �x�N�g���̃T�C�Y����
	initialBodyPosition.setSize(THREE_DIMENSION);

	unitWalkingDirection.setSize(THREE_DIMENSION);
	unitUpDirection.setSize(THREE_DIMENSION);  //20200929
	swingUp.setSize(THREE_DIMENSION);
	swingDown.setSize(THREE_DIMENSION);

	/// �r�ʒu�֌W
	int i;
	for (i=0; i<LEG_NUM; i++)
	{
		footReferencePosition[i].setSize(THREE_DIMENSION);
		initialFootPosition[i].setSize(THREE_DIMENSION);
		swingStartPosition[i].setSize(THREE_DIMENSION);
		swingStopPosition[i].setSize(THREE_DIMENSION);
	}

	/// �V�r�J�n����
	swingStartTime = new double[LEG_NUM];
	/// �V�r�I������
	swingStopTime = new double[LEG_NUM];

	//20201017  �p���ڍs
	initialJointAngle = new Vector[LEG_NUM];
	finalJointAngle = new Vector[LEG_NUM];
	settingJointAngle = new Vector[LEG_NUM];
	initialFootJointAngle = new double[LEG_NUM];
	finalFootJointAngle = new double[LEG_NUM];
	settingFootJointAngle = new double[LEG_NUM];
	angularVelosity = new Vector[LEG_NUM];
	t01 = new double[LEG_NUM];
	t02 = new double[LEG_NUM];
	t03 = new double[LEG_NUM];
	t04 = new double[LEG_NUM];
	t05 = new double[LEG_NUM];

	for (int i = 0; i < LEG_NUM; i++)
	{
		initialJointAngle[i].setSize(LEG_JOINT_NUM);
		finalJointAngle[i].setSize(LEG_JOINT_NUM);
		settingJointAngle[i].setSize(LEG_JOINT_NUM);
		initialFootJointAngle[i] = 0;
		finalFootJointAngle[i] = 0;
		settingFootJointAngle[i] = 0;
		angularVelosity[i].setSize(LEG_JOINT_NUM + 1);
		t01[i] = 0;
		t02[i] = 0;
		t03[i] = 0;
		t04[i] = 0;
		t05[i] = 0;
	}


	return;

}

/// ���s�̂��߂̃I�u�W�F�N�g����
void TripodGaitPlanner::deleteTripodGaitItems(void)
{
	delete [] footReferencePosition;
	delete [] initialFootPosition;
	delete [] swingStartPosition;
	delete [] swingStopPosition;

	delete [] swingStartTime;
	delete [] swingStopTime;

	//20201017
	delete[] initialJointAngle;
	delete[] finalJointAngle;
	delete[] initialFootJointAngle;
	delete[] finalFootJointAngle;
	delete[] angularVelosity;
	delete[] t01;
	delete[] t02;
	delete[] t03;
	delete[] t04;
	delete[] t05;

	return;
}

}	/// end of namespace Plan