/*
 * CMeP comm header
 * Copyright (C) 2020, Princess of Sleeping
 */

#ifndef _CMEP_COMM_TYPES_H_
#define _CMEP_COMM_TYPES_H_

#define CMEP_COMM_IDSTORAGE_LEAF_VERIFY_68 (3)
#define CMEP_COMM_IDSTORAGE_LEAF_RESIGN_68 (4)
#define CMEP_COMM_IDSTORAGE_LEAF_VERIFY_70 (8)
#define CMEP_COMM_IDSTORAGE_LEAF_RESIGN_70 (9)
#define CMEP_COMM_F00D_TEMP_CEX_SPOOF      (0x10)
#define CMEP_COMM_HELLO                    (0x10001)
#define CMEP_COMM_RESERVED                 (0x10000)

typedef struct SceBigmacOp {
	const void *src;
	union {
		void *dst;
		int slot_id;
	};
	SceSize len;
	int func;

	union {
		const void *key;
		int key_slot;
	};
	void *iv;
	void *next;
	int ready;

	int status;
	int res;
} SceBigmacOp;

#define SCE_BIGMAC_ALGO_MEMCPY				(0x00000000)
#define SCE_BIGMAC_ALGO_MEMSET				(0x0000000C)

#define SCE_BIGMAC_ALGO_ENCRYPT_ECB			(0x00000001)
#define SCE_BIGMAC_ALGO_DECRYPT_ECB			(0x00000002)
#define SCE_BIGMAC_ALGO_RNG				(0x00000004)
#define SCE_BIGMAC_ALGO_CBC				(0x00000008)

#define SCE_BIGMAC_ALGO_ENCRYPT_CBC			(SCE_BIGMAC_ALGO_CBC | SCE_BIGMAC_ALGO_ENCRYPT_ECB)
#define SCE_BIGMAC_ALGO_DECRYPT_CBC			(SCE_BIGMAC_ALGO_CBC | SCE_BIGMAC_ALGO_DECRYPT_ECB)

#define SCE_BIGMAC_ALGO_ENCRYPT_CTR			(0x10 | SCE_BIGMAC_ALGO_ENCRYPT_ECB)
#define SCE_BIGMAC_ALGO_DECRYPT_CTR			(0x20 | SCE_BIGMAC_ALGO_ENCRYPT_ECB)

#define SCE_BIGMAC_ALGO_BIT_1				(0x00 & 0x18)
#define SCE_BIGMAC_ALGO_BIT_224				(0x08 & 0x18)
#define SCE_BIGMAC_ALGO_BIT_256				(0x10 & 0x18)
#define SCE_BIGMAC_ALGO_MAC				(0x20)

#define SCE_BIGMAC_ALGO_AES_CMAC			(0x30 | SCE_BIGMAC_ALGO_BIT_224 | SCE_BIGMAC_ALGO_ENCRYPT_ECB | SCE_BIGMAC_ALGO_DECRYPT_ECB)

#define SCE_BIGMAC_ALGO_SHA1				(SCE_BIGMAC_ALGO_ENCRYPT_ECB | SCE_BIGMAC_ALGO_DECRYPT_ECB)
#define SCE_BIGMAC_ALGO_SHA224				(SCE_BIGMAC_ALGO_BIT_224 | SCE_BIGMAC_ALGO_ENCRYPT_ECB | SCE_BIGMAC_ALGO_DECRYPT_ECB)
#define SCE_BIGMAC_ALGO_SHA256				(SCE_BIGMAC_ALGO_BIT_256 | SCE_BIGMAC_ALGO_ENCRYPT_ECB | SCE_BIGMAC_ALGO_DECRYPT_ECB)

#define SCE_BIGMAC_ALGO_HMAC_SHA1			(SCE_BIGMAC_ALGO_MAC | SCE_BIGMAC_ALGO_BIT_1 | SCE_BIGMAC_ALGO_ENCRYPT_ECB | SCE_BIGMAC_ALGO_DECRYPT_ECB)
#define SCE_BIGMAC_ALGO_HMAC_SHA256			(SCE_BIGMAC_ALGO_MAC | SCE_BIGMAC_ALGO_BIT_256 | SCE_BIGMAC_ALGO_ENCRYPT_ECB | SCE_BIGMAC_ALGO_DECRYPT_ECB)

#define SCE_BIGMAC_UNKNOWN_0x40				(0x00000040) /* hang? */
#define SCE_BIGMAC_USE_EXTERNAL_KEY			(0x00000080)
#define SCE_BIGMAC_KEY_SIZE_64				(0x00000000 & 0x300)
#define SCE_BIGMAC_KEY_SIZE_128				(0x00000100 & 0x300)
#define SCE_BIGMAC_KEY_SIZE_192				(0x00000200 & 0x300)
#define SCE_BIGMAC_KEY_SIZE_256				(0x00000300 & 0x300)

#define SCE_BIGMAC_UNKNOWN_0x400			(0x00000400)
#define SCE_BIGMAC_UNKNOWN_0x800			(0x00000800)
#define SCE_BIGMAC_UNKNOWN_0x1000			(0x00001000)
#define SCE_BIGMAC_UNKNOWN_0x2000			(0x00002000)
#define SCE_BIGMAC_UNKNOWN_0x4000			(0x00004000)
#define SCE_BIGMAC_UNKNOWN_0x8000			(0x00008000)
#define SCE_BIGMAC_UNKNOWN_0x10000			(0x00010000)
#define SCE_BIGMAC_UNKNOWN_0x20000			(0x00020000)
#define SCE_BIGMAC_UNKNOWN_0x40000			(0x00040000)
#define SCE_BIGMAC_UNKNOWN_0x80000			(0x00080000)
#define SCE_BIGMAC_UNKNOWN_0x100000			(0x00100000)
#define SCE_BIGMAC_UNKNOWN_0x200000			(0x00200000)
#define SCE_BIGMAC_UNKNOWN_0x400000			(0x00400000)
#define SCE_BIGMAC_UNKNOWN_0x800000			(0x00800000)
#define SCE_BIGMAC_UNKNOWN_0x1000000			(0x01000000) /* hang */
#define SCE_BIGMAC_UNKNOWN_0x2000000			(0x02000000) /* hang */
#define SCE_BIGMAC_UNKNOWN_0x4000000			(0x04000000)
#define SCE_BIGMAC_UNKNOWN_0x8000000			(0x08000000)
#define SCE_BIGMAC_DST_TO_SLOT				(0x10000000)

typedef struct SceBigmacKeyringController { // 0xE0030000

	uint32_t data[0x8];

	// offset:0x20
	int slot_id;

	/*
	 * Probably can't add new flag.
	 * If the slot is disabled, enable it
	 */
	int slot_protection;
	int slot_protect_chk;

	/*
	 * if((slot_protect_resp & 2) != 0) enabled else disabled
	 */
	int slot_protect_resp;

} SceBigmacKeyringController;

typedef struct CmepCommHelloParam {
	unsigned int auth_word[4];
} CmepCommHelloParam;

typedef struct CmepCommIdStorageLeafData {
	char data[0x200];
	char hash[0x20];
} CmepCommIdStorageLeafData;

typedef struct CmepCommIdStorageLeafData2 {
	char data[0x200];
	char data2[0x200];
	char hash[0x20];
} CmepCommIdStorageLeafData2;


#endif /* _CMEP_COMM_TYPES_H_ */
