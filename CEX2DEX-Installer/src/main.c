
#include <psp2/kernel/modulemgr.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/kernel/clib.h>
#include <psp2/appmgr.h>
#include <psp2/ctrl.h>
#include <psp2/power.h>
#include <psp2/vshbridge.h>
#include <taihen.h>
#include "graphics.h"

#define printf psvDebugScreenPrintf

int install_testkit_cid(void);
int install_org_cid(void);
int install_open_psid(void);
int temp_cex_spoof(void);

void _start() __attribute__ ((weak, alias ("module_start")));
int module_start(SceSize argc, const void *args){

	int res;
	SceCtrlData pad;

	psvDebugScreenInit();

	SceUInt64 data;
	SceUID modid = _vshKernelSearchModuleByName("Cex2DexKernel", &data);
	if(modid < 0){

		modid = taiLoadKernelModule("ux0:app/CXDX00001/cex2dex.skprx", 0, NULL);
		if(modid >= 0)
			taiStartKernelModule(modid, 0, NULL, 0, NULL, &res);

		printf("Loading CEX2DEX kernel helper ...\n");
		sceKernelDelayThread(1000 * 1000);

		res = sceAppMgrLoadExec("app0:eboot.bin", NULL, NULL);
		if(res < 0)
			sceKernelExitProcess(0);

		while(1){
			sceKernelDelayThread(1000 * 1000);
		}
	}

	printf("welcome to CEX2DEX\n");
	printf("\n");
	printf("\n");
	printf("\n");
	printf("The function assigned to the key\n");
	printf("\n");
	printf("START    : install DEX cid\n");
	printf("CIRCLE   : uninstall cid (undo to device original cid)\n");
	printf("\n");
	printf("TRIANGLE : install DEX OpenPsid\n");
	printf("\n");
	printf("CROSS    : temp cex spoof\n");

	do {
		sceClibMemset(&pad, 0, sizeof(pad));
		sceCtrlPeekBufferPositive(0, &pad, 1);

		if(pad.buttons == SCE_CTRL_START){
			res = install_testkit_cid();
			printf("install DEX cid : 0x%X\n", res);

			goto exit;

		}else if(pad.buttons == SCE_CTRL_CIRCLE){
			res = install_org_cid();
			printf("install org cid : 0x%X\n", res);
			goto exit;

		}else if(pad.buttons == SCE_CTRL_CROSS){
			res = temp_cex_spoof();
			printf("temp cex spoof : 0x%X\n", res);
			goto infloop;

		}else if(pad.buttons == SCE_CTRL_TRIANGLE){
			res = install_open_psid();
			printf("install OpenPsid : 0x%X\n", res);
			goto exit;
		}

	} while((pad.buttons & SCE_CTRL_START) == 0);

exit:
	if(res == 0){
		scePowerRequestColdReset();
	}

infloop:
	while(1){}

	sceKernelExitProcess(0);

	return SCE_KERNEL_START_SUCCESS;
}
