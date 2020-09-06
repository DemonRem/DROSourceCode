/**
 @File	HspSecLib.h
 @brief	HspSecLib �������.

 (C) Copyright 2005. Ahn Lab, Inc.\n
 Any part of this source code can not be copied with\n
 any method without prior written permission from\n
 the author or authorized person.\n
*/

#ifndef _HSPSECLIB_H
#define _HSPSECLIB_H

// HShield Pro SecLib Version Info
#define	HSPRO_VERSION(w,x,y,z)	        (w) <<24|(x&0xFF)<<16|(y&0xFF)<<8|(z&0xFF)
#define HSHIELDPRO_SECLIB_VER		    HSPRO_VERSION(1,0,0,2)

// �����ڵ� ����.
#define ERROR_HSPSECLIB_BASECODE							    0x0004A000
#define ERROR_HSPSECLIB_MAKESESSIONKEY_INVALIDPARAM				ERROR_HSPSECLIB_BASECODE + 0x01
#define ERROR_HSPSECLIB_INITCRYPT_INVALIDPARAM					ERROR_HSPSECLIB_BASECODE + 0x02
#define ERROR_HSPSECLIB_GETENCMSG_INVALIDPARAM					ERROR_HSPSECLIB_BASECODE + 0x03
#define ERROR_HSPSECLIB_GETDECMSG_INVALIDPARAM					ERROR_HSPSECLIB_BASECODE + 0x04

#define ERROR_HSPSECLIB_ENCFILE_INVALIDPARAM					ERROR_HSPSECLIB_BASECODE + 0x05
#define ERROR_HSPSECLIB_ENCFILE_FOPEN							ERROR_HSPSECLIB_BASECODE + 0x06
#define ERROR_HSPSECLIB_ENCFILE_GETFILELEN						ERROR_HSPSECLIB_BASECODE + 0x07
#define ERROR_HSPSECLIB_ENCFILE_SIZEZERO						ERROR_HSPSECLIB_BASECODE + 0x08
#define ERROR_HSPSECLIB_ENCFILE_INITCRYPT						ERROR_HSPSECLIB_BASECODE + 0x09
#define ERROR_HSPSECLIB_ENCFILE_COPYFILE						ERROR_HSPSECLIB_BASECODE + 0x0A
#define ERROR_HSPSECLIB_ENCFILE_ENCFILE							ERROR_HSPSECLIB_BASECODE + 0x0B
#define ERROR_HSPSECLIB_ENCFILE_FWRITE							ERROR_HSPSECLIB_BASECODE + 0x0C

#define ERROR_HSPSECLIB_FILEENC_INVALIDPARAM					ERROR_HSPSECLIB_BASECODE + 0x10
#define ERROR_HSPSECLIB_FILEENC_FREAD							ERROR_HSPSECLIB_BASECODE + 0x11
#define ERROR_HSPSECLIB_FILEENC_GETENCMSG						ERROR_HSPSECLIB_BASECODE + 0x12

#define ERROR_HSPSECLIB_FREAD_INVALIDPARAM						ERROR_HSPSECLIB_BASECODE + 0x13
#define ERROR_HSPSECLIB_FREAD_GETFILELEN						ERROR_HSPSECLIB_BASECODE + 0x14
#define ERROR_HSPSECLIB_FREAD_SIZEZERO							ERROR_HSPSECLIB_BASECODE + 0x15
#define ERROR_HSPSECLIB_FREAD_GETPOSITION						ERROR_HSPSECLIB_BASECODE + 0x16
#define ERROR_HSPSECLIB_FREAD_RANGE								ERROR_HSPSECLIB_BASECODE + 0x17
#define ERROR_HSPSECLIB_FREAD_FREAD								ERROR_HSPSECLIB_BASECODE + 0x18
#define ERROR_HSPSECLIB_FREAD_FSEEK								ERROR_HSPSECLIB_BASECODE + 0x19
#define ERROR_HSPSECLIB_FREAD_GETDECMSG							ERROR_HSPSECLIB_BASECODE + 0x1A

