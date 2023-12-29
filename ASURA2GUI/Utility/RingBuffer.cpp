/**
 *  ファイル名
 *		RingBuffer.cpp
 *  説明
 *		リングバッファによる処理
 *  日付
 *		作成日: 2007/12/10(MON)		更新日: 2007/12/10(MON)
 */

/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */
#include "RingBuffer.h"
#include "iostream"


using namespace std;

/**
 *	----------------------------------------------------------------------
 *		RingBufferクラス
 *	----------------------------------------------------------------------
 */

/**
 *	------------------------------------------------------------
 *		RingBufferクラスのメンバ関数定義
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	コンストラクタとデストラクタ
 *	----------------------------------------
 */
RingBuffer::RingBuffer()
{
	/// 容量はデフォルト
	capacity = DEFAULT_BUFFER_SIZE;

	/// インデックスは0
	head = 0;
	count = 0;

	/// メモリ確保
	newRingBuffer();
}

RingBuffer::RingBuffer(int size)
{
	if ( size < 0 )
	{
		cerr << "Error: [RingBuffer::RingBuffer] Argument is invalid\n" << endl;
	}

	/// 容量を決定
	capacity = size;

	/// インデックスは0
	head = 0;
	count = 0;

	/// メモリ確保
	newRingBuffer();
}

/// デストラクタ
RingBuffer::~RingBuffer()
{
	deleteRingBuffer();
}

void RingBuffer::setSize(int size)
{
	/// メモリ領域を解放
	deleteRingBuffer();

	capacity = size;
	/// ベクトルオブジェクトのメモリ領域確保
    newRingBuffer();
}

/**
 *	----------------------------------------
 *	書き込み関数
 *	----------------------------------------
 */
/**
 *	説明
 *		1バイトデータ書き込み
 *	引数
 *		source:書き込むデータ
 *	戻り値
 *		成功：1
 *		失敗：-1（オーバーフロー）
 */
int RingBuffer::write(unsigned char source)
{
	int ret;

	/// 残量チェック
	if ( count < capacity )
	{
		/// バッファに値を格納
	    buffer[( head + count )  % capacity] = source;
		/// データ量を更新
		count++;
		ret = 1;		/// 正常終了
	}
	else
	{	
		ret = -1;	/// データが一杯なのでオーバーフロー
	}

	return ret;
}

/**
 *	説明
 *		複数バイトデータ書き込み
 *	引数
 *		source:書き込むデータのアドレス
 *	戻り値
 *		1：成功
 *		-1：失敗（オーバーフロー）
 */
int RingBuffer::write(unsigned char* source, int size)
{
	/// 戻り値
	int ret = -1;

	int i;
	for (i=0; i<size; i++)
	{
		/// 1データずつ格納
		ret = write(source[i]);
		
		/// オーバーフロー
		if (ret == -1)
		{
			cerr << "Error: [RingBuffer::write] Buffer Overflow\n" << endl;
			break;
		}

	}

	if ( i == size -1) ret = 1;	/// 正常終了

	return ret;
}

/**
 *	----------------------------------------
 *	読み込み関数
 *	----------------------------------------
 */
/**
 *	説明
 *		1バイトデータ読み込み
 *	引数
 *		target: 読み込み保存用のバッファ
 *	戻り値
 *		成功：1
 *		失敗：-1（データ無）
 */
int RingBuffer::read(unsigned char* target)
{
	char ret;

	/// データが残っている場合
	if ( count > 0 )
	{
		/// 読み出し値を格納
		*target = buffer[head];
		
		/// データ先頭位置の更新
		head = ( head + 1 ) % capacity;

		/// データ残量の更新
		count--;

		/// 正常終了
		ret = 1;
	}
	else	/// データ残量無
	{
		ret = -1;
	}
    
	return ret;
}

/**
 *	説明
 *		複数バイトデータ読み込み
 *	引数
 *		target: 読み込み保存用のバッファ
 *		size: targetのバイトサイズ
 *	戻り値
 *		成功：1
 *		失敗：-1（データ無）
 */
int RingBuffer::read(unsigned char* target, int size)
{
	/// 戻り値
	char ret;

	int i;
	for (i=0; i<size; i++)
	{
		/// 1データずつ格納
		ret = read(&target[i]);
		
		/// データ無し
		if (ret == -1)
		{
			cerr << "Error: [RingBuffer::write] No Buffer \n" << endl;
			break;
		}

	}

	if ( i == size -1) ret = 1;	/// 正常終了

	return ret;
}

/**
 *	----------------------------------------
 *	アクセス関数
 *	----------------------------------------
 */
/// 現在書き込めるサイズを得る
int RingBuffer::getWritableSize(void)
{
	return ( capacity - count );
}

/// 現在読み込めるサイズを得る
int RingBuffer::getReadableSize(void)
{
	return count;
}


/**
 *	------------------------------------------------------------
 *		RingBufferクラスのprivateなメンバ関数
 *	------------------------------------------------------------
 */
/**
 *	説明
 *		リングバッファのメモリ領域確保
 */
void RingBuffer::newRingBuffer(void)
{
	/// 容量が０以下の場合
	if (capacity <= 0)
	{
		cerr << "Error: [RingBuffer::newRingBuffer] Memory is not allocated\n" << endl;
        abort();
    }

	/// メモリー領域を確保する
	buffer = new unsigned char[capacity];
}

/**
 *	説明
 *		リングバッファのメモリ領域解放
 */
void RingBuffer::deleteRingBuffer(void)
{
	///メモリー領域を開放する
    delete [] buffer;

	buffer = NULL;
}