/**
 *  ファイル名
 *		FileWriter.h
 *  説明
 *		ファイルへの書込みクラス
 *  日付
 *		作成日: 2007/09/05(WED)		更新日: 2007/09/05(WED)
 */

/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */
#include "FileWriter.h"
#include <tchar.h>
#include <stdarg.h>

namespace System
{
/**
 *	------------------------------------------------------------
 *		FileWriterクラスのメンバ関数定義
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	コンストラクタとデストラクタ
 *	----------------------------------------
 */
/// デフォルトコンストラクタ
FileWriter::FileWriter()
{
	/// バッファの設定
	bufferSize = DEFAULT_BUFFER_SIZE;
	buffer = new TCHAR[bufferSize];
}

FileWriter::FileWriter(const TCHAR *fileName, FileMode mode)
{
	/// バッファの設定
	bufferSize = DEFAULT_BUFFER_SIZE;
	buffer = new TCHAR[bufferSize];

	FileWriter::openFile(fileName, mode);
}

/// デストラクタ
FileWriter::~FileWriter()
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
int FileWriter::openFile(const TCHAR* fileName, FileMode mode, bool isTimeStamped)
{
	/// ファイルモードのチェック
	/// 読込み専用で開いたらエラー
	if ( mode == READ_ONLY )
		 return ERR_INVALID_OPEN_MODE;

	return File::openFile(fileName, mode, isTimeStamped);

}

/**
 *	説明
 *		バイトデータ書き出し
 *	引数
 *		data: 書き出すデータへのポインタ
 *		size: データサイズ
 *	返却値
 *		false: 失敗
 *		true: 成功
 */
bool FileWriter::writeByte(const void* data, unsigned long size)
{
	/// ファイルが開いているかどうか
	if ( pFile == NULL || data == NULL )
		return false;

	/// 実際に書き込み
	int result = (int)fwrite( data, 1, size, pFile );

	/// 書込み予定サイズと実際の書込み
	return result == size;
}

/**
 * 説明
 *		1行テキスト書き出し
 *	引数
 *		data: 書き出すデータへのポインタ
 *		size: データサイズ
 *	返却値
 *		false: 失敗
 *		true: 成功
 */
bool FileWriter::writeLine(const TCHAR* string)
{
	/// ファイルが開いているか
	if ( pFile == NULL || string == NULL )
		return false;
	
	_fputts( string, pFile );
	_fputts( TEXT("\n"), pFile );

	return true;
}

/**
 *	説明
 *		書式付文字列書き出し
 *		fprintfのラッパー
 *	引数
 *			format: 書式指定文字列を格納するポインタ
 */
void FileWriter::writeFormattedString(const TCHAR* format, ...)
{
	/// ファイルが開いているか
	if (pFile == NULL)
		return;

	va_list ap;
	
	va_start(ap, format);
              _vftprintf( pFile, format, ap );
	va_end(ap);

}

/**
 *	説明
 *		テキストデータ書き出し
 *		改行コード\\nを\r\nに変換する
 *	引数
 *		data: 書き出すデータへのポインタ
 *		size: データサイズ
 *	返却値
 *		false: 失敗
 *		true: 成功
 */
bool FileWriter::writeText(const TCHAR* text, unsigned long size)
{
	if (text != NULL)
		return false;

	/// 安全のためバッファ拡張
	int maxSize = size*2;

	/// 以下でバッファサイズを所期のサイズまで増やす
	if (maxSize > bufferSize)
	{
		delete [] buffer;
		while (1)
		{
			bufferSize *= 2;
			if (maxSize <= bufferSize)
				break;
		}
	}

	/// Windowsの改行フィルタリング
	int writeSize = 0;
	TCHAR preTChar = TEXT('');
	int i;		/// カウンタ
	
	/// 以下で実際に改行コードのフィルタリングを行う
	for (i=0;i<(int)size;i++)
	{
		if ((text[i] == TEXT('\n')) && preTChar != TEXT('\r'))
		{
			buffer[writeSize] = TEXT('\r');
			writeSize++;
		}

		buffer[writeSize] = text[i];
		preTChar = text[i];
		writeSize++;
	}

	/// バイトデータ書出し
	return writeByte(buffer, writeSize);
}

/**
 *	説明
 *		書式指定テキストデータ書き出し
 *		改行コード\\nを\r\nに変換する
 *	引数
 *		format: 書式指定文字列を格納するポインタ
 *	返却値
 *		false: 失敗
 *		true: 成功
 */
bool FileWriter::writeFormattedText(const TCHAR* format, ...)
{
	/// 引数チェック
	if ( format == NULL )
		return false;

	/// 準備
	int formatBufferSize = FORMAT_DEFAULT_BUFFER_SIZE;
	TCHAR* formatBuffer;
	int writeSize;
	va_list ap;

	va_start(ap, format);
	while(1)
	{
		formatBuffer = new TCHAR[formatBufferSize];
		writeSize =  _vsntprintf(formatBuffer, formatBufferSize, format, ap);

		/// 関数失敗判定
		if ( writeSize != -1 )
			break;

		formatBufferSize *= 2;
		delete [] formatBuffer;
	}
	va_end(ap);

	/// NULL終端でない可能性がある（問題ないが）
	bool ret = writeText(formatBuffer, writeSize);

	/// バッファの後処理
	delete [] formatBuffer;

	/// writeText()の成否が戻り値
	return ret;
}

}	/// end of namespace System