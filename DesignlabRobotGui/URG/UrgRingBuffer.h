/**
 *  ファイル名
 *		UrgRingBuffer.h
 *  説明
 *		リングバッファによる処理
 *  日付
 *		作成日: 2014/06/20(FRI)		更新日:
 */

#ifndef __UrgRingBuffer_h__
#define __UrgRingBuffer_h__

/**
 *	----------------------------------------------------------------------
 *		UrgRingBufferクラス
 *	----------------------------------------------------------------------
 */
class UrgRingBuffer
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
	int bufferlongSize;

	/// バッファ容量
	//int bufferSize;

	/// 要素ポインタ
	long* buffer;

	/// キュー先頭
	int readIndex;
	/// データ書き込み位置
	int writeIndex;

	/// 過去のデータ読み込み位置
	int old_readIndex;

	/// 過去の測距データの保存時間[ms]
	int bufferTime;


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
 	UrgRingBuffer();
	/// デストラクタ
	virtual ~UrgRingBuffer();

/**
 *	説明
 *		初期化
 *	引数
 *		bitSize: バッファのlongサイズ
 */
	void initialize(int longSize);

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
 *		リングバッファの空きのlongサイズを取得
 *	返却値
 *		空きバッファのlongサイズ
 */
	int getWritableSize(void) const;



/**
 *	----------------------------------------
 *	読み書き関数
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
	int write(long* dst, int size);

	/**
	 *	説明
	 *		リングバッファからlongを読み込む
	 *	引数
	 *		dst: 読み込み先へのポインタ
	 *		size: 読み込むサイズ
	 *	返却値
	 *		読み込んだサイズ 
	 */
	int read(long* dst, int size);

	/**
	*	説明
	*		バッファの記録データをコピー
	*	引数
	*		dst: コピー先へのポインタ
	*		size: コピーするサイズ
	*		old_time: 遡る時間
	*/
	int UrgRingBuffer::old_read(long* dst, int size, int old_time);
	
protected:
private:

	/// コピーコンストラクタの禁止
	UrgRingBuffer(const UrgRingBuffer& copy);

	/// 代入コピーの禁止
	UrgRingBuffer& operator =(const UrgRingBuffer& copy);

/**
 *	オブジェクト生成
 */
	/// メモリの確保
	void newUrgRingBuffer(void);

	/// メモリの破棄
	void deleteUrgRingBuffer(void);

};

#endif /// __UrgRingBuffer_h__