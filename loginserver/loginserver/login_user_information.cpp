#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include"login_user_information.h"

void inituserid()
{

	userid* user1= (userid*)malloc(sizeof(userid));
userid* user2= (userid*)malloc(sizeof(userid));
userid* user3= (userid*)malloc(sizeof(userid));
userid* user4= (userid*)malloc(sizeof(userid));

	char user1id[] = "hskim";
	char user1password[] = "hskim";
	user1->identity = user1id;
	user1->password = user1password;

	char user2id[] = "jpark";
	char user2password[] = "jpark";
	user2->identity = user2id;
	user2->password = user2password;

	char user3id[] = "jhsong";
	char user3password[] = "jhsong";
	user3->identity = user3id;
	user3->password = user3password;

	char user4id[] = "jschoi";
	char user4password[] = "jschoi";
	user4->identity = user4id;
	user4->password = user4password;
}

// case error : print error and exit
void err_quit(char *msg)
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

// connecting with client
DWORD WINAPI ProcessClient(LPVOID arg)
{
	SOCKET client_sock = (SOCKET)arg;
	int retval;
	SOCKADDR_IN clientaddr;
	int addrlen;
	char buf[BUFSIZE+1];

	// get client information
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR *)&clientaddr, &addrlen);

	while(1){
		// input data
		retval = recv(client_sock, buf, BUFSIZE, 0);
		if(retval == SOCKET_ERROR){
			err_display("recv()");
			break;
		}
		else if(retval == 0)
			break;

		// print users data
		buf[retval] = '\0';
		printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
			ntohs(clientaddr.sin_port), buf);

		// 데이터 보내기
		retval = send(client_sock, buf, retval, 0);
		if(retval == SOCKET_ERROR){
			err_display("send()");
			break;
		}
	}

	// closesocket()
	closesocket(client_sock);
	printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
		inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	return 0;
}