
#ifndef DESIGNLABROBOTGUI_UDP_UDP_RING_BUFFER_H_
#define DESIGNLABROBOTGUI_UDP_UDP_RING_BUFFER_H_


namespace designlab_robot_gui::udp
{

class UdpRingBuffer final
{
public:
    static constexpr int DEFAULT_BIT_SIZE = 10;

    UdpRingBuffer();
    explicit UdpRingBuffer(int bitSize);

    UdpRingBuffer(const UdpRingBuffer& copy) = delete;
    UdpRingBuffer& operator =(const UdpRingBuffer& copy) = delete;

    ~UdpRingBuffer();

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

private:
    /// メモリの確保
    void newUdpRingBuffer();

    /// メモリの破棄
    void deleteUdpRingBuffer();


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
};

}  // namespace designlab_robot_gui::udp


#endif  // DESIGNLABROBOTGUI_UDP_UDP_RING_BUFFER_H_
