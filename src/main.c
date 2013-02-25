#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
//#include <errno.h>
//#include <string.h>

#include "dmxusb.h"
#include "artnet.h"


void artnet_to_dmxusb() {
	unsigned char recv_buf[1024];
	unsigned char* dmx;
	struct artnet_data msg_info;
	int bytes_read;
	int ret;
	int universe=0;
	while(1) {
		bytes_read = artnet_read(recv_buf, 1024);
		msg_info=artnet_validate(recv_buf,bytes_read);
		if(msg_info.type==dmx_universe && msg_info.dmx_universe==universe) {
			dmx=msg_info.dmx_data;
			ret=dmxusb_send(dmx, DMX_LENGTH);
			//if (ret) fprintf(stderr, "Sent: %d    \r", ret);
		}
	}
}


int main(int argc, char* argv[]) {
	int read=0;
	unsigned char startbyte;
	FILE* fp;

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

	fp=fopen(device, "w");
	if (fp != NULL) {
		dmxusb_init(fp);
	} else {
		perror("fopen");
		exit(1);
		dmxusb_init(stdout);
	}
	artnet_to_dmxusb();
}

