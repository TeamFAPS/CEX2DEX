
#include <psp2kern/kernel/sysclib.h>
#include <psp2kern/idstorage.h>
#include <stdint.h>
#include <taihen.h>
#include "taihen_macro.h"

int ksceIdStorageLookup(SceSize leaf_num, SceSize offset, void *data, SceSize size);
int ksceIdStorageRestart(int doFlush);

static tai_hook_ref_t scePmMgrGetProductModeForDriver_ref;
int scePmMgrGetProductModeForDriver_patched(char *result){

	int res;

	res = TAI_CONTINUE(int, scePmMgrGetProductModeForDriver_ref, result);
	if(res == 0){
		*result = 1;
	}

	return res;
}

int write_idstorage_with_check(int leaf_num, const void *data){

	int res;
	char idstorage_buf_for_check[0x200];

	memset(idstorage_buf_for_check, 0, sizeof(idstorage_buf_for_check));

	res = ksceIdStorageWriteLeaf(leaf_num, data);
	if(res < 0)
		return res;

	res = ksceIdStorageRestart(1); // flush
	if(res < 0)
		return res;

	res = ksceIdStorageReadLeaf(leaf_num, idstorage_buf_for_check);
	if(res < 0)
		return res;

	if(memcmp(data, idstorage_buf_for_check, 0x200) != 0)
		return 0xFFFFFFFF;

	res = ksceIdStorageRestart(0);
	if(res < 0)
		return res;

	return 0;
}

int write_idstorage_forced(int leaf_num, const void *data){

	int res;

	SceUID hook_uid = HookImport("SceIdStorage", 0xF13F32F9, 0x2AC815A2, scePmMgrGetProductModeForDriver);

	res = write_idstorage_with_check(leaf_num, data);

	HookRelease(hook_uid, scePmMgrGetProductModeForDriver);

	return res;
}
