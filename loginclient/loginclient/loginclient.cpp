#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE    512

// ���� �Լ� ���� ��� �� ����
void err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCWSTR)msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// ���� �Լ� ���� ���
void err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

// ����� ���� ������ ���� �Լ�
int recvn(SOCKET s, char *buf, int len, int flags)
{
	int received;
	char *ptr = buf;
	int left = len;

	while(left > 0){
		received = recv(s, ptr, left, flags);
		if(received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if(received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}

int CHECKACCOUNT(SOCKET sock)
{
	char buf[BUFSIZE+1];
	int len;
	int retval;

	//select mode
	retval = recv(sock, buf, BUFSIZE, 0);
	if(retval == SOCKET_ERROR){	err_display("recv()"); closesocket(sock); }
	else if(retval == 0){ closesocket(sock); WSACleanup(); return 0;}
	buf[retval] = '\0';
	printf("[server message] %s\n", buf);
	//mode Ȯ��
	printf("\n[mode] ");
	if(fgets(buf, BUFSIZE+1, stdin) == NULL)closesocket(sock);
	// '\n' ���� ����
	len = strlen(buf);
	if(buf[len-1] == '\n')	buf[len-1] = '\0';
	if(strlen(buf) == 0) closesocket(sock);
	retval = send(sock, buf, len, 0);
	if(retval == SOCKET_ERROR){	err_display("send()");closesocket(sock);}

	if(atoi(buf) == 1)
	{
		//receive server message
		retval = recv(sock, buf, BUFSIZE, 0);
		if(retval == SOCKET_ERROR){	err_display("recv()"); closesocket(sock); }
		else if(retval == 0){ closesocket(sock); WSACleanup(); return 0;}
		buf[retval] = '\0';
		printf("[server message] %s\n", buf);

		//send id
		printf("\n[id] ");
		if(fgets(buf, BUFSIZE+1, stdin) == NULL)closesocket(sock);
		// '\n' ���� ����
		len = strlen(buf);
		if(buf[len-1] == '\n')	buf[len-1] = '\0';
		if(strlen(buf) == 0) closesocket(sock);
		// ������ ������
		retval = send(sock, buf, BUFSIZE, 0);
		if(retval == SOCKET_ERROR){	err_display("send()");closesocket(sock);}

		//receive server message
		retval = recv(sock, buf, BUFSIZE, 0);
		if(retval == SOCKET_ERROR){	err_display("recv()"); closesocket(sock); }
		else if(retval == 0){ closesocket(sock); WSACleanup(); return 0;}
		buf[retval] = '\0';
		printf("[server message] %s\n", buf);

		//send pw
		printf("\n[pw] ");
		if(fgets(buf, BUFSIZE+1, stdin) == NULL)closesocket(sock);
		// '\n' ���� ����
		len = strlen(buf);
		if(buf[len-1] == '\n')	buf[len-1] = '\0';
		if(strlen(buf) == 0) closesocket(sock);
		// ������ ������
		retval = send(sock, buf, strlen(buf), 0);
		if(retval == SOCKET_ERROR){	err_display("send()");closesocket(sock);}
	}
	else if(atoi(buf) == 2)
	{
		//id Ȯ��
		printf("\n[id] ");
		if(fgets(buf, BUFSIZE+1, stdin) == NULL)closesocket(sock);
		// '\n' ���� ����
		len = strlen(buf);
		if(buf[len-1] == '\n')	buf[len-1] = '\0';
		if(strlen(buf) == 0) closesocket(sock);
		// ������ ������
		retval = send(sock, buf, strlen(buf), 0);
		if(retval == SOCKET_ERROR){	err_display("send()");closesocket(sock);}
		// ������ �ޱ�
		retval = recv(sock, buf, BUFSIZE, 0);
		if(retval == SOCKET_ERROR){	err_display("recv()"); closesocket(sock); }
		else if(retval == 0) { closesocket(sock); WSACleanup(); return 0;}
		// ���� ������ ���
		buf[retval] = '\0';
		printf("[server message] %s\n", buf);

		//pw Ȯ��
		printf("\n[pw] ");
		if(fgets(buf, BUFSIZE+1, stdin) == NULL)closesocket(sock);
		// '\n' ���� ����
		len = strlen(buf);
		if(buf[len-1] == '\n')	buf[len-1] = '\0';
		if(strlen(buf) == 0) closesocket(sock);
		// ������ ������
		retval = send(sock, buf, strlen(buf), 0);
		if(retval == SOCKET_ERROR){	err_display("send()");closesocket(sock);}
		// ������ �ޱ�
		retval = recv(sock, buf, BUFSIZE, 0);
		if(retval == SOCKET_ERROR){	err_display("recv()"); closesocket(sock); }
		else if(retval == 0){ closesocket(sock); WSACleanup(); return 0;}
		// ���� ������ ���
		buf[retval] = '\0';
		printf("[server message] %s\n", buf);
	}	
	else if(atoi(buf) == 3)
	{
		// ������ �ޱ�
		retval = recv(sock, buf, BUFSIZE, 0);
		if(retval == SOCKET_ERROR){	err_display("recv()"); closesocket(sock); }
		else if(retval == 0) { closesocket(sock); WSACleanup(); return 0;}
		// ���� ������ ���
		buf[retval] = '\0';
		printf("%s\n", buf);
		system("pause");
	}
}

int main(int argc, char *argv[])
{
	int retval;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if(retval == SOCKET_ERROR) err_quit("connect()");

	// ������ ��ſ� ����� ����
	char buf[BUFSIZE+1];
	int len;
	if(!CHECKACCOUNT(sock)) return 0;

	// ������ ������ ���
	while(1){
		// ������ �Է�
		printf("\n[���� ������] ");
		if(fgets(buf, BUFSIZE+1, stdin) == NULL)
			break;

		// '\n' ���� ����
		len = strlen(buf);
		if(buf[len-1] == '\n')
			buf[len-1] = '\0';
		if(strlen(buf) == 0)
			break;

		// ������ ������
		retval = send(sock, buf, strlen(buf), 0);
		if(retval == SOCKET_ERROR){
			err_display("send()");
			break;
		}

		// ������ �ޱ�
		retval = recv(sock, buf, BUFSIZE, 0);
		if(retval == SOCKET_ERROR){
			err_display("recv()");
			break;
		}
		else if(retval == 0)
			break;

		// ���� ������ ���
		buf[retval] = '\0';
		printf("[���� ������] %s\n", buf);
	}
	// closesocket()
	closesocket(sock);

	// ���� ����
	WSACleanup();
	return 0;
}

