#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum blocktype {
	IDLE,
	UNDETERMINED,
	HEADER,
	DATA
};

const int sector_size[4] = {
	128,
	256,
	512,
	1024
};


const unsigned char crclo[] = {
	0x00,0x21,0x42,0x63,0x84,0xA5,0xC6,0xE7,0x08,0x29,0x4A,0x6B,0x8C,0xAD,0xCE,0xEF,
	0x31,0x10,0x73,0x52,0xB5,0x94,0xF7,0xD6,0x39,0x18,0x7B,0x5A,0xBD,0x9C,0xFF,0xDE,
	0x62,0x43,0x20,0x01,0xE6,0xC7,0xA4,0x85,0x6A,0x4B,0x28,0x09,0xEE,0xCF,0xAC,0x8D,
	0x53,0x72,0x11,0x30,0xD7,0xF6,0x95,0xB4,0x5B,0x7A,0x19,0x38,0xDF,0xFE,0x9D,0xBC,
	0xC4,0xE5,0x86,0xA7,0x40,0x61,0x02,0x23,0xCC,0xED,0x8E,0xAF,0x48,0x69,0x0A,0x2B,
	0xF5,0xD4,0xB7,0x96,0x71,0x50,0x33,0x12,0xFD,0xDC,0xBF,0x9E,0x79,0x58,0x3B,0x1A,
	0xA6,0x87,0xE4,0xC5,0x22,0x03,0x60,0x41,0xAE,0x8F,0xEC,0xCD,0x2A,0x0B,0x68,0x49,
	0x97,0xB6,0xD5,0xF4,0x13,0x32,0x51,0x70,0x9F,0xBE,0xDD,0xFC,0x1B,0x3A,0x59,0x78,
	0x88,0xA9,0xCA,0xEB,0x0C,0x2D,0x4E,0x6F,0x80,0xA1,0xC2,0xE3,0x04,0x25,0x46,0x67,
	0xB9,0x98,0xFB,0xDA,0x3D,0x1C,0x7F,0x5E,0xB1,0x90,0xF3,0xD2,0x35,0x14,0x77,0x56,
	0xEA,0xCB,0xA8,0x89,0x6E,0x4F,0x2C,0x0D,0xE2,0xC3,0xA0,0x81,0x66,0x47,0x24,0x05,
	0xDB,0xFA,0x99,0xB8,0x5F,0x7E,0x1D,0x3C,0xD3,0xF2,0x91,0xB0,0x57,0x76,0x15,0x34,
	0x4C,0x6D,0x0E,0x2F,0xC8,0xE9,0x8A,0xAB,0x44,0x65,0x06,0x27,0xC0,0xE1,0x82,0xA3,
	0x7D,0x5C,0x3F,0x1E,0xF9,0xD8,0xBB,0x9A,0x75,0x54,0x37,0x16,0xF1,0xD0,0xB3,0x92,
	0x2E,0x0F,0x6C,0x4D,0xAA,0x8B,0xE8,0xC9,0x26,0x07,0x64,0x45,0xA2,0x83,0xE0,0xC1,
	0x1F,0x3E,0x5D,0x7C,0x9B,0xBA,0xD9,0xF8,0x17,0x36,0x55,0x74,0x93,0xB2,0xD1,0xF0
};

