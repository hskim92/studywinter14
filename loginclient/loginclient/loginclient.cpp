#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include "resource.h"
#include "messengerclient.h"

extern client c;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
		return 1;

	//변수 초기화
	c.g_chatmsg.type = CHATTING;

	//이벤트 생성
	c.g_hReadEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	if(c.g_hReadEvent == NULL) return 1;
	c.g_hReadEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	if(c.g_hReadEvent == NULL) return 1;

	//대화상자 생성
	c.g_hInst = hInstance;
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);

	c.~client();

	// 윈속 종료
	WSACleanup();
	return 0;
}

