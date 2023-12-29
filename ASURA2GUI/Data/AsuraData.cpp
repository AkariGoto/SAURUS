/**
 *  �t�@�C����
 *		AsuraData.cpp
 *  ����
 *		TITAN X�̃f�[�^�i�L�l�}�e�B�N�X�C�N���[���Ȃǁj
 *  ���t
 *		�쐬��: 2007/02/12(Mon)		�X�V��: 2007/02/19(Mon)
 */

//20200819  �N���[���֘A�R�����g�A�E�g
//20200820  �t�@�C������TitanData.cpp����AsuraData.cpp�ɕύX
//20201013  ��4�֐�
//20201014  ��4�֐�

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include "AsuraData.h"
#include "..\Utility\EngConstant.h"


using namespace std;
using namespace Math;
using namespace Const;
using namespace Asura;

namespace Data
{
/**
 *	----------------------------------------------------------------------
 *		AsuraData�N���X
 *	----------------------------------------------------------------------
 */

/**
 *	------------------------------------------------------------
 *		AsuraData�N���X�̃����o�֐���`
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	�R���X�g���N�^�ƃf�X�g���N�^
 *	----------------------------------------
 */
/// �f�t�H���g�R���X�g���N�^
AsuraData::AsuraData()
{
	newAsuraData();
}

/// �R�s�[�R���X�g���N�^
AsuraData::AsuraData(const AsuraData& asuraData)
{
	newAsuraData();
	copy(asuraData);
}

/// �f�X�g���N�^
AsuraData::~AsuraData()
{
	deleteAsuraData();
}

/**
 *	���Z�q
 */
/// ������Z�q
AsuraData& AsuraData::operator=(const AsuraData& asuraData)
{
	/// ���ȑ���̃`�F�b�N
	if ( &asuraData != this )
	{
		copy(asuraData);
	}

	return *this;
}

/// �������Z�q
bool AsuraData::operator==(const AsuraData& asuraData)
{
	/// �C���N�������g�ϐ�
	int i, j;

	/**
	 *	����
	 */
	if ( bodyTransformation	!= asuraData.bodyTransformation ){return false;}
	if ( bodyPosition			!= asuraData.bodyPosition ){return false;}
	if ( bodyVelocity			!= asuraData.bodyVelocity ){return false;}
	if ( walkingDirection		!= asuraData.walkingDirection ){return false;}
	if ( locomotionStyle		!= asuraData.locomotionStyle ) {return false;}

	/**
	 *	�r
	 */
	for (i=0; i<LEG_NUM; i++)
	{
		if ( legBaseTransformation[i]	!= asuraData.legBaseTransformation[i] ){return false;}
		if ( legBasePosition[i]			!= asuraData.legBasePosition[i] ){return false;}

		for (j=0; j<=LEG_JOINT_NUM; j++)
		{
			if ( legJointTransformation[i][j]	!= asuraData.legJointTransformation[i][j] ){return false;}
			if ( legJointPosition[i][j]			!= asuraData.legJointPosition[i][j] ){return false;}
		}

		if ( legFootTransformation[i] != asuraData.legFootTransformation[i] ){return false;}
		if ( legFootPosition[i] != asuraData.legFootPosition[i] ){return false;}

		if ( legJointAngle[i]		!= asuraData.legJointAngle[i] ){return false;}
		if ( legJointVelocity[i]	!= asuraData.legJointVelocity[i] ){return false;}
		if ( legJointTorque[i]	!= asuraData.legJointTorque[i] ){return false;}

		if ( legPhase[i] != asuraData.legPhase[i] ){return false;}
	}


	/**
	 *	�N���[��
	 */

	/*  20200819
	/// �N���[�����Ƃ̓����ϊ��s��
	for (i=0; i<TRACK_NUM; i++)
	{
		if ( trackBaseTransformation[i]	!= asuraData.trackBaseTransformation[i] ){return false;}
		if ( trackBasePosition[i]			!= asuraData.trackBasePosition[i] ){return false;}

		for (j=0; j<TRACK_JOINT_NUM; j++)
		{
			if ( trackJointTransformation[i][j]	!= asuraData.trackJointTransformation[i][j] ){return false;}
			if ( trackJointPosition[i][j]			!= asuraData.trackJointPosition[i][j] ){return false;}
		}

		if ( trackEndTransformation[i]	!= asuraData.trackEndTransformation[i] ){return false;}
		if ( trackEndPosition[i]				!= asuraData.trackEndPosition[i] ){return false;}

		if ( trackJointAngle[i]		!= asuraData.trackJointAngle[i] ){return false;}
		if ( trackJointVelocity[i]	!= asuraData.trackJointVelocity[i] ){return false;}
		if ( trackJointTorque[i]		!= asuraData.trackJointTorque[i] ){return false;}

		if ( trackSpeed[i] != asuraData.trackSpeed[i] ){return false;}
	}
	*/

	return true;
}

/// �񓙉����Z�q
bool AsuraData::operator!=(const AsuraData& asuraData)
{
	if ( *this == asuraData )
		return false;
	else
		return true;
}

/**
 *	------------------------------------------------------------
 *		AsuraData�N���X��private�ȃ����o�֐�
 *	------------------------------------------------------------
 */
/**
 *	�I�u�W�F�N�g�̃������̈���m��
 */
void AsuraData::newAsuraData(void)
{
/// �C���N�������g�ϐ�
	int i, j;

/// �����o�ϐ��̏�����
/**
 *	����
 */
	/// �����ϊ��s��̃T�C�Y����
	bodyTransformation.setSize(DH_DIMENSION, DH_DIMENSION);
	bodyTransformation.loadIdentity();

	/// ���̈ʒu
	bodyPosition.setSize(THREE_DIMENSION);

	/// ���̑��x
	bodyVelocity.setSize(THREE_DIMENSION);

	/// ���s����
	walkingDirection.setSize(THREE_DIMENSION);

	/// �ړ��l��
	locomotionStyle = LEGGED;

/**
 *	�r
 */
	/// �I�u�W�F�N�g�̃|�C���^�̃������̈�m��
	legJointTransformation	= new Matrix*[LEG_NUM];
	legJointPosition		= new Vector*[LEG_NUM];
	
	
	/// �I�u�W�F�N�g�̃������̈�m��
	legBaseTransformation	= new Matrix[LEG_NUM];
	legBasePosition			= new Vector[LEG_NUM];

	legFootTransformation	= new Matrix[LEG_NUM];
	legFootPosition			= new Vector[LEG_NUM];

	legJointAngle		= new Vector[LEG_NUM];
	legJointVelocity	= new Vector[LEG_NUM];
	legJointTorque		= new Vector[LEG_NUM];
	legActuatorPosition		= new Vector[LEG_NUM];

	/// �r�̉^����
	legPhase = new LegPhase[LEG_NUM];

	FootJointAngle = new double[LEG_NUM];  //20201013

	 for (i=0; i<LEG_NUM; i++)
	 {
		/// �I�u�W�F�N�g�̃������̈�m��
		legJointTransformation[i]	= new Matrix[LEG_JOINT_NUM];
		legJointPosition[i]			= new Vector[LEG_JOINT_NUM];

		/// �s��̃T�C�Y����		
		 for (j=0; j<LEG_JOINT_NUM; j++)
		 {
			legJointTransformation[i][j].setSize(DH_DIMENSION, DH_DIMENSION);
			legJointTransformation[i][j].loadIdentity();

			legJointPosition[i][j].setSize(THREE_DIMENSION);
		 }

		 legBaseTransformation[i].setSize(DH_DIMENSION, DH_DIMENSION);
		 legBaseTransformation[i].loadIdentity();

		 legBasePosition[i].setSize(THREE_DIMENSION);

		 legFootTransformation[i].setSize(DH_DIMENSION, DH_DIMENSION);
		 legFootTransformation[i].loadIdentity();

		 legFootPosition[i].setSize(THREE_DIMENSION);

		 legJointAngle[i].setSize(LEG_JOINT_NUM);
		 legJointVelocity[i].setSize(LEG_JOINT_NUM);
		 legJointTorque[i].setSize(LEG_JOINT_NUM);
		 legActuatorPosition[i].setSize(LEG_ACT_NUM);///<----Add

		 legPhase[i] = SUPPORT;

		 //20201014
		 FootJointAngle[i] = 0;
	 }
	 //20201013
	 //FootJointAngle.setSize(LEG_NUM);///<-------------------Add
/**
 *	�N���[��
 */
/*20200819
	/// �N���[�����Ƃ̓����ϊ��s��Ɗ֐߈ʒu
	/// �I�u�W�F�N�g�̃|�C���^�̃������̈�m��
	trackJointTransformation	= new Matrix*[TRACK_NUM];
	trackJointPosition				= new Vector*[TRACK_NUM];

	/// �I�u�W�F�N�g�̃������̈�m��
	trackBaseTransformation	= new Matrix[TRACK_NUM];
	trackBasePosition				= new Vector[TRACK_NUM];

	trackEndTransformation		= new Matrix[TRACK_NUM];
	trackEndPosition				= new Vector[TRACK_NUM];

	trackJointAngle		= new Vector[TRACK_NUM];
	trackJointVelocity	= new Vector[TRACK_NUM];
	trackJointTorque		= new Vector[TRACK_NUM];

	trackSpeed = new double[TRACK_NUM];
	
	for (i=0; i<TRACK_NUM; i++)
	{
	 	/// �I�u�W�F�N�g�̃������̈�m��
		trackJointTransformation[i]	= new Matrix[TRACK_JOINT_NUM];
		trackJointPosition[i]				= new Vector[TRACK_JOINT_NUM];

		/// �s��̃T�C�Y����		
		for (j=0; j<TRACK_JOINT_NUM; j++)
		{
			trackJointTransformation[i][j].setSize(DH_DIMENSION, DH_DIMENSION);
			trackJointTransformation[i][j].loadIdentity();
			
			trackJointPosition[i][j].setSize(THREE_DIMENSION);
		}

		trackBaseTransformation[i].setSize(DH_DIMENSION, DH_DIMENSION);
		trackBaseTransformation[i].loadIdentity();

		trackBasePosition[i].setSize(THREE_DIMENSION);

		trackEndTransformation[i].setSize(DH_DIMENSION, DH_DIMENSION);
		trackEndTransformation[i].loadIdentity();

		trackEndPosition[i].setSize(THREE_DIMENSION);

		trackJointAngle[i].setSize(TRACK_JOINT_NUM);
		trackJointVelocity[i].setSize(TRACK_JOINT_NUM);
		trackJointTorque[i].setSize(TRACK_JOINT_NUM);

		trackSpeed[i] = 0.0;
	}
	*/

	return;
}

/**
 *	�I�u�W�F�N�g�̃������̈�����
 */
void AsuraData::deleteAsuraData(void)
{
	/// �C���N�������g
	int i, j;

/**
 *	�r
 */
	/// �r�����̓����ϊ��s��
	delete [] legBaseTransformation;

	/// �r�֐߂̓����ϊ��s��
	for (i=0; i<LEG_NUM; i++)
		delete [] legJointTransformation[i];

	delete [] legJointTransformation;

	/// ����̓����ϊ��s��
	delete [] legFootTransformation;

	/// �r�����̈ʒu�x�N�g��
	delete [] legBasePosition;

	/// �r�֐߂̈ʒu�x�N�g��
	for (j=0; j<LEG_NUM; j++)
		delete [] legJointPosition[j];

	delete [] legJointPosition;

	/// ���ʒu
	delete [] legFootPosition;

	/// �r�֐߂̊p�x�C�p���x�C�g���N�x�N�g��
	delete [] legJointAngle;
	delete [] legJointVelocity;
	delete [] legJointTorque;
	//delete [] FootJointAngle;//----Add

	/// �r�̉^����
	delete [] legPhase;

/**
 *	�N���[��
 */
	/*  20200819
	/// �N���[�������̓����ϊ��s��
	delete [] trackBaseTransformation;

	/// �N���[���̓����ϊ��s��
	for (i=0; i<TRACK_NUM; i++)
		delete[]  trackJointTransformation[i];

	delete [] trackJointTransformation;

	/// �N���[����[�̓����ϊ��s��
	delete [] trackEndTransformation;

	/// �N���[�������ʒu
	delete [] trackBasePosition;

	/// �N���[���֐߂̈ʒu�x�N�g��
	for (j=0; j<TRACK_NUM; j++)
		delete [] trackJointPosition[j];

	delete [] trackJointPosition;

	/// �N���[����[�ʒu
	delete [] trackEndPosition;

	/// �N���[���֐߂̊p�x�C�p���x�C�g���N�x�N�g��
	delete [] trackJointAngle;
	delete [] trackJointVelocity;
	delete [] trackJointTorque;

	/// �N���[�����x
	delete [] trackSpeed;
	*/

	return;
}
/**
 *	�R�s�[�R���X�g���N�^�Ƒ�����Z�q�̃w���v�֐�
 */
void AsuraData::copy(const AsuraData& asuraData)
{
	/// �C���N�������g�ϐ�
	int i, j;

	/**
	 *		����
	 */
	bodyTransformation	= asuraData.bodyTransformation;
	bodyPosition			= asuraData.bodyPosition;
	bodyVelocity			= asuraData.bodyVelocity;
	walkingDirection		= asuraData.walkingDirection;
	locomotionStyle		= asuraData.locomotionStyle;

	/**
	 *		�r
	 */
	for (i=0; i<LEG_NUM; i++)
	{
		legBaseTransformation[i]	= asuraData.legBaseTransformation[i];
		legBasePosition[i]				= asuraData.legBasePosition[i];

		for (j=0; j<LEG_JOINT_NUM; j++)
		{
			legJointTransformation[i][j]	 = asuraData.legJointTransformation[i][j];
			legJointPosition[i][j]			 = asuraData.legJointPosition[i][j];
		}
		for (j=0; j<LEG_ACT_NUM; j++)
		{
			legActuatorPosition[i]		 = asuraData.legActuatorPosition[i];//---Add
		}
		legFootTransformation[i] = asuraData.legFootTransformation[i];
		legFootPosition[i] = asuraData.legFootPosition[i];

		legJointAngle[i]		= asuraData.legJointAngle[i];
		legJointVelocity[i]		= asuraData.legJointVelocity[i];
		legJointTorque[i]		= asuraData.legJointTorque[i];
		FootJointAngle[i]		= asuraData.FootJointAngle[i];//---Add

		legPhase[i] = asuraData.legPhase[i];
	}

	/**
	 *		�N���[��
	 */
	/*20200819
	for (i=0; i<TRACK_NUM; i++)
	{
		trackBaseTransformation[i]	= asuraData.trackBaseTransformation[i];
		trackBasePosition[i]				= asuraData.trackBasePosition[i];

		for (j=0; j<TRACK_JOINT_NUM; j++)
		{
			trackJointTransformation[i][j]	= asuraData.trackJointTransformation[i][j];
			trackJointPosition[i][j]				= asuraData.trackJointPosition[i][j];
		}

		trackEndTransformation[i]		= asuraData.trackEndTransformation[i];
		trackEndPosition[i]				= asuraData.trackEndPosition[i];

		trackJointAngle[i]		= asuraData.trackJointAngle[i];
		trackJointVelocity[i]		= asuraData.trackJointVelocity[i];
		trackJointTorque[i]		= asuraData.trackJointTorque[i];

		trackSpeed[i] = asuraData.trackSpeed[i];
	}
	*/

	return;
}

}	/// end of namespace Asura