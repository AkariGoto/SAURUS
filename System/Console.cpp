/**
 *  ファイル名
 *		Console.cpp
 *  説明
 *		コンソールに出力するためのクラス
 *  日付
 *		作成日: 2007/05/12(SAT)		更新日: 2007/05/19(SAT)
 */

/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
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

#include "..\Utility\Asc2Int.h"		/// 文字列→数値変換ルーチン
#include "Console.h"

/**
 *	----------------------------------------------------------------------
 *		PlanDataクラス
 *	----------------------------------------------------------------------
 */

/**
 *	------------------------------------------------------------
 *		staticメンバ変数の初期化
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
 *		PlanDataクラスのメンバ関数定義
 *	------------------------------------------------------------
 */
/**
 *		コンストラクタとデストラクタ
 */
Console::Console()
{
}

Console::~Console()
{
}

/**
 *	説明
 *		デバッグ出力オプションの初期化
 */
void Console::initialize(LPCWSTR sectionString)
{
	TCHAR	appNameString[ENTRYBUFFSIZE];
	TCHAR	keyNameString[ENTRYBUFFSIZE];
	TCHAR	iniFilePathString[MAX_PATH+1];

	/// クリティカルセクションを初期化する
	InitializeCriticalSection(&loggingCS);

	/// セクション指定がない
	if (sectionString == NULL)
	{
		if( isDeleteOnStart == TRUE)
		{
			_unlink((char*)logFileName);			/// ログファイルを削除する
		}
		return;
	}

	lstrcpy(sectionName, sectionString);		/// セクション名を保存

	/// デバッグモードを初期化する
	isDebugMode		= FALSE;
	debugLevel		= 0L;
	*logFileName	= '\0';

	if (logFile != NULL)
	{
		fclose(logFile);
	}
	logFile = NULL;

	/// iniファイル名を生成する
	*iniFilePathString = '\0';
	GetCurrentDirectory(sizeof(iniFilePathString), iniFilePathString);
	lstrcat(iniFilePathString, INIDIRNAME);
	lstrcat(iniFilePathString, INIFILENAME);
	
	/// ログファイルを消してから実行する
	lstrcpy( keyNameString, TEXT("isDeleteOnStart") );

	/// iniファイルの指定されたセクション内にある, 指定されたキーに関連付けられている文字列を取得
	GetPrivateProfileString(sectionString, keyNameString, YESNO[FALSE], appNameString, sizeof(appNameString), iniFilePathString);
	
	if ( lstrcmp(appNameString, YESNO[TRUE]) == 0 )
	{
		isDeleteOnStart = TRUE;
	}

	/// デバッグモード
	lstrcpy( keyNameString, TEXT("DebugMode") );
	GetPrivateProfileString(sectionString, keyNameString, YESNO[FALSE], appNameString, sizeof(appNameString), iniFilePathString);
	
	if ( lstrcmp(appNameString, YESNO[TRUE]) == 0 )
	{
		isDebugMode = TRUE;
	}

	/// デバッグコンソール表示モード
	lstrcpy( keyNameString, TEXT("ConsoleMode") );
	GetPrivateProfileString(sectionString, keyNameString, YESNO[FALSE], appNameString, sizeof(appNameString), iniFilePathString);
	
	if ( (lstrcmp(appNameString, YESNO[TRUE]) == 0) && (isDebugMode == TRUE) )
	{
		Console::createConsole();
	}

	/// デバッグレベル
	lstrcpy( keyNameString, TEXT("DebugLevel") );
	
	GetPrivateProfileString(sectionString, keyNameString, TEXT("0"), appNameString, sizeof(appNameString), iniFilePathString);
	debugLevel = (DWORD)ascToInt( (PSTR)appNameString );		/// asc2int.c を使う

	/// デバッグログファイル名（フルパス名）
	lstrcpy( keyNameString, TEXT("LogFile") );
	GetPrivateProfileString(sectionString, keyNameString, TEXT(""), appNameString, sizeof(appNameString), iniFilePathString);
	
	GetCurrentDirectory(sizeof(logFileName), logFileName);
	lstrcat(logFileName, INIDIRNAME);
	lstrcat(logFileName, appNameString);

	if (isDeleteOnStart == TRUE)
	{
		DeleteFile(logFileName);				/// ログファイルを削除する
	}


	logFile = Console::openLogFile();				/// ログファイルをオープンする

	return;
}

