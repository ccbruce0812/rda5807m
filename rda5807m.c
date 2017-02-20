#include <i2c/i2c.h>

#include <FreeRTOS.h>
#include <task.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>

#include "rda5807m.h"

#define SWAP(a, b) { a^=b^=a^=b; }
#define MSEC2TICKS(n) (n/portTICK_PERIOD_MS)

#ifndef RDA5807M_NDEBUG
static inline int __dbg__(const char *loc, const char *fmt, ...) {
	va_list args;
	char _fmt[256]="";
	int res;
	
	va_start(args, fmt);
	sprintf(_fmt, "[%s] %s", loc, fmt);
	res=vprintf(_fmt, args);
	va_end(args);
	
	return res;
}

#define DBG(...) __dbg__(__func__, __VA_ARGS__)
#else
#define DBG(...)
#endif

static int readReg(unsigned char regAddr, unsigned short *pData) {
	if(!pData) {
		DBG("Bad argument. Check your code.\n");
		assert(false);
	}
	
	if(!i2c_slave_read(RDA5807M_ADDR, regAddr, (unsigned char *)pData, sizeof(unsigned short)))
		goto failed;

	unsigned char *pArr=(unsigned char *)pData;
	
	SWAP(pArr[0], pArr[1]);
	
	return 0;

failed:
	return -1;
}

static int writeReg(unsigned char regAddr, unsigned short data) {
	unsigned char buf[3]={regAddr, (unsigned char)(data>>8), (unsigned char)data};

	if(!i2c_slave_write(RDA5807M_ADDR, buf, sizeof(buf)))
		goto failed;

	return 0;

failed:
	return -1;
}

/*
static int readRegs(unsigned short *pData, unsigned int count) {
	int i;
	
	if(count<1)
		goto failed;
	
	count=Wire.requestFrom((int)RDA5807M_SEQADDR, count*2);
	for(i=0;i<count/2;i++) {
		pData[i]=Wire.read();
		pData[i]<<=8;
		pData[i]|=Wire.read();
	}
	
	return 0;

failed:
	DBG("Failed.\n");
	return -1;
}

static int writeRegs(unsigned short *pData, unsigned int count) {
	int i;
	
	if(count<1)
		goto failed;
	
	Wire.beginTransmission(RDA5807M_SEQADDR);
	for(i=0;i<count;i++) {
		Wire.write(pData[i]>>8);
		Wire.write(pData[i]&0xff);
	}
	Wire.endTransmission();
	
	return 0;
	
failed:
	return -1;
}
*/

void RDA5807M_dump(void) {
	unsigned short reg=0;
	int i=0;
	
	for(i=0;i<16;i++) {
		readReg(RDA5807M_REG_ADDR_00+i, &reg);
		DBG("reg %02X: %04X\n", i, reg);
	}
}

