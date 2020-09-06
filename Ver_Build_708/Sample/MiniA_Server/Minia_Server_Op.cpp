#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>
#include <tchar.h>

#include "Minia_Server_Op.h"
#include "NetSvMngr.h"
#include "AntiCpSvrFunc.h"

DWORD m_dwCheckInterval;
BOOL m_bUseAntiCpX;
int m_nFirstCheckCrcOpt;
int m_nNextCheckCrcOpt;
AHNHS_SERVER_HANDLE	m_hServer;

//
/*******************************************************************************
*	Receive�� �������� ��������� �̿��Ͽ� � �۾��� ���ϴ��� Ȯ���� ��
*	������ �۾��� �����Ѵ�.

* Parameters: 
*   pIoData : Client�κ��� ���޹��� ������
*	pHandleData : Socket Handle�� ���õ� ������
* 
* Return: 
*	NONE
*******************************************************************************/

DWORD WINAPI JobProcessing (IN PIO_DATA pIoData, 	IN PHANDLE_DATA pHandleData)
{
	DWORD dwRet = ERROR_SUCCESS;

	switch ( pIoData->msgHeader.opCode )
	{
	// Ŭ���̾�Ʈ �α��� 
	case OP_LOGON:
		dwRet = Op_ClientLogon ( pIoData, pHandleData);
		break;
	// Ŭ���̾�Ʈ �α׿���
	case OP_LOGOFF:
		Op_ClientLogoff ( pIoData, pHandleData );
		break;	
	// Ŭ���̾�Ʈ ACK-GUID �޼���
	case OP_ACKGUID:
		{
			// [Ȯ�� ��������] Ȯ�� ����������, GuidAck �� CrcAck �� ������ ����.
			if ( m_bUseAntiCpX )
				dwRet = Op_CrcAck ( pIoData, pHandleData);
			// [�� ��������]
			else
				dwRet = Op_GuidAck ( pIoData, pHandleData );			
		}		
		break;
	// Ŭ���̾�Ʈ ACK-CRC �޼��� 
	case OP_ACKCRC:
		dwRet = Op_CrcAck ( pIoData, pHandleData);
		break;

	// ������: Ŭ���̾�Ʈ�κ� �����ð��ȿ� ����޼����� �������� ���ϴ� ���,
	//		   �ش� Ŭ���̾�Ʈ ���� ������ �����ؾ��Ѵ�.

	default:
		break;
	}

	return dwRet;
}

