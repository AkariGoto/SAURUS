/**
 *  File: DataHandler.cpp
 *
 *  Description:	���{�b�g�̃f�[�^�n���h��
 *
 *  Created: 2007/02/05(Mon)
 *  Updated: 2018/02/05(Mon)
 *
 *  Copyright (C) Tokyo Institute of Technology Hirose Lab.
 */

//  20200819  �N���[���֘A�R�����g�A�E�g

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include "DataHandler.h"

using namespace std;
using namespace Math;
using namespace Const;
using namespace Asura;
using namespace Plan;

namespace Data
{
/**
 *	----------------------------------------------------------------------
 *		DataHandler�N���X
 *	----------------------------------------------------------------------
 */

/**
 *	------------------------------------------------------------
 *		DataHandler�N���X�̃����o�֐���`
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	�R���X�g���N�^�ƃf�X�g���N�^
 *	----------------------------------------
 */
/// �f�t�H���g�R���X�g���N�^
DataHandler::DataHandler(AsuraX* asuraPtr, AsuraData* asuraDataPtr, Planner* planPtr, PlanData* planDataPtr)
: asuraDataSourcePointer(NULL), 
  asuraDataTargetPointer(NULL),
  planDataSourcePointer(NULL),
  planDataTargetPointer(NULL)
{
	asuraDataSourcePointer = asuraPtr;
	asuraDataTargetPointer = asuraDataPtr;
	planDataSourcePointer = planPtr;
	planDataTargetPointer = planDataPtr;
}

DataHandler::~DataHandler()
{
}

/**
 *	�f�[�^���̃|�C���^�̎擾�E���
 */
void DataHandler::acquireAsuraDataSource(AsuraX* asuraPointer)
{
	releaseAsuraDataSource();

	asuraDataSourcePointer = asuraPointer;
}

void DataHandler::releaseAsuraDataSource(void)
{
	asuraDataSourcePointer = NULL;
}

void DataHandler::acquirePlanDataSource(Planner* planPointer)
{
	releasePlanDataSource();

	planDataSourcePointer = planPointer;
}

void DataHandler::releasePlanDataSource(void)
{
	planDataSourcePointer = NULL;
}

/**
 *	�f�[�^��̃|�C���^�̎擾�E���
 */
/// �擾
void DataHandler::acquireAsuraDataTarget(AsuraData* dataPointer)
{
	releaseAsuraDataTarget();

	asuraDataTargetPointer = dataPointer;
}

/// ���
void DataHandler::releaseAsuraDataTarget(void)
{
	asuraDataTargetPointer = NULL;
}

/// �擾
void DataHandler::acquirePlanDataTarget(PlanData* dataPointer)
{
	releasePlanDataTarget();

	planDataTargetPointer = dataPointer;
}

/// ���
void DataHandler::releasePlanDataTarget(void)
{
	planDataTargetPointer = NULL;
}

/**
 *	�f�[�^�̎�荞�݂Ɠ]��
 */
/// �]��
void DataHandler::exportAsuraData(void)
{

}

/// ��荞��
void DataHandler::importAsuraData(void)
{
	/// �C���N�������g�ϐ�
	int i;
	int j;

	/// ���̂Ɋւ���f�[�^���X�V
	asuraData.setBodyTransformation( asuraDataSourcePointer->getBodyTransformation() );
	asuraData.setBodyPosition( asuraDataSourcePointer->getBodyPosition() );
	asuraData.setBodyVelocity( asuraDataSourcePointer->getBodyVelocity() );
	
	/**
	 *	�r�Ɋւ���f�[�^���X�V
	 *	���W�n�͐�΍��W�n�ɕϊ�
	 */
	for (i=0;i<LEG_NUM;i++)
	{
		asuraData.setLegBaseTransformation(i+1, 
			asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getLegBaseTransformation(i+1))
			);
		asuraData.setLegBasePosition(i+1, 
			asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getLegBasePosition(i+1))
			);