int RDA5807M_init(const RDA5807M_SETTING *pSetting) {
	RDA5807M_REG_00 r00;
	RDA5807M_REG_02 r02;
	RDA5807M_REG_03 r03;
	RDA5807M_REG_04 r04;
	RDA5807M_REG_07 r07;
	
	if(!pSetting)
		goto failed;

	r02.f.SOFT_RESET=1;
	r02.f.ENABLE=1;
	if(writeReg(RDA5807M_REG_ADDR_02, r02.b)<0) {
		DBG("[%d] Failed to invoke writeReg().\n", __LINE__);
		goto failed;
	}

	if(readReg(RDA5807M_REG_ADDR_02, &r02.b)<0) {
		DBG("[%d] Failed to invoke readReg().\n", __LINE__);
		goto failed;
	}
	
	r02.f.RCLK_NON_CAL=pSetting->clkSetting.isClkNoCalb?1:0;
	r02.f.RCLK_DIR_INP=pSetting->clkSetting.isClkDirInp?1:0;
	r02.f.CLK_MODE=pSetting->clkSetting.freq;
	r02.f.RDS_EN=pSetting->useRDS?1:0;
	r02.f.NEW_METHOD=pSetting->useNewDMMethod?1:0;
	r02.f.SOFT_RESET=0;
	r02.f.ENABLE=1;
	r04.f.DE=pSetting->isDECNST50us?1:0;
	r03.f.BAND=pSetting->system.band;
	r03.f.SPACE=pSetting->system.space;
	r07.f.BAND_OPT=pSetting->system.is6575Sys?1:0;

	if(writeReg(RDA5807M_REG_ADDR_02, r02.b)<0) {
		DBG("[%d] Failed to invoke writeReg().\n", __LINE__);
		goto failed;
	}

	if(writeReg(RDA5807M_REG_ADDR_03, r03.b)<0) {
		DBG("[%d] Failed to invoke writeReg().\n", __LINE__);
		goto failed;
	}

	if(writeReg(RDA5807M_REG_ADDR_04, r04.b)<0) {
		DBG("[%d] Failed to invoke writeReg().\n", __LINE__);
		goto failed;
	}

	if(writeReg(RDA5807M_REG_ADDR_07, r07.b)<0) {
		DBG("[%d] Failed to invoke writeReg().\n", __LINE__);
		goto failed;
	}
	
	return RDA5807M_OK;

failed:
	return RDA5807M_ERR_FAILED;
}

int RDA5807M_enableOutput(RDA5807M_BOOL flag) {
	RDA5807M_REG_02 r02;
		
	if(readReg(RDA5807M_REG_ADDR_02, &r02.b)<0) {
		DBG("[%d] Failed to invoke readReg().\n", __LINE__);
		goto failed;
	}

	r02.f.DHIZ=flag?1:0;

	if(writeReg(RDA5807M_REG_ADDR_02, r02.b)<0) {
		DBG("[%d] Failed to invoke writeReg().\n", __LINE__);
		goto failed;
	}
	
	return RDA5807M_OK;
	
failed:
	return RDA5807M_ERR_FAILED;
}

int RDA5807M_isOutputEnable(RDA5807M_BOOL *pFlag) {
	RDA5807M_REG_02 r02;
		
	if(readReg(RDA5807M_REG_ADDR_02, &r02.b)<0) {
		DBG("[%d] Failed to invoke readReg().\n", __LINE__);
		goto failed;
	}

	*pFlag=r02.f.DHIZ?RDA5807M_TRUE:RDA5807M_FALSE;
	
	return RDA5807M_OK;
	
failed:
	return RDA5807M_ERR_FAILED;
}

int RDA5807M_unmute(RDA5807M_BOOL flag) {
	RDA5807M_REG_02 r02;
		
	if(readReg(RDA5807M_REG_ADDR_02, &r02.b)<0) {
		DBG("[%d] Failed to invoke readReg().\n", __LINE__);
		goto failed;
	}

	r02.f.DMUTE=flag?1:0;

	if(writeReg(RDA5807M_REG_ADDR_02, r02.b)<0) {
		DBG("[%d] Failed to invoke writeReg().\n", __LINE__);
		goto failed;
	}
	
	return RDA5807M_OK;
	
failed:
	return RDA5807M_ERR_FAILED;
}

int RDA5807M_isUnmute(RDA5807M_BOOL *pFlag) {
	RDA5807M_REG_02 r02;
		
	if(readReg(RDA5807M_REG_ADDR_02, &r02.b)<0) {
		DBG("[%d] Failed to invoke readReg().\n", __LINE__);
		goto failed;
	}

	*pFlag=r02.f.DMUTE?RDA5807M_TRUE:RDA5807M_FALSE;
	
	return RDA5807M_OK;
	
failed:
	return RDA5807M_ERR_FAILED;
}

int RDA5807M_setBB(RDA5807M_BOOL flag) {
	RDA5807M_REG_02 r02;
		
	if(readReg(RDA5807M_REG_ADDR_02, &r02.b)<0) {
		DBG("[%d] Failed to invoke readReg().\n", __LINE__);
		goto failed;
	}

	r02.f.BASS=flag?1:0;

	if(writeReg(RDA5807M_REG_ADDR_02, r02.b)<0) {
		DBG("[%d] Failed to invoke writeReg().\n", __LINE__);
		goto failed;
	}
	
	return RDA5807M_OK;
	
failed:
	return RDA5807M_ERR_FAILED;
}

