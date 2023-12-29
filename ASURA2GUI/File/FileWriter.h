/**
 *  ファイル名
 *		FileWriter.h
 *  説明
 *		ファイルへの書込みクラス
 *  日付
 *		作成日: 2007/09/05(WED)		更新日: 2007/09/05(WED)
 */

#ifndef __FileWriter_h__
#define __FileWriter_h__

/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */
#include "File.h"

namespace System
{

/**
 *	----------------------------------------------------------------------
 *		FileWriterクラス
 *	----------------------------------------------------------------------
 */
class FileWriter : public File
{
/**
 *	------------------------------------------------------------
 *		メンバ変数
 *	------------------------------------------------------------
 */

private:
	/// バッファ
	TCHAR* buffer;

	/// バッファサイズ
	int bufferSize;


/**
 *	------------------------------------------------------------
 *		メンバ関数
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	コンストラクタとデストラクタ
 *	----------------------------------------
 */
public:
	/// デフォルトコンストラクタ
	FileWriter();

	/// ファイルによるコンストラクタ
	explicit FileWriter(const TCHAR* fileName, FileMode mode);

	/// デストラクタ
	virtual ~FileWriter();

/**
 *	説明
 *		指定したパスのファイルを開く
 *	引数
 *		fileName: ファイル名
 *		mode: ファイルモード
 *		isTimeStamped: 時刻フォルダを作成するか
 */
	virtual int openFile(const TCHAR* fileName, FileMode mode, bool isTimeStamped = true);

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
	bool writeByte(const void* data, unsigned long size);

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
	bool writeLine(const TCHAR* string);

/**
 *	説明
 *		書式付文字列書き出し
 *		fprintfのラッパー
 *	引数
 *			format: 書式指定文字列を格納するポインタ
 */
	void writeFormattedString(const TCHAR* format, ...);

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
	bool writeText(const TCHAR* text, unsigned long size);

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
	bool writeFormattedText(const TCHAR* format, ...);



private:


};	/// end of class FileWriter

}	/// end of namespace System

#endif	/// __FileWriter_h__