// Ŭ���̾�Ʈ�� GUID�� ���;� �Ѵ�.
DWORD Op_ClientLogon (IN PIO_DATA pIoData, IN PHANDLE_DATA pHandleData)
{
	BYTE byGuidReqMsg[SIZEOF_GUIDREQMSG] = { 0, };	// [�� ��������] Ŭ���̾�Ʈ�� �����ϰ� �� ����
	AHNHS_TRANS_BUFFER tbRequest;					// [Ȯ�� ��������] Ŭ���̾�Ʈ�� �����ϰ� �� ����
	DWORD dwRet = ERROR_SUCCESS;
	MSGHEADER header = { 0, 0 };
	
	// [Ȯ�� ��������]
	if ( m_bUseAntiCpX )
	{
		// ------------------------------------------------------------------------
		// [_AhnHS_CreateClientObject ]
		// ------------------------------------------------------------------------
		//  - DESCRIPTION:
		//    ���� �ڵ��� �Է� �޾� Ŭ���̾�Ʈ �ڵ��� �����մϴ�. Ŭ���̾�Ʈ �ڵ��� 
		//    Ŭ���̾�Ʈ�� ������ �� ���� �����ϸ�, ������ �����Ǵ� ���� �ڵ��� ���� �ʰ� �����մϴ�.
		//
		//  - SYNTAX:
		//    AHNHS_CLIENT_HANDLE _AhnHS_CreateClientObject (IN AHNHS_SERVER_HANDLE hServer);
		//
		//  - PARAMETERS:
		//    hServer	  :AHNHS_SERVER_HANDLE	���� �ڵ�(Server Handle) 
		//
		//  - RETURN VALUE
		//    Ŭ���̾�Ʈ �ڵ�(Client Handle)

		pHandleData->hClient = _AhnHS_CreateClientObject ( m_hServer );

		if ( pHandleData->hClient == ANTICPX_INVALID_HANDLE_VALUE )
		{
			dwRet = ERROR_INVALID_HANDLE;
		}
		else 
		{
			// -----------------------------------------------------------
			// [_AhnHS_MakeRequest ]
			// -----------------------------------------------------------
			// - DESCRIPTION:
			//    ���� ���ǿ� �´� Ŭ���̾�Ʈ �ڵ��� �Է��Ͽ� ��û �޽����� �����մϴ�. 
			//    ��û �޽����� AHNHS_TRANS_BUFFER ����ü ���·� ��µǸ�, 
			//    ��� ���� ���� ������ �����ϴ�.
			//    typedef struct _AHNHS_TRANS_BUFFER
			//    {
			//      	unsigned short nLength;			// ��û �޽��� ������ ���� ���� ����
			//          unsigned char byBuffer[ANTICPX_TRANS_BUFFER_MAX]; // ��û �޽��� ������ ���� �� �ִ� �ִ� ����Ʈ ����
			//    } AHNHS_TRANS_BUFFER, *PAHNHS_TRANS_BUFFER;
			//
			//  ������:
			//    byBuffer�� ��û �޽��� ������ ���� �� �ִ� �ִ� ���� ũ���̹Ƿ� ��Ʈ��ũ�� ������ �� nLength 
			//    ��ŭ�� �����ؾ� �մϴ�.
			//
			// - SYNTAX:
			//   unsigned long __stdcall _AhnHS_MakeRequest (IN AHNHS_CLIENT_HANDLE hClient, 
			//												OUT PAHNHS_TRANS_BUFFER pRequestBuffer);
			// - PARAMETERS:
			//   .hClient			:AHNHS_CLIENT_HANDLE	Ŭ���̾�Ʈ �ڵ�
			//   .pRequestBuffer	:PAHNHS_TRANS_BUFFER	���� ������ ����/����
			// 
			// - RETURN VALUE:
			//   . ERROR_SUCCESS:
			//	   �Լ� ȣ���� �������� �� �����ϴ� ���Դϴ�. (Value = 0x00000000)
			//   . ��Ÿ				: ��Ÿ ���ϰ��� ���ؼ��� �ٽ��� ���α׷��� ���̵� ����ٶ�.


			dwRet = _AhnHS_MakeRequest( pHandleData->hClient, &tbRequest );

			TCHAR szMsg[256]={0,};
			_stprintf(szMsg, "##_AhnHS_MakeRequest #1 result:%x\r\n", dwRet);
			OutputDebugString(szMsg);
		}
	}
	// [�� ��������]
	else
	{
		// ----------------------------------------------------
		// [_AntiCpSvr_MakeGuidReqMsg]
		// ----------------------------------------------------
		// - DESCRIPTION:
		//   Ŭ���̾�Ʈ�� ������ ��ȣȭ�� ���� �䱸 �޽����� �����մϴ�.
		//
		// - SYNTAX:
		//	unsigned long __stdcall _AntiCpSvr_MakeGuidReqMsg (	OUT unsigned char *pbyGuidReqMsg,
		//														OUT unsigned char *pbyGuidReqInfo)
		// - PARAMETERS:
		//   . pbyGuidReqMsg	unsigned char *				: Ŭ���̾�Ʈ�� ������ ��ȣȭ�� ���� �䱸 �޽����� ������ ũ��� 
		//													  AntiCpSvrFunc.h�� ���ǵ�  SIZEOF_GUIDREQMSG�̾�� �մϴ�. 
		//   . pbyGuidReqInfo	unsigned char *				: _AntiCpSvr_AnalyzeGuidAckMsg �Լ����� �м��� �� �ʿ��� ���� 
		//												      �䱸 �޽��� ������ ������ ũ��� AntiCpSvrFunc.h�� ���ǵ�
		//												      SIZEOF_GUIDREQINFO�̾�� �մϴ�.
		// - RETURN VALUE:
		//   . ERROR_SUCCESS								: �Լ� ȣ���� �������� �� �����ϴ� ���Դϴ�. (Value = 0x00000000)
		//   . ��Ÿ											: ��Ÿ ���ϰ��� ���ؼ��� �ٽ��� ���α׷��� ���̵� ����ٶ�.

		dwRet = _AntiCpSvr_MakeGuidReqMsg ( byGuidReqMsg, pHandleData->byClientInfo );
	}	
	
	if ( dwRet != ERROR_SUCCESS )
	{
		TCHAR szMsg[MAX_PATH] = { 0, };

		_stprintf ( szMsg, TEXT ( "Minia_Server : _AntiCpSvr_MakeGuidReqMsg Fail ( 0x%X ) IP : %d.%d.%d.%d\t%d\n" ),
					dwRet,
					pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b1,
					pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b2,
					pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b3,
					pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b4,
					ntohs ( pHandleData->sockaddr.sin_port ) );

		OutputDebugString ( szMsg );

		header.opCode = OP_ERROR;
		header.length = htonl ( dwRet );

		// 
		IoWrite ( pHandleData, &header, NULL, 0 );
	}
	else
	{
		// [Ȯ�� ��������]	tbRequest�� Ŭ���̾�Ʈ�� �۽���.
		if ( m_bUseAntiCpX )
		{
			header.opCode = OP_REQGUID;
			header.length = htonl ( tbRequest.nLength );

			IoWrite ( pHandleData, &header, tbRequest.byBuffer, tbRequest.nLength );
		}
		// [�� ��������] byGuidReqMsg�� Ŭ���̾�Ʈ �۽���.
		else
		{
			header.opCode = OP_REQGUID;
			header.length = htonl ( SIZEOF_GUIDREQMSG );

			IoWrite ( pHandleData, &header, byGuidReqMsg, SIZEOF_GUIDREQMSG );
		}
	}

	return dwRet;
}


