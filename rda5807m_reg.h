#ifndef RDA5807M_REG_H
#define RDA5807M_REG_H

//RDA5807M's Register addresses & maps
#define RDA5807M_REG_ADDR_00		(0x00)
#define RDA5807M_REG_ADDR_02		(0x02)
#define RDA5807M_REG_ADDR_03		(0x03)
#define RDA5807M_REG_ADDR_04		(0x04)
#define RDA5807M_REG_ADDR_05		(0x05)
#define RDA5807M_REG_ADDR_06		(0x06)
#define RDA5807M_REG_ADDR_07		(0x07)
#define RDA5807M_REG_ADDR_0A		(0x0a)
#define RDA5807M_REG_ADDR_0B		(0x0b)
#define RDA5807M_REG_ADDR_0C		(0x0c)
#define RDA5807M_REG_ADDR_0D		(0x0d)
#define RDA5807M_REG_ADDR_0E		(0x0e)
#define RDA5807M_REG_ADDR_0F		(0x0f)

typedef union {
	struct {
		unsigned short unused0		: 8;
		unsigned short CHIPID		: 8;
	} f;
	unsigned short b;
} __attribute__((__packed__)) RDA5807M_REG_00;

typedef union {
	struct {
		unsigned short ENABLE		: 1;
		unsigned short SOFT_RESET	: 1;
		unsigned short NEW_METHOD	: 1;
		unsigned short RDS_EN		: 1;
		unsigned short CLK_MODE		: 3;
		unsigned short SKMODE		: 1;
		unsigned short SEEK			: 1;
		unsigned short SEEKUP		: 1;
		unsigned short RCLK_DIR_INP	: 1;
		unsigned short RCLK_NON_CAL	: 1;
		unsigned short BASS			: 1;
		unsigned short MONO			: 1;
		unsigned short DMUTE		: 1;
		unsigned short DHIZ			: 1;
	} f;
	unsigned short b;
} __attribute__((__packed__)) RDA5807M_REG_02;

typedef union {
	struct {
		unsigned short SPACE		: 2;
		unsigned short BAND			: 2;
		unsigned short TUNE			: 1;
		unsigned short DIR_MODE		: 1;
		unsigned short CHAN			: 10;
	} f;
	unsigned short b;
} __attribute__((__packed__)) RDA5807M_REG_03;

typedef union {
	struct {
		unsigned short unused0		: 8;
		unsigned short AFCD			: 1;
		unsigned short SOFTMUTE_EN	: 1;
		unsigned short unused1		: 1;
		unsigned short DE			: 1;
		unsigned short unused2		: 4;
	} f;
	unsigned short b;
} __attribute__((__packed__)) RDA5807M_REG_04;

typedef union {
	struct {
		unsigned short VOLUME		: 4;
		unsigned short unused0		: 4;
		unsigned short SEEKTH		: 4;
		unsigned short unused1		: 3;
		unsigned short INT_MODE		: 1;
	} f;
	unsigned short b;
} __attribute__((__packed__)) RDA5807M_REG_05;

typedef union {
	struct {
		unsigned short unused0		: 13;
		unsigned short OPEN_MODE	: 2;
		unsigned short unused1		: 1;
	} f;
	unsigned short b;
} __attribute__((__packed__)) RDA5807M_REG_06;

typedef union {
	struct {
		unsigned short FREQ_MODE	: 1;
		unsigned short SOFTBLEND_EN	: 1;
		unsigned short SEEK_TH_OLD	: 6;
		unsigned short unused0		: 1;
		unsigned short BAND_OPT		: 1;
		unsigned short TH_SOFRBLEND	: 5;
		unsigned short unused1		: 1;
	} f;
	unsigned short b;
} __attribute__((__packed__)) RDA5807M_REG_07;

typedef union {
	struct {
		unsigned short READCHAN		: 10;
		unsigned short ST			: 1;
		unsigned short BLK_E		: 1;
		unsigned short RDSS			: 1;
		unsigned short SF			: 1;
		unsigned short STC			: 1;
		unsigned short RDSR			: 1;
	} f;
	unsigned short b;
} __attribute__((__packed__)) RDA5807M_REG_0A;

typedef union {
	struct {
		unsigned short BLERB		: 2;
		unsigned short BLERA		: 2;
		unsigned short ABCD_E		: 1;
		unsigned short unused0		: 2;
		unsigned short FM_READY		: 1;
		unsigned short FM_TRUE		: 1;
		unsigned short RSSI			: 7;
	} f;
	unsigned short b;
} __attribute__((__packed__)) RDA5807M_REG_0B;

typedef union {
	struct {
		unsigned short RDSA			: 16;
	} f;
	unsigned short b;
} __attribute__((__packed__)) RDA5807M_REG_0C;

typedef union {
	struct {
		unsigned short RDSB			: 16;
	} f;
	unsigned short b;
} __attribute__((__packed__)) RDA5807M_REG_0D;

typedef union {
	struct {
		unsigned short RDSC			: 16;
	} f;
	unsigned short b;
} __attribute__((__packed__)) RDA5807M_REG_0E;

typedef union {
	struct {
		unsigned short RDSD			: 16;
	} f;
	unsigned short b;
} __attribute__((__packed__)) RDA5807M_REG_0F;

#endif
