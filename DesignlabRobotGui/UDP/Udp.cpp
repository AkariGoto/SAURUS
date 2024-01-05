
#include "pch.h"

#include "UDP/udp.h"


namespace designlab_robot_gui::udp
{

UDPSocket::UDPSocket() :
    server_addr(),
    client_addr()
{
    sock = INVALID_SOCKET;

}

int UDPSocket::open(const char* addr, int port)
{
    // ソケット作る
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    //ソケット作成のエラー処理
    if (sock == INVALID_SOCKET)
    {
        std::cerr << "ERROR: socket() failed. ("
            << __FILE__ << ", " << __LINE__ << ")" << std::endl;

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
    if (bind(sock, (sockaddr*)&client_addr, sizeof(sockaddr_in)) == SOCKET_ERROR)
    {
        std::cerr << "ERROR: bind() failed. ("
            << __FILE__ << ", " << __LINE__ << ")" << std::endl;

        return 0;
    }

    // ノンブロッキング（非同期）モード設定
    u_long nonblock = 1;
    if (ioctlsocket(sock, FIONBIO, &nonblock) == SOCKET_ERROR)
    {
        std::cerr << "ERROR: ioctlsocket() failed. ("
            << __FILE__ << ", " << __LINE__ << ")" << std::endl;

        return 0;
    }

    // ポート番号を再利用可能にする
    int reuse = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) == SOCKET_ERROR)
    {
        std::cerr << "ERROR: setsockopt() failed. ("
            << __FILE__ << ", " << __LINE__ << ")" << std::endl;

        return 0;
    }

    return 1;
}

int UDPSocket::send2(void* data, int size)
{
    // ソケットが有効でない
    if (sock == INVALID_SOCKET)
    {
        return 0;
    }

    // 送信
    int n = sendto(sock, (char*)data, size, 0, (sockaddr*)&server_addr, sizeof(sockaddr_in));

    if (n < 1)
    {
        return 0;
    }

    return n;
}

int UDPSocket::sendf(char* str, ...)
{
    char* arg;
    char msg[1024];

    // ソケットが有効でない
    if (sock == INVALID_SOCKET)
    {
        return 0;
    }

    // 書式を適用
    va_start(arg, str);
    vsprintf_s(msg, sizeof(msg), str, arg);
    va_end(arg);

    // 送信
    return send2(msg, (int)strlen(msg) + 1);
}

int UDPSocket::receive(void* data, int size)
{
    // ソケットが有効でない
    if (sock == INVALID_SOCKET)
    {
        return 0;
    }

    // 受信
    sockaddr_in addr{};
    int len = (int)sizeof(sockaddr_in);
    int n = recvfrom(sock, (char*)data, size, 0, (sockaddr*)&addr, &len);
    if (n < 1)
    {
        return 0;
    }

    // 指定外のアドレス
    if (addr.sin_addr.S_un.S_addr != server_addr.sin_addr.S_un.S_addr)
    {
        return 0;
    }

    return n;
}

void UDPSocket::close()
{
    if (sock != INVALID_SOCKET)
    {
        closesocket(sock);
        sock = INVALID_SOCKET;
    }
}


}  // namespace designlab_robot_gui::udp
