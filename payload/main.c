/*
 * MeP code sample for update_service_sm.self
 * Copyright (C) 2020, Princess of Sleeping
 */

#include "types.h"
#include "../cmep_comm_types.h"

typedef struct SceSblSmschedProxyArg {
	unsigned int size;
	int service_id;
	int response;
	char padding[0x34];
} SceSblSmschedProxyArg;

void *memset(void *dst, int ch, SceSize len){

	void *(* us_memset)(void *dst, int ch, SceSize len) = (void *)0x812342;

	return us_memset(dst, ch, len);
}

void *memcpy(void *dst, const void *src, SceSize len){

	void *(* us_memcpy)(void *dst, const void *src, SceSize len) = (void *)0x812398;

	return us_memcpy(dst, src, len);
}

__attribute__((noinline, optimize("O0")))
int FUN_00806266_doBigmacUpdateKeySlot0(void){

	int res;
	volatile SceBigmacKeyringController *keyring_ctrl = (volatile SceBigmacKeyringController *)0xE0030000;
	volatile SceBigmacOp *bigmac_op = (volatile SceBigmacOp *)0xE0050000;

	int data[4];

	data[0] = 0x14571C82;
	data[1] = 0x04985E41;
	data[2] = 0x24E3AAD6;
	data[3] = 0xFEDD3DEB;

	bigmac_op->src      = data;
	bigmac_op->slot_id  = 0;
	bigmac_op->len      = 0x10;
	bigmac_op->func     = 0x1 | 0x10000000;
	bigmac_op->key_slot = 0x204;

	bigmac_op->ready = 1;

	do {
	} while ((bigmac_op->res & 1) != 0);

	keyring_ctrl->slot_protect_chk = 0;
	// *(int *)(0xE0030028) = 0;

	res = bigmac_op->res & 1;

	// if((*(int *)(0xE003002C) & 2) == 0){
	if((keyring_ctrl->slot_protect_resp & 2) == 0){
		res = 0x800F0C29;
	}

	return res;
}

__attribute__((noinline, optimize("O0")))
void FUN_008062e8_doBigmacSomeCryptoMode0x33bWithKey0x212(void *dst, void *src, SceSize len){

	volatile SceBigmacOp *bigmac_op = (volatile SceBigmacOp *)0xE0050000;

	bigmac_op->src      = src;
	bigmac_op->dst      = dst;
	bigmac_op->len      = len & 0xFFFFFF;
	bigmac_op->func     = 0x33B;
	bigmac_op->key_slot = 0x212;
	bigmac_op->ready    = 1;

	do {} while((bigmac_op->res & 1) != 0);
}

__attribute__((noinline, optimize("O0")))
int FUN_0080632e_doBigmacAes128EcbDecrypt(void *dst, void *src, SceSize len){

	int res;

	volatile SceBigmacKeyringController *keyring_ctrl = (volatile SceBigmacKeyringController *)(0xE0030000);
	volatile SceBigmacOp *bigmac_op = (volatile SceBigmacOp *)0xE0050000;

	if((src == NULL) || (dst == NULL) || (len != 0x10))
		return 0x800F0C16;

	keyring_ctrl->slot_protect_chk = 0;
	// *(int *)(0xE0030028) = 0;

	res = 0x800F0C29;

	// if(((*(int *)(0xE003002C) & 2) >> 1) == 1){
	if(((keyring_ctrl->slot_protect_resp & 2) >> 1) == 1){

		bigmac_op->func = 0x102; // decrypt
		bigmac_op->key_slot = 0;

		bigmac_op->src = src;
		bigmac_op->dst = dst;
		bigmac_op->len = len;
		bigmac_op->ready = 1;

		do {
			res = bigmac_op->res & 1;
		} while (res != 0);
	}

	return res;
}

/*
 * Bigmac ctr exploit
 * Performs encryption even if encryption is not allowed
 */
__attribute__((noinline, optimize("O0")))
int doBigmacAes128EcbEncrypt(void *dst, void *src, SceSize len){

	int res;
	volatile SceBigmacKeyringController *keyring_ctrl = (volatile SceBigmacKeyringController *)0xE0030000;
	volatile SceBigmacOp *bigmac_op = (volatile SceBigmacOp *)0xE0050000;

	if((src == NULL) || (dst == NULL) || (len != 0x10))
		return 0x800F0C16;

	keyring_ctrl->slot_protect_chk = 0;

	res = 0x800F0C29;

	char my_src[0x10];
	char iv[0x10];

	memset(my_src, 0, 0x10);
	memcpy(iv, src, 0x10);

	if(((keyring_ctrl->slot_protect_resp & 2) >> 1) == 1){

		bigmac_op->func = SCE_BIGMAC_ALGO_DECRYPT_CTR;
		bigmac_op->func = 0x12;
		bigmac_op->key_slot = 0;

		bigmac_op->src = my_src;
		bigmac_op->dst = dst;
		bigmac_op->len = len;
		bigmac_op->iv  = iv;
		bigmac_op->ready = 1;

		do {
			res = bigmac_op->res & 1;
		} while (res != 0);
		res = bigmac_op->res;
	}

	return res;
}

