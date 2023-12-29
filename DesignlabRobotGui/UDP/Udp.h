/**
 *  ファイル名
 *		Udp.h
 *  説明
 *		UDP通信に関するクラス http://pukulab.blog.fc2.com/blog-entry-6.html
 *  日付
 *		作成日: 2020/02/20		更新日:
 */





#ifndef __HEADER_UDP_LIB__
#define __HEADER_UDP_LIB__
#include <stdio.h>
#include <stdlib.h>

// Win32API
#include <windows.h>

// WinSock
#include <winsock.h>
#pragma comment(lib, "wsock32.lib")
 
// UDPクラス
class UDPSocket {

private:
    SOCKET sock;                            // ソケット
    sockaddr_in server_addr, client_addr;   // 送受信アドレス

public:
    UDPSocket();                            // コンストラクタ
    ~UDPSocket();                           // デストラクタ
    int  open(const char *addr, int port);  // 初期化           （失敗:0 成功:1）
    int  send2(void *data, int size);       // 送信             （失敗:0 成功:送信バイト数）
    int  sendf(char *str, ...);             // 書式付きで送信   （失敗:0 成功:送信バイト数）
    int  receive(void *data, int size);     // 受信             （失敗:0 成功:受信バイト数）
    void close(void);                       // 閉じる

  
	
};



#endif