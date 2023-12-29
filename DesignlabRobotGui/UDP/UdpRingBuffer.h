/**
 *  ファイル名
 *		UdpRingBuffer.h
 *  説明
 *		リングバッファによる処理
 *  日付
 *		作成日: 2020/08/19
 */

#ifndef __UdpRingBuffer_h__
#define __UdpRingBuffer_h__

/**
 *	----------------------------------------------------------------------
 *		UdpRingBufferクラス
 *	----------------------------------------------------------------------
 */
class UdpRingBuffer
{
/**
 *	------------------------------------------------------------
 *		メンバ変数
 *	------------------------------------------------------------
 */
public:
	static const int DEFAULT_BIT_SIZE = 10;
	
protected:
private:
	/// バッファビットサイズ
	int bufferBitSize;

	/// バッファ容量
	int bufferSize;

	/// 要素ポインタ
	unsigned char* buffer;

	/// キュー先頭
	int readIndex;
	/// データ書き込み位置
	int writeIndex;


/**
 *	------------------------------------------------------------
 *		メンバ関数
 *	------------------------------------------------------------
 */
public:
/**
 *	----------------------------------------
 *	コンストラクタとデストラクタ
 *	----------------------------------------
 */
 	/// デフォルトコンストラクタ
 	UdpRingBuffer();
	/// コンストラクタ
	explicit UdpRingBuffer(int bitSize);
	/// デストラクタ
	virtual ~UdpRingBuffer();

/**
 *	説明
 *		初期化
 *	引数
 *		bitSize: バッファのビットサイズ
 */
	void initialize(int bitSize);

/**
 *	説明
 *		バッファのクリア
 */
	void clear(void);
	
/**
 *	説明
 *		リングバッファの記録サイズを取得
 *	返却値
 *		記録しているバイトサイズ
 */
	int getReadableSize(void) const;
	
/**
 *	説明
 *		リングバッファの空きのバッファサイズを取得
 *	返却値
 *		空きバッファのバイトサイズ
 */
	int getWritableSize(void) const;

/**
 *	----------------------------------------
 *	読み書き関数
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
	int put(unsigned char buff);

	/**
	 *	説明
	 *		リングバッファへ複数バイト格納する
	 *	引数
	 *		dst: 書き込むデータへのポインタ
	 *		size: 書き込むサイズ
	 *	返却値
	 *		実際に書き込んだサイズ
	 */
	int write(unsigned char* dst, int size);

	/**
	 *	説明
	 *		リングバッファから1バイト取得
	 *	引数
	 *		buff: 読み込み先へのポインタ
	 *	返却値
	 *		-1: データが空
	 *		0: 読み込み成功 
	 */
	int get(unsigned char* buff);

	/**
	 *	説明
	 *		リングバッファから複数バイト読み込む
	 *	引数
	 *		dst: 読み込み先へのポインタ
	 *		size: 読み込むサイズ
	 *	返却値
	 *		読み込んだサイズ 
	 */
	int read(unsigned char* dst, int size);

	/**
	*	説明
	*		バッファの記録データをコピー
	*	引数
	*		dst: コピー先へのポインタ
	*		size: コピーするサイズ
	*/
	int copy(unsigned char* dst, int size);
	
protected:
private:

	/// コピーコンストラクタの禁止
	UdpRingBuffer(const UdpRingBuffer& copy);

	/// 代入コピーの禁止
	UdpRingBuffer& operator =(const UdpRingBuffer& copy);

/**
 *	オブジェクト生成
 */
	/// メモリの確保
	void newUdpRingBuffer(void);

	/// メモリの破棄
	void deleteUdpRingBuffer(void);

};

#endif /// __UdpRingBuffer_h__