int RDA5807M_isBB(RDA5807M_BOOL *pFlag) {
	RDA5807M_REG_02 r02;
		
	if(readReg(RDA5807M_REG_ADDR_02, &r02.b)<0) {
		DBG("[%d] Failed to invoke readReg().\n", __LINE__);
		goto failed;
	}

	*pFlag=r02.f.BASS?RDA5807M_TRUE:RDA5807M_FALSE;
	
	return RDA5807M_OK;
	
failed:
	return RDA5807M_ERR_FAILED;
}

int RDA5807M_setFreq(unsigned long freq) {
	RDA5807M_REG_03 r03;
	RDA5807M_REG_07 r07;
	RDA5807M_REG_0A r0A;
	
	unsigned long base[4]={87000, 76000, 76000, 65000},
				space[4]={100, 200, 50, 25},
				channel=0;
				
	unsigned int now=0, prev=0;
				
	if(readReg(RDA5807M_REG_ADDR_03, &r03.b)<0) {
		DBG("[%d] Failed to invoke readReg().\n", __LINE__);
		goto failed;
	}

	if(readReg(RDA5807M_REG_ADDR_07, &r07.b)<0) {
		DBG("[%d] Failed to invoke readReg().\n", __LINE__);
		goto failed;
	}
	
	if(r07.f.BAND_OPT)
		base[3]=50000;
	
	channel=(freq-base[r03.f.BAND])/space[r03.f.SPACE];
	r03.f.CHAN=(unsigned short)channel;
	r03.f.TUNE=1;
	
	if(writeReg(RDA5807M_REG_ADDR_03, r03.b)<0) {
		DBG("[%d] Failed to invoke writeReg().\n", __LINE__);
		goto failed;
	}
	
	now=prev=xTaskGetTickCount();
	while(1) {
		if(readReg(RDA5807M_REG_ADDR_0A, &r0A.b)<0) {
			DBG("[%d] Failed to invoke readReg().\n", __LINE__);
			goto failed;
		}
		
		if(r0A.f.STC)
			break;
		
		now=xTaskGetTickCount();
		if(now-prev>=MSEC2TICKS(1500)) {
			DBG("[%d] Failed to set frequency.\n", __LINE__);
			goto failed;
		}
		vTaskDelay(MSEC2TICKS(500));
	}

	return RDA5807M_OK;
	
failed:
	return RDA5807M_ERR_FAILED;
}

int RDA5807M_getFreq(unsigned long *pFreq) {
	RDA5807M_REG_03 r03;
	RDA5807M_REG_07 r07;
	RDA5807M_REG_0A r0A;
	
	unsigned long base[4]={87000, 76000, 76000, 65000},
				space[4]={100, 200, 50, 25};
				
	if(readReg(RDA5807M_REG_ADDR_03, &r03.b)<0) {
		DBG("[%d] Failed to invoke readReg().\n", __LINE__);
		goto failed;
	}

	if(readReg(RDA5807M_REG_ADDR_07, &r07.b)<0) {
		DBG("[%d] Failed to invoke readReg().\n", __LINE__);
		goto failed;
	}
	
	if(readReg(RDA5807M_REG_ADDR_0A, &r0A.b)<0) {
		DBG("[%d] Failed to invoke readReg().\n", __LINE__);
		goto failed;
	}
	
	if(r07.f.BAND_OPT)
		base[3]=50000;
	
	*pFreq=r0A.f.READCHAN;
	*pFreq=*pFreq*space[r03.f.SPACE]+base[r03.f.BAND];

	return RDA5807M_OK;

failed:
	return RDA5807M_ERR_FAILED;
}

