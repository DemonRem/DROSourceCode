#ifndef _NETSERVICE_SUPPORT_H
#define _NETSERVICE_SUPPORT_H

#include "AntiCpSvrFunc.h"
#include "AntiCpXSvr.h"

#pragma pack(8)

typedef struct 
{
	unsigned int unPort;
	unsigned int unMaxBodyLen;
	PBOOL pbStop;

	// 
	PVOID	pJobProcessingFunc;
} NETTHREAD_PARAM, *PNETTHREAD_PARAM;

typedef struct
{
	DWORD		opCode;
	DWORD		length;
} MSGHEADER, *PMSGHEADER;

//	���Ӽ������� Ŭ���̾�Ʈ ���� �����Ǵ� ����ü(�Ǵ� Ŭ����)���� 
//  �������� TYPE�� ���� �ش� "Ŭ���̾�Ʈ ���� ��ü"�� �߰��մϴ�.
typedef struct
{
	//
	SOCKET					socket;
	SOCKADDR_IN				sockaddr;
	CRITICAL_SECTION		cs;

	// [�߰�][Ȯ�� ��������]		Ŭ���̾�Ʈ ���� ��ü 	
	AHNHS_CLIENT_HANDLE		hClient;

	// [�߰�][�� ��������]			Ŭ���̾�Ʈ ���� ��ü
	HSHIELD_CLIENT_CONTEXT	Context;
	BYTE					byClientInfo[SIZEOF_REQINFO];

} HANDLE_DATA, *PHANDLE_DATA;

typedef struct
{
	OVERLAPPED			overlapped;
	MSGHEADER			msgHeader;	
	int					nBufLen;
	int					nOpType;
	BOOL				bHeader;
	DWORD				dwTotalBytes;
	unsigned char*		szBuf;
} IO_DATA, *PIO_DATA;

#pragma pack()


UINT WINAPI NetworkThreadProc ( LPVOID lParam );
VOID IoWrite ( PHANDLE_DATA pHandleData, MSGHEADER *pHeader, PBYTE pBody, UINT unBodyLen );
VOID ShutDownSocket ( PIO_DATA pIoData, PHANDLE_DATA pHandleData );

// ���� ���
DWORD WINAPI BOANET_LoadWinsock2();
DWORD WINAPI BOANET_UnloadWinsock2();

// Client ���	
DWORD WINAPI BOANET_InitializeCli(OUT SOCKET* lpSocket, OUT int* nErrCode);
DWORD WINAPI BOANET_ConnectCliA(IN SOCKET socket, IN int nPort, IN PSTR szIP, OUT int* nErrCode);
DWORD WINAPI BOANET_ConnectCliW(IN SOCKET socket, IN int nPort, IN PWSTR szIP, OUT int* nErrCode);
DWORD WINAPI BOANET_CloseCli(IN SOCKET* lpSocket);
DWORD WINAPI BOANET_SendCli(IN SOCKET socket, IN PSTR lpBuf, IN int nLen, OUT int* nErrCode);
DWORD WINAPI BOANET_ReceiveCli(IN SOCKET socket, OUT PSTR lpBuf, IN int nLen, OUT int* nErrCode);
DWORD WINAPI BOANET_ServerAliveA(IN int nPort, IN PSTR szIP, OUT int* nErrCode);
DWORD WINAPI BOANET_ServerAliveW(IN int nPort, IN PWSTR szIP, OUT int* nErrCode);

// Server ���
DWORD WINAPI BOANET_InitializeSvr(IN int nPort, IN int nBackLog, OUT SOCKET* lpListenSocket, OUT int* nErrCode);
DWORD WINAPI BOANET_ConnectSvr(IN SOCKET ListenSocket, OUT SOCKET* lpClientSocket, OUT SOCKADDR_IN* lpClientAddr, OUT int* nErrCode);
DWORD WINAPI BOANET_CloseSvr(IN SOCKET* lpSocket);
DWORD WINAPI BOANET_SendSvr(IN SOCKET socket, IN LPWSABUF lpBuf, OUT LPWSAOVERLAPPED lpOverlapped, OUT int* nErrCode);
DWORD WINAPI BOANET_ReceiveSvr(IN SOCKET socket, OUT LPWSABUF lpBuf, OUT LPWSAOVERLAPPED lpOverlapped, OUT int* nErrCode);

#ifdef UNICODE
	#define BOANET_ConnectCli BOANET_ConnectCliW
#else
	#define BOANET_ConnectCli BOANET_ConnectCliA
#endif

#ifdef UNICODE
	#define BOANET_ServerAlive BOANET_ServerAliveW
#else
	#define BOANET_ServerAlive BOANET_ServerAliveA
#endif

#endif
