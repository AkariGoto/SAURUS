/**
 *  �t�@�C����
 *		Console.cpp
 *  ����
 *		�R���\�[���ɏo�͂��邽�߂̃N���X
 *  ���t
 *		�쐬��: 2007/05/12(SAT)		�X�V��: 2007/05/19(SAT)
 */

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <memory.h>
#include <io.h>
#include <fcntl.h>
#include <ctype.h>

#include "..\Utility\Asc2Int.h"		/// �����񁨐��l�ϊ����[�`��
#include "Console.h"

/**
 *	----------------------------------------------------------------------
 *		PlanData�N���X
 *	----------------------------------------------------------------------
 */

/**
 *	------------------------------------------------------------
 *		static�����o�ϐ��̏�����
 *	------------------------------------------------------------
 */
	const TCHAR	Console::CONSOLENAME[MAX_PATH+1]	= TEXT("Console Debugging");
	const TCHAR	Console::INIFILENAME[MAX_PATH+1]	= TEXT("\\Debug.ini");
	const TCHAR	Console::INIDIRNAME[MAX_PATH+1]		= TEXT("\\Setting");
	const TCHAR*	Console::YESNO[2]				= { TEXT("No"), TEXT("Yes") };

	const WORD	Console::FOREGROUND_BLACK	= (WORD)0x0000;
	const WORD	Console::FOREGROUND_CYAN	= (WORD)0x0003;
	const WORD	Console::FOREGROUND_WHITE	= (WORD)0x0007;
	const WORD	Console::FOREGROUND_CUSTOM	= (WORD)0x0003 | (WORD)0x0008;

	const WORD	Console::BACKGROUND_BLACK	= (WORD)0x0000;
	const WORD	Console::BACKGROUND_CYAN	= (WORD)0x0030;
	const WORD	Console::BACKGROUND_WHITE	= (WORD)0x0070;
	const WORD	Console::BACKGROUND_CUSTOM	= (WORD)0x0030 | (WORD)0x0080;

	const DWORD	INFO						= (DWORD)0x00000001L;
	const DWORD	WARNING						= (DWORD)0x00000002L;
	const DWORD	ALART						= (DWORD)0x00000004L;
	const DWORD	EMERGENCY					= (DWORD)0x00000008L;

	const DWORD DUMP						= (DWORD)0x80000000L;

	bool Console::isDebugMode				= FALSE;
	bool Console::isDeleteOnStart			= FALSE;
	DWORD Console::debugLevel				= 0x0L;
	FILE* Console::logFile					= NULL;

	TCHAR	Console::logFileName[ENTRYBUFFSIZE]	= TEXT("");
	TCHAR	Console::sectionName[ENTRYBUFFSIZE]	= TEXT("");

	CRITICAL_SECTION Console::loggingCS;
/**
 *	------------------------------------------------------------
 *		PlanData�N���X�̃����o�֐���`
 *	------------------------------------------------------------
 */
/**
 *		�R���X�g���N�^�ƃf�X�g���N�^
 */
Console::Console()
{
}

Console::~Console()
{
}

/**
 *	����
 *		�f�o�b�O�o�̓I�v�V�����̏�����
 */
