#pragma once
#define BUFSIZE    256
#define SERVERPORT 9000

DWORD WINAPI ProcessClient(LPVOID arg);
typedef struct ACCOUNT{
		char* id;
		char* pw;
		int check; //회원정보 여부
	};
	
	typedef struct SOCKETINFO{
		SOCKET sock;
		char buf[BUFSIZE];
		int recvbytes;
	};

class messanger{
public:	
	char buf[BUFSIZE+1];
	int usernumber;
	int connectnumber;
	ACCOUNT* wholeuser[10];
	SOCKETINFO* SocketinfoArray[FD_SETSIZE];
	CRITICAL_SECTION cs;

	messanger()
	{
		for(usernumber = 0; usernumber < 10 ; usernumber++)
		{
			wholeuser[usernumber] = (ACCOUNT*)malloc(sizeof(ACCOUNT));
			wholeuser[usernumber]->id = (char*)calloc(sizeof(char),BUFSIZE);
			wholeuser[usernumber]->pw = (char*)calloc(sizeof(char),BUFSIZE);
			wholeuser[usernumber]->check = -1;
		}
		usernumber = 0;
		connectnumber = 0;
		InitializeCriticalSection(&cs);
	}
	~messanger()
	{
		free(wholeuser);
		DeleteCriticalSection(&cs);
	}
		
	void err_quit(char *msg);
	void err_display(char *msg);
	
	BOOL AddSocketInfo(SOCKET sock);
	void RemoveSocketInfo(int nIndex);
};