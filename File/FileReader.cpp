/**
 *  File: FileReader.cpp
 *
 *  Description: ファイルへからの読込みクラス
 *
 *  Created: 2007/09/04(Tue)
 *  Updated: 2007/09/04(Tue)
 *
 *  Copyright (C) Tokyo Institute of Technology Hirose Lab.
 */

/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */
#include "FileReader.h"
#include <tchar.h>
#include <stdarg.h>

namespace System
{
/**
 *	----------------------------------------------------------------------
 *		AsuraDataクラス
 *	----------------------------------------------------------------------
 */

/**
 *	------------------------------------------------------------
 *		FileReaderクラスのメンバ関数定義
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	コンストラクタとデストラクタ
 *	----------------------------------------
 */
/// デフォルトコンストラクタ
FileReader::FileReader()
{
/**
 *	読込みの準備
 */
	bufferSize = DEFAULT_BUFFER_SIZE;
	buffer = new TCHAR[bufferSize];
}

/// コンストラクタ
FileReader::FileReader(const TCHAR* fileName, FileMode mode)
{
/**
 *	読込みの準備
 */
	bufferSize = DEFAULT_BUFFER_SIZE;
	buffer = new TCHAR[bufferSize];

	FileReader::openFile(fileName, mode);
}

/// デストラクタ
FileReader::~FileReader()
{
	if ( buffer != NULL )
		delete [] buffer;
}

/**
 *	説明
 *		指定したパスのファイルを開く
 *	引数
 *		fileName: ファイル名
 *		mode: ファイルモード
 *		isTimeStamped: 時刻フォルダを作成するか
 */
int FileReader::openFile(const TCHAR* fileName, FileMode mode, bool isTimeStamped)
{
	/// ファイルモードのチェック
	/// 書込み専用で開いたらエラー
	if ( mode == WRITE_ONLY )
		 return ERR_INVALID_OPEN_MODE;

	return File::openFile(fileName, mode, isTimeStamped);
}

/**
 *	説明
 *		バイトデータ読込み
 *	引数
 *		data: 読み出しデータへのポインタ
 *		size: データサイズ
 *	返却値
 *		true: 成功
 *		false: 失敗
 */
bool FileReader::readByte(void* data, int size)
{
	/// ファイルが開かれているかどうか
	if ( pFile == NULL )
		return false;

	/// 読込みバイト数
	int result = (int)fread( data, 1, size, pFile );

	return result == size;
}

/**
 *	説明
 *		テキストデータ一行読込み
 *	引数
 *		string: 読み出しデータへのポインタ
 *		size: 読み出す上限サイズ
 *	返却値
 *		-1: 失敗
 *		その他: 読み出したサイズ
 */
int FileReader::readLine(TCHAR* string, size_t size)
{
	/// ファイルが開かれているかどうか
	if (pFile == NULL)
		return -1;

	if (_fgetts(string, size, pFile) == NULL)
		return 0;
	
	return _tcslen(string);
}

}	/// end of namespace System