VOID
Op_ClientLogoff (
	IN PIO_DATA pIoData,
	IN PHANDLE_DATA pHandleData
	)
{

	pIoData;

	// [Ȯ�� ��������]
	if ( m_bUseAntiCpX == TRUE )
	{
		// ----------------------------------------------------------
		// [_AhnHS_CloseClientHandle]
		// ----------------------------------------------------------
		// - DESCRIPTION:
		//   ������ Ŭ���̾�Ʈ �ڵ��� Ŭ���̾�Ʈ ������ ����� �� �����Ǿ�� �մϴ�. 
		//   �̶� Ŭ���̾�Ʈ �ڵ� ������ ���Ǿ��� �޸𸮳� �ý��� �ڿ��� �����ϰ� �˴ϴ�.
		//
		// - SYNTAX:
		//	 void __stdcall_AhnHS_CloseClientHandle (IN AHNHS_CLIENT_HANDLE hClient)
		//
		// - PARAMETERS:
		//   hClient	:AHNHS_CLIENT_HANDLE	Ŭ���̾�Ʈ �ڵ�(Client Handle)
		//
		// - RETURN VALUE:
		//   ����.
		//
		// - ������ : Ŭ���̾�Ʈ �ڵ��� ��� ������ ���� �����ڵ��� �����Ǿ�� �Ѵ� 

		_AhnHS_CloseClientHandle ( pHandleData->hClient );
		pHandleData->hClient = ANTICPX_INVALID_HANDLE_VALUE;
	}

	ShutDownSocket ( pIoData, pHandleData );
}


