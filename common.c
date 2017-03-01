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

#include "common.h"
#include "rda5807m.h"

int RDA5807M_readReg(unsigned char regAddr, unsigned short *pData) {
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

int RDA5807M_writeReg(unsigned char regAddr, unsigned short data) {
	unsigned char buf[3]={regAddr, (unsigned char)(data>>8), (unsigned char)data};

	if(!i2c_slave_write(RDA5807M_ADDR, buf, sizeof(buf)))
		goto failed;

	return 0;

failed:
	return -1;
}

/*
int readRegs(unsigned short *pData, unsigned int count) {
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

int writeRegs(unsigned short *pData, unsigned int count) {
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