int RDA5807M_seek(RDA5807M_BOOL isUp, RDA5807M_BOOL dontWrap) {
	RDA5807M_REG_02 r02;
	
	if(readReg(RDA5807M_REG_ADDR_02, &r02.b)<0) {
		DBG("[%d] Failed to invoke readReg().\n", __LINE__);
		goto failed;
	}
	
	r02.f.SEEKUP=isUp?1:0;
	r02.f.SKMODE=dontWrap?1:0;
	r02.f.SEEK=1;
	
	if(writeReg(RDA5807M_REG_ADDR_02, r02.b)<0) {
		DBG("[%d] Failed to invoke writeReg().\n", __LINE__);
		goto failed;
	}
	
	return RDA5807M_OK;

failed:
	return RDA5807M_ERR_FAILED;
}

int RDA5807M_isSeekingComplete(RDA5807M_BOOL *pFlag) {
	RDA5807M_REG_0A r0A;
	
	if(readReg(RDA5807M_REG_ADDR_0A, &r0A.b)<0) {
		DBG("[%d] Failed to invoke readReg().\n", __LINE__);
		goto failed;
	}
	
	*pFlag=r0A.f.STC?RDA5807M_TRUE:RDA5807M_FALSE;

	return RDA5807M_OK;

failed:
	return RDA5807M_ERR_FAILED;
}

int RDA5807M_isSeekingFailed(RDA5807M_BOOL *pFlag) {
	RDA5807M_REG_0A r0A;
	
	if(readReg(RDA5807M_REG_ADDR_0A, &r0A.b)<0) {
		DBG("[%d] Failed to invoke readReg().\n", __LINE__);
		goto failed;
	}
	
	*pFlag=r0A.f.SF?RDA5807M_TRUE:RDA5807M_FALSE;

	return RDA5807M_OK;

failed:
	return RDA5807M_ERR_FAILED;
}

int RDA5807M_stopSeeking(void) {
	RDA5807M_REG_02 r02;
	
	if(readReg(RDA5807M_REG_ADDR_02, &r02.b)<0) {
		DBG("[%d] Failed to invoke readReg().\n", __LINE__);
		goto failed;
	}
	
	r02.f.SEEK=0;

	if(writeReg(RDA5807M_REG_ADDR_02, r02.b)<0) {
		DBG("[%d] Failed to invoke writeReg().\n", __LINE__);
		goto failed;
	}
	
	return RDA5807M_OK;

failed:
	return RDA5807M_ERR_FAILED;
}

int RDA5807M_setAFCDisable(RDA5807M_BOOL flag) {
	RDA5807M_REG_04 r04;
	
	if(readReg(RDA5807M_REG_ADDR_04, &r04.b)<0) {
		DBG("[%d] Failed to invoke readReg().\n", __LINE__);
		goto failed;
	}
	
	r04.f.AFCD=flag?1:0;
	
	if(writeReg(RDA5807M_REG_ADDR_04, r04.b)<0) {
		DBG("[%d] Failed to invoke writeReg().\n", __LINE__);
		goto failed;
	}
	
	return RDA5807M_OK;

failed:
	return RDA5807M_ERR_FAILED;
}

int RDA5807M_getAFCDisable(RDA5807M_BOOL *pFlag) {
	RDA5807M_REG_04 r04;
	
	if(readReg(RDA5807M_REG_ADDR_04, &r04.b)<0) {
		DBG("[%d] Failed to invoke readReg().\n", __LINE__);
		goto failed;
	}
	
	*pFlag=r04.f.AFCD?RDA5807M_TRUE:RDA5807M_FALSE;
	
	return RDA5807M_OK;

failed:
	return RDA5807M_ERR_FAILED;
}

int RDA5807M_setSeekSNRThrshold(unsigned char threshold) {
	RDA5807M_REG_05 r05;
	
	if(readReg(RDA5807M_REG_ADDR_05, &r05.b)<0) {
		DBG("[%d] Failed to invoke readReg().\n", __LINE__);
		goto failed;
	}
	
	r05.f.SEEKTH=threshold;
	
	if(writeReg(RDA5807M_REG_ADDR_05, r05.b)<0) {
		DBG("[%d] Failed to invoke writeReg().\n", __LINE__);
		goto failed;
	}
	
	return RDA5807M_OK;

failed:
	return RDA5807M_ERR_FAILED;
}