// [�� ��������]
DWORD
Op_GuidAck (
	IN PIO_DATA pIoData,
	IN PHANDLE_DATA pHandleData
	)
{
	BYTE byReqMsg[SIZEOF_REQMSG] = { 0, };
	DWORD dwRet = ERROR_SUCCESS;
	MSGHEADER header = { 0, 0 };

	// [�� ��������]
	// -----------------------------------------------------------
	// [_AntiCpSvr_AnalyzeGuidAckMsg]
	// ------------------------------------------------------------
	// - DESCRIPTION:
	//   Ŭ���̾�Ʈ���� ������ ��ȣȭ�� ���� ���� �޽����� ��ȣȭ�Ͽ� 
	//   ���� ����ϰ� �ִ� Ŭ���̾�Ʈ ������ �������� ����ϴ� �������� Ȯ���մϴ�.
	//
	// - SYNTAX:
	//	 unsigned long __stdcall _AntiCpSvr_AnalyzeGuidAckMsg (	IN unsigned char *pbyGuidAckMsg,
	//															IN unsigned char *pbyGuidReqInfo,
	//															OUT unsigned long **ppCrcInfo)
	// - PARAMETERS:
	//	.pbyAckMsg	unsigned char *							:Ŭ���̾�Ʈ���� ���� ��ȣȭ�� ���� ���� �޽���
	//	.pbyGuidReqInfo	unsigned char *						:_AntiCpSvr_MakeGuidReqMsg �Լ����� ������ ���� 
	//														 �䱸 �޽����� ���� ����
	//	.ppCrcInfo	Unsigned long **						:�ش� Ŭ���̾�Ʈ�� ����� CRC ������ ���� �޸� 
	//														 �����͸� ������ ���� ����
	//
	// - RETURN VALUE:
	//	 .ERROR_SUCCESS:
	//    �Լ� ȣ���� �������� �� �����ϴ� ���Դϴ�. (Value = 0x00000000) ��� ������ ���� Ŭ���̾�Ʈ���� ��Ÿ���ϴ�.
	//   . ��Ÿ												: ��Ÿ ���ϰ��� ���ؼ��� �ٽ��� ���α׷��� ���̵� ����ٶ�.
	//
	//   . ����             : ERROR_SUCCESS�� �ƴ� ���� �ڵ尡 ���ϵǾ��� ��쿡�� Ŭ���̾�Ʈ���� ������ ��� 
	//                        ������ �� �̻� ������ �� ������ �� ���� �����մϴ�.

	dwRet = _AntiCpSvr_AnalyzeGuidAckMsg ( pIoData->szBuf, pHandleData->byClientInfo, &pHandleData->Context );

	// ERROR_SUCCESS�� �ƴ� ��� ����-Ŭ���̾�Ʈ�� ���� ������ ���Ḧ �����մϴ�.
	if ( dwRet != ERROR_SUCCESS )
	{
		TCHAR szMsg[MAX_PATH] = { 0, };

		_stprintf ( szMsg, TEXT ( "Minia_Server : _AntiCpSvr_AnalyzeGuidAckMsg Fail ( 0x%X ) IP : %d.%d.%d.%d\t%d\n" ),
					dwRet,
					pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b1,
					pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b2,
					pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b3,
					pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b4,
					ntohs ( pHandleData->sockaddr.sin_port ) );

		
		OutputDebugString ( szMsg );

		header.opCode = OP_ERROR;
		header.length = htonl ( dwRet );

		IoWrite ( pHandleData, &header, NULL, 0 );
	}
	else
	{
		ZeroMemory ( pHandleData->byClientInfo, SIZEOF_REQINFO );
		// [�� ��������]
		// ---------------------------------------------------------------
		// [_AntiCpSvr_MakeReqMsg]
		// ---------------------------------------------------------------
		// - DESCRIPTION:
		//   Ŭ���̾�Ʈ�� ������ ��ȣȭ�� CRC �䱸 �޽����� �����մϴ�.
		// - SYNTAX:
		//	DWORD __stdcall _AntiCpSvr_MakeReqMsg (	IN unsigned long *pCrcInfo,
		//											OUT unsigned char *pbyReqMsg,
		//											OUT unsigned char *pbyReqInfo,
		//											IN unsigned long ulOption)
		// - PARAMETERS:
		//   . pCrcInfo	unsigned long *		:  _AntiCpSvr_AnalyzeGuidAckMsg �Լ����� ���ϵ� �ش� Ŭ���̾�Ʈ�� 
		//										����� CRC ������ ���� ������
		//   . pbyReqMsg	unsigned char * :	Ŭ���̾�Ʈ�� ������ ��ȣȭ�� CRC �䱸 �޽���, ������ ũ��� 
		//										AntiCpSvrFunc.h�� ���ǵ� SIZEOF_REQMSG�̾�� �մϴ�. 
		//   . pbyReqInfo	unsigned char *	:	_AntiCpSvr_AnalyzeAckMsg �Լ����� �м��� �� �ʿ��� ���� CRC 
		//										�䱸 �޽��� ������ ������ ũ��� AntiCpSvrFunc.h�� ���ǵ� 
		//										SIZEOF_REQINFO�̾�� �մϴ�.
		//   . ulOption	unsigned long		:  _AntiCpSvr_AnalyzeAckMsg �Լ����� �м��� �� � �����鿡 ���� 
		//										Request Message�� �������� ���� Flag�� AntiCpSvrFunc.h�� 
		//										ANTICPSVR_CHECK_GAME_MEMORY, ANTICPSVR_CHECK_HACKSHIELD_FILE,
		//										ANTICPSVR_CHECK_GAME_FILE, ANTICPSVR_CHECK_ENGINE_FILE,
		//										ANTICPSVR_CHECK_ALL�� ���ǵǾ� �ֽ��ϴ�. ��, ������ ���� ���� ȣ�� 
		//										�ô� ANTICPSVR_CHECK_ALL Option�� �̿��Ͽ� ��ü�� ���� ���� ������ 
		//										�˻��ϰ� �� �������ʹ� Performance�� ���� ANTICPSVR_CHECK_GAME_MEMORY 
		//										Option�� ����ϱ� �����մϴ�. �����, ���� ����(.v3d)�� ���������� 
		//										�˻��ϰ� �ʹٸ� ANTICPSVR_CHECK_ALL | ANTICPSVR_CHECK_ENGINE_FILE�� 
		//										�����ϰ� ����Ͻʽÿ�. ��, ���� ���� (.v3d)�� ��ü�� ������ 
		//										HackShield.crc ������ �ٽ� �����Ͽ� ������ �ε��ؾ� �մϴ�.
		//
		// - RETURN VALUE:
		//    .ERROR_SUCCESS		: �Լ� ȣ���� �������� �� �����ϴ� ���Դϴ�. (Value = 0x00000000)
		//    . ��Ÿ				: ��Ÿ ���ϰ��� ���ؼ��� �ٽ��� ���α׷��� ���̵� ����ٶ�.

		// �� ����: ���� ���ɿ� ������ ���� �ʵ��� ó�� �ѹ��� ANTICPSVR_CHECK_ALL �ɼ��� ����ϰ�
		//          �� ���� ���ʹ� ANTICPSVR_CHECK_GAME_MEMORY �ɼ��� �����ؼ� ����ؾ� �Ѵ�.
		//          ��:) m_nFirstCheckCrcOpt = ANTICPSVR_CHECK_ALL 
		//               m_nNextCheckCrcOpt = ANTICPSVR_CHECK_GAME_MEMORY
		dwRet = _AntiCpSvr_MakeReqMsg ( &pHandleData->Context, byReqMsg, pHandleData->byClientInfo, m_nFirstCheckCrcOpt );

		if ( dwRet != ERROR_SUCCESS )
		{
			TCHAR szMsg[MAX_PATH] = { 0, };

			_stprintf ( szMsg, TEXT ( "Minia_Server : _AntiCpSvr_MakeReqMsg Fail ( 0x%X ) IP : %d.%d.%d.%d\t%d\n" ),
						dwRet,
						pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b1,
						pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b2,
						pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b3,
						pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b4,
						ntohs ( pHandleData->sockaddr.sin_port ) );

			OutputDebugString ( szMsg );

			header.opCode = OP_ERROR;
			header.length = htonl ( dwRet );

			IoWrite ( pHandleData, &header, NULL, 0 );
		}
		else
		{
			header.opCode = OP_REQCRC;
			header.length = htonl ( SIZEOF_REQMSG );

			IoWrite ( pHandleData, &header, byReqMsg, SIZEOF_REQMSG );
		}
	}

	return dwRet;
}


