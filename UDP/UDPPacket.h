/**
 *  �t�@�C����
 *		SCIPacket.h
 *  ����
 *		�V���A���ʐM�p�̃p�P�b�g��`
 *  ���t
 *		�쐬��: 2007/11/17(Sat)		�X�V��: 2007/11/17(Sat)
 */

#ifndef __UDPPacket_h__
#define __UDPPacket_h__

namespace UDP//	1	IntelliSense: PCH... �ǂ��ɂ�include����ĂȂ��ƂȂ�

{
/**
 *		//////////////////////////////////////////////////
 *				�p�P�b�g�t�H�[�}�b�g�ɂ���
 *		//////////////////////////////////////////////////
 */

/**
 *		�V���[�g�t�H�[�}�b�g
 *		�P�֐߃f�[�^�𑗂�
 *		�f�[�^�F9 Byte
 *		| �w�b�_1 | �w�b�_2 | �p�P�b�g�T�C�Y | �w�ߊ֐ߔԍ� | �w�߃^�C�v | �w�b�_�`�F�b�N�T�� | �֐ߎw�ߒl��ʃo�C�g | �֐ߎw�ߒl���ʃo�C�g | �`�F�b�N�T�� |
 *
 */

/**
 *		�����O�t�H�[�}�b�g
 *		�S�֐߃f�[�^����ɑ���
 *		�f�[�^�F32 Byte
 *		| �w�b�_1 | �w�b�_2 | �p�P�b�g�T�C�Y | �w�ߊ֐ߔԍ� | �w�߃^�C�v | �w�b�_�`�F�b�N�T�� |
 *
 *		| ��1�֐ߎw�ߒl��ʃo�C�g | ��1�֐ߎw�ߒl���ʃo�C�g | ��2�֐ߏ�ʃo�C�g | �E�E�E | ��12�֐߉��ʃo�C�g | 
 *		| �`�F�b�N�T��1 | �`�F�b�N�T��2 |
 *
 *		--------------------------------------------------------------------------------------------------------------------------------------------
 *		�w�b�_1�F1111 1111�i0xFF�j
 *		�w�b�_2�F1111 1111�i0xFF�j
 *		�p�P�b�g�T�C�Y�F0001 1110�i0x1E�j
 *		�֐ߔԍ��F1�`12�A100�i�S�֐߁j�A101�i��1�r�m1�`3�n�j�A102�i��2�r�m4�`6�n�j�A101�i��1�r�m7�`9�n�j�A104�i��1�r�m10�`12�n�j
 *		�w�߃^�C�v�F�p�x�w�߁A���x�w�߁A�`�A���s�w�߁B�N���[���w�߂Ȃ�1�o�C�g
 *		�w�b�_�`�F�b�N�T��:��L�o�C�g�̍��v�̉���8�r�b�g
 *		--------------------------------------------------------------------------------------------------------------------------------------------
 *		�w�ߒl�F	�֐ߊp�x�i��֐ߊp�xaaa.bb[deg]��5���̐����ŕ\���B�܂�aaabb��2�o�C�g�𑗂�B�j
 *			�@		�֐ߑ��x
 *		--------------------------------------------------------------------------------------------------------------------------------------------
 *		�`�F�b�N�T��1�F�S�w�ߒl�f�[�^�̍��v�̏�ʃo�C�g
 *		�`�F�b�N�T��2�F�S�w�ߒl�f�[�^�̍��v�̉��ʃo�C�g
 *		--------------------------------------------------------------------------------------------------------------------------------------------
 */

/**
 *		�w�ߒl����
 */
const int SCI_MAX_ACT_NUM		= 14;		/// �r�̑��A�N�`���G�[�^��
const int SCI_MAX_JOINT_NUM		= 12;		/// �r�̑��֐ߐ�
const int SCI_MAX_ANGLE			= 360;		/// �ő�֐ߊp�x
const int SCI_MIN_ANGLE			= 0;		/// �ŏ��֐ߊp�x
const int SCI_MAX_SPEED			= 360;		/// �ő�N���[�����x
const int SCI_ALL_JOINTS		= 0;

const int UDP_MAX_ACT_NUM		= 24;//���ǉ� �r�̑��֐ߐ�

/**
 *		�p�P�b�g�Ɋւ��邱��
 */
enum class SciPacketType :short{
	SHORT,
	LONG,
	DEBUGGING
};

/**
 *		�p�P�b�g�T�C�Y
 */
const int SCI_PACKET_SIZE_SHORT			= 0x07;		/// �p�P�b�g�T�C�Y
const int SCI_PACKET_SIZE_LONG			= 0xff;		/// �p�P�b�g�T�C�Y
const int SCI_PACKET_SIZE_DEBUG			= 0x0A;		/// �p�P�b�g�T�C�Y
const int SCI_PACKET_HEADER_SIZE		= 0x06;		/// �p�P�b�g�w�b�_�T�C�Y
const int SCI_PACKET_START_BYTE_SIZE	= 0x02;		/// �p�P�b�g�X�^�[�g�o�C�g�̃T�C�Y
const int SCI_PACKET_SIZE_ONELEG		= 0x20;     ///�p�r��������Ȃ��p�P�b�g�T�C�Y�i210513�ЎR�j

const int SCI_PACKET_HEADER		= 0xFF;		/// �p�P�b�g�w�b�_�̃X�^�[�g
const int SCI_PACKET_VALID		= 0x80;		/// �p�P�b�g�f�[�^�����̗L���l

/**
 *		�֐ߔԍ�
 */
const int PACKET_LEG1_WAIST			= 1;
const int PACKET_LEG1_HIP			= 2;
const int PACKET_LEG1_KNEE			= 3;
const int PACKET_LEG2_WAIST			= 4;
const int PACKET_LEG2_HIP			= 5;
const int PACKET_LEG2_KNEE			= 6;
const int PACKET_LEG3_WAIST			= 7;
const int PACKET_LEG3_HIP			= 8;
const int PACKET_LEG3_KNEE			= 9;
const int PACKET_LEG4_WAIST			= 10;
const int PACKET_LEG4_HIP			= 11;
const int PACKET_LEG4_KNEE			= 12;

const int PACKET_LEG_ALL_JOINTS		= 100;
const int PACKET_LEG1_JOINTS		= 101;
const int PACKET_LEG2_JOINTS		= 102;
const int PACKET_LEG3_JOINTS		= 103;
const int PACKET_LEG4_JOINTS		= 104;

/**
 *		�R�}���h�̎��
 *		SH2�����̃v���O�����Ɓi���l�́j���ʉ����邱��
 */
const int PACKET_CMD_JNT_POS	= 0xA0;		/// �p�x�w��
const int PACKET_CMD_JNT_SPD	= 0xA1;		/// ���x�w��
const int PACKET_CMD_JNT_TRQ	= 0xA2;		/// �g���N�w�� 
const int PACKET_CMD_JNT_PGAIN	= 0xA3;		/// �o�Q�C���ύX
const int PACKET_CMD_JNT_DGAIN	= 0xA4;		/// D�Q�C���ύX
const int PACKET_CMD_JNT_IGAIN	= 0xA5;		/// I�Q�C���ύX
const int PACKET_CMD_MODE		= 0xA6;		/// ���[�h�ύX
const int PAKCET_CMD_WALK		= 0xA7;		/// ���s�w�߁i�S�֐ߊp�x�w�߁j
const int PAKCET_CMD_TRACK		= 0xA8;		/// �N���[���w�߁i�p�x�Ƒ��x�w�߂̍����j
const int PACKET_CMD_TRACK2LEG	= 0xA9;		/// �N���[������r�֌`�Ԑؑ�
const int PACKET_CMD_LEG2TRACK	= 0xAA;		/// �r����N���[���֌`�Ԑؑ�


//changeflag�@�r�̎w��
const int FLAG_TRACK	=0x0F;	//0000 1111	�N���[��
const int FLAG_WALK		=0x00;	//0000 0000	���s
const int FLAG_HIBRID	=0x09;	//0000 1001	�n�C�u���b�h

}	/// end of namespace Comm
/******************************************************************************************
* CAN_StdId Table
*	CAN�ʐM�ł�StdId�̏����������D�悳���
*	(�f�[�^�ƃ����[�g�̓f�[�^���D��)
*	0 to 0x7FF(2047)
*******************************************************************************************/
typedef enum controller_CAN_ID {
//*** Gateway_controller *****
	controller_0_0 	=	10 ,//(Gateway_controller)

//*** Leg_1_controller *****
	controller_1_1	=	110,
 	controller_1_2	=	120,

//*** Leg_2_controller *****
	controller_2_1	=	210,//�}�C�R����ID
	controller_2_2	=	220,

//*** Leg_3_controller *****
	controller_3_1	=	310,
	controller_3_2	=	320,

//*** Leg_4_controller *****
	controller_4_1	=	410,
	controller_4_2	=	420,

//*** Leg_5_controller *****
	controller_5_1	=	510,
	controller_5_2	=	520,

//*** Leg_6_controller *****
	controller_6_1	=	610,
	controller_6_2	=	620,

}CAN_ID;

/******************************************************************************************
* CAN_Data_type Table
* 	CAN�ő���f�[�^�̎�� , uint8_t
*
*	com_ballscllew_X_pos	== 	�{�[���˂��ʒu�̎w�ߒl
*	com_joint_X_angle		==	�֐ߊp�x�w�ߒl
*	encoder_X_count			==	�G���R�[�_�̃J�E���g�l
*	potentio_X_val			==	�|�e���V�������[�^�[�l
*
*******************************************************************************************/
typedef enum CAN_send_Data_type{
	//*** Command *****
	com_ballscllew_1_pos	=	10,
	com_ballscllew_2_pos	=	20,

	com_joint_1_angle	=	30,
	com_joint_2_angle	=	40,
	com_joint_3_angle	=	50,
	com_joint_4_angle	=	60,

	//*** Sensor data *****
	encoder_1_count		=	110,
	encoder_2_count		=	120,
	potentio_1_val		=	130,
	potentio_2_val  	=	140

}Data_type;
#endif	/// __SCIPacket_h__