		for (j=0;j<LEG_JOINT_NUM;j++)
		{
			asuraData.setLegJointTransformation(i+1, j+1, 
				asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getLegJointTransformation(i+1, j+1))
				);
			asuraData.setLegJointPosition( i+1, j+1, 
				asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getLegJointPosition(i+1, j+1))
				);
		}

		asuraData.setLegFootTransformation(i+1, 
			asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getLegFootTransformation(i+1))
			);
		asuraData.setLegFootPosition(i+1, 
			asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getLegFootPosition(i+1))
			);

		asuraData.setLegJointAngle(i+1, asuraDataSourcePointer->getLegJointAngle(i+1));
		asuraData.setLegJointVelocity(i+1, asuraDataSourcePointer->getLegJointVelocity(i+1));
		asuraData.setLegJointTorque(i+1, asuraDataSourcePointer->getLegJointTorque(i+1));
		asuraData.setLegPhase(i+1, asuraDataSourcePointer->getLegPhase(i+1));
		asuraData.setLegActuatorPosition(i+1, asuraDataSourcePointer->getLegActuatorPosition(i+1));///-----add
		asuraData.setFootJointAngle(i+1, asuraDataSourcePointer->getFootJointAngle(i+1));///---------------add
	}

	/// �N���[���Ɋւ���f�[�^���X�V
	/*  20200819
	for (i=0;i<TRACK_NUM;i++)
	{
		asuraData.setTrackBaseTransformation(i+1, 
			asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getTrackBaseTransformation(i+1))
			);
		asuraData.setTrackBasePosition(i+1, 
			asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getTrackBasePosition(i+1))
			);

		for (j=0;j<TRACK_JOINT_NUM;j++)
		{
			asuraData.setTrackJointTransformation( +1, j+1, 
				asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getTrackJointTransformation(i+1, j+1))
				);
			asuraData.setTrackJointPosition(i+1, j+1, 
				asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getTrackJointPosition(i+1, j+1))
				);
		}

		asuraData.setTrackEndTransformation(i+1, 
			asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getTrackEndTransformation(i+1))
			);
		asuraData.setTrackEndPosition(i+1, 
			asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getTrackEndPosition(i+1))
			);

		asuraData.setTrackJointAngle(i+1, asuraDataSourcePointer->getTrackJointAngle(i+1));
		asuraData.setTrackJointVelocity(i+1, asuraDataSourcePointer->getTrackJointVelocity(i+1));
		asuraData.setTrackJointTorque(i+1, asuraDataSourcePointer->getTrackJointTorque(i+1));
		
		asuraData.setTrackSpeed(i+1, asuraDataSourcePointer->getTrackSpeed(i+1));
	}
	*/
	return;
}

/// �]��
void DataHandler::exportPlanData(void)
{
}

/// ��荞��
void DataHandler::importPlanData(void)
{
	//planData.setStabilityMargin(planDataSourcePointer->getStabilityMargin());  20200819
	planData.setElapsedTime(planDataSourcePointer->getElapsedTime());

	return;	
}

/**
 *		�f�[�^�n��
 *			���ڃf�[�^������f�[�^��ւ̑��
 */
