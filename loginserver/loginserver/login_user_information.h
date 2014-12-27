#pragma once

#define SERVERPORT 9000
#define BUFSIZE    512

typedef struct userid{
	char* id;
	char* pw;
}ACCOUNT;

void err_quit(char *msg);
void err_display(char *msg);
DWORD WINAPI ProcessClient(LPVOID arg);
bool finduserdata(char* buf);