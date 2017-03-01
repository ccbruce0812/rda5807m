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

#ifndef RDA5807M_H
#define RDA5807M_H

#include "rda5807m_reg.h"
#include "rda5807m_data.h"

//RDA5807M's I2C addresses
#define RDA5807M_ADDR				(0x11)
#define RDA5807M_SEQADDR			(0x10)

void RDA5807M_dump(void);

int RDA5807M_init(const RDA5807M_SETTING *pSetting);

int RDA5807M_enableOutput(RDA5807M_BOOL flag);

int RDA5807M_isOutputEnable(RDA5807M_BOOL *pFlag);

int RDA5807M_unmute(RDA5807M_BOOL flag);

int RDA5807M_isUnmute(RDA5807M_BOOL *pFlag);

int RDA5807M_setBB(RDA5807M_BOOL flag);

int RDA5807M_isBB(RDA5807M_BOOL *pFlag);

int RDA5807M_setFreq(unsigned long freq);

int RDA5807M_getFreq(unsigned long *pFreq);

int RDA5807M_seek(RDA5807M_BOOL isUp, RDA5807M_BOOL dontWrap);

int RDA5807M_isSeekingComplete(RDA5807M_BOOL *pFlag);

int RDA5807M_isSeekingFailed(RDA5807M_BOOL *pFlag);

int RDA5807M_stopSeeking(void);

int RDA5807M_setAFCDisable(RDA5807M_BOOL flag);

int RDA5807M_getAFCDisable(RDA5807M_BOOL *pFlag);

int RDA5807M_setSeekSNRThrshold(unsigned char threshold);

int RDA5807M_getSeekSNRThrshold(unsigned char *pThreshold);

int RDA5807M_setVolume(unsigned char volume);

int RDA5807M_getVolume(unsigned char *pVolume);

int RDA5807M_setSoftBlendThreshold(unsigned char threshold);

int RDA5807M_getSoftBlendThreshold(unsigned char *pThreshold);

int RDA5807M_setSoftBlendEnable(RDA5807M_BOOL flag);

int RDA5807M_isSoftBlendEnable(RDA5807M_BOOL *pFlag);

int RDA5807M_isRDSReady(RDA5807M_BOOL *pFlag);

int RDA5807M_isRDSSynced(RDA5807M_BOOL *pFlag);

int RDA5807M_isBlockEFound(RDA5807M_BOOL *pFlag);

int RDA5807M_isStereo(RDA5807M_BOOL *pFlag);

int RDA5807M_getRSSI(unsigned char *pRSSI);

int RDA5807M_isStation(RDA5807M_BOOL *pFlag);

int RDA5807M_isFMReady(RDA5807M_BOOL *pFlag);

#endif
