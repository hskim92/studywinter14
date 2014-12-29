#pragma once

#define SERVERPORT 9000
#define BUFSIZE    512

typedef struct ACCOUNT{
	char* id;
	char* pw;
};

typedef struct USER_NODE{
	struct ACCOUNT* useraccount;
	struct useraccountlistnode* next;
};

typedef struct USER_HEAD{
	int count;
	struct USER_NODE* head;
};

void err_quit(char *msg);
void err_display(char *msg);
DWORD WINAPI ProcessClient(LPVOID arg);
DWORD WINAPI SignupClient(LPVOID arg);