int RDA5807M_getSeekSNRThrshold(unsigned char *pThreshold) {
	RDA5807M_REG_05 r05;
	
	if(readReg(RDA5807M_REG_ADDR_05, &r05.b)<0) {
		DBG("[%d] Failed to invoke readReg().\n", __LINE__);
		goto failed;
	}
	
	*pThreshold=r05.f.SEEKTH;
	
	return RDA5807M_OK;

failed:
	return RDA5807M_ERR_FAILED;
}

int RDA5807M_setVolume(unsigned char volume) {
	RDA5807M_REG_05 r05;
	
	if(readReg(RDA5807M_REG_ADDR_05, &r05.b)<0) {
		DBG("[%d] Failed to invoke readReg().\n", __LINE__);
		goto failed;
	}
	
	r05.f.VOLUME=volume;
	
	if(writeReg(RDA5807M_REG_ADDR_05, r05.b)<0) {
		DBG("[%d] Failed to invoke writeReg().\n", __LINE__);
		goto failed;
	}
	
	return RDA5807M_OK;

failed:
	return RDA5807M_ERR_FAILED;
}

int RDA5807M_getVolume(unsigned char *pVolume) {
	RDA5807M_REG_05 r05;
	
	if(readReg(RDA5807M_REG_ADDR_05, &r05.b)<0) {
		DBG("[%d] Failed to invoke readReg().\n", __LINE__);
		goto failed;
	}
	
	*pVolume=r05.f.VOLUME;
	
	return RDA5807M_OK;

failed:
	return RDA5807M_ERR_FAILED;
}

int RDA5807M_setSoftBlendThreshold(unsigned char threshold) {
	RDA5807M_REG_07 r07;
	
	if(readReg(RDA5807M_REG_ADDR_07, &r07.b)<0) {
		DBG("[%d] Failed to invoke readReg().\n", __LINE__);
		goto failed;
	}
	
	r07.f.TH_SOFRBLEND=threshold;
	
	if(writeReg(RDA5807M_REG_ADDR_07, r07.b)<0) {
		DBG("[%d] Failed to invoke writeReg().\n", __LINE__);
		goto failed;
	}
	
	return RDA5807M_OK;

failed:
	return RDA5807M_ERR_FAILED;
}

int RDA5807M_getSoftBlendThreshold(unsigned char *pThreshold) {
	RDA5807M_REG_07 r07;
	
	if(readReg(RDA5807M_REG_ADDR_07, &r07.b)<0) {
		DBG("[%d] Failed to invoke readReg().\n", __LINE__);
		goto failed;
	}
	
	*pThreshold=r07.f.TH_SOFRBLEND;
	
	return RDA5807M_OK;

failed:
	return RDA5807M_ERR_FAILED;
}

int RDA5807M_setSoftBlendEnable(RDA5807M_BOOL flag) {
	RDA5807M_REG_07 r07;
	
	if(readReg(RDA5807M_REG_ADDR_07, &r07.b)<0) {
		DBG("[%d] Failed to invoke readReg().\n", __LINE__);
		goto failed;
	}
	
	r07.f.SOFTBLEND_EN=flag?1:0;
	
	if(writeReg(RDA5807M_REG_ADDR_07, r07.b)<0) {
		DBG("[%d] Failed to invoke writeReg().\n", __LINE__);
		goto failed;
	}
	
	return RDA5807M_OK;

failed:
	return RDA5807M_ERR_FAILED;
}

int RDA5807M_isSoftBlendEnable(RDA5807M_BOOL *pFlag) {
	RDA5807M_REG_07 r07;
	
	if(readReg(RDA5807M_REG_ADDR_07, &r07.b)<0) {
		DBG("[%d] Failed to invoke readReg().\n", __LINE__);
		goto failed;
	}
	
	*pFlag=r07.f.SOFTBLEND_EN?RDA5807M_TRUE:RDA5807M_FALSE;
	
	return RDA5807M_OK;

failed:
	return RDA5807M_ERR_FAILED;
}