const unsigned char crchi[] = {
	0x00,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x81,0x91,0xA1,0xB1,0xC1,0xD1,0xE1,0xF1,
	0x12,0x02,0x32,0x22,0x52,0x42,0x72,0x62,0x93,0x83,0xB3,0xA3,0xD3,0xC3,0xF3,0xE3,
	0x24,0x34,0x04,0x14,0x64,0x74,0x44,0x54,0xA5,0xB5,0x85,0x95,0xE5,0xF5,0xC5,0xD5,
	0x36,0x26,0x16,0x06,0x76,0x66,0x56,0x46,0xB7,0xA7,0x97,0x87,0xF7,0xE7,0xD7,0xC7,
	0x48,0x58,0x68,0x78,0x08,0x18,0x28,0x38,0xC9,0xD9,0xE9,0xF9,0x89,0x99,0xA9,0xB9,
	0x5A,0x4A,0x7A,0x6A,0x1A,0x0A,0x3A,0x2A,0xDB,0xCB,0xFB,0xEB,0x9B,0x8B,0xBB,0xAB,
	0x6C,0x7C,0x4C,0x5C,0x2C,0x3C,0x0C,0x1C,0xED,0xFD,0xCD,0xDD,0xAD,0xBD,0x8D,0x9D,
	0x7E,0x6E,0x5E,0x4E,0x3E,0x2E,0x1E,0x0E,0xFF,0xEF,0xDF,0xCF,0xBF,0xAF,0x9F,0x8F,
	0x91,0x81,0xB1,0xA1,0xD1,0xC1,0xF1,0xE1,0x10,0x00,0x30,0x20,0x50,0x40,0x70,0x60,
	0x82,0x93,0xA3,0xB3,0xC3,0xD3,0xE3,0xF3,0x02,0x12,0x22,0x32,0x42,0x52,0x62,0x72,
	0xB5,0xA5,0x95,0x85,0xF5,0xE5,0xD5,0xC5,0x34,0x24,0x14,0x04,0x74,0x64,0x54,0x44,
	0xA7,0xB7,0x87,0x97,0xE7,0xF7,0xC7,0xD7,0x26,0x36,0x06,0x16,0x66,0x76,0x46,0x56,
	0xD9,0xC9,0xF9,0xE9,0x99,0x89,0xB9,0xA9,0x58,0x48,0x78,0x68,0x18,0x08,0x38,0x28,
	0xCB,0xDB,0xEB,0xFB,0x8B,0x9B,0xAB,0xBB,0x4A,0x5A,0x6A,0x7A,0x0A,0x1A,0x2A,0x3A,
	0xFD,0xED,0xDD,0xCD,0xBD,0xAD,0x9D,0x8D,0x7C,0x6C,0x5C,0x4C,0x3C,0x2C,0x1C,0x0C,
	0xEF,0xFF,0xCF,0xDF,0xAF,0xBF,0x8F,0x9F,0x6E,0x7E,0x4E,0x5E,0x2E,0x3E,0x0E,0x1E
};



/* CRC Settings
 *
 * Header CRC: Polynomial 0x1021 length 16 initial value 0xffff
 * Data CRC: Polynomial 0x140a0445 length 32 initial value 0xffffffff
 */


//#define POLY 0x140a0445
//#define POLY 0x104c981
//#define POLY 0xd4d7ca20
#define POLY 0xa00805


/*
    def update_crc32(self, byte):
        self.crc32_accum ^= byte
        for i in range(8):
            odd = self.crc32_accum % 1
            self.crc32_accum >>= 1
            if odd:
                self.crc32_accum ^= 0x140a0445
*/

void crc32c(uint32_t *crc, const unsigned char val)
{
	uint32_t accum = *crc;
	accum ^= val;
	for (int i = 0; i < 8; i++) {
		int odd = accum & 1;
		accum >>= 1;
		if (odd) {
			accum ^= POLY;
		}
	}

//	printf("CRC: %02x %08x -> %08x\n", val, *crc, accum);

	*crc = accum;
}

int crc32(int byte, int crc)
{

   int j;
   crc = crc ^ (byte << 24);
   for (j = 1; j <= 8; j++) {   // Assuming 8 bits per byte
      if (crc & 0x80000000) {   // if leftmost (most significant) bit is set
         crc = (crc << 1) ^ POLY;
      } else {
         crc = crc << 1;
      }
   }
   return crc;
}


void upd_crc(unsigned char val, unsigned char *low, unsigned char *high) {
	unsigned char zl = val;
	zl = zl ^ *high;
	*high = crchi[zl];
	*high = *high ^ *low;
	*low = crclo[zl];	
}

void shift16(char *data) {
	for (int i = 16; i < strlen(data) + 1; i++) {
		data[i - 16] = data[i];
	}
}

int shift2(char *data) {
	int res = 0;
	
	if (data[0] == '0' && data[1] == '1') res = 1;
	//printf("%c%c = %d\n", data[0], data[1], res);
	for (int i = 2; i < strlen(data) + 1; i++) {
		data[i - 2] = data[i];
	}

	return res;
}

int shift1(char *data) {
	int res = 0;
	
	for (int i = 1; i < strlen(data) + 1; i++) {
		data[i - 1] = data[i];
	}

	return res;
}

