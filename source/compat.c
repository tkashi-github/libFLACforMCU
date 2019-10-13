/**
 * @file		compat.c
 * @brief		stdlib for libFLAC
 * @author		Takashi Kashiwagi
 * @date		2019/10/01
 * @version     0.1
 * @details 
 * --
 * License Type <MIT License>
 * --
 * Copyright 2019 Takashi Kashiwagi
 * 
 * Permission is hereby granted, FLAC_FREE of charge, to any person obtaining a 
 * copy of this software and associated documentation files (the "Software"), 
 * to deal in the Software without restriction, including without limitation 
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included 
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
 * DEALINGS IN THE SOFTWARE.
 *
 * @par Update:
 * - 2019/10/01: Takashi Kashiwagi:
 */

#include "include/compat.h"

#ifndef BUILD_TEST
#include "fsl_common.h"
typedef struct
{
	uintptr_t addr;
	uint32_t u32size;
	char *psz;
	uint32_t u32Line;
} stMallocDebug_t;

static stMallocDebug_t s_stMallocDbgTable[1000] = {0};

void AddMallocInfo(uintptr_t addr, uint32_t u32size, char *psz, uint32_t u32Line)
{
	uint32_t primask = DisableGlobalIRQ();
	for (uint32_t i = 0; i < 1000; i++)
	{
		if (s_stMallocDbgTable[i].addr == 0)
		{
			s_stMallocDbgTable[i].addr = addr;
			s_stMallocDbgTable[i].u32size = u32size;
			s_stMallocDbgTable[i].psz = psz;
			s_stMallocDbgTable[i].u32Line = u32Line;
			break;
		}
	}
	EnableGlobalIRQ(primask);
}
void DelMallocInfo(uintptr_t addr)
{
	uint32_t primask = DisableGlobalIRQ();
	for (uint32_t i = 0; i < 1000; i++)
	{
		if (s_stMallocDbgTable[i].addr == addr)
		{
			s_stMallocDbgTable[i].addr = 0;
			break;
		}
	}
	EnableGlobalIRQ(primask);
}
void DumpMallocInfo(void)
{

	flac_printf("[%s (%d)] ENTER\r\n", __FUNCTION__, __LINE__);
	for (uint32_t i = 0; i < 1000; i++)
	{
		if (s_stMallocDbgTable[i].addr != 0)
		{
			flac_printf("DBG :: [%s (%d)] ptr = 0x%08lX, size = %lu\r\n", s_stMallocDbgTable[i].psz, s_stMallocDbgTable[i].u32Line, s_stMallocDbgTable[i].addr, s_stMallocDbgTable[i].u32size);
		}
	}
	flac_printf("[%s (%d)] EXIT\r\n", __FUNCTION__, __LINE__);
}
#endif

int flac_chmod(const char szFilePath[], int mode)
{
#ifndef BUILD_TEST
	if (szFilePath == NULL)
	{
		return -1;
	}
	/** 多分 flac_fstatとつじつまが合ってればよい? */
	f_chmod(szFilePath, (BYTE)mode, (AM_RDO | AM_ARC | AM_SYS | AM_HID));
#endif
	return 0;
}

_Bool flac_fopen(FLAC_FILE *fp, const char path[], enLFlacFopenMode_t enMode)
{
	_Bool bret = false;
#ifndef BUILD_TEST
	if ((fp == NULL) && (path == NULL))
	{
		goto _END;
	}
	if((enMode == enFlacFopenModeOpen)
	{
		if (FR_OK == f_open(fp, szFilePath, FA_CREATE_NEW))
		{
			bret = true;
		}
	}
	else if(enMode == enFlacFopenModeOpenAdd){
		if (FR_OK == f_open(fp, szFilePath, FA_OPEN_APPEND))
		{
			bret = true;
		}
	}
	else
	{
		/* NOP */
	}
_END:
#endif
	return bret;
}
uint32_t flac_fwrite(const void *buf, uint32_t size, uint32_t n, FLAC_FILE *fp)
{
	uint32_t bw = 0;
#ifndef BUILD_TEST
	if (FR_OK != f_write(fp, buf, size * n, &bw))
	{
		return 0;
	}
#endif
	return bw;
}

uint32_t flac_fread(void *buf, uint32_t size, uint32_t n, FLAC_FILE *fp)
{
	uint32_t br = 0;
#ifndef BUILD_TEST
	FRESULT res = f_read(fp, buf, size * n, &br);
	if (FR_OK != res)
	{
		mimic_printf("[%s (%d)] EXIT (%d)\r\n", __FUNCTION__, __LINE__, res);
		return 0;
	}
#endif
	return br;
}

int flac_fseeko(FLAC_FILE *fp, int32_t offset, int32_t whence)
{
	int64_t i64pos;
#ifndef BUILD_TEST
	if (fp == NULL)
	{
		return -1;
	}
	if ((whence != SEEK_SET) && (whence != SEEK_CUR))
	{
		return -1;
	}
	if (whence == SEEK_SET)
	{
		i64pos = 0;
	}
	else
	{
		i64pos = f_tell(fp);
	}
	if (i64pos >= offset)
	{
		i64pos -= offset;
	}
	else
	{
		i64pos = 0;
	}

	if (FR_OK != f_lseek(fp, i64pos))
	{
		return -1;
	}
#endif
	return 0;
}

void flac_fclose(FLAC_FILE* fp)
{
#ifndef BUILD_TEST
	f_close(fp);
#endif
}