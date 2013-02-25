#ifndef DMXUSB_H_
#define DMXUSB_H_ 1

#define DMX_START 18
#define DMX_LENGTH 512
#define DMX_MAX DMX_START + DMX_LENGTH
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#define SAVEFILE "dmxstate.dat"

int dmxusb_init(FILE* fp);
int dmxusb_send(unsigned char* in_buf, int in_length, int save_current);

struct usbdmx_data {
	unsigned char start;
	unsigned char chan;
	unsigned char val;
};

#endif //ifndef
