/**
 @File	HspLauncherLib.h
 @brief	HspLauncherLib �������.

 (C) Copyright 2005. Ahn Lab, Inc.\n
 Any part of this source code can not be copied with\n
 any method without prior written permission from\n
 the author or authorized person.\n
 \n
*/

#ifndef _HSPLAUNCHERLIB_H
#define _HSPLAUNCHERLIB_H

// HShield Pro Library Version Info
#define	HSPRO_VERSION(w,x,y,z)		(w) <<24|(x&0xFF)<<16|(y&0xFF)<<8|(z&0xFF)
#define HSHIELDPRO_LLIB_VER		    HSPRO_VERSION(1,0,0,12)

// �����ڵ� ����.
#define ERROR_HSPLAUNCHERLIB_BASECODE						0x0004B000

#define ERROR_STARTLAUNCHER_INVALIDPARAM					ERROR_HSPLAUNCHERLIB_BASECODE + 0x01
#define ERROR_STARTLAUNCHER_MAKEMEMMAPNAME_FAIL				ERROR_HSPLAUNCHERLIB_BASECODE + 0x02
#define ERROR_STARTLAUNCHER_CREATEPROCESS					ERROR_HSPLAUNCHERLIB_BASECODE + 0x03
#define ERROR_STARTLAUNCHER_SHAREMEMINIT_FAIL				ERROR_HSPLAUNCHERLIB_BASECODE + 0x04
#define ERROR_STARTLAUNCHER_WAITEVENT_FAIL					ERROR_HSPLAUNCHERLIB_BASECODE + 0x05
#define ERROR_STARTLAUNCHER_LAUNCHERSTART_OK				ERROR_HSPLAUNCHERLIB_BASECODE + 0x07
#define ERROR_STARTLAUNCHER_ALEADYSTART						ERROR_HSPLAUNCHERLIB_BASECODE + 0x09
#define ERROR_STARTLAUNCHER_GETEXITCODE						ERROR_HSPLAUNCHERLIB_BASECODE + 0x0A
#define ERROR_STARTLAUNCHER_RCV_RESULT						ERROR_HSPLAUNCHERLIB_BASECODE + 0x0B
#define ERROR_STARTLAUNCHER_INVALIDSESSION					ERROR_HSPLAUNCHERLIB_BASECODE + 0x0C

#define ERROR_MAKEREQSESSION_INVALIDPARAM					ERROR_HSPLAUNCHERLIB_BASECODE + 0x10
#define ERROR_MAKEREQSESSION_WAITEVENT_FAIL					ERROR_HSPLAUNCHERLIB_BASECODE + 0x11
#define ERROR_MAKEREQSESSION_MAKESESSIONKEY_FAIL			ERROR_HSPLAUNCHERLIB_BASECODE + 0x12
#define ERROR_MAKEREQSESSION_INITCRYPT_FAIL					ERROR_HSPLAUNCHERLIB_BASECODE + 0x13
#define ERROR_MAKEREQSESSION_ENCRYPT_FAIL					ERROR_HSPLAUNCHERLIB_BASECODE + 0x14
#define ERROR_MAKEREQSESSION_SHAREMEMINIT_FAIL				ERROR_HSPLAUNCHERLIB_BASECODE + 0x15
#define ERROR_MAKEACKSESSION_INVALIDSESSION					ERROR_HSPLAUNCHERLIB_BASECODE + 0x16
#define ERROR_MAKEACKSESSION_SESSION_OK						ERROR_HSPLAUNCHERLIB_BASECODE + 0x17
#define ERROR_MAKEACKSESSION_RCV_RESULT						ERROR_HSPLAUNCHERLIB_BASECODE + 0x18
#define ERROR_MAKEREQSESSION_SETEVENT_FAIL					ERROR_HSPLAUNCHERLIB_BASECODE + 0x19

#define ERROR_STARTGAME_INVALIDPARAM						ERROR_HSPLAUNCHERLIB_BASECODE + 0x28
#define ERROR_STARTGAME_MAKEFILEKEY_FAIL					ERROR_HSPLAUNCHERLIB_BASECODE + 0x29 
#define ERROR_STARTGAME_SHAREMEMINIT_FAIL					ERROR_HSPLAUNCHERLIB_BASECODE + 0x2A
#define ERROR_STARTGAME_WAITEVENT_FAIL						ERROR_HSPLAUNCHERLIB_BASECODE + 0x2B
#define ERROR_STARTGAME_GAMESTART_OK						ERROR_HSPLAUNCHERLIB_BASECODE + 0x2D
#define ERROR_STARTGAME_INITCRYPT_FAIL						ERROR_HSPLAUNCHERLIB_BASECODE + 0x2F
#define ERROR_STARTGAME_ENCRYPT_FAIL						ERROR_HSPLAUNCHERLIB_BASECODE + 0x40
#define ERROR_STARTGAME_HSPL_NOTEXEC						ERROR_HSPLAUNCHERLIB_BASECODE + 0x41
#define ERROR_STARTGAME_GETEXITCODE							ERROR_HSPLAUNCHERLIB_BASECODE + 0x42

#define ERROR_GETVERSION_INVALIDPARAM						ERROR_HSPLAUNCHERLIB_BASECODE + 0x50
#define ERROR_GETVERSION_CREATEFILE							ERROR_HSPLAUNCHERLIB_BASECODE + 0x51
#define ERROR_GETVERSION_SETFILEPOINTER						ERROR_HSPLAUNCHERLIB_BASECODE + 0x52
#define ERROR_GETVERSION_SETFILEPOINTER						ERROR_HSPLAUNCHERLIB_BASECODE + 0x52

