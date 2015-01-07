#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include"login_user_information.h"

messanger m = messanger();

// case identityerror : print error and exit
void messanger::err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCTSTR)msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// print error
void messanger::err_display(char *msg)
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

//add socket information
BOOL messanger::AddSocketInfo(SOCKET sock)
{
	if(m.connectnumber >= FD_SETSIZE)
	{
		printf("[ERROR] ���������� �߰��� �� �����ϴ�.\n");
		return FALSE;
	}

	SOCKETINFO* ptr = new SOCKETINFO;
	if(ptr == NULL)
	{
		printf("[ERRPR] �޸� ����\n");
		return FALSE;
	}

	ptr->sock = sock;
	ptr->recvbytes = 0;
	m.SocketinfoArray[m.connectnumber++] = ptr;

	return TRUE;
}

//remove socket information
void messanger::RemoveSocketInfo(int nIndex)
{
	SOCKETINFO* ptr = m.SocketinfoArray[nIndex];

	//���� client info print
	SOCKADDR_IN clientaddr;
	int addrlen = sizeof(clientaddr);
	getpeername(ptr->sock, (SOCKADDR*)&clientaddr, &addrlen);
	printf("[TCPv4 server] client ���� : [%s]:%d\n",
		inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	closesocket(ptr->sock);
	delete ptr;
	
	//array ���ڸ� ä���
	if(nIndex != (m.connectnumber - 1))
		m.SocketinfoArray[nIndex] = m.SocketinfoArray[m.connectnumber-1];

	--m.connectnumber;
}