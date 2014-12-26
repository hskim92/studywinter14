#pragma once

#define SERVERPORT 9000
#define BUFSIZE    512

typedef struct userid{
	void* identity;
	void* password;
};



void inituserid();
void err_quit(char *msg);
void err_display(char *msg);
DWORD WINAPI ProcessClient(LPVOID arg);