/**
 *	説明
 *		コンソールの作成
 */
BOOL Console::createConsole(void)
{
	FILE* filePointer;
	TCHAR	titleString[MAX_PATH+1];

	if ( (GetVersion() & 0x80000000) && ( (GetVersion() & 0xFF) == 3) )
	{
		return FALSE;		/// Win32s はコンソールAPIを利用できない
	}

	FreeConsole();			/// コンソールが存在すればデタッチする
	AllocConsole();			/// コンソールをアタッチする

	/// コンソールのタイトルを設定する
	wsprintf(titleString, TEXT("%s - %s"), CONSOLENAME, sectionName);
	SetConsoleTitle(titleString);

	/// stdout を得る
	if ( ( filePointer = getConsoleFileHandle(STD_OUTPUT_HANDLE) ) == NULL)
	{
		/// 失敗判定
		FreeConsole();
		return FALSE;
	}
	*stdout = *filePointer;

	/// stderr を得る
	if ( ( filePointer = getConsoleFileHandle(STD_ERROR_HANDLE) ) == NULL )
	{
		/// 失敗判定
		FreeConsole();
		return FALSE;
	}
	*stderr = *filePointer;

	return TRUE;
}

/**
 *	説明
 *		デバッグレベルのチェック
 */
BOOL Console::checkLevel(DWORD level)
{
	if ( debugLevel == 0L )
	{
		return TRUE;			/// レベル設定がない
	}

	if( !(debugLevel & level) )
	{
		return FALSE;
	}

	return TRUE;
}

/**
 *	説明
 *		デバッグログファイルの作成
 */
FILE* Console::openLogFile(void)
{
	/// 既にオープンされていたら
	if ( logFile != NULL )
		return logFile;

	/// ファイルの指定がなかったら
	if ( lstrlen(logFileName) <= 0 )
		return NULL;

	/// 指定ファイルを開く
	return fopen( (char*)logFileName, "a");
}

/**
 *	説明
 *		結果をコンソールに出力
 */
void Console::print(PSTR msgString)
{
	/// コンソールに出力
	fputs(msgString, stderr);

	/// ファイルに出力
	if( logFile != NULL )
	{
		fputs(msgString, logFile);
		fflush(logFile);
	}

	return;
}

/**
 *	説明
 *		詳細結果をコンソールに出力
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

	/// レベルが不正
	if ( !checkLevel(level) )
	{
		return;
	}

	/// 待ち開始
	EnterCriticalSection(&loggingCS);

	plpParam = ( (PSTR FAR *)&fmtStrPointer ) + 1;
	
	wsprintf( (LPWSTR)formatBuff, TEXT("%08lX:%s"), GetCurrentThreadId(), fmtStrPointer );
	vsprintf( lineBuff, formatBuff, (PSTR)plpParam );
	
	Console::print(lineBuff);

	/// 待ち終了
	LeaveCriticalSection(&loggingCS);

	return;
}

/**
 *	説明
 *		デバッグダンプ出力
 *	引数
 *		DWORD  level		... 出力レベル
 *		PSTR  titleStrPtr	... タイトルを指すポインタ
 *		LPBYTE buffBytePtr	... バッファを指すポインタ
 *		DWORD  buffSize		... バッファサイズ	
 */
