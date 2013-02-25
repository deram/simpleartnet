#include <stdio.h>
//#include <unistd.h>
//#include <errno.h>
//#include <string.h>
//#include <stdlib.h>

#define DMX_START 18
#define DMX_LENGTH 512
#define DMX_MAX DMX_START + DMX_LENGTH

FILE* dmxusb_fp=0;

int dmxusb_init(FILE* fp) {
	unsigned char out_buf[4];
	int i;
	dmxusb_fp=fp;

	out_buf[0]=0x22;
	out_buf[1]=0x22;
	out_buf[2]=0x22;
	fwrite(out_buf, 1, 3, dmxusb_fp);
	fflush(dmxusb_fp);
	sleep(1);
	
	out_buf[0]=0x44;
	i=fwrite(out_buf, 1, 1, dmxusb_fp);
	fflush(dmxusb_fp);
	sleep(1);
	return i;
}

struct usbdmx_data {
	unsigned char start;
	unsigned char chan;
	unsigned char val;
};

int dmxusb_send(unsigned char* in_buf, int in_length) {
	static unsigned char prev_buf[DMX_LENGTH]={0};
	static struct usbdmx_data out_buf[DMX_LENGTH];
	static int first_run=1;
	int out_length=0;
	int startbyte=0x48;
	int i;

	for(i=0; i<in_length; i++){
		if(prev_buf[i] != in_buf[i] || first_run == 1) {
			prev_buf[i] = in_buf[i];
			if (i==0x100) startbyte=0x49;
			out_buf[out_length].start=startbyte;
			out_buf[out_length].chan=(unsigned char)(i%256);
			out_buf[out_length].val=in_buf[i];
			out_length++;
		}
	}
	first_run=0;
	i=fwrite(out_buf, 3, out_length, dmxusb_fp);
	fflush(dmxusb_fp);
	return i;
}