int check_sign(const void *s1, const void *s2, SceSize len){

	int res = 0;

	while(len != 0){
		res |= ((*(char *)s1) ^ (*(char *)s2));
		s1++; s2++; len--;
	}

	return res;
}

int get_hash(const void *src, int len, void *dst){

	int off = 0;

	while(len >= 4){
		*(int *)(dst + (off & 0x1F)) = *(int *)(src + off);
		off += 4;
		len -= 4;
	}

	while(len >= 2){
		*(int *)(dst + (off & 0x1F)) = *(int *)(src + off);
		off += 2;
		len -= 2;
	}

	while(len >= 1){
		*(int *)(dst + (off & 0x1F)) = *(int *)(src + off);
		off += 1;
		len -= 1;
	}

	return 0;
}

int cmepCommIdStorageLeafVerify68(CmepCommIdStorageLeafData *pArg){

	int res;

	char tmp_data[0x10];
	char tmp_dataD8[0xD8];

	memset(tmp_data, 0, sizeof(tmp_data));
	memset(tmp_dataD8, 0, sizeof(tmp_dataD8));

	res = FUN_00806266_doBigmacUpdateKeySlot0();

	FUN_008062e8_doBigmacSomeCryptoMode0x33bWithKey0x212(tmp_dataD8, &pArg->data[0xA0], 0xD8);

	if(res < 0)
		goto end;

	res = FUN_0080632e_doBigmacAes128EcbDecrypt(tmp_data, &pArg->data[0x178], 0x10);
	if(res < 0)
		goto end;

	if(check_sign(tmp_data, tmp_dataD8, 0x10) != 0)
		res = -1;

end:
	return res;
}

int cmepCommIdStorageLeafResign68(CmepCommIdStorageLeafData *pArg){

	int res;

	char tmp_data[0x10];
	char some_hash[0x18];

	memset(tmp_data, 0, sizeof(tmp_data));
	memset(some_hash, 0, sizeof(some_hash));

	res = FUN_00806266_doBigmacUpdateKeySlot0();

	FUN_008062e8_doBigmacSomeCryptoMode0x33bWithKey0x212(some_hash, &pArg->data[0xA0], 0xD8);

	if(res < 0)
		goto end;

	res = doBigmacAes128EcbEncrypt(tmp_data, some_hash, 0x10);
	if(res < 0)
		goto end;

	memcpy(&pArg->data[0x178], tmp_data, 0x10);

	get_hash(&pArg->data[0xA0], 0xE8, &pArg->hash);

end:
	return res;
}

int cmepCommIdStorageLeafVerify70(CmepCommIdStorageLeafData2 *pArg){

	int res;

	char tmp_data[0x10];
	char tmp_dataD8[0x18];

	memset(tmp_data, 0, sizeof(tmp_data));
	memset(tmp_dataD8, 0, sizeof(tmp_dataD8));

	res = FUN_00806266_doBigmacUpdateKeySlot0();

	FUN_008062e8_doBigmacSomeCryptoMode0x33bWithKey0x212(tmp_dataD8, &pArg->data[0x128], 0xD8);

	if(res < 0)
		goto end;

	res = FUN_0080632e_doBigmacAes128EcbDecrypt(tmp_data, &pArg->data2[0], 0x10);
	if(res < 0)
		goto end;

	if(check_sign(tmp_data, tmp_dataD8, 0x10) != 0)
		res = -1;

end:
	return res;
}

