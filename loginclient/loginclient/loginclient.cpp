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

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
		return 1;

	//���� �ʱ�ȭ
	c.g_chatmsg.type = CHATTING;

	//�̺�Ʈ ����
	c.g_hReadEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	if(c.g_hReadEvent == NULL) return 1;
	c.g_hReadEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	if(c.g_hReadEvent == NULL) return 1;

	//��ȭ���� ����
	c.g_hInst = hInstance;
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);

	c.~client();

	// ���� ����
	WSACleanup();
	return 0;
}

