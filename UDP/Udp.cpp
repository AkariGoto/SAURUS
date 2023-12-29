#include "udp.h"
#include<initializer_list>

 

// --------------------------------------------------------------------------
// UDPSocket::UDPSocket()
// �R���X�g���N�^�ł��BUDPSocket�N���X�𐶐�����ۂɌĂяo����܂��B
// --------------------------------------------------------------------------
UDPSocket::UDPSocket() :server_addr(), client_addr() 
{
        sock = INVALID_SOCKET;
   
}
 
// --------------------------------------------------------------------------
// UDPSocket::~UDPSocket()
// �f�X�g���N�^�ł��BUDPSocket�N���X��j������ۂɌĂ΂�܂�
// --------------------------------------------------------------------------
UDPSocket::~UDPSocket()
{
}
 
// --------------------------------------------------------------------------
// UDPSocket::open(IP�A�h���X, �|�[�g�ԍ�)
// �\�P�b�g�̏�����
// �߂�l ����:1 ���s:0
// --------------------------------------------------------------------------
int UDPSocket::open(const char *addr, int port)
{
    // �\�P�b�g���
    sock = socket(AF_INET, SOCK_DGRAM, 0);
	//�\�P�b�g�쐬��error����
    if (sock == INVALID_SOCKET) {
        printf("ERROR: socket() failed. (%s, %d)\n", __FILE__, __LINE__);     
        return 0;
    }
 
    // ���M�p�A�h���X�ƃ|�[�g�̐ݒ�
    memset(&server_addr, 0, sizeof(sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons((u_short)port);
    server_addr.sin_addr.s_addr = inet_addr(addr);
 
    // ��M�p�A�h���X�ƃ|�[�g�̐ݒ�
    memset(&client_addr, 0, sizeof(sockaddr_in));
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons((u_short)port);
    client_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
 
    // �|�[�g���J��
    if (bind(sock, (sockaddr*)&client_addr, sizeof(sockaddr_in)) == SOCKET_ERROR) {
        printf("ERROR: bind() failed. (%s, %d)\n", __FILE__, __LINE__);  
        return 0;
    }
 
    // �m���u���b�L���O�i�񓯊��j���[�h�ݒ�
    u_long nonblock = 1;
    if (ioctlsocket(sock, FIONBIO, &nonblock) == SOCKET_ERROR) {
        printf("ERROR: ioctlsocket() failed. (%s, %d)\n", __FILE__, __LINE__);  
        return 0;
    }
 
    // �|�[�g�ԍ����ė��p�\�ɂ���
    int reuse = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) == SOCKET_ERROR) {
        printf("ERROR: setsockopt() failed. (%s, %d)\n", __FILE__, __LINE__);
        return 0;
    }
 
    return 1;
}
 
// --------------------------------------------------------------------------
// UDPSocket:::send2(���M�f�[�^, �T�C�Y)
// ���M
// �߂�l ����:���M�o�C�g�� ���s:0
// --------------------------------------------------------------------------
int UDPSocket::send2(void *data, int size)
{
    // �\�P�b�g���L���łȂ�
    if (sock == INVALID_SOCKET) return 0;
 
    // ���M
    int n = sendto(sock, (char*)data, size, 0, (sockaddr*)&server_addr, sizeof(sockaddr_in));
    if (n < 1) return 0;
 
    return n;
}
 
// --------------------------------------------------------------------------
// UDPSocket:::sendf(���b�Z�[�W)
// �����t�����M
// �߂�l ����:���M�o�C�g�� ���s:0
// --------------------------------------------------------------------------
int UDPSocket::sendf(char *str, ...)
{
    char *arg;
    char msg[1024];
 
    // �\�P�b�g���L���łȂ�
    if (sock == INVALID_SOCKET) return 0;
 
    // ������K�p
    va_start(arg, str);
    vsprintf_s(msg, sizeof(msg), str, arg);
    va_end(arg);
 
    // ���M
    return send2(msg, (int)strlen(msg) + 1);
}
 
// --------------------------------------------------------------------------
// UDPSocket:::receive(��M�f�[�^, �T�C�Y)
// ��M
// �߂�l ����:��M�o�C�g�� ���s:0
// --------------------------------------------------------------------------
int UDPSocket::receive(void *data, int size)
{
    // �\�P�b�g���L���łȂ�
    if (sock == INVALID_SOCKET) return 0;
 
    // ��M
    sockaddr_in addr{};
    int len = (int)sizeof(sockaddr_in);
    int n = recvfrom(sock, (char*)data, size, 0, (sockaddr*)&addr, &len);
    if (n < 1) return 0;
 
    // �w��O�̃A�h���X
    if (addr.sin_addr.S_un.S_addr != server_addr.sin_addr.S_un.S_addr) return 0;
 
    return n;
}
 
// --------------------------------------------------------------------------
// UDPSocket::close(�����Ȃ�)
// �\�P�b�g�����
// �߂�l �Ȃ�
// --------------------------------------------------------------------------
void UDPSocket::close(void)
{
    if (sock != INVALID_SOCKET) {
        closesocket(sock);
        sock = INVALID_SOCKET;
    }
}