void Console::initialize(LPCWSTR sectionString)
{
	TCHAR	appNameString[ENTRYBUFFSIZE];
	TCHAR	keyNameString[ENTRYBUFFSIZE];
	TCHAR	iniFilePathString[MAX_PATH+1];

	/// �N���e�B�J���Z�N�V����������������
	InitializeCriticalSection(&loggingCS);

	/// �Z�N�V�����w�肪�Ȃ�
	if (sectionString == NULL)
	{
		if( isDeleteOnStart == TRUE)
		{
			_unlink((char*)logFileName);			/// ���O�t�@�C�����폜����
		}
		return;
	}

	lstrcpy(sectionName, sectionString);		/// �Z�N�V��������ۑ�

	/// �f�o�b�O���[�h������������
	isDebugMode		= FALSE;
	debugLevel		= 0L;
	*logFileName	= '\0';

	if (logFile != NULL)
	{
		fclose(logFile);
	}
	logFile = NULL;

	/// ini�t�@�C�����𐶐�����
	*iniFilePathString = '\0';
	GetCurrentDirectory(sizeof(iniFilePathString), iniFilePathString);
	lstrcat(iniFilePathString, INIDIRNAME);
	lstrcat(iniFilePathString, INIFILENAME);
	
	/// ���O�t�@�C���������Ă�����s����
	lstrcpy( keyNameString, TEXT("isDeleteOnStart") );

	/// ini�t�@�C���̎w�肳�ꂽ�Z�N�V�������ɂ���, �w�肳�ꂽ�L�[�Ɋ֘A�t�����Ă��镶������擾
	GetPrivateProfileString(sectionString, keyNameString, YESNO[FALSE], appNameString, sizeof(appNameString), iniFilePathString);
	
	if ( lstrcmp(appNameString, YESNO[TRUE]) == 0 )
	{
		isDeleteOnStart = TRUE;
	}

	/// �f�o�b�O���[�h
	lstrcpy( keyNameString, TEXT("DebugMode") );
	GetPrivateProfileString(sectionString, keyNameString, YESNO[FALSE], appNameString, sizeof(appNameString), iniFilePathString);
	
	if ( lstrcmp(appNameString, YESNO[TRUE]) == 0 )
	{
		isDebugMode = TRUE;
	}

	/// �f�o�b�O�R���\�[���\�����[�h
	lstrcpy( keyNameString, TEXT("ConsoleMode") );
	GetPrivateProfileString(sectionString, keyNameString, YESNO[FALSE], appNameString, sizeof(appNameString), iniFilePathString);
	
	if ( (lstrcmp(appNameString, YESNO[TRUE]) == 0) && (isDebugMode == TRUE) )
	{
		Console::createConsole();
	}

	/// �f�o�b�O���x��
	lstrcpy( keyNameString, TEXT("DebugLevel") );
	
	GetPrivateProfileString(sectionString, keyNameString, TEXT("0"), appNameString, sizeof(appNameString), iniFilePathString);
	debugLevel = (DWORD)ascToInt( (PSTR)appNameString );		/// asc2int.c ���g��

	/// �f�o�b�O���O�t�@�C�����i�t���p�X���j
	lstrcpy( keyNameString, TEXT("LogFile") );
	GetPrivateProfileString(sectionString, keyNameString, TEXT(""), appNameString, sizeof(appNameString), iniFilePathString);
	
	GetCurrentDirectory(sizeof(logFileName), logFileName);
	lstrcat(logFileName, INIDIRNAME);
	lstrcat(logFileName, appNameString);

	if (isDeleteOnStart == TRUE)
	{
		DeleteFile(logFileName);				/// ���O�t�@�C�����폜����
	}


	logFile = Console::openLogFile();				/// ���O�t�@�C�����I�[�v������

	return;
}

/**
 *	����
 *		�R���\�[���̍쐬
 */
BOOL Console::createConsole(void)
{
	FILE* filePointer;
	TCHAR	titleString[MAX_PATH+1];

	if ( (GetVersion() & 0x80000000) && ( (GetVersion() & 0xFF) == 3) )
	{
		return FALSE;		/// Win32s �̓R���\�[��API�𗘗p�ł��Ȃ�
	}

	FreeConsole();			/// �R���\�[�������݂���΃f�^�b�`����
	AllocConsole();			/// �R���\�[�����A�^�b�`����

	/// �R���\�[���̃^�C�g����ݒ肷��
	wsprintf(titleString, TEXT("%s - %s"), CONSOLENAME, sectionName);
	SetConsoleTitle(titleString);

	/// stdout �𓾂�
	if ( ( filePointer = getConsoleFileHandle(STD_OUTPUT_HANDLE) ) == NULL)
	{
		/// ���s����
		FreeConsole();
		return FALSE;
	}
	*stdout = *filePointer;

	/// stderr �𓾂�
	if ( ( filePointer = getConsoleFileHandle(STD_ERROR_HANDLE) ) == NULL )
	{
		/// ���s����
		FreeConsole();
		return FALSE;
	}
	*stderr = *filePointer;

	return TRUE;
}

/**
 *	����
 *		�f�o�b�O���x���̃`�F�b�N
 */
BOOL Console::checkLevel(DWORD level)
{
	if ( debugLevel == 0L )
	{
		return TRUE;			/// ���x���ݒ肪�Ȃ�
	}

	if( !(debugLevel & level) )
	{
		return FALSE;
	}

	return TRUE;
}

