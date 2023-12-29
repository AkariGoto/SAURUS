/**
 *  File: PlanParameter.h
 *
 *  Description: ����v��̒萔
 *
 *  Created: 2007/03/19(Mon)
 *  Updated: 2017/03/19(Mon)
 *
 *  Copyright (C) Tokyo Institute of Technology Hirose Lab.
 */

//  20200820  TROTGAIT��TRIPODGAIT�ɒu���E�N���[���E��������E�`�ԕω��֘A���R�����g�A�E�g�C���䃂�[�h�̕s�v�����R�����g�A�E�g
//  20200929  �x���r���Ɏg�p����z��������ǉ�
//  20201005  1�����ŏI��
//  20201017  �p���ڍs�̊p���x�ǉ�

/**
 *		�^���w�̌��ʈꗗ
 */
#ifndef __PlanParameter_h__
#define __PlanParameter_h__

#include "..\Math\MathLibrary.h"
#include "..\Kinematics\AsuraParameter.h"
#include "..\Math\MathConstant.h"
#include "iostream"

using namespace Asura;
using namespace Const;
using namespace std;

namespace Plan
{

/**
 *		���쐶���̂��߂̓���X�e�[�^�X
 */
typedef enum 
{
	RUN,						// 0 // ���s��
	END,						// 1 // ����I��
	REQUEST,					// 2 // ����v��
	SUSPEND,					// 3 // ���f
	WAIT,						// 4 // �ҋ@
	INVALID,					// 5 // ����
} PlanStatus;

/**
 *		���䃂�[�h
 */
typedef enum 
{
	NO_STRATEGY,				// 00 /// �v��Ȃ�
	//CRAWL,						// 01 /// �N���[�����e  20200820
	//INTERMITTENT_CRAWL,			// 02 /// �ԟ[�N���[�����e  20200820
	//TROT,						// 03 /// �g���b�g���e
	TRIPOD,						// 03 /// �g���C�|�b�h���e  //  20200820
	TRIPOD_1_CYCLE				// 1�����݂̂̃g���C�|�b�h���e  //20201005
	//INTERMITTENT_TROT,			// 04 /// �ԟ[�g���b�g���e  20200820
	//SPINNING,					// 05 /// ���̏������e  20200820
	//TURNNING,					// 06 /// ������e  20200820
	//CRAB_FREE,					// 07 /// ���R���s���e  20200820
	//OFF_LINE,					// 08 /// �I�t���C�������Č�  20200820
	//TEACHING_PLAYBACK,			// 09 /// �e�B�[�`���O�v���C�o�b�N  20200820
	//AXIS_CONTROL,				// 10 /// ���ڊ֐ߎ�����  20200820
	//TRACK_DRIVE,				// 11 /// �N���[�����i  20200820
	//HYBRID_MOTION,				// 12 /// �r�E�N���[�������^��  20200820
	//MODE_CHANGE,				// 13 /// �`�ԕω�  20200820
	//URG_MOTION,					// 14 /// URG�ɂ��Βn�K������@		//�ǉ�  20200820
} Strategy;

/**
 *		������̃p�����[�^
 */
const int AC_DLG_AXIS_NUM = 6;

/**
 *		�v�����ɗp���鋤�ʃp�����[�^
 */
/// �ړ�����
const double DEFAULT_LOCOMOTION_DIRECTION[] = {1.0, 0.0, 0.0};

//20200929  �x���r���Ɏg�p����z������
const double UP_DIRECTION[] = { 0.0, 0.0, 1.0 };

/**
 *	�N���[�����e�̃p�����[�^
 */
/*  20200820
/// �f���[�e�B��
const double CRAWLGAIT_DUTY_FACTOR = 0.75;
/// �d�S�X�g���C�h
const double CRAWLGAIT_STRIDE = 250.0;	/// [mm]
/// ���s���x
const double CRAWLGAIT_WALKING_SPEED = 235.294118;//50.0;	/// [mm/s]
/// �V�r�̐U��グ
const double CRAWLGAIT_SWING_UP[] = {0.0, 0.0, 60.0};	/// [mm]
/// �V�r�̐U�艺��
const double CRAWLGAIT_SWING_DOWN[] = {0.0, 0.0, -60.0};		/// [mm]

/// �����d�S����
const double CRAWLGAIT_INITIAL_BODY_POSITION[] = {0, 0, 371.5};
*/

/// �r��ʒu
/**
 *	�r���W�n�ł̊�ʒu��{167.0, 220.0, -371.5}
 *	����ɓ��̂̕��C�����̊e���l�𑫂������̂����̍��W�n�ł̊�ʒu
 */
/*  20200820
const double CRAWLGAIT_FOOT_REF_POSITION_1[] = {304.0, 340.0, -371.5};	/// [mm]
const double CRAWLGAIT_FOOT_REF_POSITION_2[] = {-330.0, 340.0, -371.5};
const double CRAWLGAIT_FOOT_REF_POSITION_3[] = {-330.0, -340.0, -371.5};
const double CRAWLGAIT_FOOT_REF_POSITION_4[] = {304.0, -340.0, -371.5}; 
*/

/**
 *	�g���C�|�b�h���e�̃p�����[�^
 */
/// �f���[�e�B��
const double TRIPODGAIT_DUTY_FACTOR = 0.5;//0.50001;
/// �d�S�X�g���C�h
const double TRIPODGAIT_STRIDE = 400.0;	/// [mm]250
/// ���s���x
const double TRIPODGAIT_WALKING_SPEED = 100.0;	//SAURUS v<460.0�Ȃ�ok(12/27) / [mm/s]200
/// �V�r�̐U��グ
const double TRIPODGAIT_SWING_UP[] = {0.0, 0.0, 100.0};	//SAURUS 94.0(12/27)/// 100[mm]//down��200[mm]
/// �V�r�̐U�艺��
const double TRIPODGAIT_SWING_DOWN[] = {0.0, 0.0, -100.0};		//SAURUS -94.0(12/27)	/// -100[mm]//down��-300[mm]

/// �����d�S����
//const double TRIPODGAIT_INITIAL_BODY_POSITION[] = {0, 0, 371.5+100};  //371.5[mm] //B�n�`���Č�����Ƃ��͏d�S�������グ��΋r�����ɂ߂荞�܂Ȃ�
const double TRIPODGAIT_INITIAL_BODY_POSITION[] = { 0, 0, 400.0 };
//const double TRIPODGAIT_INITIAL_BODY_POSITION[] = { 0, 0, 450 };

/// �r��ʒu
/**
 *	�r���W�n�ł̊�ʒu��{167.0, 220.0, -371.5}
 *	����ɓ��̂̕��C�����̊e���l�𑫂������̂����̍��W�n�ł̊�ʒu
 */
//const double TRIPODGAIT_FOOT_REF_POSITION_1[] = {304.0, 340.0, -371.5};	/// [mm]
//const double TRIPODGAIT_FOOT_REF_POSITION_2[] = {-330.0, 340.0, -371.5};
//const double TRIPODGAIT_FOOT_REF_POSITION_3[] = {-330.0, -340.0, -371.5};
//const double TRIPODGAIT_FOOT_REF_POSITION_4[] = {304.0, -340.0, -371.5};
/*
const double TRIPODGAIT_FOOT_REF_POSITION_1[] = {304.0+100,	340.0+100,	-371.5};	/// [mm]
const double TRIPODGAIT_FOOT_REF_POSITION_2[] = {0.0,			340.0+300,	-371.5};
const double TRIPODGAIT_FOOT_REF_POSITION_3[] = {-330.0-100,	340.0+100,	-371.5};
const double TRIPODGAIT_FOOT_REF_POSITION_4[] = {-330.0-100,	-340.0-100, -371.5};
const double TRIPODGAIT_FOOT_REF_POSITION_5[] = {0.0,			-340.0-300, -371.5};
const double TRIPODGAIT_FOOT_REF_POSITION_6[] = {304.0+100,	-340.0-100, -371.5};

const double TRIPODGAIT_FOOT_REF_POSITION_1[] = {400.0,	400.0,	-371.5};	/// [mm]
const double TRIPODGAIT_FOOT_REF_POSITION_2[] = {0.00,	600.0,	-371.5};
const double TRIPODGAIT_FOOT_REF_POSITION_3[] = {-400.0,	400.0,	-371.5};
const double TRIPODGAIT_FOOT_REF_POSITION_4[] = {-400.0,	-400.0, -371.5};
const double TRIPODGAIT_FOOT_REF_POSITION_5[] = {0.00,	-600.0, -371.5};
const double TRIPODGAIT_FOOT_REF_POSITION_6[] = {400.0,	-400.0, -371.5};
*/ 

const double TRIPODGAIT_FOOT_REF_POSITION_1[] = {500.0,	500.0, -TRIPODGAIT_INITIAL_BODY_POSITION[2] };	/// [mm]  20200930 +100����
const double TRIPODGAIT_FOOT_REF_POSITION_2[] = { 0.00,	700.0 + BODY_WIDTH / 2 , -TRIPODGAIT_INITIAL_BODY_POSITION[2] };  //���s�͋[�����Ɏg�p
//const double TRIPODGAIT_FOOT_REF_POSITION_2[] = { 0.00,	600.0 + BODY_WIDTH / 2 , -TRIPODGAIT_INITIAL_BODY_POSITION[2] };  //���s�͋[�����Ɏg�p
const double TRIPODGAIT_FOOT_REF_POSITION_3[] = {-500.0,	500.0,	-TRIPODGAIT_INITIAL_BODY_POSITION[2] };
const double TRIPODGAIT_FOOT_REF_POSITION_4[] = {-500.0,	-500.0, -TRIPODGAIT_INITIAL_BODY_POSITION[2] };
const double TRIPODGAIT_FOOT_REF_POSITION_5[] = {0.00,	-700.0, -TRIPODGAIT_INITIAL_BODY_POSITION[2]};//down��+100
const double TRIPODGAIT_FOOT_REF_POSITION_6[] = {500.0,	-500.0, -TRIPODGAIT_INITIAL_BODY_POSITION[2] };

//20201017
const double ANGULAR_VELOCITY = 0.02 * Const::PI;  //[rad/s]  �p���x�̉��̒l

/**
 *	�N���[���`�Ԃ̃p�����[�^
 */
/*  20200820
/// �N���[���̊֐ߐ���
typedef enum 
{
	NORMAL,			/// 0 /// �ʏ푖�s�i���E�������j
	TURN,			/// 1 /// ���񑖍s
	SKID_STEER,		/// 2 /// �X�L�b�h�X�e�A
	DIRECT,			/// 3 /// ���ڐ���
} DrivePattern;

/// ���̏����ʒu
const double TRACK_INITIAL_BODY_POSITION[] = {0, 0, 150.5};

/// ���s���x
const double TRACK_DRIVE_SPEED = 80.0;	/// [mm/s]
*/

/**
 *	��������̃p�����[�^
 */

/*  20200820
/// ��������̕��@
typedef enum 
{
	SMALL,			/// 0 /// ���K�́i�N���[���̂݁j
	MIDDLE,			/// 1 /// ���K��
	LARGE,			/// 2 /// ��K��
} HybridMotionPattern;

/// �i�s���x
const double ROBOT_SPEED = 40.0;	/// [mm/s]
const double ROBOT_SPEED_z = 50.0;	// [mm/s]

*/

/// �r��ʒu
/**
 *	�r���W�n�ł̊�ʒu��{167.0, 220.0, -371.5}
 *	����ɓ��̂̕��C�����̊e���l�𑫂������̂����̍��W�n�ł̊�ʒu
 */
/*  20200820
const double HYBRIDMOTION_FOOT_REF_POSITION_1[] = {105, 310.5, -150.5};	/// [mm]
const double HYBRIDMOTION_FOOT_REF_POSITION_2[] = {-105, 310.5, -150.5};
const double HYBRIDMOTION_FOOT_REF_POSITION_3[] = {-105, -310.5, -150.5};
const double HYBRIDMOTION_FOOT_REF_POSITION_4[] = {105, -310.5, -150.5};

///�p�x�Ǝ���
//���K��
const double sGFHA1 = -(asin((STEP-WHEELRADIUS)/sqrt(pow(LINK2,2)+pow(WHEELRADIUS,2)))+asin(WHEELRADIUS/sqrt(pow(LINK2,2)+pow(WHEELRADIUS,2))));//��������
const double sGFHA2 = -(asin((STEP-WHEELRADIUS)/sqrt(pow(LINK2,2)+pow(WHEELRADIUS,2)))+asin(WHEELRADIUS/sqrt(pow(LINK2,2)+pow(WHEELRADIUS,2))));//��������
//const double CORRECTION= (asin((STEP-WHEELRADIUS)/sqrt(pow(LINK2,2)+pow(WHEELRADIUS,2)))+asin(WHEELRADIUS/sqrt(pow(LINK2,2)+pow(WHEELRADIUS,2))));
const double CORRECTION = asin((STEP-WHEELRADIUS+50)/LINK2);
const double sGRHA2 = (CORRECTION-asin(WHEELRADIUS*(1-cos(CORRECTION))/BODY_LENGTH));// 50*DEG2RAD;//��������
//const double sGRHA2 =-(-asin((STEP-WHEELRADIUS)/sqrt(pow(LINK2,2)+pow(WHEELRADIUS,2)))+asin((-WHEELRADIUS/sqrt(pow(LINK2,2)+pow(WHEELRADIUS,2)))));// 50*DEG2RAD;//��������

const double SMALL_HYBRID_MOTION_SEQUENCE_TIME_1 = (395-(WHEELRADIUS*sin(-sGFHA1)+LINK2*cos(-sGFHA1))+15)/ROBOT_SPEED;	/// [s]
const double SMALL_HYBRID_MOTION_SEQUENCE_TIME_2 = 2.0*(LINK2+WHEELRADIUS*(PI/2+sGFHA1))/(ROBOT_SPEED/cos(-sGFHA1));	/// [s]	///�W���␳1.34
const double SMALL_HYBRID_MOTION_SEQUENCE_TIME_3 = (300-WHEELRADIUS*sin(sGRHA2+sGFHA2))/ROBOT_SPEED;	/// [s]
const double SMALL_HYBRID_MOTION_SEQUENCE_TIME_4 = sGRHA2/(PI/2)*5;	/// [s]
const double SMALL_HYBRID_MOTION_SEQUENCE_TIME_5 = 7.0;	/// [s]

const double LEG_CAHNGE_TIME =4;
*/

/**
 *		�`�ԕω��p�̃p�����[�^
 */
/*  20200820
const double MODE_CAHNGE_SEQUENCE_TIME_1 = 10.0;	/// [s]
const double MODE_CAHNGE_SEQUENCE_TIME_2 = 10.0;	/// [s]
const double MODE_CAHNGE_SEQUENCE_TIME_3 = 15.0;	/// [s]
const double MODE_CAHNGE_SEQUENCE_TIME_4 = 10.0;	/// [s]
const double MODE_CAHNGE_SEQUENCE_TIME_5 = 10.0;	/// [s]
const double MODE_CAHNGE_SEQUENCE_TIME_6 = 10.0;	/// [s]
*/
/**
 *		�`�ԕω��p�̃p�����[�^(Track>>Crowl�̍ۂ̋r�W�J�������ڕW�r��)		///�ǉ�
 */
/*  20200820
const double MODE_CHANGE_START_FOOT_REF_POSITION_1[]={262.3, 340.0, -150.5+20};	/// [mm];
const double MODE_CHANGE_START_FOOT_REF_POSITION_2[]={-455.0, 340.0, -150.5+20};
const double MODE_CHANGE_START_FOOT_REF_POSITION_3[]={-288.3, -340.0, -150.5+20};
const double MODE_CHANGE_START_FOOT_REF_POSITION_4[]={429.0, -340.0, -150.5+20};
*/
/**
 *		�`�ԕω��p�̃p�����[�^(Track>>Crowl�̍ۂ̏I�����ڕW�r��)		///�ǉ�
 */
/*  20200820
const double MODE_CHANGE_FINAL_FOOT_REF_POSITION_1[]={262.3, 340.0, -371.5};	/// [mm];
const double MODE_CHANGE_FINAL_FOOT_REF_POSITION_2[]={-455.0, 340.0, -371.5};
const double MODE_CHANGE_FINAL_FOOT_REF_POSITION_3[]={-288.3, -340.0, -371.5};
const double MODE_CHANGE_FINAL_FOOT_REF_POSITION_4[]={429.0, -340.0, -371.5};
*/
/**
 *		�`�ԕω��p�̃p�����[�^(Crowl>>Track�̍ۂ̓��̉��~���ڕW�r��)		///�ǉ�
 */
/*  20200820
const double MODE_CHANGE_FOOT_REF_POSITION_1[]={292.6, 314.0, -100.4};	/// [mm];
const double MODE_CHANGE_FOOT_REF_POSITION_2[]={-292.6, 314.0, -100.4};
const double MODE_CHANGE_FOOT_REF_POSITION_3[]={-292.6, -314.0, -100.4};
const double MODE_CHANGE_FOOT_REF_POSITION_4[]={292.6, -314.0, -100.4};
*/
}

	extern unsigned char changeflag;
	//extern Plan::HybridMotionPattern hybridmotionPattern;

	/// end of namespace Plan
#endif /// __PlanParameter_h__