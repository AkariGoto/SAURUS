﻿
#include "pch.h"

#include "UDP/udp_ring_buffer.h"


namespace designlab_robot_gui::udp
{

UdpRingBuffer::UdpRingBuffer()
{
    // 容量はデフォルト
    bufferBitSize = DEFAULT_BIT_SIZE;
    bufferSize = 1 << bufferBitSize;
    buffer = NULL;

    // インデックスは0
    readIndex = 0;
    writeIndex = 0;

    // メモリ確保
    newUdpRingBuffer();
}

UdpRingBuffer::UdpRingBuffer(int bitSize)
{
    // インデックスは0
    readIndex = 0;
    writeIndex = 0;
    buffer = NULL;

    initialize(bitSize);
}

UdpRingBuffer::~UdpRingBuffer()
{
    deleteUdpRingBuffer();
}

void UdpRingBuffer::initialize(int bitSize)
{
    if (bitSize <= 0)
    {
        std::cerr << "Error: [UdpRingBuffer::UdpRingBuffer] Argument is invalid\n" << std::endl;
    }

    /// メモリ領域を解放
    deleteUdpRingBuffer();

    /// 容量を決定
    bufferBitSize = bitSize;
    bufferSize = 1 << bufferBitSize;

    /// ベクトルオブジェクトのメモリ領域確保
    newUdpRingBuffer();
}

/**
 *	説明
 *		バッファのクリア
 */
void UdpRingBuffer::clear(void)
{
    readIndex = 0;
    writeIndex = 0;
}

/**
 *	説明
 *		リングバッファの記録サイズを取得
 *	返却値
 *		記録しているバイトサイズ
 */
int UdpRingBuffer::getReadableSize(void) const
{
    return ((writeIndex >= readIndex) ? (writeIndex - readIndex) : (bufferSize - (readIndex - writeIndex)));
}

/**
 *	説明
 *		リングバッファの空きのバッファサイズを取得
 *	返却値
 *		空きバッファのバイトサイズ
 */
int UdpRingBuffer::getWritableSize(void) const
{
    return (bufferSize - getReadableSize() - 1);
}



/**
 *	----------------------------------------
 *	書き込み関数
 *	----------------------------------------
 */
 /**
  *	説明
  *		リングバッファへ1バイト格納
  *	引数
  *		buff: 書き込む値
  *	返却値
  *		-1: 失敗
  *		0: 成功
  */
int UdpRingBuffer::put(unsigned char buff)
{
    /// リングバッファが空かどうか
    if (getWritableSize() <= 0)
    {
        /// 空き領域無し
        return -1;
    }

    /// バッファを格納
    buffer[writeIndex] = buff;
    /// 書き込み位置を更新
    writeIndex = (writeIndex + 1) & (bufferSize - 1);

    return 0;
}

/**
 *	説明
 *		リングバッファへ複数バイト格納する
 *	引数
 *		dst: 書き込むデータへのポインタ
 *		size: 書き込むサイズ
 *	返却値
 *		実際に書き込んだサイズ
 */
int UdpRingBuffer::write(unsigned char* dst, int size)
{
    int i;

    /// 書き込みサイズの決定
    int writableSize = getWritableSize();
    int sizeToWrite = (size > writableSize) ? writableSize : size;

    /// 書き込み終了インデックスの計算
    int endIndex = (sizeToWrite > (bufferSize - writeIndex)) ? bufferSize : writeIndex + sizeToWrite;

    unsigned char* p = &buffer[writeIndex];
    unsigned char* q = dst;

    for (i = writeIndex; i < endIndex; ++i)
        *p++ = *q++;

    /// 最後の書き込みアドレスの計算
    unsigned char* data_last = dst + sizeToWrite;

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

    writeIndex = static_cast<int>((p - &buffer[0]) & (bufferSize - 1));
    return sizeToWrite;
}

/**
 *	説明
 *		リングバッファから1バイト取得
 *	引数
 *		buff: 読み込み先へのポインタ
 *	返却値
 *		-1: データが空
 *		0: 読み込み成功
 */
int UdpRingBuffer::get(unsigned char* buff)
{
    /// データがあるかどうか
    if (readIndex == writeIndex)
        return -1;

    *buff = buffer[readIndex];
    /// 読み込み位置を更新
    readIndex = (readIndex + 1) & (bufferSize - 1);

    return 0;
}

/**
 *	説明
 *		リングバッファから複数バイト読み込む
 *	引数
 *		dst: 読み込み先へのポインタ
 *		size: 読み込むサイズ
 *	返却値
 *		読み込んだサイズ
 */
int UdpRingBuffer::read(unsigned char* dst, int size)
{
    int i;

    /// 読み込みサイズの決定
    int readableSize = getReadableSize();
    int sizeToRead = (size > readableSize) ? readableSize : size;

    /// 読み込みインデックスの計算
    int endIndex = (sizeToRead > (bufferSize - readIndex)) ? bufferSize : readIndex + sizeToRead;

    unsigned char* p = dst;
    unsigned char* q = &buffer[readIndex];

    for (i = readIndex; i < endIndex; ++i)
        *p++ = *q++;

    unsigned char* data_last = dst + sizeToRead;
    if (p < data_last)
    {
        q = &buffer[0];
        while (p < data_last)
        {
            *p++ = *q++;
        }
    }

    readIndex = static_cast<int>((q - &buffer[0]) & (bufferSize - 1));

    return sizeToRead;
}

/**
 *	説明
 *		バッファの記録データをコピー
 *	引数
 *		dst: コピー先へのポインタ
 *		size: コピーするサイズ
 *	返却値
 *		コピーしたサイズ
 */
int UdpRingBuffer::copy(unsigned char* dst, int size)
{
    int i;

    /// 読み込みサイズの決定
    int readableSize = getReadableSize();
    int sizeToRead = (size > readableSize) ? readableSize : size;

    /// 読み込みインデックスの計算
    int endIndex = (sizeToRead > (bufferSize - readIndex)) ? bufferSize : readIndex + sizeToRead;

    unsigned char* p = dst;
    unsigned char* q = &buffer[readIndex];

    for (i = readIndex; i < endIndex; ++i)
        *p++ = *q++;

    unsigned char* data_last = dst + sizeToRead;
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
 *		UdpRingBufferクラスのprivateなメンバ関数
 *	------------------------------------------------------------
 */
 /**
  *	説明
  *		リングバッファのメモリ領域確保
  */
void UdpRingBuffer::newUdpRingBuffer(void)
{
    /// 容量が０以下の場合
    if (bufferSize <= 0)
    {
        std::cerr << "Error: [UdpRingBuffer::newUdpRingBuffer] Memory is not allocated\n" << std::endl;
        abort();
    }

    /// メモリー領域を確保する
    buffer = new unsigned char[bufferSize];
}

/**
 *	説明
 *		リングバッファのメモリ領域解放
 */
void UdpRingBuffer::deleteUdpRingBuffer(void)
{
    ///メモリー領域を開放する
    if (buffer != NULL)
    {
        delete[] buffer;
        buffer = NULL;
    }
}

}  // namespace designlab_robot_gui::udp
