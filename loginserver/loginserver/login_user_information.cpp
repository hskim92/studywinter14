#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include"login_user_information.h"

messanger m = messanger();

// case identityerror : print error and exit
void messanger::err_quit(char *msg)
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
void messanger::err_display(char *msg)
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

//add socket information
BOOL messanger::AddSocketInfo(SOCKET sock)
{
	if(m.connectnumber >= FD_SETSIZE)
	{
		printf("[ERROR] 소켓정보를 추가할 수 없습니다.\n");
		return FALSE;
	}

	SOCKETINFO* ptr = new SOCKETINFO;
	if(ptr == NULL)
	{
		printf("[ERRPR] 메모리 부족\n");
		return FALSE;
	}

	ptr->sock = sock;
	ptr->recvbytes = 0;
	m.SocketinfoArray[m.connectnumber++] = ptr;

	return TRUE;
}

//remove socket information
void messanger::RemoveSocketInfo(int nIndex)
{
	SOCKETINFO* ptr = m.SocketinfoArray[nIndex];

	//종료 client info print
	SOCKADDR_IN clientaddr;
	int addrlen = sizeof(clientaddr);
	getpeername(ptr->sock, (SOCKADDR*)&clientaddr, &addrlen);
	printf("[TCPv4 server] client 종료 : [%s]:%d\n",
		inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	closesocket(ptr->sock);
	delete ptr;
	
	//array 빈자리 채우기
	if(nIndex != (m.connectnumber - 1))
		m.SocketinfoArray[nIndex] = m.SocketinfoArray[m.connectnumber-1];

	--m.connectnumber;
}
// 컨트롤 초기화
		SendMessage(hEditMsg, EM_SETLIMITTEXT, MSGSIZE, 0);
		EnableWindow(g_hButtonSendMsg, FALSE);
		SetDlgItemText(hDlg, IDC_IPADDR, SERVERIPV4);
		SetDlgItemInt(hDlg, IDC_PORT, SERVERPORT, FALSE);
		SendMessage(hColorRed, BM_SETCHECK, BST_CHECKED, 0);
		SendMessage(hColorGreen, BM_SETCHECK, BST_UNCHECKED, 0);
		SendMessage(hColorBlue, BM_SETCHECK, BST_UNCHECKED, 0);

		// 윈도우 클래스 등록
		WNDCLASS wndclass;
		wndclass.style = CS_HREDRAW|CS_VREDRAW;
		wndclass.lpfnWndProc = WndProc;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hInstance = g_hInst;
		wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		wndclass.lpszMenuName = NULL;
		wndclass.lpszClassName = "MyWndClass";
		if(!RegisterClass(&wndclass)) return 1;

		// 자식 윈도우 생성
		g_hDrawWnd = CreateWindow("MyWndClass", "그림 그릴 윈도우", WS_CHILD,
			450, 38, 425, 415, hDlg, (HMENU)NULL, g_hInst, NULL);
		if(g_hDrawWnd == NULL) return 1;
		ShowWindow(g_hDrawWnd, SW_SHOW);
		UpdateWindow(g_hDrawWnd);

		return TRUE;
