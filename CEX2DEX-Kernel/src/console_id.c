
#include <stdint.h>

int cid_check(const void *cid){

	const uint8_t *cid8 = cid;

	if(cid8[0] != 0x00)
		return -1;

	if(cid8[1] != 0x00)
		return -1;

	if(cid8[2] != 0x00)
		return -1;

	if(cid8[3] != 0x01)
		return -1;

	if(cid8[4] != 0x01)
		return -1;

	/*
	 * https://github.com/CelesteBlue-dev/PS-ConsoleId-wiki/blob/master/PS-ConsoleId-wiki.txt#L91
	 */
	if(cid8[5] > 0x0D)
		return -1;

	if(cid8[5] == 0x01) // Do not allow this as CelesteBlue warned that it could be brick
		return -1;

	if(cid8[5] == 0x00) // Do not allow this as CelesteBlue warned that it could be brick
		return -1;

	/*
	 * https://github.com/CelesteBlue-dev/PS-ConsoleId-wiki/blob/master/PS-ConsoleId-wiki.txt#L168
	 */
	if(cid8[6] == 0x00 && cid8[7] == 0x03)
		goto check_8;

	if(cid8[6] == 0x00 && cid8[7] == 0x05)
		goto check_8;

	if(cid8[6] == 0x00 && cid8[7] == 0x09)
		goto check_8;

	if(cid8[6] == 0x00 && cid8[7] == 0x0A)
		goto check_8;

	if(cid8[6] == 0x00 && cid8[7] == 0x0B)
		goto check_8;

	if(cid8[6] == 0x00 && cid8[7] == 0x0C)
		goto check_8;

	if(cid8[6] == 0x00 && cid8[7] == 0x0D)
		goto check_8;

	if(cid8[6] == 0x00 && cid8[7] == 0x0E)
		goto check_8;

	if(cid8[6] == 0x00 && cid8[7] == 0x0F)
		goto check_8;

	if(cid8[6] == 0x00 && cid8[7] == 0x10)
		goto check_8;

	if(cid8[6] == 0x00 && cid8[7] == 0x11)
		goto check_8;

	if(cid8[6] == 0x00 && cid8[7] == 0x12)
		goto check_8;

	if(cid8[6] == 0x00 && cid8[7] == 0x14)
		goto check_8;

	if(cid8[6] == 0x00 && cid8[7] == 0x18)
		goto check_8;

	if(cid8[6] == 0x02 && cid8[7] == 0x01)
		goto check_8;

	if(cid8[6] == 0x02 && cid8[7] == 0x02)
		goto check_8;

	/*
	 * if(cid8[6] == 0x04 && cid8[7] == 0x08) // PS TV Development Kit
	 *	goto check_8;
	 */

	if(cid8[6] == 0x06 && cid8[7] == 0x02) // PS TV Prototype
		goto check_8;

	return -1;

check_8:
	if(cid8[8] == 0x0C)
		return 0;

	if(cid8[8] == 0x90)
		return 0; // Diag

	if(cid8[8] == 0xF4)
		return 0;

	return -1;
}
