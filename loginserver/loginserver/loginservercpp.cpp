#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include"login_user_information.h"

#define SERVERPORT 9000
#define BUFSIZE    256
extern messanger m;

int main(int argc, char *argv[])
{
	int retval;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(listen_sock == INVALID_SOCKET) m.err_quit("socket()");

	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if(retval == SOCKET_ERROR) m.err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if(retval == SOCKET_ERROR) m.err_quit("listen()");

	// ������ ��ſ� ����� ����
	FD_SET rset;
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen, i, j;

	while(1)
	{
		//initialized socket set
		FD_ZERO(&rset);
		FD_SET(listen_sock, &rset);
		for(i = 0; i < m.connectnumber; i++)
			FD_SET(m.SocketinfoArray[i]->sock,&rset);

		//select()
		retval = select(0, &rset, NULL, NULL, NULL);
		if(retval == SOCKET_ERROR){	m.err_display("select()");	break;}

		//���� �� �˻�(1) : Ŭ���̾�Ʈ ���� ����
		if(FD_ISSET(listen_sock, &rset))
		{	addrlen = sizeof(clientaddr);
			client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
			if(client_sock == INVALID_SOCKET){m.err_display("accept()");	break; 	}
			else
			{	printf("[TCP ����] Ŭ���̾�Ʈ ���� : [%s]:%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
				m.AddSocketInfo(client_sock);
			}
		}

		//���� �� �˻�(2): ������ ���
		for(i = 0; i < m.connectnumber; i++)
		{
			SOCKETINFO* ptr = m.SocketinfoArray[i];
			if(FD_ISSET(ptr->sock, &rset))
			{
				//������ �ޱ�
				retval = recv(ptr->sock, ptr->buf + ptr->recvbytes, BUFSIZE - ptr->recvbytes, 0);
				if(retval == 0 || retval == SOCKET_ERROR){	m.RemoveSocketInfo(i);	continue;	}
									
				//����Ʈ �� ����
				ptr->recvbytes = 0;

				//������ ����
				for(j = 0; j < m.connectnumber; j++)
				{	SOCKETINFO *ptr2 = m.SocketinfoArray[j];
					retval = send(ptr2->sock, ptr->buf, BUFSIZE, 0);
					if(retval == SOCKET_ERROR)	{	m.err_display("send()");	m.RemoveSocketInfo(j);	--j;	continue;	}
				}
			}
		}
	}
	return 0;
}