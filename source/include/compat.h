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

#if defined(FLAC_ENV_EMBEDDED)
#define USE_MIMICLIB
#define USE_FAT_FS
#endif

#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>

#define FLAC__off_t off_t
#define FLAC__U64L(x) x##ULL
#include <strings.h>
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

#include "mimiclib.h"
#define flac_printf mimic_printf

#include <stdarg.h>


#if defined(USE_FAT_FS)
#include "ff.h"
/*FRESULT f_open (FIL* fp, const TCHAR* path, BYTE mode);*/
#define flac_fopen f_open
#define flac_utime f_utime
#define flac_unlink f_unlink
#define flac_rename f_rename
#define flac_stat f_stat
#define flac_ftello f_tell
#else
#error "Unkown Filesystem"
#endif

extern uint32_t flac_fwrite(const void *buf, size_t size, size_t n, FIL *fp);
extern uint32_t flac_fread(void *buf, size_t size, size_t n, FIL *fp);
extern int flac_chmod(const char szFilePath[], int mode);
extern int flac_fseeko(FIL *fp, int32_t offset, int32_t whence);




extern void *pvlibSYSMalloc( size_t xWantedSize );
extern void vlibSYSPortFree( void *pv );
extern size_t xlibSYSPortGetFreeHeapSize( void );
#define FLAC__FREE(x) FlacSysFree((uintptr_t)(x), __FUNCTION__, __LINE__)
#define FLAC_MALLOC(x) FlacSysMalloc((x), __FUNCTION__, __LINE__)
#define FLAC_CALLOC(x,y) FlacSysCalloc((x),(y), __FUNCTION__, __LINE__)
#define FLAC_REALLOC(x,y) FlacSysRealloc((x),(y), __FUNCTION__, __LINE__)


extern void AddMallocInfo(uintptr_t addr, uint32_t u32size, char *psz, uint32_t u32Line);
extern void DelMallocInfo(uintptr_t addr);
extern void DumpMallocInfo(void);

static inline void *FlacSysMalloc(size_t xWantedSize, const char pszFunc[], uint32_t u32Line){
	
	void *ptr = pvlibSYSMalloc(xWantedSize);
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
		vlibSYSPortFree((void*)pv);
		//DelMallocInfo(pv);
		//flac_printf("[%s (%d)] ptr = 0x%08lX\r\n", pszFile, u32Line, pv);
	}
}



static inline void *FlacSysCalloc(uint32_t i, uint32_t j, const char pszFunc[], uint32_t u32Line){
	void *ptr = FlacSysMalloc(i*j, pszFunc, u32Line);
	if(ptr != NULL){
		memset(ptr, 0, i*j);
	}else{
		flac_printf("[%s (%d)] pvlibSYSMalloc NG (Free = %lu, Wanted = %lu)\r\n", pszFunc, u32Line, xlibSYSPortGetFreeHeapSize(), i*j);
	}

	return ptr;
}
static inline void *FlacSysRealloc(void *ptrOld, size_t size, const char pszFunc[], uint32_t u32Line){
	void *ptrNew = FlacSysMalloc(size, pszFunc, u32Line);
	if(ptrNew != NULL){
		memcpy(ptrNew, ptrOld, size);
		FlacSysFree((uintptr_t)ptrOld, pszFunc, u32Line);
	}

	return ptrNew;
}

static inline char *flac_strdup(const char szStr[]){
	char *pret = NULL;
	if(szStr != NULL){
		pret = pvlibSYSMalloc(strlen(szStr) + 1);
		if(pret != NULL){
			strcpy(pret, szStr);
		}
	}

	return pret;
}



#ifdef __cplusplus
};
#endif

#endif /* FLAC__SHARE__COMPAT_H */