#define ERROR_HSPSECLIB_DECFILE_INVALIDPARAM					ERROR_HSPSECLIB_BASECODE + 0x20
#define ERROR_HSPSECLIB_DECFILE_FOPEN							ERROR_HSPSECLIB_BASECODE + 0x21
#define ERROR_HSPSECLIB_DECFILE_FILESEEK						ERROR_HSPSECLIB_BASECODE + 0x22
#define ERROR_HSPSECLIB_DECFILE_INITCRYPT						ERROR_HSPSECLIB_BASECODE + 0x23
#define ERROR_HSPSECLIB_DECFILE_FWRITE							ERROR_HSPSECLIB_BASECODE + 0x24
#define ERROR_HSPSECLIB_DECFILE_GETFILELEN						ERROR_HSPSECLIB_BASECODE + 0x25
#define ERROR_HSPSECLIB_DECFILE_SIZEZERO						ERROR_HSPSECLIB_BASECODE + 0x26

#define ERROR_HSPSECLIB_FILEDEC_INVALIDPARAM					ERROR_HSPSECLIB_BASECODE + 0x30
#define ERROR_HSPSECLIB_FILEDEC_FREAD							ERROR_HSPSECLIB_BASECODE + 0x31
#define ERROR_HSPSECLIB_FILEDEC_GETDECMSG						ERROR_HSPSECLIB_BASECODE + 0x32

#define ERROR_HSPSECLIB_GETFILEHASH_INVALIDPARAM				ERROR_HSPSECLIB_BASECODE + 0x40
#define ERROR_HSPSECLIB_GETFILEHASH_CREATEFILE					ERROR_HSPSECLIB_BASECODE + 0x41
#define ERROR_HSPSECLIB_GETFILEHASH_CREATEFILEMAP				ERROR_HSPSECLIB_BASECODE + 0x42
#define ERROR_HSPSECLIB_GETFILEHASH_GETFILESIZE					ERROR_HSPSECLIB_BASECODE + 0x43
#define ERROR_HSPSECLIB_GETFILEHASH_MAPVIEWFILE					ERROR_HSPSECLIB_BASECODE + 0x44


#include "../../Inc/CryptAlgo/AES.h"