#define ERROR_STRING_CONVERSION_FAILED						ERROR_HSPLAUNCHERLIB_BASECODE + 0x60





#define MAX_PATH_LEN			1024
#define FILE_VERSION_LEN		8

#pragma pack(push)
#pragma pack(8)
/**
 @brief ���ӽ��࿡ �ʿ��� ������ ������ ����ü
*/

typedef struct _AHN_GAMEEXECINFOA
{
	CHAR szFilePath[MAX_PATH_LEN];			        //< ������ ���� ��ü���. 
	CHAR szCommandLine[MAX_PATH_LEN];				//< ���࿡ �ʿ��� ����. 
} AHN_GAMEEXECINFOA, *PAHN_GAMEEXECINFOA;

typedef struct _AHN_GAMEEXECINFOW
{
	WCHAR szFilePath[MAX_PATH_LEN];			        //< ������ ���� ��ü���. 
	WCHAR szCommandLine[MAX_PATH_LEN];				//< ���࿡ �ʿ��� ����. 
} AHN_GAMEEXECINFOW, *PAHN_GAMEEXECINFOW;
#if defined(UNICODE) | defined(_UNICODE)
	#define AHN_GAMEEXECINFO	AHN_GAMEEXECINFOW
#else
	#define AHN_GAMEEXECINFO	AHN_GAMEEXECINFOA
#endif

#pragma pack(pop)

#if defined(__cplusplus)
extern "C"
{
#endif
	
/*!
 * 
 * @remarks	�ٽ��� ��ó�� �����Ѵ�.
*
 * @param	szHspLauncherPath : [IN] �ٽ��己ó ��ü���.
 *
 * @retval	ERROR_SUCCESS : �ٽ��� ��ó���� ����.
 * @retval	ERROR_STARTLAUNCHER_INVALIDPARAM : �߸��� �Ķ���� �Է�.
 * @retval	ERROR_STARTLAUNCHER_ALEADYSTART : �ٽ��己ó�� �̹� ���۵Ǿ���.
 * @retval	ERROR_STARTLAUNCHER_GETEXITCODE : �ٽ��己ó �������� Ȯ�ν���.
 * @retval	ERROR_STARTLAUNCHER_MAKEMEMMAPNAME_FAIL : �޸𸮸� �̸����� ����.
 * @retval	ERROR_STARTLAUNCHER_CREATEPROCESS : �ٽ��己ó ���μ��� ���� ����.
 * @retval	ERROR_STARTLAUNCHER_SHAREMEMINIT_FAIL : ���� �޸𸮸� �ʱ�ȭ ����.
 * @retval	ERROR_STARTLAUNCHER_WAITEVENT_FAIL : �̺�Ʈ ���ð� �ʰ�.
 * @retval	�׹��� Win32 Error Codes : ���н�
 *
 * @attention	
 *
 */
DWORD
__stdcall
_AhnHsp_StartLauncherA (
		IN LPCSTR szHspLauncherPathA
		);
DWORD
__stdcall
_AhnHsp_StartLauncherW (
		IN LPCWSTR szHspLauncherPathW
		);
#if defined(UNICODE) | defined(_UNICODE)
#define _AhnHsp_StartLauncher	_AhnHsp_StartLauncherW
#else
#define _AhnHsp_StartLauncher   _AhnHsp_StartLauncherA
#endif

/*!
 * 
 * @remarks	�ٽ��� ��ó�� ���� ����Ŭ���̾�Ʈ�� �����Ѵ�.
 *
 * @param	pAhnGameExecInfo : [IN]���� ������ ����.
 *
 * @retval	ERROR_SUCCESS : ���� Ŭ���̾�Ʈ ���� ����.
 * @retval	ERROR_STARTGAME_INVALIDPARAM : �߸��� �Ķ���� �Է�.
 * @retval	ERROR_STARTGAME_HSPL_NOTEXEC : �ٽ��己ó�� �����ϰ� �����ʴ�.
 * @retval	ERROR_STARTGAME_GETEXITCODE : �ٽ��己ó �������� Ȯ�ν���.
 * @retval	ERROR_STARTGAME_SHAREMEMINIT_FAIL : �Ÿ𸮸� �ʱ�ȭ ����.
 * @retval	ERROR_STARTGAME_MAKEFILEKEY_FAIL : ����Ű ���� ����.
 * @retval	ERROR_STARTGAME_INITCRYPT_FAIL : ��/��ȣȭ �ʱ�ȭ�� ����.
 * @retval	ERROR_STARTGAME_WAITEVENT_FAIL : �̺�Ʈ ��� ����.
 * @retval	ERROR_STARTGAME_ENCRYPT_FAIL : ������ ��ȣȭ�� ����.
 * @retval	�׹��� Win32 Error Codes : ���н�
 *
 * @attention	
 *
 */
DWORD
__stdcall
_AhnHsp_StartGameA (
		IN PAHN_GAMEEXECINFOA pAhnGameExecInfoA 
		);
DWORD
__stdcall
_AhnHsp_StartGameW (
		IN PAHN_GAMEEXECINFOW pAhnGameExecInfoW
		);
#if defined(UNICODE) | defined(_UNICODE)
	#define _AhnHsp_StartGame	_AhnHsp_StartGameW
#else
	#define _AhnHsp_StartGame	_AhnHsp_StartGameA
#endif


/*!
 * 
 * @remarks	�ٽ��� ��ó�� ���μ����ڵ��� �����Ѵ�.
 *
 * @param	����.
 *
 * @retval	����.
 * @attention	
 *
 */
void __stdcall _AhnHsp_CloseHandle();

#if defined(__cplusplus)
}

#endif //(__cplusplus)

#endif //_HSPLAUNCHERLIB_H
