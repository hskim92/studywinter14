#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include"login_user_information.h"

ACCOUNT* wholeuser[10];
int usernumber = 0;

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
	int count=0;
	
	// get client information
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR *)&clientaddr, &addrlen);

	//get users choice
	char sendmodemessage[] = "choose system mode(1.signup, 2.login, 3.chaeck user)";
	retval = send(client_sock, sendmodemessage, strlen(sendmodemessage), 0);
	if(retval == SOCKET_ERROR){	err_display("recv()");	closesocket(client_sock); }

	retval = recv(client_sock, buf, BUFSIZE, 0);
	if(retval == SOCKET_ERROR){ err_display("recv()");	closesocket(client_sock); }
	else if(retval == 0)	closesocket(client_sock);
	buf[retval] = '\0';
	printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), buf);

	//signup mode
	if(atoi(buf) == 1)
	{
		++usernumber;
		//send message to client
		char sendidmessage[] = "Enter the ID";
		retval = send(client_sock, sendidmessage, strlen(sendidmessage), 0);
		if(retval == SOCKET_ERROR){	err_display("recv()");	closesocket(client_sock); }
	
		//receive new user id
		retval = recv(client_sock, buf, BUFSIZE, 0);
		if(retval == SOCKET_ERROR){ err_display("recv()");	closesocket(client_sock); }
		else if(retval == 0)	closesocket(client_sock);
		buf[retval] = '\0';
		//wholeuser[usernumber]->id = (char*)malloc(sizeof(char)*retval);
		strcpy(wholeuser[usernumber]->id, buf);

		//send message to client
		char sendpwmessage[] = "Enter the password";
		retval = send(client_sock, sendpwmessage, strlen(sendpwmessage), 0);
		if(retval == SOCKET_ERROR){	err_display("recv()");	closesocket(client_sock); }
	
		//receive new user pw
		retval = recv(client_sock, buf, BUFSIZE, 0);
		if(retval == SOCKET_ERROR){ err_display("recv()");	closesocket(client_sock); }
		else if(retval == 0)	closesocket(client_sock);
		buf[retval] = '\0';
		//wholeuser[usernumber]->pw=(char*)calloc(sizeof(char), retval);
		strcpy(wholeuser[usernumber]->pw, buf);
		wholeuser[usernumber]->count = 0;
	}
	//login mode
	else if(atoi(buf) == 2)
	{
		// input data
		retval = recv(client_sock, buf, BUFSIZE, 0);
		if(retval == SOCKET_ERROR){	err_display("recv()");	closesocket(client_sock);	}
		else if(retval == 0)	closesocket(client_sock);
	
		// print users data
		buf[retval] = '\0';
		printf("[TCP/%s:%d] id : %s\n", inet_ntoa(clientaddr.sin_addr),
		ntohs(clientaddr.sin_port), buf);
		
		//search
		while(strcmp(buf, wholeuser[usernumber]->id)!=0 && count<10)
		{
			usernumber = (usernumber+1) % 10;
			count++;
		}
		if( count == 10 )
			closesocket(client_sock);
		count=0;
	
		if(strcmp(buf, wholeuser[usernumber]->id) == 0)
		{
			char existusermsg[] = "Enter the user password";
			retval = send(client_sock, existusermsg, strlen(existusermsg), 0);
			if(retval == SOCKET_ERROR){	err_display("recv()");	closesocket(client_sock);	}
			retval = recv(client_sock, buf, BUFSIZE, 0);
			if(retval == SOCKET_ERROR){	err_display("recv()");	closesocket(client_sock);	}
			buf[retval] = '\0';
			printf("[TCP/%s:%d] pw : %s\n", inet_ntoa(clientaddr.sin_addr),
			ntohs(clientaddr.sin_port), buf);
			if(strcmp(buf, wholeuser[usernumber]->pw) == 0)
			{
				char loginsuccess[] = "login success";
				retval = send(client_sock, loginsuccess, strlen(loginsuccess), 0);
				if(retval == SOCKET_ERROR){	err_display("recv()");	closesocket(client_sock);	}
				while(1)
				{
					(wholeuser[usernumber]->count)++;
					retval = recv(client_sock, buf, BUFSIZE, 0);
					if(retval == SOCKET_ERROR){ err_display("recv()"); break;}
					else if(retval == 0) break;
					buf[retval] = '\0';
					printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
					ntohs(clientaddr.sin_port), buf);
					retval = send(client_sock, buf, strlen(buf), 0);
					if(retval == SOCKET_ERROR){	err_display("recv()");	closesocket(client_sock);	}
				}
				wholeuser[usernumber]->count = 0;
			}			
		}
	}
	//check mode
	else if(atoi(buf) == 3)
	{
		char checkusers[BUFSIZE] = "login users : ";
		//search
		while(count<10)
		{
			if(wholeuser[usernumber]->count > 0)
			{
				strcat(checkusers, wholeuser[usernumber]->id);
				strcat(checkusers, ", ");
			}
			usernumber = (usernumber+1) % 10;
			count++;
		}
		count=0;
		retval = send(client_sock, checkusers, strlen(checkusers), 0);
		if(retval == SOCKET_ERROR){	err_display("recv()");	closesocket(client_sock);	}
	}

	// closesocket()
	closesocket(client_sock);
	printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
		inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	return 0;
}