DWORD
Op_CrcAck (
	IN PIO_DATA pIoData,
	IN PHANDLE_DATA pHandleData
	)
{
	BYTE byReqMsg[SIZEOF_REQMSG] = { 0, };
	DWORD dwRet = ERROR_SUCCESS;
	MSGHEADER header = { 0, 0 };
	TCHAR szMsg[MAX_PATH] = { 0, };
	AHNHS_TRANS_BUFFER ptsRequestBuf;
	BOOL bVerify = FALSE;

	// [Ȯ�� ��������]
	if ( m_bUseAntiCpX )
	{
		// -----------------------------------------------------------------
		// [_AhnHS_VerifyResponse ]
		// -----------------------------------------------------------------
		// - DESCRIPTION:
		//  _AhnHS_MakeRequest �Լ��� ���� ��û(�޽���)�� ���� Ŭ���̾�Ʈ�� 
		//   ������ �ùٸ��� �˻��ϴ� �Լ��Դϴ�.
		//
		// - SYNTAX:
		//  unsigned long __stdcall _AhnHS_VerifyResponse (IN AHNHS_CLIENT_HANDLE hClient,
		//													IN unsigned char *pbyResponse,
		//												IN unsigned long nResponseLength)
		// - PARAMETERS:
		//  .hClient		AHNHS_CLIENT_HANDLE 	:Ŭ���̾�Ʈ �ڵ�
		//	.pbyResponse	char *					:Ŭ���̾�Ʈ�� ���� ���� ������ ����
		//  .nResponseLength	unsigned long		:Ŭ���̾�Ʈ�� ���� ���� ������ ����
		//
		// - RETURN VALUE:
		//   . ERROR_SUCCESS    :
		//     �Լ� ȣ���� �������� �� �����ϴ� ���Դϴ�. (Value = 0x00000000) ���Ӱ� HackShield�� 
		//     ���������� ����ǰ� �ִٴ� �ǹ��Դϴ�.
		//   . ��Ÿ				: ��Ÿ ���ϰ��� ���ؼ��� �ٽ��� ���α׷��� ���̵� ����ٶ�.
		

		dwRet = _AhnHS_VerifyResponse ( pHandleData->hClient, pIoData->szBuf, pIoData->nBufLen );

		//   . ����             : �ϱ��� ���ϰ��� ���(bVerify = FALSE)�� �ش� Ŭ���̾�Ʈ ������ *�����ϸ�
		//                        ����(ERROR_SUCCESS)�� �ƴ� �� ���� ���ϰ��� ���� ������ �������� �ʰ� 
		//                        ���̻� AhnHS_MakeRequest �Լ��� ȣ������ �ʴ°��� �����մϴ�.
		if (  dwRet == ERROR_ANTICPXSVR_BAD_MESSAGE || 
				dwRet == ERROR_ANTICPXSVR_REPLY_ATTACK ||
				dwRet == ERROR_ANTICPXSVR_HSHIELD_FILE_ATTACK ||
				dwRet == ERROR_ANTICPXSVR_CLIENT_FILE_ATTACK ||
				dwRet == ERROR_ANTICPXSVR_MEMORY_ATTACK || 
				dwRet == ERROR_ANTICPXSVR_NANOENGINE_FILE_ATTACK ||
				dwRet == ERROR_ANTICPXSVR_UNKNOWN_CLIENT || 
				dwRet == ERROR_ANTICPXSVR_INVALID_HACKSHIELD_VERSION ||
				dwRet == ERROR_ANTICPXSVR_INVALID_ENGINE_VERSION )
		{
			bVerify = FALSE;
		}
		else
		{
			bVerify = TRUE;
		}

	}
	// [�� ��������]
	else
	{
		// -----------------------------------------------------------------
		// [_AntiCpSvr_AnalyzeAckMsg]
		// -----------------------------------------------------------------
		// - DESCRIPTION:
		//    Ŭ���̾�Ʈ���� ������ ��ȣȭ�� CRC ���� �޽����� ��ȣȭ�Ͽ� 
		//    ���� ���� �� ��Ŷ�� ���Ἲ, �ٽ��� ���� ���� ���� �� �ٽ��� 
		//    ����� ���Ἲ, �׸��� �޸� ���Ἲ�� Ȯ���մϴ�.
		// - SYNTAX:
		//    unsigned long __stdcall _AntiCpSvr_AnalyzeAckMsg (
		//									IN unsigned long *pCrcInfo,
		//									IN unsigned char *pbyAckMsg,
		//										IN unsigned char *pbyReqInfo)
		// -PARAMETERS:
		//	.pCrcInfo	unsigned long *	: _AntiCpSvr_AnalyzeGuidAckMsg �Լ����� ���ϵ� �ش� Ŭ���̾�Ʈ�� ����� 
		//															  CRC ������ ���� ������
		//	.pbyAckMsg	unsigned char *	: Ŭ���̾�Ʈ���� ���� ��ȣȭ�� CRC ���� �޽���
		//  .pbyReqInfo	unsigned char *	: _AntiCpSvr_MakeReqMsg �Լ����� ���ϵ� CRC �䱸 �޽��� ����
		//
		// -RETURN VALUE:
		//	 .ERROR_SUCCESS		: �Լ� ȣ���� �������� �� �����ϴ� ���Դϴ�. (Value = 0x00000000) ���Ӱ� 
		//						  HackShield�� ���������� �����ϰ� �ִٴ� �ǹ��Դϴ�.
		//   . ��Ÿ				: ��Ÿ ���ϰ��� ���ؼ��� �ٽ��� ���α׷��� ���̵� ����ٶ�.
		//
		dwRet = _AntiCpSvr_AnalyzeAckMsg ( &pHandleData->Context, pIoData->szBuf, pHandleData->byClientInfo );

		//   . ����             : �ϱ��� ���ϰ��� ���(bVerify = FALSE)�� �ش� Ŭ���̾�Ʈ ������ *�����ϸ�
		//                        ����(ERROR_SUCCESS)�� �ƴ� �� ���� ���� ������ �������� �ʴ°��� �����մϴ�.
		if (dwRet == ERROR_ANTICPSVR_ANALACKMSG_HSHIELD_ERROR ||
			dwRet == ERROR_ANTICPSVR_ANALACKMSG_PACKET_ERROR  ||
            dwRet == ERROR_ANTICPSVR_ANALACKMSG_FILECRC_ERROR ||
			dwRet == ERROR_ANTICPSVR_ANALACKMSG_MEMORYCRC_ERROR ||
			dwRet == ERROR_ANTICPSVR_ANALACKMSG_NANOENGINECRC_ERROR	)
		{
			bVerify = FALSE;
		}
		else
		{
			bVerify = TRUE;
		}

	}	

	//
	if ( bVerify != TRUE ) 
	{
		_stprintf ( szMsg, TEXT ( "Minia_Server : _AntiCpSvr_AnalyzeAckMsg Fail ( 0x%X ) IP : %d.%d.%d.%d\t%d\n" ),
			dwRet,
			pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b1,
			pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b2,
			pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b3,
			pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b4,
			ntohs ( pHandleData->sockaddr.sin_port ) );
		
		
		OutputDebugString ( szMsg );
		
		header.opCode = OP_ERROR;
		header.length = htonl ( dwRet );
	
		// ������:
		// �ش� ������ Ŭ���̾�Ʈ���� �����κ��Ϳ� � ������ ���� ����Ǿ����� Ȯ���� ����
		// Ŭ���̾�Ʈ�� �ش� ����(dwRet)���� �����ϳ�
		
		// *���� ���������� �ش� Ŭ���̾�Ʈ�� ������ �ٷ� *�����ؾ� �մϴ�.
		// Close(..)

		IoWrite ( pHandleData, &header, NULL, 0 );
	}
	else
	{
		_stprintf ( szMsg, TEXT ( "Minia_Server : CRC Check OK, IP : %d.%d.%d.%d\t%d\n" ),
			pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b1,
			pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b2,
			pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b3,
			pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b4,
			ntohs ( pHandleData->sockaddr.sin_port ) );
		
		Sleep ( m_dwCheckInterval );
		
		ZeroMemory ( pHandleData->byClientInfo, SIZEOF_REQINFO );

		// [Ȯ�� ��������]
		if ( m_bUseAntiCpX )
		{
			// �Լ����� ���� ���� 
			dwRet = _AhnHS_MakeRequest ( pHandleData->hClient, &ptsRequestBuf );

			TCHAR szMsg[256]={0,};
			_stprintf(szMsg, "##_AhnHS_MakeRequest #2 result:%x\r\n", dwRet);
			OutputDebugString(szMsg);
		}
		// [�� ��������]
		else
		{
			// �Լ����� ���� ����

			// �� ����: ���� ���ɿ� ������ ���� �ʵ��� ó�� �ѹ��� ANTICPSVR_CHECK_ALL �ɼ��� ����ϰ�
			//          �� ���� ���ʹ� ANTICPSVR_CHECK_GAME_MEMORY �ɼ��� �����ؼ� ����ؾ� �Ѵ�.
			//          ��:) m_nFirstCheckCrcOpt = ANTICPSVR_CHECK_ALL 
			//               m_nNextCheckCrcOpt = ANTICPSVR_CHECK_GAME_MEMORY
			dwRet = _AntiCpSvr_MakeReqMsg ( &pHandleData->Context, byReqMsg, pHandleData->byClientInfo, m_nNextCheckCrcOpt );
		}
		
		if ( dwRet != ERROR_SUCCESS )
		{

			_stprintf ( szMsg, TEXT ( "Minia_Server : _AntiCpSvr_MakeReqMsg Fail ( 0x%X ) IP : %d.%d.%d.%d\t%d\n" ),
				dwRet,
				pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b1,
				pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b2,
				pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b3,
				pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b4,
				ntohs ( pHandleData->sockaddr.sin_port ) );
			
			
			OutputDebugString ( szMsg );
			
			header.opCode = OP_ERROR;
			header.length = htonl ( dwRet );
			
			IoWrite ( pHandleData, &header, NULL, 0 );
		}
		else
		{
			
			// [Ȯ�� ��������]
			if ( m_bUseAntiCpX )
			{
				header.opCode = OP_REQCRC;
				header.length = htonl ( ptsRequestBuf.nLength );

				IoWrite ( pHandleData, &header, ptsRequestBuf.byBuffer, ptsRequestBuf.nLength );
			}
			// [�� ��������]
			else
			{
				header.opCode = OP_REQCRC;
				header.length = htonl ( SIZEOF_REQMSG );

				IoWrite ( pHandleData, &header, byReqMsg, SIZEOF_REQMSG );				
			}
		}
	}

	return dwRet;
}
