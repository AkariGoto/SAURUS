/**
 *  �t�@�C����
 *		Udp.h
 *  ����
 *		UDP�ʐM�Ɋւ���N���X http://pukulab.blog.fc2.com/blog-entry-6.html
 *  ���t
 *		�쐬��: 2020/02/20		�X�V��:
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
 
// UDP�N���X
class UDPSocket {

private:
    SOCKET sock;                            // �\�P�b�g
    sockaddr_in server_addr, client_addr;   // ����M�A�h���X

public:
    UDPSocket();                            // �R���X�g���N�^
    ~UDPSocket();                           // �f�X�g���N�^
    int  open(const char *addr, int port);  // ������           �i���s:0 ����:1�j
    int  send2(void *data, int size);       // ���M             �i���s:0 ����:���M�o�C�g���j
    int  sendf(char *str, ...);             // �����t���ő��M   �i���s:0 ����:���M�o�C�g���j
    int  receive(void *data, int size);     // ��M             �i���s:0 ����:��M�o�C�g���j
    void close(void);                       // ����

  
	
};



#endif