void Console::dump(DWORD level, PSTR titleStrPtr, LPBYTE buffBytePtr, DWORD buffSize)
{
	DWORD	lineCount   = 0L;			/// 1行の出力バイト数
	DWORD	buffCount   = 0L;			/// バッファ全体の出力バイト数
	DWORD	address     = 0L;
	DWORD	bytePtrLine = 16L;		/// 1行に出力するバイト数 */
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

	/// レベルが不正
	if ( !checkLevel(level) )
	{
		return;
	}

	if (buffSize > 0x00fffff0L)
	{
		buffSize = 0x00fffff0L;		/* 最大表示桁 * 16 [bytes] */
	}

	/* 待ち開始 */
	EnterCriticalSection(&loggingCS);

	Console::output(level, " << %s >> ( %ld bytes )\n", titleStrPtr, buffSize);
	Console::output(level,	"ｱﾄﾞﾚｽ 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n");
	Console::output(level,	"-----+-----------------------------------------------\n");

	for (buffCount=0L; buffCount<=(buffSize - 1L); )
	{
		Console::output(level, "%05X ", buffCount);
		tempBytePtr = buffBytePtr;

		/// 16進ダンプ出力
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

		/// キャラクタダンプ出力
		for (lineCount=0L; lineCount <= (bytePtrLine-1L); lineCount++)
		{
			if ( (*tempBytePtr >= 0x20) && (*tempBytePtr < 0x80) )
			{
				wsprintf(lineBuff, TEXT("%c"), *tempBytePtr);
				Console::print( (PSTR)lineBuff );				/// 1文字出力
			}
			else
			{
				Console::print(".");						/// 表示できないキャラクタ
			}

			tempBytePtr++;
		}

		Console::print("\n");
	}
	Console::output(level, "-----------------------------------------------------\n");

	/// 待ち終了
	LeaveCriticalSection(&loggingCS);
}

/**
 *	説明
 *		コンソールの破棄
 */
void Console::destroyConsole(void)
{
	/// デバッグしてなかったら
	if ( !isDebugMode )
	{
		return;
	}

	/// ログファイル開いていたら
	if ( logFile != NULL )
	{
		fclose(logFile);
		logFile = NULL;
	}

	fclose(stdout);
	fclose(stderr);
	
	/// コンソールデタッチ
	FreeConsole();

	return;
}

/**
 *	------------------------------------------------------------
 *		Consoleクラスのprivateなメンバ関数
 *	------------------------------------------------------------
 */
/**
 *	説明
 *		コンソールの属性を設定
 */
BOOL Console::setConsoleAttribute(HANDLE consoleHandle)
{
	CONSOLE_CURSOR_INFO			cci;		/// コンソールカーソルに関する構造体
	CONSOLE_SCREEN_BUFFER_INFO	csbi;		/// コンソールスクリーンバッファに関する構造体
	COORD						coordScreen = { 0, 0 };			/// キャラクターセルの座標
	DWORD						consoleSize, charsWritten;		/// コンソールのサイズとか
	
	/// コンソールの表示色設定
	WORD attribute = BACKGROUND_BLACK | FOREGROUND_CUSTOM;

	cci.dwSize   = 100;
	cci.bVisible = FALSE;

	/// コンソールスクリーンバッファのカーソルのサイズと可視性を設定
	SetConsoleCursorInfo(consoleHandle, &cci);

	/// コンソールスクリーンバッファに関する情報を取得
	GetConsoleScreenBufferInfo(consoleHandle, &csbi);
	
	consoleSize = csbi.dwSize.X * csbi.dwSize.Y;
	
	/// スクリーンバッファの指定した座標から指定した数の文字セル分だけ, テキストと背景色を設定
	FillConsoleOutputAttribute(consoleHandle, attribute, consoleSize, coordScreen, &charsWritten);
	
	/// 字の前景（テキスト）色属性と背景色属性を設定
	SetConsoleTextAttribute(consoleHandle, attribute);

	return TRUE;
}

/**
 *	説明
 *		コンソールの標準ファイルハンドルを得る
 */
FILE* Console::getConsoleFileHandle(DWORD dwDevice)
{
	HANDLE	hStdDevice;
	int 	hCrt;
	FILE*	fpConsole = NULL;

	if ((hStdDevice = GetStdHandle(dwDevice) ) != INVALID_HANDLE_VALUE)
	{
		/// コンソールハンドルからファイルハンドルを得る
		hCrt      = _open_osfhandle((intptr_t)hStdDevice, _O_TEXT);
		fpConsole = _fdopen(hCrt, "w");

		setvbuf(fpConsole, NULL, _IONBF, 0);

		/// コンソールの属性を設定する
		setConsoleAttribute(hStdDevice);
	}

	return fpConsole;
}