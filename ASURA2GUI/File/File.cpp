/**
 *  ファイル名
 *		File.cpp
 *  説明
 *		ファイルを操作するクラス
 *		 fopen, fwrite, freadなどCRTのラッパークラス
 *  日付
 *		作成日: 2007/09/04(Tue)		更新日: 2007/09/04(Tue)
 */

/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */

#include "File.h"
#include <tchar.h>

namespace System
{

/**
 *	------------------------------------------------------------
 *		Fileクラスのメンバ関数定義
 *	------------------------------------------------------------
 */
/**
 *	コンストラクタとデストラクタ
 */
/// コンストラクタ
File::File()
{
	pFile = NULL;
}

/// コンストラクタ
File::File(const TCHAR* fileName, FileMode mode)
{
	openFile(fileName, mode);
}

/// デストラクタ
File::~File()
{
	closeFile();
}

/**
 *	説明
 *		指定したパスのファイルを開く
 *	引数：
 *		fileName: ファイル名
 *		mode: ファイルモード
 *		isTimeStamped: 時刻フォルダを作成するか
 */
int File::openFile(const TCHAR* fileName, FileMode mode, bool isTimeStamped)
{

	/// 作成するパス
	TCHAR path[256];
	errno_t err;


	/// ファイルモードのチェック
	if ( mode != READ_ONLY &&
		 mode != WRITE_ONLY &&
		 mode != APPEND_ONLY &&
		 mode != READ_WRITE &&
		 mode != READ_APPEND )
		 return ERR_INVALID_OPEN_MODE;

	/// フォルダ付きのパス名作成
	if (isTimeStamped)
	{
		createTimeStampedFolder();
		wsprintf(path, TEXT(".\\%s\\%s"), folderName, fileName);
	}
	else	/// それ以外だったらファイル名がそのままパス名
	{
		lstrcpy(path, fileName);
	}


	/// Win32APIのしばりでバイナリ形式のみ（テキストも問題なし）
	switch (mode)
	{
		case READ_ONLY:	/// バイナリモードでの読込み専用
			err = _tfopen_s(&pFile, path, TEXT("rb"));
			break;

		case WRITE_ONLY:	/// バイナリモードでの書き込み専用
			err = _tfopen_s(&pFile, path, TEXT("wb"));
			break;

		case APPEND_ONLY:	/// バイナリモードでの追加書込み
			err = _tfopen_s(&pFile, path, TEXT("ab"));
			break;

		case READ_WRITE:	/// バイナリモードでの読み込みと書き込み
			err = _tfopen_s(&pFile, path, TEXT("r+b"));
			break;

		case WRITE_READ:	/// バイナリモードでの書き込みと読み込み
			err = _tfopen_s(&pFile, path, TEXT("w+b"));
			break;

		case READ_APPEND:	/// バイナリモードでの読み込みと追加書込み
			err = _tfopen_s(&pFile, path, TEXT("a+b"));
			break;
		
		default:
			break;
	}

	/// ::CreateFile()の成功判定
	if (err == 0)
	{
		fileMode = mode;
		return NO_FILE_ERROR;
	}
	else
	{
		/// 失敗したら
		::MessageBox(0, TEXT("[File::openFile] Failed"), TEXT(""), 0);
	}

	/// ファイルがなかったら
	if (fileMode == READ_ONLY)
		return ERR_FILE_NOT_FOUND;

	return ERR_FAIL_CREATE_FILE;
}

/**
 *	説明
 *		ファイルを閉じる
 */
int File::closeFile(void)
{
	/// ファイルが開かれていなかったら
	if (!pFile)
		return ERR_NO_OPEN;

	/// ファイルを閉じる
	if (fclose( pFile ) == 0)
	{
		pFile = 0;
		return NO_FILE_ERROR;
	}

	return ERR_FAIL_CLOSE;
}

/**
 *	説明
 *		ファイルが開かれているかどうか
 */
bool File::isOpen(void) const
{
	return pFile != NULL;
}

/**
 *	説明
 *		ファイルポインタの位置を設定
 */
long File::getPosition(void) const
{
	if (pFile == NULL)
		return 0;

	long pos = ftell(pFile);

	/// ftell()の失敗判定
	if (pos < 0) pos = -1;

	return pos;
}

/**
 *	説明
 *		ファイルポインタの位置を設定
 */
bool File::setPosition(long position)
{
	if (pFile == NULL)
		return false;

	int ret = fseek(pFile, position, SEEK_SET);

	if (ret != 0)
		return false;

	return true;
}

/**
 *	------------------------------------------------------------
 *		Fileクラスのprivateなメンバ関数
 *	------------------------------------------------------------
 */
void File::createTimeStampedFolder(void)
{
	/// SYSTEMTIME 構造体
	SYSTEMTIME sysTime;
	
	/// ローカル時間の取得
	::GetLocalTime(&sysTime);

	wsprintf(	folderName,
				TEXT("%.4d%.2d%.2d-%.2d%.2d%.2d"),
				sysTime.wYear, sysTime.wMonth, sysTime.wDay,
				sysTime.wHour, sysTime.wMinute, sysTime.wSecond
			);

	/// タイムスタンプ名でフォルダ作成
	_tmkdir(folderName);

	return;
}

}	/// end of namespace System