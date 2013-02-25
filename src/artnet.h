#ifndef SOCKETS_H_
#define SOCKETS_H_ 1


#include <stdint.h>

#define ARTNET_DMX_START 18

#define ARTNET_IDENTIFIER "Art-Net"
#define ARTNET_OP_DMX 0x5000

enum artnet_pkg_type {
	unknown=0,
	dmx_universe=1
};

struct artnet_data {
	enum artnet_pkg_type type;
	uint16_t dmx_universe;
	uint8_t *dmx_data;
};

typedef struct artnet_generic {
	uint8_t identifier[8];
	uint16_t opcode;
} artnet_generic;

typedef struct artnet_dmx {
	artnet_generic parent;
	uint8_t protVerH;
	uint8_t protVer;
	uint8_t sequence;
	uint8_t physical;
	uint16_t universe;
	uint8_t lengthHi;
	uint8_t length;
	uint8_t dmx[];
} artnet_dmx;

int artnet_init(char* addr,int port, int reuseaddr);
int artnet_read(unsigned char* recv_buf, int recv_len);
struct artnet_data artnet_validate(unsigned char* recv_buf, int recv_len);
#endif //ifndef
