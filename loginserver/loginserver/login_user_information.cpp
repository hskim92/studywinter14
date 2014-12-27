#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include"login_user_information.h"

// case identityerror : print error and exit
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
	
	//userid
	ACCOUNT* user1 = (ACCOUNT*)malloc(sizeof(ACCOUNT));
	char user1id[] = "hskim";
	char user1pw[] = "mobile";
	user1->id = user1id;
	user1->pw = user1pw;
	printf("user1 id: %s, pw: %s \n", user1->id, user1->pw);

	// get client information
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR *)&clientaddr, &addrlen);

		// input data
		retval = recv(client_sock, buf, BUFSIZE, 0);
		if(retval == SOCKET_ERROR){
			err_display("recv()");
			closesocket(client_sock);
		}
		else if(retval == 0)
			closesocket(client_sock);

		// print users data
		buf[retval] = '\0';
			printf("[TCP/%s:%d] id : %s\n", inet_ntoa(clientaddr.sin_addr),
				ntohs(clientaddr.sin_port), buf);

			if(strcmp(buf, user1->id) == 0)
			{
				char existusermsg[] = "Enter the user password";
				retval = send(client_sock, existusermsg, strlen(existusermsg), 0);
				if(retval == SOCKET_ERROR){	err_display("recv()");	closesocket(client_sock);	}
				retval = recv(client_sock, buf, BUFSIZE, 0);
				if(retval == SOCKET_ERROR){	err_display("recv()");	closesocket(client_sock);	}
				buf[retval] = '\0';
				printf("[TCP/%s:%d] pw : %s\n", inet_ntoa(clientaddr.sin_addr),
				ntohs(clientaddr.sin_port), buf);
				if(strcmp(buf, user1->pw) == 0)
				{
					char loginsuccess[] = "login success";
					retval = send(client_sock, loginsuccess, strlen(loginsuccess), 0);
					if(retval == SOCKET_ERROR){	err_display("recv()");	closesocket(client_sock);	}
					while(1)
					{
						retval = recv(client_sock, buf, BUFSIZE, 0);
						if(retval == SOCKET_ERROR){ err_display("recv()");
							break;}
						else if(retval == 0) break;
						buf[retval] = '\0';
						printf("[TCP/%s:%d] id : %s\n", inet_ntoa(clientaddr.sin_addr),
						ntohs(clientaddr.sin_port), buf);

						retval = send(client_sock, buf, strlen(buf), 0);
						if(retval == SOCKET_ERROR){	err_display("recv()");	closesocket(client_sock);	}
					}
				}			
			}
	// closesocket()
	closesocket(client_sock);
	printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
		inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	return 0;
}