/**
 *	����
 *		�f�o�b�O���O�t�@�C���̍쐬
 */
FILE* Console::openLogFile(void)
{
	/// ���ɃI�[�v������Ă�����
	if ( logFile != NULL )
		return logFile;

	/// �t�@�C���̎w�肪�Ȃ�������
	if ( lstrlen(logFileName) <= 0 )
		return NULL;

	/// �w��t�@�C�����J��
	return fopen( (char*)logFileName, "a");
}

/**
 *	����
 *		���ʂ��R���\�[���ɏo��
 */
void Console::print(PSTR msgString)
{
	/// �R���\�[���ɏo��
	fputs(msgString, stderr);

	/// �t�@�C���ɏo��
	if( logFile != NULL )
	{
		fputs(msgString, logFile);
		fflush(logFile);
	}

	return;
}

/**
 *	����
 *		�ڍ׌��ʂ��R���\�[���ɏo��
 */
void __cdecl Console::output(DWORD level, PSTR fmtStrPointer, ...)
{
	LPSTR FAR*	plpParam;
	char formatBuff[LINEBUFFSIZE];
	char lineBuff[LINEBUFFSIZE];

	if (!isDebugMode)
	{
		if (logFile != NULL)
		{
			fclose(logFile);
		}

		logFile = NULL;
		return;
	}

	/// ���x�����s��
	if ( !checkLevel(level) )
	{
		return;
	}

	/// �҂��J�n
	EnterCriticalSection(&loggingCS);

	plpParam = ( (PSTR FAR *)&fmtStrPointer ) + 1;
	
	wsprintf( (LPWSTR)formatBuff, TEXT("%08lX:%s"), GetCurrentThreadId(), fmtStrPointer );
	vsprintf( lineBuff, formatBuff, (PSTR)plpParam );
	
	Console::print(lineBuff);

	/// �҂��I��
	LeaveCriticalSection(&loggingCS);

	return;
}

/**
 *	����
 *		�f�o�b�O�_���v�o��
 *	����
 *		DWORD  level		... �o�̓��x��
 *		PSTR  titleStrPtr	... �^�C�g�����w���|�C���^
 *		LPBYTE buffBytePtr	... �o�b�t�@���w���|�C���^
 *		DWORD  buffSize		... �o�b�t�@�T�C�Y	
 */
void Console::dump(DWORD level, PSTR titleStrPtr, LPBYTE buffBytePtr, DWORD buffSize)
{
	DWORD	lineCount   = 0L;			/// 1�s�̏o�̓o�C�g��
	DWORD	buffCount   = 0L;			/// �o�b�t�@�S�̂̏o�̓o�C�g��
	DWORD	address     = 0L;
	DWORD	bytePtrLine = 16L;		/// 1�s�ɏo�͂���o�C�g�� */
	TCHAR 	lineBuff[LINEBUFFSIZE];
	LPBYTE	tempBytePtr;

	if (isDebugMode)
	{
		if ( logFile != NULL)
		{
			fclose(logFile);
		}
		logFile = NULL;
		return;
	}

	/// ���x�����s��
	if ( !checkLevel(level) )
	{
		return;
	}

	if (buffSize > 0x00fffff0L)
	{
		buffSize = 0x00fffff0L;		/* �ő�\���� * 16 [bytes] */
	}

	/* �҂��J�n */
	EnterCriticalSection(&loggingCS);

	Console::output(level, " << %s >> ( %ld bytes )\n", titleStrPtr, buffSize);
	Console::output(level,	"���ڽ 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n");
	Console::output(level,	"-----+-----------------------------------------------\n");

	for (buffCount=0L; buffCount<=(buffSize - 1L); )
	{
		Console::output(level, "%05X ", buffCount);
		tempBytePtr = buffBytePtr;

		/// 16�i�_���v�o��
		for (lineCount=0L; lineCount<=(bytePtrLine-1L); lineCount++)
		{
			wsprintf(lineBuff, TEXT("%02X "), *buffBytePtr++);
			Console::print( (PSTR)lineBuff );
			
			buffCount++;
			if (buffCount >= buffSize)
			{
				DWORD dwSub;
				for (dwSub=1L; dwSub<=(bytePtrLine-lineCount-1L); dwSub++)
				{
					Console::print("   ");
				}

				bytePtrLine = lineCount + 1;
				break;
			}
		}

		/// �L�����N�^�_���v�o��
		for (lineCount=0L; lineCount <= (bytePtrLine-1L); lineCount++)
		{
			if ( (*tempBytePtr >= 0x20) && (*tempBytePtr < 0x80) )
			{
				wsprintf(lineBuff, TEXT("%c"), *tempBytePtr);
				Console::print( (PSTR)lineBuff );				/// 1�����o��
			}
			else
			{
				Console::print(".");						/// �\���ł��Ȃ��L�����N�^
			}

			tempBytePtr++;
		}

		Console::print("\n");
	}
	Console::output(level, "-----------------------------------------------------\n");

	/// �҂��I��
	LeaveCriticalSection(&loggingCS);
}

