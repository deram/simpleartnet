#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
//#include <unistd.h>
//#include <errno.h>
//#include <stdlib.h>

#include "artnet.h"

int artnet_sock=0;

int artnet_init(char* addr,int port, int reuseaddr) {
	int retval=0;
	struct sockaddr_in server_addr;

	//open udp socket
	if ((artnet_sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		retval = -1;
		goto err1;
	}

	if(reuseaddr) {
		if (setsockopt(artnet_sock, SOL_SOCKET, 
					SO_REUSEADDR, &reuseaddr, 4) ==-1) {
			perror("setsockopt");
			retval = -1;
			goto err1;
		}
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = inet_addr(addr);
	bzero(&(server_addr.sin_zero),8);

	if (bind(artnet_sock,(struct sockaddr *)&server_addr,
				sizeof(struct sockaddr)) == -1)
	{
		perror("bind");
		retval = -1;
		goto err1;
	}

	return artnet_sock;
err1:
	return retval;

}

int artnet_read(unsigned char* recv_buf, int recv_len) {
	struct sockaddr_in client_addr;
	int addr_len;
	addr_len = sizeof(struct sockaddr);
	return recvfrom(artnet_sock,recv_buf,recv_len,0, 
			(struct sockaddr *)&client_addr, &addr_len);
}

struct artnet_data artnet_validate(unsigned char* recv_buf, int recv_len) {
	artnet_generic *ag = (artnet_generic *) recv_buf;

	if (strncmp(ag->identifier, ARTNET_IDENTIFIER, 8) == 0) {
		if(ag->opcode == ARTNET_OP_DMX) {
			artnet_dmx *ad=(artnet_dmx *)ag;
			//fprintf(stderr, "dmx universe: 0x%x\n", ad->universe);
			struct artnet_data ret;
			ret.type=dmx_universe;
			ret.dmx_universe=ad->universe;
			ret.dmx_data=ad->dmx;
			return ret;
		}
		//else fprintf(stderr, "opcode: %x\n", ag->opcode);
	}
}