void DataHandler::handleAsuraData(void)
{
	/// �C���N�������g�ϐ�
	int i;
	int j;

	/// ���̂Ɋւ���f�[�^���X�V
	asuraDataTargetPointer->setBodyTransformation(asuraDataSourcePointer->getBodyTransformation());
	asuraDataTargetPointer->setBodyPosition(asuraDataSourcePointer->getBodyPosition());
	asuraDataTargetPointer->setBodyVelocity(asuraDataSourcePointer->getBodyVelocity());

	/**
	 *		�r�Ɋւ���f�[�^���X�V
	 *		���W�n�͐�΍��W�n�ɕϊ�
	 */
	for (i=0;i<LEG_NUM;i++)
	{
		asuraDataTargetPointer->setLegBaseTransformation(i+1, 
			asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getLegBaseTransformation(i+1))
			);
		asuraDataTargetPointer->setLegBasePosition(i+1, 
			asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getLegBasePosition(i+1))
			);

		for (j=0;j<LEG_JOINT_NUM;j++)
		{
			asuraDataTargetPointer->setLegJointTransformation(i+1, j+1, 
				asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getLegJointTransformation(i+1, j+1))
				);
			asuraDataTargetPointer->setLegJointPosition(i+1, j+1, 
				asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getLegJointPosition(i+1, j+1))
				);
		}

		asuraDataTargetPointer->setLegFootTransformation(i+1, 
			asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getLegFootTransformation(i+1))
			);
		asuraDataTargetPointer->setLegFootPosition(i+1, 
			asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getLegFootPosition(i+1))
			);

		asuraDataTargetPointer->setLegJointAngle(i+1, asuraDataSourcePointer->getLegJointAngle(i+1));
		asuraDataTargetPointer->setLegJointVelocity(i+1, asuraDataSourcePointer->getLegJointVelocity(i+1));
		asuraDataTargetPointer->setLegJointTorque(i+1, asuraDataSourcePointer->getLegJointTorque(i+1));
		asuraDataTargetPointer->setLegPhase(i+1, asuraDataSourcePointer->getLegPhase(i+1));
		asuraDataTargetPointer->setFootJointAngle(i+1, asuraDataSourcePointer->getFootJointAngle(i+1));//--add
		asuraDataTargetPointer->setLegActuatorPosition(i+1, asuraDataSourcePointer->getLegActuatorPosition(i+1));//--add
	}

	/// �N���[���Ɋւ���f�[�^���X�V
	/*  20200819
	for (i=0; i<TRACK_NUM; i++)
	{
		asuraDataTargetPointer->setTrackBaseTransformation(i+1, 
			asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getTrackBaseTransformation(i+1))
			);
		asuraDataTargetPointer->setTrackBasePosition(i+1, 
			asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getTrackBasePosition(i+1))
			);

		for (j=0;j<TRACK_JOINT_NUM;j++)
		{
			asuraDataTargetPointer->setTrackJointTransformation(i+1, j+1, 
				asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getTrackJointTransformation(i+1, j+1))
				);
			asuraDataTargetPointer->setTrackJointPosition(i+1, j+1, 
				asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getTrackJointPosition(i+1, j+1))
				);
		}

		asuraDataTargetPointer->setTrackEndTransformation(i+1, 
			asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getTrackEndTransformation(i+1))
			);
		asuraDataTargetPointer->setTrackEndPosition(i+1, 
			asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getTrackEndPosition(i+1))
			);

		asuraDataTargetPointer->setTrackJointAngle(i+1, asuraDataSourcePointer->getTrackJointAngle(i+1));
		asuraDataTargetPointer->setTrackJointVelocity(i+1, asuraDataSourcePointer->getTrackJointVelocity(i+1));
		asuraDataTargetPointer->setTrackJointTorque(i+1, asuraDataSourcePointer->getTrackJointTorque(i+1));

		asuraDataTargetPointer->setTrackSpeed(i+1, asuraDataSourcePointer->getTrackSpeed(i+1));
	}
	*/
	return;
}

void DataHandler::handlePlanData(void)
{
	if (planDataTargetPointer == NULL)
		return;

	//planDataTargetPointer->setStabilityMargin(planDataSourcePointer->getStabilityMargin());  20200819
	planDataTargetPointer->setElapsedTime(planDataSourcePointer->getElapsedTime());

	return;
}

/**
 *	----------------------------------------
 *	�A�N�Z�X�֐�
 *	----------------------------------------
 */
	/**
	 *		�r
	 */
/// �r�������Ƃ̓����ϊ��s��
const Matrix& DataHandler::getLegBaseTransformation(int legNo) const
{
	/// �����`�F�b�N
	assert(1 <= legNo && legNo <= LEG_NUM);

	return asuraData.getLegBaseTransformation(legNo);
}

/// �r�֐߂��Ƃ̓����ϊ��s��
const Math::Matrix& DataHandler::getLegJointTransformation(int legNo, int jointNo) const
{
	/// �����`�F�b�N
	assert(1 <= legNo && legNo <= LEG_NUM);
	assert(1 <= jointNo && jointNo <= LEG_JOINT_NUM);

	return asuraData.getLegJointTransformation(legNo, jointNo);
}

/// ���ʒu���Ƃ̓����ϊ��s��
const Matrix& DataHandler::getLegFootTransformation(int legNo) const
{
	/// �����`�F�b�N
	assert(1 <= legNo && legNo <= LEG_NUM);

	return asuraData.getLegFootTransformation(legNo);
}

/// �r�̍����ʒu
const Vector& DataHandler::getLegBasePosition(int legNo) const
{
	/// �����`�F�b�N
	assert(1 <= legNo && legNo <= LEG_NUM);

	return asuraData.getLegBasePosition(legNo);
}

/// �r�̊֐߈ʒu
const Vector& DataHandler::getLegJointPosition(int legNo, int jointNo) const
{
	/// �����`�F�b�N
	assert(1 <= legNo && legNo <= LEG_NUM);
	assert(1 <= jointNo && jointNo <= LEG_JOINT_NUM);

	return asuraData.getLegJointPosition(legNo, jointNo);
}

