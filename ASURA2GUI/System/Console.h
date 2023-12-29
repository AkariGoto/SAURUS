/**
 *  �t�@�C����
 *		Console.h
 *  ����
 *		�R���\�[���ɏo�͂��邽�߂̃N���X
 *  ���t
 *		�쐬��: 2007/05/12(SAT)		�X�V��: 2007/05/19(SAT)
 */

#ifndef __Console_h__
#define __Console_h__

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include <windows.h>
#include <stdio.h>

/**
 *	----------------------------------------------------------------------
 *		Console�N���X
 *			MonoState�p�^�[���ō쐬�i�R���X�g���N�^�n��static, �����o��private�j
 *			�p�����Ȃ�����
 *			Console.ini�t�@�C�����܂�Setting�f�B���N�g����p��
 *	----------------------------------------------------------------------
 */
class Console
{

/**
 *	------------------------------------------------------------
 *		�����o�萔
 *	------------------------------------------------------------
 */
	static const int	MAX_NAME			= 256;
	static const int	ENTRYBUFFSIZE		= 256;
	static const int	LINEBUFFSIZE		= 256;

	/// �t�@�C����
	static const TCHAR	CONSOLENAME[MAX_PATH+1];
	static const TCHAR	INIFILENAME[MAX_PATH+1];
	static const TCHAR	INIDIRNAME[MAX_PATH+1];
	static const TCHAR*	YESNO[2];

	/// �R���\�[������
	static const WORD	FOREGROUND_BLACK;
	static const WORD	FOREGROUND_CYAN;
	static const WORD	FOREGROUND_WHITE;
	static const WORD	FOREGROUND_CUSTOM;

	static const WORD	BACKGROUND_BLACK;
	static const WORD	BACKGROUND_CYAN;
	static const WORD	BACKGROUND_WHITE;
	static const WORD	BACKGROUND_CUSTOM;

	/// �o�̓��x��
	static const WORD	INFO;
	static const WORD	WARNING;
	static const WORD	ALART;
	static const WORD	EMERGENCY;

	static const WORD	DUMP;
/**
 *	------------------------------------------------------------
 *		�����o�ϐ�
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	���O
 *	----------------------------------------
 */
	/// ���O�t�@�C����
	static TCHAR logFileName[ENTRYBUFFSIZE];
	/// ini�t�@�C���̃Z�N�V������
	static TCHAR sectionName[ENTRYBUFFSIZE];

/**
 *	----------------------------------------
 *	�f�o�b�O�ݒ�
 *	----------------------------------------
 */
	/// �f�o�b�O���O���o�͂��邩
	static bool 	isDebugMode;
	/// ���O�t�@�C�����폜���邩
	static bool	isDeleteOnStart;
	/// �f�o�b�O���O�o�̓��x��
	static DWORD	debugLevel;
	/// ���O�t�@�C���n���h��
	static FILE*	logFile;

	/// �N���e�B�J���Z�N�V����
	/// ���O�o�͒�
	static CRITICAL_SECTION	loggingCS;

/**
 *	------------------------------------------------------------
 *		�����o�֐�
 *	------------------------------------------------------------
 */
public:
/**
 *	����
 *		�f�o�b�O�o�̓I�v�V�����̏�����
 */
	static void initialize(LPCWSTR  lpszSection);

/**
 *	����
 *		�R���\�[���̍쐬
 */
	static BOOL createConsole(void);

/**
 *	����
 *		�f�o�b�O���x���̃`�F�b�N
 */
	static BOOL checkLevel(DWORD level);

/**
 *	����
 *		�f�o�b�O���O�t�@�C���̍쐬
 */
	static FILE*	openLogFile(void);

/**
 *	����
 *		���ʂ��R���\�[���ɏo��
 */
	static void print(LPSTR msgString);

/**
 *	����
 *		�ڍ׌��ʂ��R���\�[���ɏo��
 */
	static void	__cdecl output(DWORD level, PSTR fmtStrPointer, ...);

/**
 *	����
 *		�f�o�b�O�_���v�o��
 *	����
 *		DWORD  level		... �o�̓��x��
 *		PSTR  titleStrPtr	... �^�C�g�����w���|�C���^
 *		LPBYTE buffBytePtr	... �o�b�t�@���w���|�C���^
 *		DWORD  buffSize		... �o�b�t�@�T�C�Y	
 */
	static void dump(DWORD level, PSTR titleStrPtr, LPBYTE buffBytePtr, DWORD buffSize);

/**
 *	����
 *		�R���\�[���̔j��
 */
	static void destroyConsole(void);

private:
/**
 *	����
 *		�R���X�g���N�^�ƃf�X�g���N�^�̉B��
 */
	Console();
	virtual ~Console();

/**
 *	����
 *		�R���\�[���̑�����ݒ�
 */
	static BOOL setConsoleAttribute(HANDLE consoleHandle);

/**
 *	����
 *		�R���\�[���̕W���t�@�C���n���h���𓾂�
 */
	static FILE* getConsoleFileHandle(DWORD dwDevice);

};	/// end of class Console

#endif	/// __Console_h__