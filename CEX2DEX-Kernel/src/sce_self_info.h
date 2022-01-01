/*
 * PS Vita kernel module manager RE
 * Copyright (C) 2019, Princess of Sleeping
 */

#ifndef _PSP2_KERNEL_SBL_AUTH_INFO_H_
#define _PSP2_KERNEL_SBL_AUTH_INFO_H_

typedef struct SceSelfCapability // size is 0x20
{
	uint32_t self_type1;    // 0x80 kernel, 0x40 user, 0x20 npdrm app
	uint16_t self_type2;    // 0x00 kernel, 0x40 user, 0xC0 super user?
	uint16_t unk_0x06;      // Always 0xF0 or 0x0F?
	uint32_t unk_0x08;      // Always 0x00?
	uint32_t unk_0x0C;      // Always 0xFFFFFFFF?
	uint32_t unk_0x10[4];   // ex : unk_0x10[0] : 0 or 0xC0 or 0xE0, unk_0x10[1] : 0, unk_0x10[2] : 0, unk_0x10[3] : 0
} SceSelfCapability;

typedef struct SceSelfAttributes // size is 0x20
{
	uint16_t unk_0x00;               // Always 0x980?, 1.05 game : 0x540, 0x280000000002801B:0xFE0
	uint8_t self_type1;              // 0x00 kernel, 0x20 plugin?, 0x80 user
	/*
	 * (self_load_allow_device == 0) : LoadAllowAll?
	 * (self_load_allow_device & 4 == 1) : LoadAllowRetail
	 * (self_load_allow_device & 2 == 1) : LoadAllowTestkit?
	 * (self_load_allow_device & 1 == 1) : LoadAllowDevkit?
	 */
	uint8_t self_load_allow_device;
	uint32_t unk_0x04;               // Always 0xC30000?, 1.05 game 0x330040
	uint16_t unk_0x08;               // zero? 1.05 game is 0x04
	uint8_t self_load_allow_path;    // 0x40 os0, 0x20 vs0
	uint8_t unk_0x0B;                // Always 0x09?, 1.05 game is 0x25
	uint32_t self_type2;             // 0x80 kernel, 0x40 user
	uint32_t unk_0x10[3];            // Always 0x00?
	uint32_t unk_0x1C;               // Always 0xFFFFFFFF?
} SceSelfAttributes;

typedef struct SceSelfAuthInfo // size is 0x90
{
	SceUInt64 program_authority_id;
	SceUInt64 padding1;
	SceSelfCapability capability;
	SceSelfAttributes attributes;
	uint8_t padding2[0x10];
	uint8_t klicensee[0x10]; // offset 0x60
	uint32_t unk_70;
	uint32_t unk_74;
	uint32_t unk_78;
	uint32_t unk_7C;
	uint32_t unk_80;
	uint32_t unk_84;
	uint32_t unk_88;
	uint32_t unk_8C;
} SceSelfAuthInfo;

typedef struct SceSblSmCommContext130 // size is 0x130 as its name indicates
{
	uint32_t unk_0;
	uint32_t self_type;                    // kernel = 0, user = 1, sm = 2
	SceSelfAuthInfo caller_self_auth_info; // can be obtained with sceKernelGetSelfInfoForKernel
	SceSelfAuthInfo called_self_auth_info; // set by F00D in F00D SceSblSmCommContext130 response
	int path_id;                           // can be obtained with sceSblACMgrGetPathIdForKernel or sceIoGetPathIdExForDriver
	uint32_t unk_12C;
} SceSblSmCommContext130;

#endif /* _PSP2_KERNEL_SBL_AUTH_INFO_H_ */
