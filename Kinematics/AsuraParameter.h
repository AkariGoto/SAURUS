/**
 *  File: AsuraParameter.h
 *
 *  Description: AsuraX�Ɋւ���萔�p�����[�^
 *
 *  Created: 2007/01/31(Wed)
 *  Updated: 2018/01/24(Mon)
 *
 *  Copyright (C) Tokyo Institute of Technology Hirose Lab.
 */

//  20200819  �N���[���֘A�R�����g�A�E�g
//  20200824
//	20220713  �p�����[�^�C��

#ifndef __AsuraParameter_h__
#define __AsuraParameter_h__

#include "..\Utility\Constants.h"
#include "..\Math\MathLibrary.h"
#include "..\Math\MathConstant.h"

namespace Asura
{

const double	STEP				=200;	//�i������
//const double	Depth				=6;		//�ˋN�̐H�����ݐ[�����i�������ɂ���ĕς���	
/**
 *		�r�p�����[�^
 */

/// �֐ߐ�
const int		LEG_JOINT_NUM		= 3;  //��4�֐߂͎󓮊֐�//��S�֐߂͑��񈵂�20231127
//const int		LEG_JOINT_NUM = 4;  //20200824

/// �֐�No
const int		FIRST_JOINT			= 1;
const int		SECOND_JOINT		= 2;
const int		THIRD_JOINT			= 3;

/// 1�r�̃A�N�`���G�[�^��
const int		LEG_ACT_NUM		    = 4; //�ǉ�
//const int		LEG_ACT_NUM			= 4; //�ǉ�

/// �e�����N��
const double	LINK1 = 161.0; //106.5;
const double	LINK2 = 342.0;
const double	LINK3 = 233.0;
const double	FOOT = 524.0;//32.0

const double	OFFSET1				= 0.0;//106.5;//0.0;
const double	OFFSET2				= 0.0;//JOINT2�̎�����OFFSET//100.5;
const double	OFFSET3				= 0.0;//JOINT3�̎�����OFFSET//49.95;

/// �e�����N��(�ǉ��p�����[�^)
const double	LINK3H				= 30.0;					//�������N�̌���

/// �v�[���[���a
//const double	Pulley_Radius		= 33.00;//26.0;
const double	Pulley_Radius		= 33.75;//26.0;

/// �e�֐߂̉��͈� (�P��: degree)
//SAURUS
const double	LEG_ANGLE1_MIN = -45.0 / 180 * Const::PI;
const double	LEG_ANGLE1_MAX = 45.0 / 180 * Const::PI;
const double	LEG_ANGLE2_MIN = -75.0 / 180 * Const::PI;
const double	LEG_ANGLE2_MAX = 75.0 / 180 * Const::PI;
const double	LEG_ANGLE3_MIN = -130.0 / 180 * Const::PI;
const double	LEG_ANGLE3_MAX = 130.0 / 180 * Const::PI;
const double	LEG_ANGLE4_MIN = -90.0 / 180 * Const::PI;
const double	LEG_ANGLE4_MAX = 140.0 / 180 * Const::PI;
/*ASURA
const double	LEG_ANGLE1_MIN		=	-45.0 / 180 * Const::PI;
const double	LEG_ANGLE1_MAX		=	45.0 / 180 * Const::PI;
const double	LEG_ANGLE2_MIN		=	-75.0 / 180 * Const::PI;
const double	LEG_ANGLE2_MAX		=	75.0 / 180 * Const::PI;
const double	LEG_ANGLE3_MIN		=	-130.0 / 180 * Const::PI;
const double	LEG_ANGLE3_MAX		=	130.0 / 180 * Const::PI;
const double	LEG_ANGLE4_MIN		=	-90.0 / 180 * Const::PI;
const double	LEG_ANGLE4_MAX		=	140.0 / 180 * Const::PI;
*/

/// �r�̓��B�͈� (�P�� : mm)
/// ���a����
const double	REACH_RADIUS_MIN	=	0.0;
const double	REACH_RADIUS_MAX	=	1500.0;
/// ��������
const double	REACH_HEIGHT_MIN	=	-2000.0;
const double	REACH_HEIGHT_MAX	=	2000.0;

/// �r��ɂ�����͂̋��e�͈� (�P�� : kgf)
//SAURUS
const double	FOOT_REACTION_X     =   10.0;
const double	FOOT_REACTION_Y     =   10.0;
const double	FOOT_REACTION_Z     =   10.0;
/*ASURA
const double	FOOT_REACTION_X		= 30.0;
const double	FOOT_REACTION_Y		= 30.0;
const double	FOOT_REACTION_Z		= 30.0;
*/



/**
 *		�N���[���p�����[�^
 */
/*  20200819
/// �֐ߐ�
const int		TRACK_JOINT_NUM		= 3;		//�C��

/// �e�֐߂̉��͈� (�P�� : degree)
const double	TRACK_ANGLE1_MIN	=	-180.0;
const double	TRACK_ANGLE1_MAX	=	180.0;
const double	TRACK_ANGLE2_MIN	=	-180.0;
const double	TRACK_ANGLE2_MAX	=	180.0;
const double	TRACK_ANGLE3_MIN	=	-180.0;		//�ǉ�
const double	TRACK_ANGLE3_MAX	=	470.0;		//���K�͉��肨��ё�K�͉��蓮��̂��߂ɕύX

/// �N���[���̍ő呬�i�P�ʁFrad/s�j
const double	TRACK_SPEED_MAX		= 1.0; //1��6s���炢
*/

/**
 *		���{�b�g�p�����[�^
 */
/// �r�̐�
const int		LEG_NUM				= 6;//�ύX
const int		TRACK_NUM			= 4;   // 20200819
const int		TOTAL_JOINT_NUM		= 12;

/// �S�A�N�`���G�[�^��
const int		TOTAL_ACT_NUM			= 16;

/// �r�̕t�����̋���
const double	BODY_LENGTH         = 180.2;//350;//300.0;
const double	BODY_WIDTH			= 165;//350;ASURA//240.0;
const double	BODY_HEIGHT			= 156;//69.0;ASURA_20231128

const double	WIDTH_CLAWLER		=441.0;
const double	WIDTH_CL_FOOT		=491.0;

//const double	LEG_ROOT_POSX[]		= {BODY_LENGTH/2,		-BODY_LENGTH/2,	-BODY_LENGTH/2,	BODY_LENGTH/2};
//const double	LEG_ROOT_POSY[]		= {BODY_WIDTH/2,		BODY_WIDTH/2,		-BODY_WIDTH/2,		-BODY_WIDTH/2};
//const double	LEG_ROOT_POSZ[]		= {0,							0,							0,							0};
//const double	LEG_ROOT_ANGLE[]	= {Const::PI/2,			Const::PI/2,				-Const::PI/2,			-Const::PI/2};
const double	LEG_ROOT_POSX[]		= {BODY_LENGTH/Const::ROOT_2/2,	0.0,			-BODY_LENGTH/Const::ROOT_2/2,	-BODY_LENGTH/Const::ROOT_2/2,	0.0,				BODY_LENGTH/Const::ROOT_2/2};
const double	LEG_ROOT_POSY[]		= {BODY_WIDTH/Const::ROOT_2/2,	BODY_WIDTH/2,	BODY_WIDTH/Const::ROOT_2/2,		-BODY_WIDTH/Const::ROOT_2/2,	-BODY_WIDTH/2,		-BODY_WIDTH/Const::ROOT_2/2};
const double	LEG_ROOT_POSZ[]		= {0,							0,				0,								0,								0,					0	};
const double	LEG_ROOT_ANGLE[]	= {Const::PI/4,					Const::PI/2,	Const::PI*3/4,					-Const::PI*3/4,					-Const::PI/2,		-Const::PI/4	};

//const double	LEG_INITIAL_ANGLE1[]	= {-Const::PI/2, 0.0, Const::PI};
//const double	LEG_INITIAL_ANGLE2[]	= {Const::PI/2, 0.0, Const::PI};
//const double	LEG_INITIAL_ANGLE3[]	= {-Const::PI/2, 0.0, Const::PI};
//const double	LEG_INITIAL_ANGLE4[]	= {Const::PI/2, 0.0, Const::PI};
const double	LEG_INITIAL_ANGLE1[]	= {0.0, 0.0, 0.0, 0.0};
const double	LEG_INITIAL_ANGLE2[]	= {0.0, 0.0, 0.0, 0.0};
const double	LEG_INITIAL_ANGLE3[]	= {0.0, 0.0, 0.0, 0.0};
const double	LEG_INITIAL_ANGLE4[]	= {0.0, 0.0, 0.0, 0.0};
const double	LEG_INITIAL_ANGLE5[]	= {0.0, 0.0, 0.0, 0.0};
const double	LEG_INITIAL_ANGLE6[]	= {0.0, 0.0, 0.0, 0.0};

///��,��K�͕ϐ�	�ǉ�
const double	STABILITY_MARGIN	=40	;//[mm]
const double	BODY_CLOSE_DISTANCE	=25	;//[mm]
const double	RLP=290;


const double	HG					=150.5;//�d�S����
const double	WHEELRADIUS			=75;//�N���[�����a

const double	GROUSER				=50.0;//�O���[�T�[�ԋ���[mm]
//�v�Z�Ɏg������
const double	HGr=HG-WHEELRADIUS;

const double	GroDis=15;	//�O���[�T�̋���
/**
 *		�񋓎q�̒�`
 */
/**
 *		�^���w�̌���
 */
typedef enum 
{
	NO_KINE_ERROR,						///  0 /// �G���[�Ȃ�
	KINE_ERROR_UNREACHABLE,				///  1 /// �w�肵���ꏊ�ɓ͂��Ȃ�
	KINE_ERROR_SINGULAR,				///  2 /// ���َp��
	KINE_ERROR_ELSE,					///  3 /// ���̑��̃G���[
	KINE_ERROR_REACH_RADIUS_UNDER,		///  4 /// �ŏ����B���a�ȉ�
	KINE_ERROR_REACH_RADIUS_OVER,		///  5 /// �ő哞�B���a�ȏ�
	KINE_ERROR_REACH_HEIGHT_UNDER,		///  6 /// �ŏ����B�Ⴓ�ȉ�
	KINE_ERROR_REACH_HEIGHT_OVER,		///  7 /// �ő哞�B�����ȏ�
	KINE_ERROR_JOINT_UNDER_LIMIT,		///  8 /// �֐ߊp���͈͈ȉ�
	KINE_ERROR_JOINT_OVER_LIMIT,		///  9 /// �֐ߊp���͈͈ȏ�
	KINE_ERROR_JOINT_VELOCITY_LIMIT,	/// 10 /// �֐ߑ��x�������ȏ�
	KINE_ERROR_JOINT_TORQUE_LIMIT,		/// 11 /// �֐߃g���N�������ȏ�
} Kinematics;

/**
 *		�r�̋쓮����
 */
typedef enum 
{
	LEG,
	TRACK,
} DriveSystem;

/**
 *		�r�̑����
 */
typedef enum 
{
	SUPPORT,
	SWING
} LegPhase;

/**
 *		���{�b�g�̈ړ��`��
 */
typedef enum
{
	LEGGED,
	TRACKED,
	HYBRID,
} LocomotionStyle;

}


#endif /// __AsuraParameter_h__
