/* libFLAC - Free Lossless Audio Codec library
 * Copyright (C) 2012-2016  Xiph.org Foundation
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * - Neither the name of the Xiph.org Foundation nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* This is the preferred location of all CPP hackery to make $random_compiler
 * work like something approaching a C99 (or maybe more accurately GNU99)
 * compiler.
 *
 * It is assumed that this header will be included after "config.h".
 */

#ifndef FLAC__SHARE__COMPAT_H
#define FLAC__SHARE__COMPAT_H


#if __STDC_VERSION__ < 201112L
#error /** Only C11 */
#endif

#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#ifndef BUILD_TEST
#include "mimiclib.h"
#define flac_printf mimic_printf
#define FLAC_ENV_EMBEDDED
#else
#include <stdio.h>
#include <stdlib.h>
#define flac_printf printf
#endif

#if defined(FLAC_ENV_EMBEDDED)
#define USE_MIMICLIB
#define USE_FAT_FS
#endif

#include <stdint.h>
#include <stdbool.h>
#include <strings.h>
#include <sys/stat.h>
#include <unistd.h>
#include <utime.h>
#include <stdarg.h>
#define FLAC__off_t off_t
#define FLAC__U64L(x) x##ULL

#define FLAC__STRCASECMP strcasecmp
#define FLAC__STRNCASECMP strncasecmp

#define flac_restrict
#define flac_stat_s stat /* stat struct */


#ifndef M_LN2
#define M_LN2 0.69314718055994530942
#endif
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define FLAC__HAS_OGG 0




#if defined(USE_FAT_FS)
#include "ff.h"
/*FRESULT f_open (FIL* fp, const TCHAR* path, BYTE mode);*/
#define flac_utime f_utime
#define flac_unlink f_unlink
#define flac_rename f_rename
#define flac_stat f_stat
#define flac_ftello f_tell
typedef FIL FLAC_FILE;
#elif defined(BUILD_TEST)
#include <stdio.h>
#define flac_utime utime
#define flac_unlink unlink
#define flac_rename rename
#define flac_stat stat
#define flac_ftello ftell
typedef FILE FLAC_FILE;

#else
#error "Unkown Filesystem"
#endif

typedef enum{
	enFlacFopenModeOpen,
	enFlacFopenModeOpenAdd,
    enFlacFopenModeRead,
    enFlacFopenModeWrite,
}enLFlacFopenMode_t;

extern _Bool flac_fopen(FLAC_FILE* fp, const char path[], enLFlacFopenMode_t enMode);
extern uint32_t flac_fwrite(const void *buf, uint32_t size, uint32_t n, FLAC_FILE *fp);
extern uint32_t flac_fread(void *buf, uint32_t size, uint32_t n, FLAC_FILE *fp);
extern int flac_chmod(const char szFilePath[], int mode);
extern int flac_fseeko(FLAC_FILE *fp, int32_t offset, int32_t whence);



#ifndef BUILD_TEST
extern void *pvlibTCMMalloc( uint32_t xWantedSize );
extern void vlibTCMPortFree( void *pv );
extern uint32_t xlibSYSPortGetFreeHeapSize( void );
#define FLAC_FREE(x) FlacSysFree((uintptr_t)(x), __FUNCTION__, __LINE__)
#define FLAC_MALLOC(x) FlacSysMalloc((x), __FUNCTION__, __LINE__)
#define FLAC_CALLOC(x,y) FlacSysCalloc((x),(y), __FUNCTION__, __LINE__)
#define FLAC_REALLOC(x,y) FlacSysRealloc((x),(y), __FUNCTION__, __LINE__)

static inline void *FlacSysMalloc(uint32_t xWantedSize, const char pszFunc[], uint32_t u32Line){
	
	void *ptr = pvlibTCMMalloc(xWantedSize);
	if(ptr != NULL){
		//AddMallocInfo((uintptr_t)ptr, xWantedSize, pszFunc, u32Line);
		//flac_printf("[%s (%d)] ptr = 0x%08lX, xWantedSize = %lu\r\n", pszFunc, u32Line, ptr, xWantedSize);
	}else{
		flac_printf("[%s (%d)] pvlibSYSMalloc NG (Free = %lu, Wanted = %lu)\r\n", pszFunc, u32Line, xlibSYSPortGetFreeHeapSize(), xWantedSize);
	}
	return ptr;
}
static inline void FlacSysFree(uintptr_t pv, const char pszFile[], uint32_t u32Line){
	if(pv != 0){
		vlibTCMPortFree((void*)pv);
		//DelMallocInfo(pv);
		//flac_printf("[%s (%d)] ptr = 0x%08lX\r\n", pszFile, u32Line, pv);
	}
}



static inline void *FlacSysCalloc(uint32_t i, uint32_t j, const char pszFunc[], uint32_t u32Line){
	void *ptr = FlacSysMalloc(i*j, pszFunc, u32Line);
	if(ptr != NULL){
		flac_memset(ptr, 0, i*j);
	}else{
		flac_printf("[%s (%d)] pvlibSYSMalloc NG (Free = %lu, Wanted = %lu)\r\n", pszFunc, u32Line, xlibSYSPortGetFreeHeapSize(), i*j);
	}

	return ptr;
}
static inline void *FlacSysRealloc(void *ptrOld, uint32_t size, const char pszFunc[], uint32_t u32Line){
	void *ptrNew = FlacSysMalloc(size, pszFunc, u32Line);
	if(ptrNew != NULL){
		flac_memcpy(ptrNew, ptrOld, size);
		FlacSysFree((uintptr_t)ptrOld, pszFunc, u32Line);
	}

	return ptrNew;
}
#else
#define FLAC_FREE(x) free(x)
#define FLAC_MALLOC(x) malloc(x)
#define FLAC_CALLOC(x,y) calloc(x,y)
#define FLAC_REALLOC(x,y) realloc(x,y)
#endif