int RDA5807M_isRDSReady(RDA5807M_BOOL *pFlag) {
	RDA5807M_REG_0A r0A;
	
	if(readReg(RDA5807M_REG_ADDR_0A, &r0A.b)<0) {
		DBG("[%d] Failed to invoke readReg().\n", __LINE__);
		goto failed;
	}
	
	*pFlag=r0A.f.RDSR?RDA5807M_TRUE:RDA5807M_FALSE;
	
	return RDA5807M_OK;

failed:
	return RDA5807M_ERR_FAILED;
}

int RDA5807M_isRDSSynced(RDA5807M_BOOL *pFlag) {
	RDA5807M_REG_0A r0A;
	
	if(readReg(RDA5807M_REG_ADDR_0A, &r0A.b)<0) {
		DBG("[%d] Failed to invoke readReg().\n", __LINE__);
		goto failed;
	}
	
	*pFlag=r0A.f.RDSS?RDA5807M_TRUE:RDA5807M_FALSE;
	
	return RDA5807M_OK;

failed:
	return RDA5807M_ERR_FAILED;
}

int RDA5807M_isBlockEFound(RDA5807M_BOOL *pFlag) {
	RDA5807M_REG_0A r0A;
	
	if(readReg(RDA5807M_REG_ADDR_0A, &r0A.b)<0) {
		DBG("[%d] Failed to invoke readReg().\n", __LINE__);
		goto failed;
	}
	
	*pFlag=r0A.f.BLK_E?RDA5807M_TRUE:RDA5807M_FALSE;
	
	return RDA5807M_OK;

failed:
	return RDA5807M_ERR_FAILED;
}

int RDA5807M_isStereo(RDA5807M_BOOL *pFlag) {
	RDA5807M_REG_0A r0A;
	
	if(readReg(RDA5807M_REG_ADDR_0A, &r0A.b)<0) {
		DBG("[%d] Failed to invoke readReg().\n", __LINE__);
		goto failed;
	}
	
	*pFlag=r0A.f.ST?RDA5807M_TRUE:RDA5807M_FALSE;
	
	return RDA5807M_OK;

failed:
	return RDA5807M_ERR_FAILED;
}

int RDA5807M_getRSSI(unsigned char *pRSSI) {
	RDA5807M_REG_0B r0B;
	
	if(readReg(RDA5807M_REG_ADDR_0B, &r0B.b)<0) {
		DBG("[%d] Failed to invoke readReg().\n", __LINE__);
		goto failed;
	}
	
	*pRSSI=r0B.f.RSSI;
	
	return RDA5807M_OK;

failed:
	return RDA5807M_ERR_FAILED;
}

int RDA5807M_isStation(RDA5807M_BOOL *pFlag) {
	RDA5807M_REG_0B r0B;
	
	if(readReg(RDA5807M_REG_ADDR_0B, &r0B.b)<0) {
		DBG("[%d] Failed to invoke readReg().\n", __LINE__);
		goto failed;
	}
	
	*pFlag=r0B.f.FM_TRUE?RDA5807M_TRUE:RDA5807M_FALSE;
	
	return RDA5807M_OK;

failed:
	return RDA5807M_ERR_FAILED;
}

int RDA5807M_isFMReady(RDA5807M_BOOL *pFlag) {
	RDA5807M_REG_0B r0B;

	if(readReg(RDA5807M_REG_ADDR_0B, &r0B.b)<0) {
		DBG("[%d] Failed to invoke readReg().\n", __LINE__);
		goto failed;
	}
	
	*pFlag=r0B.f.FM_READY?RDA5807M_TRUE:RDA5807M_FALSE;
	
	return RDA5807M_OK;

failed:
	return RDA5807M_ERR_FAILED;
}
