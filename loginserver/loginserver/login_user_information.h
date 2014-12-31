#pragma once

#define SERVERPORT 9000
#define BUFSIZE    512

typedef struct ACCOUNT{
	char* id;
	char* pw;
	int count;
};

typedef struct USER_NODE{
	struct ACCOUNT* useraccount;
	struct useraccountlistnode* next;
};

void err_quit(char *msg);
void err_display(char *msg);
DWORD WINAPI ProcessClient(LPVOID arg);
DWORD WINAPI SignupClient(LPVOID arg);