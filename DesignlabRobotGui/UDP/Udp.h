
#ifndef DESIGNLABROBOTGUI_UDP_UDP_H_
#define DESIGNLABROBOTGUI_UDP_UDP_H_

// Win32API
#include <windows.h>

// WinSock
#include <winsock.h>
#pragma comment(lib, "wsock32.lib")


namespace designlab_robot_gui::udp
{

class UDPSocket final
{
public:
    UDPSocket();                            //!< コンストラクタ
    ~UDPSocket() = default;                 //!< デストラクタ
    int  open(const char* addr, int port);  // 初期化           （失敗:0 成功:1）
    int  send2(void* data, int size);       // 送信             （失敗:0 成功:送信バイト数）
    int  sendf(char* str, ...);             // 書式付きで送信   （失敗:0 成功:送信バイト数）
    int  receive(void* data, int size);     // 受信             （失敗:0 成功:受信バイト数）
    void close(void);                       // 閉じる

private:
    SOCKET sock;                            //!< ソケット
    sockaddr_in server_addr, client_addr;   //!< 送受信アドレス
};

}  // namespace designlab_robot_gui::udp


#endif  // DESIGNLABROBOTGUI_UDP_UDP_H_