#ifndef BUILD_TEST
extern void AddMallocInfo(uintptr_t addr, uint32_t u32size, char *psz, uint32_t u32Line);
extern void DelMallocInfo(uintptr_t addr);
extern void DumpMallocInfo(void);
#endif

static inline _Bool flac_memcmp(const void * p1, const void * p2, uint32_t u32ByteCnt)
{
	/*-- var --*/
	uint8_t *pu81 = (uint8_t *)p1;
	uint8_t *pu82 = (uint8_t *)p2;
	_Bool bret = true;

	/*-- begin --*/
	if ((p1 != NULL) && (p2 != NULL))
	{
		for (uint32_t i = 0u; i < u32ByteCnt; i++)
		{
			if (pu81[i] != pu82[i])
			{
				bret = false;
				break;
			}
		}
	}
	return bret;
}

static inline uint32_t flac_strlen(const char pszStr[])
{
	/*-- var --*/
	uint32_t u32Cnt = 0u;

	/*-- begin --*/
	if (pszStr != (const char *)NULL)
	{
		while (pszStr[u32Cnt] != '\0')
		{
			u32Cnt++;
		}
	}
	return u32Cnt;
}
static inline char *flac_strcpy(char szDst[], const char szSrc[])
{
	/*-- var --*/
	uint32_t i = 0u;

	/*-- begin --*/
	if ((szDst != (char *)NULL) && (szSrc != (const char *)NULL))
	{
		i = 0u;
		while (szSrc[i] != (char)'\0')
		{
			szDst[i] = szSrc[i];
			i++;
		}
	}

	return szDst;
}
static inline char *flac_strncpy(char szDst[], const char szSrc[], uint32_t u32MaxSizeOfSrc)
{
	/*-- var --*/
	uint32_t i = 0u;

	/*-- begin --*/
	if ((szDst != (char *)NULL) && (szSrc != (const char *)NULL))
	{
		i = 0u;
		while (szSrc[i] != (char)'\0')
		{
			szDst[i] = szSrc[i];
			i++;
			if(i >= u32MaxSizeOfSrc)
			{
				break;
			}
		}
	}

	return szDst;
}
static inline char *flac_strdup(const char szStr[]){
	char *pret = NULL;
	if(szStr != NULL){
		pret = FLAC_MALLOC(flac_strlen(szStr) + 1);
		if(pret != NULL){
			flac_strcpy(pret, szStr);
		}
	}

	return pret;
}

static inline _Bool flac_memcpy(void * p1, const void * p2, uint32_t u32ByteCnt)
{
	/*-- var --*/
	uint8_t *pu81 = (uint8_t *)p1;
	uint8_t *pu82 = (uint8_t *)p2;
	_Bool bret = false;

	/*-- begin --*/
	if ((p1 != NULL) && (p2 != NULL))
	{
		for (uint32_t i = 0u; i < u32ByteCnt; i++)
		{
			pu81[i] = pu82[i];
		}
		bret = true;
	}
	return bret;
}

static inline _Bool flac_memset(void * p1, uint8_t val, uint32_t u32ByteCnt)
{
	/*-- var --*/
	uint8_t *pu81 = (uint8_t *)p1;
	_Bool bret = false;

	/*-- begin --*/
	if (p1 != NULL)
	{
		for (uint32_t i = 0u; i < u32ByteCnt; i++)
		{
			pu81[i] = val;
		}
		bret = true;
	}
	return bret;
}

char *flac_strncat(char pszStr1[], const char pszStr2[], uint32_t u32SizeOfStr2)
{
	uint32_t u32Pos = 0;
	uint32_t u32Index = 0;

	if ((pszStr1 == NULL) || (pszStr2 == 0))
	{
		return NULL;
	}
	while (pszStr1[u32Pos] != '\0')
	{
		u32Pos++;
	}

	while (pszStr2[u32Index] != '\0')
	{
		pszStr1[u32Pos] = pszStr2[u32Index];
		u32Pos++;
		u32Index++;
		if(u32Index >= u32SizeOfStr2)
		{
			pszStr1[u32Pos] = '\0';
		}
	}

	return pszStr1;
}

static inline int32_t flac_strcmp(const char szStr1[], const char szStr2[])
{
	for (uint32_t u32Cnt = 0u; ; u32Cnt++)
	{
		if (szStr1[u32Cnt] < szStr2[u32Cnt])
		{
			return -1;
		}
		if (szStr1[u32Cnt] > szStr2[u32Cnt])
		{
			return 1;
		}
		if ((szStr1[u32Cnt] == '\0') && (szStr2[u32Cnt] == '\0'))
		{
			return 0;
		}
	}
}


static inline const char *flac_strrchr(const char szStr[], char c)
{
	if(szStr == NULL)
	{
		return NULL;
	}

	uint32_t i=0;
	const char *pret = NULL;

	while(szStr[i] != '\0')
	{
		if(szStr[i] == c)
		{
			pret = &szStr[i];
		}
		i++;
	}

	return pret;
}

static inline char *safe_strncat(char *dest, const char *src, size_t dest_size)
{
	char * ret;

	if (dest_size < 1)
		return dest;

	ret = flac_strncat(dest, src, dest_size - flac_strlen (dest));
	dest [dest_size - 1] = 0;

	return ret;
}

static inline char *safe_strncpy(char *dest, const char *src, size_t dest_size)
{
	char * ret;

	if (dest_size < 1)
		return dest;

	ret = flac_strncpy(dest, src, dest_size);
	dest [dest_size - 1] = 0;

	return ret;
}



#ifdef __cplusplus
};
#endif

#endif /* FLAC__SHARE__COMPAT_H */
