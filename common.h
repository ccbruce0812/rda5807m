/*
 * MIT License
 * 
 * Copyright (c) 2017 ccbruce0812 (ccbruce@gmail.com)
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef COMMON_H
#define COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include <i2c/i2c.h>

#include <FreeRTOS.h>
#include <task.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>

#define SWAP(a, b) { a^=b^=a^=b; }
#define MSEC2TICKS(n) (n/portTICK_PERIOD_MS)

#ifndef RDA5807M_NDEBUG
static inline int RDA5807M___dbg__(const char *loc, const char *fmt, ...) {
	va_list args;
	char _fmt[256]="";
	int res;
	
	va_start(args, fmt);
	sprintf(_fmt, "[%s] %s", loc, fmt);
	res=vprintf(_fmt, args);
	va_end(args);
	
	return res;
}

#define DBG(...) RDA5807M___dbg__(__func__, __VA_ARGS__)
#else
#define DBG(...)
#endif

int RDA5807M_readReg(unsigned char regAddr, unsigned short *pData);
int RDA5807M_writeReg(unsigned char regAddr, unsigned short data);
#define readReg(a, b) RDA5807M_readReg(a, b)
#define writeReg(a, b) RDA5807M_writeReg(a, b)

/*
static int readRegs(unsigned short *pData, unsigned int count);
static int writeRegs(unsigned short *pData, unsigned int count);
*/

#ifdef __cplusplus
}
#endif

#endif