int decode_byte(char *data) {
	unsigned char b = 0;

	for (int i = 0; i < 16; i += 2) {

		if (data[i] == '0' && data[i+1] == '1') {
			b |= (1 << (7 - (i/2)));
		}
	}

	shift16(data);


	return b;
}



int main(int argc, char **argv) {

	if (argc != 2) {
		printf("Usage: ts2l <filename>\n");
		return -1;
	}

	double last = 0;
	double lastsync = 0;
	char line[100];
	unsigned char data[100];
	unsigned char sectordata[8192] = {0};
	int sectorpos = 0;
	unsigned char byte;

	unsigned char trackdata[17 * 512];

	int cyl = 0;
	int head = 0;
	int sector = 0;
	int sectorsize = 0;
	int blocktype = IDLE;
	unsigned char blockbyte = 0;

	int readyfordata = 0;
	char ascii[17] = {0};

	int bit = 0;

	data[0] = 0;

	bzero(data, 100);
	bzero(sectordata, 8192);
	memset(trackdata, 0xee, 17 * 512);

	FILE *f = fopen(argv[1], "r");
	if (!f) {
		printf("Unable to open %s\n", argv[1]);
		return -1;
	}

	float clock = 0.00000025;

	while (fgets(line, 100, f) > 0) {

		if (line[0] == ';') continue;
		if (line[0] == '#') continue;

		char *ts = strtok(line, ",");
		char *val0 = strtok(NULL, ",");
	//	char *val1 = strtok(NULL, ",");

		

		if (!val0) {
			continue;
		}

		int v0 = val0[0] == '1' ? 1 : 0;
	//	int v1 = val1[0] == '1' ? 0 : 1;

//		int v = v0 + v1;

		if (v0 == 0) continue;

		double f = strtod(ts, NULL);
		double d = f - last;
		last = f;


		if (d < 0.00000015) {
			continue;
		}

		if (d > 0.00000045) {
			continue;
		}

		if (d < 0.00000027) {
			if (clock > d) {
				clock -= ((clock - d) / 2.0);
			} else {
				clock += ((d - clock) / 2.0);
			}
		}

		double cv = clock / 2.0;

		double shortl = 0.000000150; //cv * 1.8;
		double shorth = 0.000000250; //cv * 2.2;

		double medl = 0.000000250; //cv * 2.8;
		double medh = 0.000000350; //cv * 3.2;

		double highl = 0.000000350; //cv * 3.8;
		double highh = 0.000000450; //cv * 4.2;


		if (d > shortl && d < shorth) {
			strcat(data, "01");
//			printf("%.9f 01\n", d);
		} else if (d > medl && d < medh) {
			strcat(data, "001");
//			printf("%.9f 001\n", d);
		} else if (d > highl && d < highh) {
			strcat(data, "0001");
//			printf("%.9f 0001\n", d);
		}



		while (strlen(data) >= 16) { // A whole byte's worth

//			printf("%.9f {%s}\n", f, data);

			if (blocktype == IDLE) {
				if (strncmp(data, "0100010010001001", 16) == 0) { // Sync word found
					printf("SYNC found: %s %02x\n", data, decode_byte(data));
	//					shift16(data);
					blocktype = UNDETERMINED; // Undetermined
					byte = 0;
					bit = 0;
					sectorpos = 0;

					double st = f - lastsync;
					lastsync = f;
					printf("Time since last sync: %.9f (%.9f)\n", st, f);
				} else {
					shift1(data);
				}
			} else {
				int bval = shift2(data);
				byte <<= 1;
				byte |= bval;
				bit++;

				if (bit == 8) {
//					printf("    -> %02x [%c]\n", byte, (byte >= 32 && byte <= 127) ? byte : '.');
					if (blocktype == UNDETERMINED) {
						blockbyte = byte;
						switch (byte) {
							case 0xF8:
							case 0xFB:
								if (readyfordata == 0) {
									blocktype = IDLE;
									printf("Unexpected data block found\n");
								} else {
									blocktype = DATA;
									printf("Data block found\n");
								}
								break;
							case 0xFE:
								blocktype = HEADER;
								printf("Header block found\n");
								break;
							default:
								blocktype = IDLE;
								readyfordata = 0;
								printf("Unknown block type 0x%02x found\n", byte);
						}
					} else {
						sectordata[sectorpos++] = byte;
						if (blocktype == HEADER) {
							if (sectorpos >= 12) {
								printf("Header data: %02x %02x %02x %02x %02x %02x\n",
									sectordata[0], sectordata[1], sectordata[2],
									sectordata[3], sectordata[4], sectordata[5]
								);
								printf("Header extra data: %02x %02x %02x %02x %02x %02x\n",
									sectordata[6], sectordata[7], sectordata[8],
									sectordata[9], sectordata[10], sectordata[11]
								);

								cyl = sectordata[0] | ((sectordata[1] & 0xf0) << 4);
								head = sectordata[1] & 0x0f;
								sector = sectordata[2];
								sectorsize = sector_size[sectordata[3]];

								printf("Track %d, head %d, sector %d\n", cyl,head,sector);


								unsigned char cl = 0xff;
								unsigned char ch = 0xff;
								upd_crc(0xa1, &cl, &ch);
								upd_crc(0xfe, &cl, &ch);
								upd_crc(sectordata[0], &cl, &ch);
								upd_crc(sectordata[1], &cl, &ch);
								upd_crc(sectordata[2], &cl, &ch);
								upd_crc(sectordata[3], &cl, &ch);

								printf("CRC: %02x %02x\n", ch, cl);
								if (sectordata[4] == ch && sectordata[5] == cl) {
									printf("Checksum good!\n");
									readyfordata = 1;
								} else {
									printf("Checksum bad!\n");
									head = 0;
									sector = 0;
									cyl = 0;
									readyfordata = 0;
								}
								blocktype = IDLE;

							}
						}

						if (blocktype == DATA) {

							if (readyfordata == 0) {
								blocktype = IDLE;
								continue;
							}

							if (sectorpos >= sectorsize+4) {
//								unsigned char cl = 0xff;
//								unsigned char ch = 0xff;
//								upd_crc(0xa1, &cl, &ch);
//								upd_crc(blockbyte, &cl, &ch);


								printf("Sector %d\n", sector);

								memcpy(trackdata + (sector * 512), sectordata, 512);

								ascii[0] = 0;

								int count = 0;

								uint32_t crc = 0xFFFFFFFF;
								//uint32_t crc = 0;
								//uint32_t crc = 0xd4d7ca20;
								//uint32_t crc = 0x104c981;
								crc = crc32(0xa1, crc);
								crc = crc32(blockbyte, crc);

								for (int i = 0; i < sectorsize; i++) {
									printf("%02x ", sectordata[i]);
									
									ascii[count++] = (sectordata[i] >= 32 && sectordata[i] <= 127) ? sectordata[i] : '.';
									ascii[count] = 0;

									if (count == 16) {
										printf("  %s\n", ascii);
										count = 0;
										ascii[0] = 0;
									}

//									upd_crc(sectordata[i], &cl, &ch);
									crc = crc32(sectordata[i], crc);
								}

//								crc32c(&crc, sectordata[512]);
//								crc32c(&crc, sectordata[513]);
//								crc32c(&crc, sectordata[514]);
//								crc32c(&crc, sectordata[515]);

								printf("%02x%02x%02x%02x\n", sectordata[512], sectordata[513], sectordata[514], sectordata[515]);
								printf("\n");
								printf("CRC: %08x\n", crc);
								//if (sectordata[sectorsize] == ch && sectordata[sectorsize+1] == cl) {
							//		printf("Checksum good!\n");
						//		} else {
					//				printf("Checksum bad!\n");
				//				}
								blocktype = IDLE;
							}
						}
					}
					bit = 0;
				}

			}

		}


	}


	fclose(f);
								ascii[0] = 0;

								int count = 0;

								for (int i = 0; i < (512 * 17); i++) {
									if (i % 512 == 0) {
										printf("\n");
									}
									if (i % 16 == 0) {
										printf("%04x : ", i);
									}
									printf("%02x ", trackdata[i]);
									
									ascii[count++] = (trackdata[i] >= 32 && trackdata[i] <= 127) ? trackdata[i] : '.';
									ascii[count] = 0;

									if (count == 16) {
										printf("  %s\n", ascii);
										count = 0;
										ascii[0] = 0;
									}

								}

	return 0;
}
