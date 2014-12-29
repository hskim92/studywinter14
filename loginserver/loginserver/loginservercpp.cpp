#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include"login_user_information.h"

#define SERVERPORT 9000
#define BUFSIZE    512
extern ACCOUNT* wholeuser[10];
extern int usernumber;

int main(int argc, char *argv[])
{
	int retval;
	for(usernumber = 0; usernumber < 10 ; usernumber++)
	{
		wholeuser[usernumber] = (ACCOUNT*)malloc(sizeof(ACCOUNT));
		wholeuser[usernumber]->id = (char*)malloc(sizeof(char)*BUFSIZE);
		wholeuser[usernumber]->pw = (char*)malloc(sizeof(char)*BUFSIZE);
	}
	usernumber = 0;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if(retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if(retval == SOCKET_ERROR) err_quit("listen()");

	// ������ ��ſ� ����� ����
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	HANDLE hThread;
	USER_HEAD* wholeuser = (USER_HEAD*)malloc(sizeof(USER_HEAD));

	while(1){
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if(client_sock == INVALID_SOCKET){	err_display("accept()");	break; 	}

		// ������ Ŭ���̾�Ʈ ���� ���
		printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
		
		hThread = CreateThread(NULL, 0, ProcessClient, (LPVOID)client_sock,
			0, NULL);
		if(hThread == NULL){closesocket(client_sock);}
		else { CloseHandle(hThread);}
	}

	// closesocket()
	closesocket(client_sock);
	printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
		inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	// closesocket()
	closesocket(listen_sock);

	// ���� ����
	WSACleanup();
	return 0;
}