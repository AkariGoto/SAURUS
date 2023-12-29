/**
 *  ファイル名
 *		File.h
 *  説明
 *		ファイルを操作するクラス
 *		 fopen, fwrite, freadなどCRTのラッパークラス
 *  日付
 *		作成日: 2007/09/04(Tue)		更新日: 2007/09/04(Tue)
 */

#ifndef __File_h__
#define __File_h__

/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */

#include <windows.h>
#include <cstdio>
#include <ctime>

namespace System
{
/**
 *	----------------------------------------------------------------------
 *		Fileクラス
 *	----------------------------------------------------------------------
 */
class File
{
/**
 *	------------------------------------------------------------
 *		メンバ変数
 *	------------------------------------------------------------
 */
public:
/**
 *	エラーコード
 */
enum FileError
{
	NO_FILE_ERROR,				/// エラー無し
	ERR_FILE_NOT_FOUND,			/// ファイルが見つからない
	ERR_FAIL_CREATE_FILE,		/// ファイルが作成できない
	ERR_FAIL_WRITE_FILE,		/// 書込みエラー
	ERR_FAIL_READ_FILE,			/// 読込みエラー
	ERR_FAIL_CLOSE,				/// ファイルを閉じるのを失敗
	ERR_END_OF_FILE,			/// EOFエラー
	ERR_NO_OPEN,				/// ファイルが開けない
	ERR_INVALID_OPEN_MODE,		/// ファイルモードが無効
};

/**
 *		ファイルオープンモード
 */
enum FileMode
{
	READ_ONLY,		/// 読込み専用
	WRITE_ONLY,		/// 書込み専用
	APPEND_ONLY,	/// 追加書込み専用
	READ_WRITE,		/// 読込みと書込み
	WRITE_READ,		/// 書込みと読込み
	READ_APPEND,	/// 読み込みと追加書込み
	INVALID_MODE,	/// 無効な処理
};

protected:

	/// デフォルトバッファサイズ
	static const int DEFAULT_BUFFER_SIZE = 1024;
	/// フォーマットデフォルトバッファサイズ
	static const int FORMAT_DEFAULT_BUFFER_SIZE = 1024;

/**
 *	ファイルハンドル
 */
	FILE* pFile;

/**
 *	ファイルモード
 */
	FileMode fileMode;

/**
 *	フォルダ名（タイムスタンプ）
 */
	TCHAR folderName[128];


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
	File();

	/// コンストラクタ
	explicit File(const TCHAR* fileName, FileMode mode);

	/// デストラクタ
	virtual ~File();

/**
 *	説明
 *		指定したパスのファイルを開く
 *	引数：
 *		fileName: ファイル名
 *		mode: ファイルモード
 *		isTimeStamped: 時刻フォルダを作成するか
 */
	virtual int openFile(const TCHAR* fileName, FileMode mode, bool isTimeStamped = true);

/**
 *	説明
 *		ファイルを閉じる
 */
	int closeFile(void);

/**
 *	説明
 *		ファイルが開かれているかどうか
 */
	bool isOpen(void) const;

/**
 *	説明
 *		ファイルポインタの位置を取得
 */
	long getPosition(void) const;

/**
 *	説明
 *		ファイルポインタの位置を設定
 */
	bool setPosition(long position);

private:
	void createTimeStampedFolder(void);

};	/// end of class File

}	/// end of namespace System

#endif  /// __File_h__