# RDA5807M
RDA5807M driver for ESP8266 (esp-open-rtos)


## Description
RDA5807M is a low cost FM receiver with I2C interface. Users can use it to build a small FM radio receiver. Due to the small size of the chip, the module RRD-102 is more popular among creators. RRD-102 integrates RDA5807M, surrounding parts and pads for easily soldering. This software provides interfaces to access registers in RDA5807M / RRD-102 and gives meaningful result to user.


## How to use this software
First at all, build your own RDA5807M circuit. If you don't know how, the reference design of [datasheet](http://wiki.seeedstudio.com/wiki/File:RDA5807M_datasheet_v1.1.pdf) would be helpful for you.


Make sure you are using esp-open-rtos. The current version of this software is for esp-open-rtos only. Put this software in the extra folder of esp-open-rtos and add "extras/rda5807m" and "extras/i2c" to "EXTRA_COMPONENTS" variable of the makefile of your project. You may want to disable debug outputs by setting 'RDA5807M_NDEBUG' variable if they are annoying.

## Task-safe
All APIs of this software are NOT task-safe. You should do synchronization in callers of these APIs.


## Initialization
You should initialize RDA5807M before any API call. Here is an example:

	RDA5807M_SETTING rxSetting={
		.clkSetting={
			.isClkNoCalb=RDA5807M_FALSE,
			.isClkDirInp=RDA5807M_FALSE,
			.freq=RDA5807M_CLK_FREQ_32_768KHZ,
		},
		.useRDS=RDA5807M_TRUE,
		.useNewDMMethod=RDA5807M_FALSE,
		.isDECNST50us=RDA5807M_FALSE,
		.system={
			.band=RDA5807M_BAND_87_108_MHZ,
			.is6575Sys=RDA5807M_FALSE,
			.space=RDA5807M_SPACE_100_KHZ
		}
	};
	.
	.
	.
	RDA5807M_init(&rxSetting);
	RDA5807M_setFreq(96000);
	RDA5807M_enableOutput(RDA5807M_TRUE);
	RDA5807M_setVolume(1);
	RDA5807M_unmute(RDA5807M_TRUE);

Above statements configure RDA5807M as:
* Don't do clock calibrating.
* Clock source is crystal oscillator.
* Crystal oscillator is rated at 32.768KHz.
* Receive and parse RDS data.
* Enable new demodulate method.
* Use 75us de-emphasize time.
* Use 87 - 108MHz band range.
* Use 65 - 76MHz system.
* Use 100KHz channel space.
* Initial frequency is 96MHz.
* Enable audio power amplifier.
* Initial volume level is 1.
* Unmute.

## Debugging
You can invoke RDA5807M_dump() to print register values in your project.

