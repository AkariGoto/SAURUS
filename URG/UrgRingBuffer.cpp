/**
 *  ファイル名
 *		UrgRingBuffer.cpp
 *  説明
 *		リングバッファによる処理
 *  日付
 *		作成日: 2014/06/20(FRI)		更新日:
 */

/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */
#include "UrgRingBuffer.h"
#include "iostream"

using namespace std;

/**
 *	----------------------------------------------------------------------
 *		UrgRingBufferクラス
 *	----------------------------------------------------------------------
 */

/**
 *	------------------------------------------------------------
 *		UrgRingBufferクラスのメンバ関数定義
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	コンストラクタとデストラクタ
 *	----------------------------------------
 */
UrgRingBuffer::UrgRingBuffer()
{
	/// インデックスは0
	readIndex = 0;
	writeIndex = 0;
	buffer = NULL;

	bufferTime=5000;		//[ms]	200[ms]の倍数
	initialize(684 * bufferTime / 200);
}


/// デストラクタ
UrgRingBuffer::~UrgRingBuffer()
{
	deleteUrgRingBuffer();
}

void UrgRingBuffer::initialize(int longSize)
{
	if (longSize <= 0)
		cerr << "Error: [UrgRingBuffer::UrgRingBuffer] Argument is invalid\n" << endl;

	/// メモリ領域を解放
	deleteUrgRingBuffer();

	/// 容量を決定
	bufferlongSize = longSize;

	/// ベクトルオブジェクトのメモリ領域確保
    newUrgRingBuffer();
}

/**
 *	説明
 *		バッファのクリア
 */
void UrgRingBuffer::clear(void)
{
	readIndex = 0;
	writeIndex = 0;
}

/**
 *	説明
 *		リングバッファの記録サイズを取得
 *	返却値
 *		記録しているlongのサイズ
 */
int UrgRingBuffer::getReadableSize(void) const
{
	return ((writeIndex >= readIndex) ? (writeIndex - readIndex) : (bufferlongSize - (readIndex - writeIndex)));
}

/**
 *	説明
 *		リングバッファの空きのlongサイズを取得
 *	返却値
 *		空きバッファのlongサイズ
 */
int UrgRingBuffer::getWritableSize(void) const
{
	return (bufferlongSize - getReadableSize() - 1);
}



/**
 *	----------------------------------------
 *	書き込み関数
 *	----------------------------------------
 */

/**
 *	説明
 *		リングバッファへlongを格納する
 *	引数
 *		dst: 書き込むデータへのポインタ
 *		size: 書き込むサイズ
 *	返却値
 *		実際に書き込んだサイズ
 */
int UrgRingBuffer::write(long* dst, int size)
{
	int i;

	/// 書き込みサイズの決定
	int writableSize = getWritableSize();
	int sizeToWrite = (size > writableSize) ? writableSize : size;

	/// 書き込み終了インデックスの計算
	int endIndex = (sizeToWrite > (bufferlongSize - writeIndex)) ? bufferlongSize : writeIndex + sizeToWrite;

	long* p = &buffer[writeIndex];
	long* q = dst;
	
	for (i=writeIndex;i<endIndex;++i)
		*p++ = *q++;
	
	/// 最後の書き込みアドレスの計算
	long* data_last = dst + sizeToWrite;
	
	/// 書き込めていなかった残りのバッファを格納
	if (q < data_last)
	{
		p = &buffer[0];
		while (q < data_last)
		{
			/// データを格納してポインタをインクリメント
			*p++ = *q++;
		}
	}

	readIndex = writeIndex;
	writeIndex  = ((writeIndex + sizeToWrite) >= bufferlongSize ) ? (writeIndex + sizeToWrite) - bufferlongSize : writeIndex + sizeToWrite;


	return sizeToWrite;
}

/**
 *	説明
 *		リングバッファからlongを読み込む
 *	引数
 *		dst: 読み込み先へのポインタ
 *		size: 読み込むサイズ
 *	返却値
 *		読み込んだサイズ 
 */
int UrgRingBuffer::read(long* dst, int size)
{
	int i;

	/// 読み込みサイズの決定
	int readableSize = getReadableSize();
	int sizeToRead = (size > readableSize) ? readableSize : size;

	/// 読み込みインデックスの計算
	int endIndex = (sizeToRead > (bufferlongSize - readIndex)) ? bufferlongSize : readIndex + sizeToRead;

	long* p = dst;
	long* q = &buffer[readIndex];

	for (i=readIndex;i<endIndex;++i)
		*p++ = *q++;

	long* data_last = dst + sizeToRead;
	if (p < data_last)
	{
		q = &buffer[0];
		while (p < data_last)
		{
			*p++ = *q++;
		}
	}

	return sizeToRead;
}

/**
 *	説明
 *		バッファの過去の記録データを読み込む
 *	引数
 *		dst: コピー先へのポインタ
 *		size: コピーするサイズ
 *		old_time: 遡る時間 [ms]
 *	返却値
 *		読み込んだサイズ  
 */
int UrgRingBuffer::old_read(long* dst, int size, int old_time)
{
	/// 遡る時間がURGスレッドの実行周期の倍数でない場合	または貯めているバッファ以前の値を取得しようとした場合
	if (old_time % 200 != 0 || old_time >= 200 * bufferTime / 200)	//URGスレッドの実行周期は200[ms]	貯めているバッファは10回分[2000ms]
	{
		cerr << "Error: [UrgRingBuffer::old_read] old_time is not correct\n" << endl;
		return 0;
    }

	int i;

	old_readIndex = ((readIndex - old_time/200 * 684) > 0) ? readIndex - old_time/200 * 684 : bufferlongSize + (readIndex - old_time/200 * 684);

	/// 読み込みサイズの決定

	int sizeToRead = size;


	/// 読み込みインデックスの計算
	int endIndex = (sizeToRead > (bufferlongSize - old_readIndex)) ? bufferlongSize : old_readIndex + sizeToRead;

	long* p = dst;
	long* q = &buffer[old_readIndex];

	for (i=old_readIndex;i<endIndex;++i)
		*p++ = *q++;

	long* data_last = dst + sizeToRead;
	if (p < data_last)
	{
		q = &buffer[0];
		while (p < data_last)
		{
			*p++ = *q++;
		}
	}
	
	return sizeToRead;
}

/**
 *	------------------------------------------------------------
 *		UrgRingBufferクラスのprivateなメンバ関数
 *	------------------------------------------------------------
 */
/**
 *	説明
 *		リングバッファのメモリ領域確保
 */
void UrgRingBuffer::newUrgRingBuffer(void)
{
	/// 容量が０以下の場合
	if (bufferlongSize <= 0)
	{
		cerr << "Error: [UrgRingBuffer::newUrgRingBuffer] Memory is not allocated\n" << endl;
        abort();
    }

	/// メモリー領域を確保する
	buffer = new long[bufferlongSize];

	readIndex  = 0;
	writeIndex  = 0;
}

/**
 *	説明
 *		リングバッファのメモリ領域解放
 */
void UrgRingBuffer::deleteUrgRingBuffer(void)
{
	///メモリー領域を開放する
    if (buffer != NULL)
	{
		delete [] buffer;
		buffer = NULL;
	}
}