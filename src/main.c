#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "dmxusb.h"
#include "artnet.h"

volatile sig_atomic_t running = 1;
volatile sig_atomic_t save = 1;

void artnet_to_dmxusb() {
	unsigned char recv_buf[1024];
	unsigned char* dmx;
	struct artnet_data msg_info;
	int bytes_read;
	int ret;
	int universe=0;
	while(running) {
		bytes_read = artnet_read(recv_buf, 1024);
		msg_info=artnet_validate(recv_buf,bytes_read);
		if(msg_info.type==dmx_universe && 
		   msg_info.dmx_universe==universe) {
			dmx=msg_info.dmx_data;
			ret=dmxusb_send(dmx, DMX_LENGTH, save);
			save=0;
		}
	}
}


void handler(int sig) {
	switch (sig) {
	case SIGINT:
		running = 0;
		break;
	case SIGUSR1:
		save = 1;
		break;
	}
}


int main(int argc, char* argv[]) {
	int read=0;
	unsigned char startbyte;
	FILE* dmxfp;

	FILE* savefp;

	char* ip="127.255.255.255";
	char* port="6454";
	char* device="/dev/ttyUSB0";

	switch(argc) {
	case 4:
		device=argv[3];
	case 3:
		port=argv[2];
	case 2:
		ip=argv[1];
	}

	if(artnet_init(ip, atoi(port), 1) == -1)
	{ exit(1); }

	dmxfp=fopen(device, "w");
	if (dmxfp != NULL) {
		dmxusb_init(dmxfp);
	} else {
		perror("fopen");
		exit(1);
	}

	signal(SIGINT, handler);
	signal(SIGUSR1, handler);

	savefp=fopen(SAVEFILE, "rb");
	if (savefp) {
		unsigned char dmx[DMX_LENGTH]= {0};
		fread(dmx, 1, sizeof(dmx), savefp);
		fclose(savefp);
		dmxusb_send(dmx, DMX_LENGTH, 0);
	}

	artnet_to_dmxusb();
}