/// ���ʒu
const Vector& DataHandler::getLegFootPosition(int legNo) const
{
	/// �����`�F�b�N
	assert(1 <= legNo && legNo <= LEG_NUM);

	return asuraData.getLegFootPosition(legNo);
}

/// �r�̊֐ߊp�x
const Vector& DataHandler::getLegJointAngle(int legNo) const
{
	/// �����`�F�b�N
	assert(1 <= legNo && legNo <= LEG_NUM);

	return asuraData.getLegJointAngle(legNo);
}

/// �r�̊֐ߑ��x
const Vector& DataHandler::getLegJointVelocity(int legNo) const
{
	/// �����`�F�b�N
	assert(1 <= legNo && legNo <= LEG_NUM);

	return asuraData.getLegJointVelocity(legNo);
}

/// �r�̊֐߃g���N
const Vector& DataHandler::getLegJointTorque(int legNo) const
{
	/// �����`�F�b�N
	assert(1 <= legNo && legNo <= LEG_NUM);

	return asuraData.getLegJointTorque(legNo);
}

/// �r�̉^����
const LegPhase DataHandler::getLegPhase(int legNo) const
{
	/// �����`�F�b�N
	assert(1 <= legNo && legNo <= LEG_NUM);

	return asuraData.getLegPhase(legNo);
}

/**
 *		�N���[��
 */
/*  20200819

/// �N���[���������Ƃ̓����ϊ��s��
const Matrix& DataHandler::getTrackBaseTransformation(int trackNo) const
{
	/// �����`�F�b�N
	assert(1 <= trackNo && trackNo <= TRACK_NUM);

	return asuraData.getTrackBaseTransformation(trackNo);
}

/// �N���[�����Ƃ̓����ϊ��s��
const Matrix& DataHandler::getTrackJointTransformation(int trackNo, int jointNo) const
{
	/// �����`�F�b�N
	assert(1 <= trackNo && trackNo <= TRACK_NUM);
	assert(1 <= jointNo && jointNo <= TRACK_JOINT_NUM);

	return asuraData.getTrackJointTransformation(trackNo, jointNo);
}

/// �N���[����[���Ƃ̓����ϊ��s��
const Matrix& DataHandler::getTrackEndTransformation(int trackNo) const
{
	/// �����`�F�b�N
	assert(1 <= trackNo && trackNo <= TRACK_NUM);

	return asuraData.getTrackEndTransformation(trackNo);
}

/// �N���[���̍����ʒu
const Vector& DataHandler::getTrackBasePosition(int trackNo) const
{
	/// �����`�F�b�N
	assert(1 <= trackNo && trackNo <= TRACK_NUM);

	return asuraData.getTrackBasePosition(trackNo);
}

/// �N���[���̊֐߈ʒu
const Vector& DataHandler::getTrackJointPosition(int trackNo, int jointNo) const
{
	/// �����`�F�b�N
	assert(1 <= trackNo && trackNo <= TRACK_NUM);
	assert(1 <= jointNo && jointNo <= TRACK_JOINT_NUM);

	return asuraData.getTrackJointPosition(trackNo, jointNo);
}

/// �N���[���̐�[�ʒu
const Vector& DataHandler::getTrackEndPosition(int trackNo) const
{
	/// �����`�F�b�N
	assert(1 <= trackNo && trackNo <= TRACK_NUM);

	return asuraData.getTrackEndPosition(trackNo);
}

/// �N���[���̊֐ߊp�x
const Vector& DataHandler::getTrackJointAngle(int trackNo) const
{
	/// �����`�F�b�N
	assert(1 <= trackNo && trackNo <= TRACK_NUM);

	return asuraData.getTrackJointAngle(trackNo);
}

/// �N���[���̊֐ߑ��x
const Math::Vector& DataHandler::getTrackJointVelocity(int trackNo) const
{
	/// �����`�F�b�N
	assert(1 <= trackNo && trackNo <= TRACK_NUM);

	return asuraData.getTrackJointVelocity(trackNo);
}

/// �N���[���̊֐߃g���N
const Math::Vector& DataHandler::getTrackJointTorque(int trackNo) const
{
	/// �����`�F�b�N
	assert(1 <= trackNo && trackNo <= TRACK_NUM);

	return asuraData.getTrackJointTorque(trackNo);
}

*/

/**
 *		DataHandler�N���X��private�ȃ����o�֐�
 */


}