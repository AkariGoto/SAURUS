/**
 *  �t�@�C����
 *		WinUserMessage.h
 *  ����
 *		���[�U��`��Windows���b�Z�[�W�j
 *  ���t
 *		�쐬��: 2007/04/01(SAT)		�X�V��: 2008/07/26(SAT)
 */

//  20200820  URG�EAxisControl�_�C�A���O�֘A�R�����g�A�E�g

/**
 *		�ʐM�֌W
 */
/// �V���A���ʐM
#define	WM_COMM_BREAK_DETECTED			WM_APP+101		/// ���͂ɂ����Ē��f�_�����m�ł������b�Z�[�W
#define	WM_COMM_CTS_DETECTED			WM_APP+102		/// CTS (clear-to-send) �M���ɂ���ԕω��̃��b�Z�[�W 
#define	WM_COMM_DSR_DETECTED			WM_APP+103		/// DSR (data-set-ready) �M���ɂ���ԕω��̃��b�Z�[�W 
#define	WM_COMM_ERR_DETECTED			WM_APP+104		/// LI (Line-status) �G���[. LI�G���[��CE_FRAME, CE_OVERRUN, CE_RXPARITY. 
#define	WM_COMM_RING_DETECTED			WM_APP+105		/// RI (ring indicator) �����m���ꂽ���b�Z�[�W. 
#define	WM_COMM_RLSD_DETECTED			WM_APP+106		/// RLSD (receive-line-signal-detect) �M���ɂ���ԕω��̃��b�Z�[�W. 
#define	WM_COMM_RXCHAR					WM_APP+107		/// ���� (character) �����̓o�b�t�@�Ɏ�M���ꂽ 
#define	WM_COMM_RXFLAG_DETECTED			WM_APP+108		/// �C�x���g (character) �����̓o�b�t�@�Ɏ�M���ꂽ  
#define	WM_COMM_TXEMPTY_DETECTED		WM_APP+109		/// �o�̓o�b�t�@�̍Ō�̕���������ꂽ.


/**
 *		�X���b�h�ւ̃��b�Z�[�W
 */
/// �h�L�������g�E�r���[�N���X����MotionPlan�X���b�h�ւ̃��b�Z�[�W
#define	WM_PLAN_END_THREAD				WM_APP+110		/// �X���b�h�I��
#define	WM_PLAN_REQUEST_VIEW_DATA		WM_APP+111		/// View�f�[�^����̃f�[�^�v��
#define	WM_PLAN_SETUP_MOTION			WM_APP+112		/// Doc�N���X����̓��쏀��
#define	WM_PLAN_START_MOTION			WM_APP+113		/// View�N���X����̓���J�n
#define	WM_PLAN_STOP_MOTION				WM_APP+114		/// View�N���X����̓����~
#define	WM_PLAN_GENERATE_MOTION			WM_APP+115		/// �^�C�}�R�[���o�b�N�N���X����̓��쐶��
#define	WM_PLAN_COMM_START				WM_APP+116		/// �ʐM�J�n
#define	WM_PLAN_COMM_STOP				WM_APP+117		/// �ʐM��~

/// AxisControl�_�C�A���O����MotionPlan�X���b�h�ւ̃��b�Z�[�W
/*  20200820
#define	WM_PLAN_AC_LEG_NO				WM_APP+118		/// �r�ԍ��̒ʒm
#define	WM_PLAN_AC_CTRL_MODE			WM_APP+119		/// ���䃂�[�h�̒ʒm
#define	WM_PLAN_AC_SENDING_SLDR_DATA	WM_APP+120		/// �X���C�_�f�[�^�̑��M
#define	WM_PLAN_AC_REQUEST_DATA			WM_APP+121		/// �f�[�^��v��
*/

/*
 *		�X���b�h����̃��b�Z�[�W
 */
/// MotionPlan�X���b�h����̃��b�Z�[�W
#define	WM_PLAN_SENDING_VIEW_DATA		WM_APP+122		/// View�N���X�ւ̃f�[�^���M
//#define	WM_PLAN_SENDING_AC_DATA			WM_APP+123		/// AxisControl�_�C�A���O�ւ̃f�[�^���M  20200820
#define	WM_PLAN_SENDING_COMM_DATA		WM_APP+124		/// �V���A���ʐM�_�C�A���O�փf�[�^���M

/**
 *		�_�C�A���O�ւ̃��b�Z�[�W
 */
