#include "udp.h"
#include<initializer_list>

 

// --------------------------------------------------------------------------
// UDPSocket::UDPSocket()
// コンストラクタです。UDPSocketクラスを生成する際に呼び出されます。
// --------------------------------------------------------------------------
UDPSocket::UDPSocket() :server_addr(), client_addr() 
{
        sock = INVALID_SOCKET;
   
}
 
// --------------------------------------------------------------------------
// UDPSocket::~UDPSocket()
// デストラクタです。UDPSocketクラスを破棄する際に呼ばれます
// --------------------------------------------------------------------------
UDPSocket::~UDPSocket()
{
}
 
// --------------------------------------------------------------------------
// UDPSocket::open(IPアドレス, ポート番号)
// ソケットの初期化
// 戻り値 成功:1 失敗:0
// --------------------------------------------------------------------------
int UDPSocket::open(const char *addr, int port)
{
    // ソケット作る
    sock = socket(AF_INET, SOCK_DGRAM, 0);
	//ソケット作成のerror処理
    if (sock == INVALID_SOCKET) {
        printf("ERROR: socket() failed. (%s, %d)\n", __FILE__, __LINE__);     
        return 0;
    }
 
    // 送信用アドレスとポートの設定
    memset(&server_addr, 0, sizeof(sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons((u_short)port);
    server_addr.sin_addr.s_addr = inet_addr(addr);
 
    // 受信用アドレスとポートの設定
    memset(&client_addr, 0, sizeof(sockaddr_in));
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons((u_short)port);
    client_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
 
    // ポートを開く
    if (bind(sock, (sockaddr*)&client_addr, sizeof(sockaddr_in)) == SOCKET_ERROR) {
        printf("ERROR: bind() failed. (%s, %d)\n", __FILE__, __LINE__);  
        return 0;
    }
 
    // ノンブロッキング（非同期）モード設定
    u_long nonblock = 1;
    if (ioctlsocket(sock, FIONBIO, &nonblock) == SOCKET_ERROR) {
        printf("ERROR: ioctlsocket() failed. (%s, %d)\n", __FILE__, __LINE__);  
        return 0;
    }
 
    // ポート番号を再利用可能にする
    int reuse = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) == SOCKET_ERROR) {
        printf("ERROR: setsockopt() failed. (%s, %d)\n", __FILE__, __LINE__);
        return 0;
    }
 
    return 1;
}
 
// --------------------------------------------------------------------------
// UDPSocket:::send2(送信データ, サイズ)
// 送信
// 戻り値 成功:送信バイト数 失敗:0
// --------------------------------------------------------------------------
int UDPSocket::send2(void *data, int size)
{
    // ソケットが有効でない
    if (sock == INVALID_SOCKET) return 0;
 
    // 送信
    int n = sendto(sock, (char*)data, size, 0, (sockaddr*)&server_addr, sizeof(sockaddr_in));
    if (n < 1) return 0;
 
    return n;
}
 
// --------------------------------------------------------------------------
// UDPSocket:::sendf(メッセージ)
// 書式付き送信
// 戻り値 成功:送信バイト数 失敗:0
// --------------------------------------------------------------------------
int UDPSocket::sendf(char *str, ...)
{
    char *arg;
    char msg[1024];
 
    // ソケットが有効でない
    if (sock == INVALID_SOCKET) return 0;
 
    // 書式を適用
    va_start(arg, str);
    vsprintf_s(msg, sizeof(msg), str, arg);
    va_end(arg);
 
    // 送信
    return send2(msg, (int)strlen(msg) + 1);
}
 
// --------------------------------------------------------------------------
// UDPSocket:::receive(受信データ, サイズ)
// 受信
// 戻り値 成功:受信バイト数 失敗:0
// --------------------------------------------------------------------------
int UDPSocket::receive(void *data, int size)
{
    // ソケットが有効でない
    if (sock == INVALID_SOCKET) return 0;
 
    // 受信
    sockaddr_in addr{};
    int len = (int)sizeof(sockaddr_in);
    int n = recvfrom(sock, (char*)data, size, 0, (sockaddr*)&addr, &len);
    if (n < 1) return 0;
 
    // 指定外のアドレス
    if (addr.sin_addr.S_un.S_addr != server_addr.sin_addr.S_un.S_addr) return 0;
 
    return n;
}
 
// --------------------------------------------------------------------------
// UDPSocket::close(引数なし)
// ソケットを閉じる
// 戻り値 なし
// --------------------------------------------------------------------------
void UDPSocket::close(void)
{
    if (sock != INVALID_SOCKET) {
        closesocket(sock);
        sock = INVALID_SOCKET;
    }
}