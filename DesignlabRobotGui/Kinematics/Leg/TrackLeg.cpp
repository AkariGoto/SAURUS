/**
 *  �t�@�C����
 *		TrackLeg.cpp
 *  ����
 *		�N���[���ό^�r�@�\�̏��C�^���w
 *  ���t
 *		�쐬��: 2007/02/05(MON)		�X�V��: 2007/02/19(MON)
 */

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include "TrackLeg.h"

using namespace std;
using namespace Math;
using namespace Const;

namespace Asura
{
/**
 *	----------------------------------------------------------------------
 *		TrackLeg�N���X
 *	----------------------------------------------------------------------
 */

/**
 *	------------------------------------------------------------
 *		TrackLeg�N���X�̃����o�֐���`
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	�R���X�g���N�^�ƃf�X�g���N�^
 *	----------------------------------------
 */
/// �f�t�H���g�R���X�g���N�^
TrackLeg::TrackLeg(int no, double x, double y, double z, double theta) : Leg(no, x, y, z, theta)
{
	/// �֐ߊp���E
	trackLimit.angleMin[0] = TRACK_ANGLE1_MIN;
	trackLimit.angleMin[1] = TRACK_ANGLE2_MIN;
	trackLimit.angleMin[2] = TRACK_ANGLE3_MIN;		//�ǉ�

	trackLimit.angleMax[0] = TRACK_ANGLE1_MAX;
	trackLimit.angleMax[1] = TRACK_ANGLE2_MAX;
	trackLimit.angleMax[2] = TRACK_ANGLE3_MAX;		//�ǉ�

	/// �ő�N���[�����x
	trackLimit.trackSpeedMax = TRACK_SPEED_MAX;

	trackLastErrorJointNo = 0;

	driveSystem = LEG;
}

TrackLeg::~TrackLeg()
{
}

/**
 *	----------------------------------------
 *	�^���w
 *	----------------------------------------
 */
/// �N���[�����j�b�g�̎p����ς���
Kinematics TrackLeg::changeTrackPosture(const Vector& angle)
{
	/// �����`�F�b�N
	if ( angle.getSize() != 3 )
	{
		cerr << "Error: [TrackLeg::changeTrackPosture] Argument is invalid\n" << endl;
        
		return KINE_ERROR_ELSE;
	}
	for (int i=1; i<=TRACK_JOINT_NUM; i++)
	trackData.jointAngle(i)=angle(i);
	/// �܂��֐߂����͈͓��ɂ��邩�ǂ������ׂ�
	Kinematics kine = checkTrackJointMotionRange();

	/// �֐߂����͈͊O
	if ( kine != NO_KINE_ERROR )	
	{
		cerr << "Error: [TrackLeg::changeTrackPosture] Invalid joint angle" << endl;
	
		return kine;
	}

	/// �N���[�������܂ł̍��W�ϊ�
	trackData.baseTransformation = trackBaseHomogeneousTransformation();
	/// �N���[�������ʒu�x�N�g������
	for (int i=1; i<=THREE_DIMENSION; i++)
		trackData.basePosition(i) = trackData.baseTransformation(i, 4);

	/// �N���[�����������1�֐߂܂ł̍��W�ϊ�
	trackData.jointTransformation[0] = trackData.baseTransformation*trackJointHomogeneousTransformation(1);
	/// �N���[����1�֐߈ʒu�x�N�g������
	for (int i=1; i<=THREE_DIMENSION; i++)
		trackData.jointPosition[0](i) = trackData.jointTransformation[0](i, 4);

	/// ��1�֐߂����2�֐߂܂ł̍��W�ϊ�
	trackData.jointTransformation[1] = trackData.jointTransformation[0]*trackJointHomogeneousTransformation(2);
	/// �N���[����2�֐߈ʒu�x�N�g������
	for (int j=1; j<=THREE_DIMENSION; j++)
		trackData.jointPosition[1](j) = trackData.jointTransformation[1](j, 4);

	/// ��2�֐߂���N���[���O����[�܂ł̍��W�ϊ�
	trackData.endTransformation = trackData.jointTransformation[1]*trackEndHomogeneousTransformation();
	/// �N���[���O����[�ʒu�x�N�g�����
	for (int k=1; k<=THREE_DIMENSION; k++)
		trackData.endPosition(k) = trackData.endTransformation(k, 4);

	return kine;
}

/**
 *	----------------------------------------
 *	��Ԍv�Z�֐�
 *	----------------------------------------
 */
/**
 *	����
 *		�֐ߑ��x: [3]
 *		�ߋ��̊p�x�Ƃ̍����Ŋp�x�v�Z
 */
void TrackLeg::calculateTrackJointVelocity(const Math::Vector& trackVelocity)
{
	return;
}

/**
 *	����
 *		�֐߃g���N: [3]
 *		�N���[����[�ɉ�������׏d����v�Z
 */
void TrackLeg::calculateTrackJointTorque(const Vector& trackReaction)
{
	return;
}

/**
 *	����
 *		�N���[�����x��ύX����
 */
void TrackLeg::changeTrackSpeed(double trackSpeed)
{
	trackData.trackSpeed = trackSpeed;
}

/**
 *	����
 *		�r�̋@�\�������`�F�b�N����
 */
	/// �֐߂̉��͈͂𒲍�
Kinematics TrackLeg::checkTrackJointMotionRange(void)
{
	for (int i=1; i<=TRACK_JOINT_NUM; i++)
	{
		if ( trackData.jointAngle(i) < trackLimit.angleMin[i-1] )
		{
			/// ���͈͂̉����ȉ�
			trackLastErrorJointNo = i;
			return KINE_ERROR_JOINT_UNDER_LIMIT;
		}
		else if ( trackData.jointAngle(i) > trackLimit.angleMax[i-1] )
		{
			/// ���͈͂̏���ȏ�
			trackLastErrorJointNo = i;
			return KINE_ERROR_JOINT_OVER_LIMIT;
		}

	}

	/// �G���[�Ȃ�
	trackLastErrorJointNo = 0;

	return NO_KINE_ERROR;
}

/**
 *	----------------------------------------------------------------------
 *		TrackLeg�̓����N���X�@TrackData�N���X
 *	----------------------------------------------------------------------
 */
/**
 *	----------------------------------------
 *	�R���X�g���N�^�ƃf�X�g���N�^
 *	----------------------------------------
 */
/// �f�t�H���g�R���X�g���N�^
TrackLeg::TrackData::TrackData()
{
	newTrackData();
}

/// �f�X�g���N�^
TrackLeg::TrackData::~TrackData()
{
	deleteTrackData();
}

/**
 *	------------------------------------------------------------
 *		TrackData�N���X��private�ȃ����o�֐�
 *	------------------------------------------------------------
 */
/// �I�u�W�F�N�g�̃������̈���m�ۂ���
void TrackLeg::TrackData::newTrackData(void)
{
/**
 *	�N���[�������̍s��E�x�N�g���̃T�C�Y����
 */
	baseTransformation.setSize(DH_DIMENSION, DH_DIMENSION);
	basePosition.setSize(THREE_DIMENSION);

	/// �����o�̃������̈�m��
	jointTransformation		=	new Matrix[TRACK_JOINT_NUM];
	jointPosition				=	new Vector[TRACK_JOINT_NUM];

/**
 *	�N���[���֐߂̍s��E�x�N�g���̃T�C�Y����
 */
	/// �����ϊ��s��
	for ( int i=0; i<TRACK_JOINT_NUM; i++)
	{
		jointTransformation[i].setSize(DH_DIMENSION, DH_DIMENSION);
		jointTransformation[i].loadIdentity();
	}

	/// �e�֐߈ʒu
	for (int j=0; j<TRACK_JOINT_NUM; j++)
		jointPosition[j].setSize(THREE_DIMENSION);

/**
 *	�N���[����[�̍s��E�x�N�g���̃T�C�Y����
 */
	endTransformation.setSize(DH_DIMENSION, DH_DIMENSION);
	endPosition.setSize(THREE_DIMENSION);

	/// �֐ߊp�x
	jointAngle.setSize(TRACK_JOINT_NUM);
	/// �֐ߑ��x
	jointVelocity.setSize(TRACK_JOINT_NUM);
	/// �֐߃g���N
	jointTorque.setSize(TRACK_JOINT_NUM);

	return;
}

/// �I�u�W�F�N�g�̃������̈���������
void TrackLeg::TrackData::deleteTrackData(void)
{
	delete [] jointTransformation;
	delete [] jointPosition;

	return;
}


} /// end of namespace Asura