int cmepCommIdStorageLeafResign70(CmepCommIdStorageLeafData2 *pArg){

	int res;

	char tmp_data[0x10];
	char some_hash[0x18];

	memset(tmp_data, 0, sizeof(tmp_data));
	memset(some_hash, 0, sizeof(some_hash));

	res = FUN_00806266_doBigmacUpdateKeySlot0();

	FUN_008062e8_doBigmacSomeCryptoMode0x33bWithKey0x212(some_hash, &pArg->data[0x128], 0xD8);

	if(res < 0)
		goto end;

	res = doBigmacAes128EcbEncrypt(tmp_data, some_hash, 0x10);
	if(res < 0)
		goto end;

	memcpy(&pArg->data2[0], tmp_data, 0x10);


	char temp[0xE8];

	memcpy(temp, &pArg->data[0x128], 0xD8);
	memcpy(&temp[0xD8], pArg->data2, 0x10);

	get_hash(temp, 0xE8, &pArg->hash);

end:
	return res;
}

/*
 * warning : If you also use this function, different words are preferred
 */
int cmepCommHello(CmepCommHelloParam *pArg){

	if(pArg->auth_word[0] != 0x525BF48D)
		return -1;

	if(pArg->auth_word[1] != 0x2D3515DA)
		return -1;

	if(pArg->auth_word[2] != 0xD92F11CA)
		return -1;

	if(pArg->auth_word[3] != 0x375F730C)
		return -1;

/*	if(pArg->auth_word[4] != 0x297A8C9D)
		return -1; */

	return 0;
}

int sceBigmacReadSlot(unsigned int slot_id, void *data){

	unsigned int *slot_data = (unsigned int *)(0xE0058000 + (slot_id * 0x20));

	((int *)data)[0] = slot_data[0];
	((int *)data)[1] = slot_data[1];
	((int *)data)[2] = slot_data[2];
	((int *)data)[3] = slot_data[3];
	((int *)data)[4] = slot_data[4];
	((int *)data)[5] = slot_data[5];
	((int *)data)[6] = slot_data[6];
	((int *)data)[7] = slot_data[7];

	return 0;
}

int sceBigmacWriteSlot(unsigned int slot_id, const void *data){

	SceBigmacKeyringController *ctrl = (SceBigmacKeyringController *)(0xE0030000);

	ctrl->data[0] = ((int *)data)[0];
	ctrl->data[1] = ((int *)data)[1];
	ctrl->data[2] = ((int *)data)[2];
	ctrl->data[3] = ((int *)data)[3];
	ctrl->data[4] = ((int *)data)[4];
	ctrl->data[5] = ((int *)data)[5];
	ctrl->data[6] = ((int *)data)[6];
	ctrl->data[7] = ((int *)data)[7];

	ctrl->slot_id = slot_id;

	return 0;
}

int cmepCommF00dTempCexSpoof(void *pArg){

	char cid[0x20];

	sceBigmacReadSlot(0x509, cid);

	cid[5] = 3;

	sceBigmacWriteSlot(0x509, cid);

	sceBigmacReadSlot(0x509, cid);

	memcpy(pArg, cid, 0x10);

	return 0;
}

int handler(SceSblSmschedProxyArg *proxy_arg, int cmd, void *arg){

	int res;

	switch(cmd){
	case CMEP_COMM_IDSTORAGE_LEAF_VERIFY_68:
		res = cmepCommIdStorageLeafVerify68((CmepCommIdStorageLeafData *)arg);
		break;

	case CMEP_COMM_IDSTORAGE_LEAF_RESIGN_68:
		res = cmepCommIdStorageLeafResign68((CmepCommIdStorageLeafData *)arg);
		break;

	case CMEP_COMM_IDSTORAGE_LEAF_VERIFY_70:
		res = cmepCommIdStorageLeafVerify70((CmepCommIdStorageLeafData2 *)arg);
		break;

	case CMEP_COMM_IDSTORAGE_LEAF_RESIGN_70:
		res = cmepCommIdStorageLeafResign70((CmepCommIdStorageLeafData2 *)arg);
		break;

	case CMEP_COMM_F00D_TEMP_CEX_SPOOF:
		res = cmepCommF00dTempCexSpoof(arg);
		break;

	case CMEP_COMM_HELLO:
		res = cmepCommHello((CmepCommHelloParam *)arg);
		break;

	case CMEP_COMM_RESERVED:
		// Do not create a function table by adding this entry
		res = -1;
		break;
	default:
		res = 0x800F0002;
		break;
	}

	proxy_arg->response = res;

	return 0;
}

int module_start(void *arg){

	SceSblSmschedProxyArg *proxy_arg = (SceSblSmschedProxyArg *)(arg);

	void *my_arg = (void *)(arg + 0x40);

	handler(proxy_arg, *(int *)(my_arg + 0x8), (void *)(*(int *)(my_arg + 0x4)));

	return 0;
}

__attribute__ ((section (".text.start"))) void start(void *arg) {
	module_start(arg);
}
