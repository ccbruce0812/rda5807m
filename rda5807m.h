/*
	Description:
		A FM receiver API for RDA5807M.
	Author
		Bruce Hsu
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