#if defined(__cplusplus)
extern "C"
{
#endif

/*!
 * 
 * @remarks	�־��� ũ�⿡ �ش��ϴ� �����õ带 �����Ѵ�.
 *
 *
 * @param	nRndSize : [IN]�õ� ������
 * @param	pbyRndSeed : [OUT]�����õ�
 *
 * @retval	����.
 *
 * @attention	
 *
 */
void __stdcall GetRandSeed ( IN int nRndSize,         
				             OUT PBYTE pbyRndSeed );

/*!
 * 
 * @remarks	���ǻ����� ����� Ű ���� ��� ����
 *
 *
 * @param	pbyRandomSeed : [IN]����Ű ������ ���� ������ �õ尪 					
 * @param	pbyRndSeed : [OUT]����Ű
 *
 * @retval	ERROR_SUCCESS : ����
 * @retval	ERROR_MAKESESSIONKEY_INVALIDPARAM : �߸��� �Ķ���� �Է�
 * @retval	�׹��� Win32 Error Codes : ���н�
 *
 * @attention	
 *
 */
DWORD __stdcall MakeFileKey ( PBYTE pbyRandomSeed,					
						      PBYTE pbySessionKey );	   



/*!
 * 
 * @remarks	���ǻ����� ����� Ű ���� ��� ����
 *
 * @param	pbyFirstSeed : [IN]����Ű ������ ���� ù��° �õ尪 					
 * @param	pbySecondSeed : [IN]����Ű ������ ���� �ι�° �õ尪 					
 * @param	pbyRndSeed : [OUT]����Ű
 *
 * @retval	ERROR_SUCCESS : ����
 * @retval	ERROR_HSPSECLIB_MAKESESSIONKEY_INVALIDPARAM : �߸��� �Ķ���� �Է�
 *
 * @attention	
 *
 * @sa		
 *
 */
ULONG __stdcall MakeSessionKey ( IN PBYTE pbyFirstSeed,					
								 IN PBYTE pbySecondSeed,				
								 OUT PBYTE pbySessionKey );



/*!
 * 
 * @remarks	��/��ȣȭ�� ���� �ʱ�ȭ ��� ����\n
	         =>Key�� �Է¹޾� �Ϻ�ȣȭ�� ���� AES_KEY�� �����Ѵ�.
 *
 * @param	pbyPwd : [IN]�н����� 					
 * @param	nKeySize : [IN]�н����� ������
 * @param	pAesEncKey : [OUT]��ȣȭŰ
 * @param	pAesDecKey : [OUT]��ȣȭŰ
 *
 * @retval	ERROR_SUCCESS : ����
 * @retval	ERROR_HSPSECLIB_INITCRYPT_INVALIDPARAM : �߸��� �Ķ���� �Է�
 *
 * @attention	
 *
 * @sa		
 *
 */

DWORD __stdcall InitCrypt ( PBYTE pbyPwd,				
							int nKeySize,				
							AES_KEY *pAesEncKey,		
							AES_KEY *pAesDecKey );

/*!
 * 
 * @remarks	AES128�� �̿��Ͽ� ��ȣȭ ��� ����\n
	         =>�Էµ� ���۸� ��ȣȭ�Ͽ� ���۷� ����Ѵ�.
 *
 * @param	pbyInput : [IN]��ȣȭ�� ���� 					
 * @param	nInLength : [IN]��ȣȭ�� ���ۻ�����
 * @param	*pAesEncKey : [IN]��ȣȭŰ
 * @param	pbyOutput : [OUT]��ȣȭ�� ����
 *
 * @retval	ERROR_SUCCESS : ����
 * @retval	ERROR_HSPSECLIB_GETENCMSG_INVALIDPARAM : �߸��� �Ķ���� �Է�
 *
 * @attention	
 *
 * @sa		
 *
 */
DWORD __stdcall GetEncMsg ( PBYTE pbyInput,		
						    int nInLength,		
						    AES_KEY *pAesEncKey,	
						    PBYTE pbyOutput );

/*!
 * 
 * @remarks	AES128�� �̿��Ͽ� ��ȣȭ ��� ����\n
	         =>�Էµ� ���۸� ��ȣȭ�Ͽ� ���۷� ����Ѵ�.
 *
 * @param	pbyInput : [IN]��ȣȭ�� ���� 					
 * @param	nInLength : [IN]��ȣȭ�� ���ۻ�����
 * @param	*pAesEncKey : [IN]��ȣȭŰ
 * @param	pbyOutput : [OUT]��ȣȭ�� ����
 *
 * @retval	ERROR_SUCCESS : ����
 * @retval	ERROR_HSPSECLIB_GETDECMSG_INVALIDPARAM : �߸��� �Ķ���� �Է�
 *
 * @attention	
 *
 * @sa		
 *
 */
DWORD __stdcall GetDecMsg ( PBYTE pbyInput,				
						    int nInLength,				
						    AES_KEY *pAesDecKey,			
						    PBYTE pbyOutput );
	
/*!
 * 
 * @remarks	���� ��ȣȭ.
 *
 * @param	lpszInputFile : [IN]��ȣȭ�� ���ϰ�� 					
 * @param	pbyInitKey : [IN] �ʱ�ȭ ��ȣŰ
 * @param	lpszOutputFile : [OUT]������ ����� ���ϰ��
 *
 * @retval	ERROR_SUCCESS : ����
 * @retval	ERROR_HSPSECLIB_ENCFILE_INVALIDPARAM : �߸��� �Ķ���� �Է�
 * @retval	ERROR_HSPSECLIB_ENCFILE_FOPEN : fopen����.
 * @retval	ERROR_HSPSECLIB_ENCFILE_GETFILELEN : ���� ����� ���ϱ� ����.
 * @retval	ERROR_HSPSECLIB_ENCFILE_SIZEZERO : ���� ������ 0.
 * @retval	ERROR_HSPSECLIB_ENCFILE_INITCRYPT : �Ϻ�ȣȭ �ʱ�ȭ ����.
 * @retval	ERROR_HSPSECLIB_ENCFILE_COPYFILE : ���� ���� ����.
 * @retval	ERROR_HSPSECLIB_ENCFILE_ENCFILE : ��ȣȭ ����.
 * @retval	ERROR_HSPSECLIB_ENCFILE_FWRITE : ���Ͼ��� ����.
 * @retval	�׹��� Win32 Error Codes : ���н�
 *
 * @attention	
 *
 */
DWORD __stdcall GetEncFile ( IN LPCSTR lpszInputFile,			
						     IN PBYTE pbyInitKey,
							 OUT LPSTR lpszOutputFile );


/*!
 * 
 * @remarks	���� ��ȣȭ.
 *
 * @param	lpszInputFile : [IN]��ȣȭ�� ���ϰ�� 					
 * @param	pbyInitKey : [IN] �ʱ�ȭ ��ȣŰ
 * @param	lpszOutputFile : [OUT]��ȣȭ�� ���ϰ��
 *
 * @retval	ERROR_SUCCESS : ����
 * @retval	ERROR_HSPSECLIB_DECFILE_INVALIDPARAM : �߸��� �Ķ���� �Է�
 * @retval	ERROR_HSPSECLIB_DECFILE_FOPEN : fopen����.
 * @retval	ERROR_HSPSECLIB_DECFILE_GETFILELEN : ���� ����� ���ϱ� ����.
 * @retval	ERROR_HSPSECLIB_DECFILE_SIZEZERO : ���� ������ 0.
 * @retval	ERROR_HSPSECLIB_DECFILE_INITCRYPT : ��ȣȭ �ʱ�ȭ ����.
 * @retval	ERROR_HSPSECLIB_DECFILE_FWRITE : ���Ͼ��� ����.
 * @retval	�׹��� Win32 Error Codes : ���н�
 *
 * @attention	
 *
 */
DWORD __stdcall GetDecFile( IN LPCSTR lpszInputFile,			
						    IN PBYTE pbyInitKey,
							OUT LPSTR lpszOutputFile);

/*!
 * 
 * @remarks	���ϱ���ü �����͸� �̿��Ͽ� ���ϴ� ���� ��ȣȭ�� ���۷� ���.
 *
 * @param	lpOutBuffer : [OUT]��ȣȭ�� ���� 					
 * @param	dwDecryptSize : [IN]���� ������
 * @param	pInputStream : [IN]���� ����������
 * @param	pAesDecKey : [IN]��ȣȭ Ű
 * @param	pdwReadLen : [OUT]��ȣȭ�� ������
 *
 * @retval	ERROR_SUCCESS : ����
 * @retval	ERROR_HSPSECLIB_FREAD_INVALIDPARAM : �߸��� �Ķ���� �Է�
 * @retval	ERROR_HSPSECLIB_FREAD_GETFILELEN : ����ũ�� ���ϱ� ����.
 * @retval	ERROR_HSPSECLIB_FREAD_SIZEZERO : ����ũ�� 0.
 * @retval	ERROR_HSPSECLIB_FREAD_GETPOSITION : ���� ���������� ���ϱ� ����.
 * @retval	ERROR_HSPSECLIB_FREAD_RANGE :��ȣȭ�� ������� ���Ϻ��� ũ��.
 * @retval	ERROR_HSPSECLIB_FREAD_FSEEK : AhnHs_FSeek ����.
 * @retval	ERROR_HSPSECLIB_FREAD_FREAD : fread ����.
 * @retval	ERROR_HSPSECLIB_FREAD_GETDECMSG : ��ȣȭ ����.
 * @retval	�׹��� Win32 Error Codes : ���н�
 *
 * @attention	
 *
 */
DWORD __stdcall AhnHs_FRead ( OUT LPVOID lpOutBuffer,			
								 IN DWORD dwDecryptSize,			
								 IN FILE *pInputStream,				
								 IN AES_KEY *pAesDecKey,			
								 OUT PDWORD pdwReadLen );



/*!
 * 
 * @remarks	������ �ؽ��� ���Ѵ�.
 *
 * @param	lpszFilePath : [IN]�ؽ��� ���� ���ϰ�� 					
 * @param	pbyHash : [OUT]�ؽ���
 *
 * @retval	ERROR_SUCCESS : ����
 * @retval	ERROR_HSPSECLIB_GETFILEHASH_INVALIDPARAM : �߸��� �Ķ���� �Է�
 * @retval	ERROR_HSPSECLIB_GETFILEHASH_CREATEFILE : ���ϻ��� ����.
 * @retval	ERROR_HSPSECLIB_GETFILEHASH_CREATEFILEMAP : ������ File�� ���� Memory Map�� ���� ����.
 * @retval	ERROR_HSPSECLIB_GETFILEHASH_GETFILESIZE : ��ü ���� ������ ���ϱ� ����.
 * @retval	ERROR_HSPSECLIB_GETFILEHASH_MAPVIEWFILE : �޸𸮸ʿ����� ����� ����.
 * @retval	�׹��� Win32 Error Codes : ���н�
 *
 * @attention	
 *
 * @sa		
 *
 */
DWORD GetFileHash( IN LPCSTR lpszFilePath,		
				   OUT PBYTE pbyHash );

#if defined(__cplusplus)
}
#endif //(__cplusplus)

#endif //_HSPSECLIB_H