/**
 *	����
 *		�R���\�[���̔j��
 */
void Console::destroyConsole(void)
{
	/// �f�o�b�O���ĂȂ�������
	if ( !isDebugMode )
	{
		return;
	}

	/// ���O�t�@�C���J���Ă�����
	if ( logFile != NULL )
	{
		fclose(logFile);
		logFile = NULL;
	}

	fclose(stdout);
	fclose(stderr);
	
	/// �R���\�[���f�^�b�`
	FreeConsole();

	return;
}

/**
 *	------------------------------------------------------------
 *		Console�N���X��private�ȃ����o�֐�
 *	------------------------------------------------------------
 */
/**
 *	����
 *		�R���\�[���̑�����ݒ�
 */
BOOL Console::setConsoleAttribute(HANDLE consoleHandle)
{
	CONSOLE_CURSOR_INFO			cci;		/// �R���\�[���J�[�\���Ɋւ���\����
	CONSOLE_SCREEN_BUFFER_INFO	csbi;		/// �R���\�[���X�N���[���o�b�t�@�Ɋւ���\����
	COORD						coordScreen = { 0, 0 };			/// �L�����N�^�[�Z���̍��W
	DWORD						consoleSize, charsWritten;		/// �R���\�[���̃T�C�Y�Ƃ�
	
	/// �R���\�[���̕\���F�ݒ�
	WORD attribute = BACKGROUND_BLACK | FOREGROUND_CUSTOM;

	cci.dwSize   = 100;
	cci.bVisible = FALSE;

	/// �R���\�[���X�N���[���o�b�t�@�̃J�[�\���̃T�C�Y�Ɖ�����ݒ�
	SetConsoleCursorInfo(consoleHandle, &cci);

	/// �R���\�[���X�N���[���o�b�t�@�Ɋւ�������擾
	GetConsoleScreenBufferInfo(consoleHandle, &csbi);
	
	consoleSize = csbi.dwSize.X * csbi.dwSize.Y;
	
	/// �X�N���[���o�b�t�@�̎w�肵�����W����w�肵�����̕����Z��������, �e�L�X�g�Ɣw�i�F��ݒ�
	FillConsoleOutputAttribute(consoleHandle, attribute, consoleSize, coordScreen, &charsWritten);
	
	/// ���̑O�i�i�e�L�X�g�j�F�����Ɣw�i�F������ݒ�
	SetConsoleTextAttribute(consoleHandle, attribute);

	return TRUE;
}

/**
 *	����
 *		�R���\�[���̕W���t�@�C���n���h���𓾂�
 */
FILE* Console::getConsoleFileHandle(DWORD dwDevice)
{
	HANDLE	hStdDevice;
	int 	hCrt;
	FILE*	fpConsole = NULL;

	if ((hStdDevice = GetStdHandle(dwDevice) ) != INVALID_HANDLE_VALUE)
	{
		/// �R���\�[���n���h������t�@�C���n���h���𓾂�
		hCrt      = _open_osfhandle((intptr_t)hStdDevice, _O_TEXT);
		fpConsole = _fdopen(hCrt, "w");

		setvbuf(fpConsole, NULL, _IONBF, 0);

		/// �R���\�[���̑�����ݒ肷��
		setConsoleAttribute(hStdDevice);
	}

	return fpConsole;
}