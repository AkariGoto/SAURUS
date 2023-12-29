/**
 *  ファイル名
 *		FileReader.h
 *  説明
 *		ファイルからの読み込みクラス
 *  日付
 *		作成日: 2007/09/05(WED)		更新日: 2007/09/05(WED)
 */

#ifndef __FileReader_h__
#define __FileReader_h__

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
 *		FileReaderクラス
 *	----------------------------------------------------------------------
 */
class FileReader : public File
{
/**
 *	------------------------------------------------------------
 *		メンバ変数
 *	------------------------------------------------------------
 */

private:
/**
 *	読込みに用いる変数
 */
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
	FileReader();

	/// ファイルによるコンストラクタ
	explicit FileReader(const TCHAR* fileName, FileMode mode);

	/// デストラクタ
	virtual ~FileReader();

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
 *		バイトデータ読込み
 *	引数
 *		data: 読み出しデータへのポインタ
 *		size: データサイズ
 *	返却値
 *		true: 成功
 *		false: 失敗
 */
	bool readByte(void* data, int size);

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
	int readLine(TCHAR* string, size_t size);

private:


};	/// end of class FileReader

}	/// end of namespace System

#endif	/// __FileReader_h__