/// �V���A���ʐM�_�C�A���O
/// �r���[�N���X����̃��b�Z�[�W
#define		WM_DLG_COMM_START_LISTEN	WM_APP+125		/// �V���A���ʐM�J�n
#define		WM_DLG_COMM_STOP_LISTEN		WM_APP+126		/// �V���A���ʐM��~
#define		WM_DLG_COMM_DISPLAY			WM_APP+127		/// �\��/��\���̃��b�Z�[�W
#define		WM_DLG_COMM_START_LOGGING	WM_APP+128		/// ���O�J�n�̃��b�Z�[�W
#define		WM_DLG_COMM_STOP_LOGGING	WM_APP+129		/// ���O��~�̃��b�Z�[�W
#define		WM_DLG_COMM_UPDATE			WM_APP+130		/// ��ʍX�V�v���̃��b�Z�[�W
#define		WM_DLG_UDPTHREAD_END		WM_APP+131		///	AsuraUDPThread�̔j�����b�Z�[�W

/// AxisControl�_�C�A���O�ւ̃��b�Z�[�W
//#define		WM_DLG_AC_DISPLAY			WM_APP+131		/// �\��/��\���̃��b�Z�[�W  20200820


/// �h�L�������g�E�r���[�N���X����URG�X���b�h�ւ̃��b�Z�[�W
/*  20200820
#define	WM_URG_END_THREAD				WM_APP+132		/// �X���b�h�I��
#define	WM_URG_SETUP_MOTION				WM_APP+133		/// Doc�N���X����̓��쏀��
#define	WM_URG_START_MOTION				WM_APP+134		/// View�N���X����̓���J�n
#define	WM_URG_STOP_MOTION				WM_APP+135		/// View�N���X����̓����~
#define	WM_URG_GENERATE_MOTION			WM_APP+136		/// �^�C�}�R�[���o�b�N�N���X����̓��쐶��
*/

/// �h�L�������g�E�r���[�N���X����MotionPlan�X���b�h�ւ̃��b�Z�[�W
#define	WM_PLAN_URGTHREAD_SET			WM_APP+137		/// URGTHREAD�̃A�h���X�Z�b�g

/// �h�L�������g�E�r���[�N���X����DataSrting�X���b�h�ւ̃��b�Z�[�W
#define	WM_DATASRTING_END_THREAD		WM_APP+138		/// �X���b�h�I��
#define	WM_DATASRTING_SETUP_MOTION		WM_APP+139		/// Doc�N���X����̓��쏀��
#define	WM_DATASRTING_START_MOTION		WM_APP+140		/// View�N���X����̓���J�n
#define	WM_DATASRTING_STOP_MOTION		WM_APP+141		/// View�N���X����̓����~
#define	WM_DATASRTING_GENERATE_MOTION	WM_APP+142		/// URG�X���b�h����̓��쐶��


/*  20200820
/// �h�L�������g�E�r���[�N���X����URG�X���b�h�ւ̃��b�Z�[�W
#define	WM_URG_DATASRTINGTHREAD_SET		WM_APP+143		/// DataSrtingTHREAD�̃A�h���X�Z�b�g

/// �h�L�������g�E�r���[�N���X����URGDelineate�X���b�h�ւ̃��b�Z�[�W
#define	WM_URGDELINEATE_END_THREAD		WM_APP+144		/// �X���b�h�I��
#define	WM_URGDELINEATE_SETUP_MOTION	WM_APP+145		/// Doc�N���X����̓��쏀��
#define	WM_URGDELINEATE_START_MOTION	WM_APP+146		/// View�N���X����̓���J�n
#define	WM_URGDELINEATE_STOP_MOTION		WM_APP+147		/// View�N���X����̓����~
#define	WM_URGDELINEATE_GENERATE_MOTION	WM_APP+148		/// URG�X���b�h����̓��쐶��

/// �h�L�������g�E�r���[�N���X����URG�X���b�h�ւ̃��b�Z�[�W
#define	WM_URG_URGDELINEATETHREAD_SET	WM_APP+149		/// URGDelineateTHREAD�̃A�h���X�Z�b�g

/// �h�L�������g�E�r���[�N���X����URG_graphic�ւ̃��b�Z�[�W
#define	WM_URG_URG_GRAPHIC				WM_APP+150		/// URG_graphic�̃��b�Z�[�W
*/