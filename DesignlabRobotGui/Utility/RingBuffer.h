/**
 *  ファイル名
 *		RingBuffer.h
 *  説明
 *		リングバッファによる処理
 *  日付
 *		作成日: 2007/12/10(MON)		更新日: 2007/12/10(MON)
 */

#ifndef __RingBuffer_h__
#define __RingBuffer_h__

/**
 *	----------------------------------------------------------------------
 *		RingBufferクラス
 *	----------------------------------------------------------------------
 */
class RingBuffer
{
/**
 *	------------------------------------------------------------
 *		メンバ変数
 *	------------------------------------------------------------
 */
public:

	static const int MAX_BUFFER_SIZE = 4096;
	static const int DEFAULT_BUFFER_SIZE = 1024;
	
protected:
private:
	/// バッファ容量
	int capacity;

	/// 要素ポインタ
	unsigned char* buffer;

	/// データ先頭
	int head;
	/// データ個数
	int count;


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
 	RingBuffer();
	/// コンストラクタ
	explicit RingBuffer(int size);
	/// デストラクタ
	virtual ~RingBuffer();

/**
 *		バッファの容量を決定
 */
	void setSize(int size);

/**
 *	----------------------------------------
 *	読み書き関数
 *	----------------------------------------
 */
	/**
	 *	説明
	 *		1バイトデータ書き込み
	 *	引数
	 *		source:書き込むデータ
	 *	戻り値
	 *		source：成功
	 *		-1：失敗（オーバーフロー）
	 */
	int write(unsigned char source);
	/**
	 *	説明
	 *		複数バイトデータ書き込み
	 *	引数
	 *		source:書き込むデータのアドレス
	 *	戻り値
	 *		1：成功
	 *		-1：失敗（オーバーフロー）
	 */
	int write(unsigned char* source, int size);

	/**
	 *	説明
	 *		1バイトデータ読み込み
	 *	引数
	 *		target: 読み込み保存用のバッファ
	 *	戻り値
	 *		成功：1
	 *		失敗：-1（データ無）
	 */
	int read(unsigned char* target);
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
	int read(unsigned char* target, int size);

/**
 *	----------------------------------------
 *	アクセス関数
 *	----------------------------------------
 */
	/// 現在の書き込み位置を得る
	int getWriteIndex(void) const {return head + count;}
	/// 現在の読み込み位置を得る
	int getReadIndex(void) const {return head;}

	/// 現在書き込めるサイズを得る
	int getWritableSize(void);
	/// 現在読み込めるサイズを得る
	int getReadableSize(void);

/**
 *	----------------------------------------
 *	セット関数
 *	----------------------------------------
 */

protected:
private:

	/// コピーコンストラクタの禁止
	RingBuffer(const RingBuffer& copy);

	/// 代入コピーの禁止
	RingBuffer& operator =(const RingBuffer& copy);

/**
 *	オブジェクト生成
 */
	/// メモリの確保
	void newRingBuffer(void);

	/// メモリの破棄
	void deleteRingBuffer(void);

};

#endif /// __RingBuffer_h__