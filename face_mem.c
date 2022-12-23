#include <ucos_ii.h>
#include "face_errno.h"
#include <stdlib.h>
#include <stdio.h>
OS_MEM *mem[5];
INT8U CommTxPart8[512][8];
INT8U CommTxPart16[256][16];
INT8U CommTxPart32[256][32];
INT8U CommTxPart64[256][64];
INT8U CommTxPart128[5][128];


/*
* os_cfg.h中OS_MAX_MEM_PART配置分区数量
* blksize必须大于sizeof(void *)
*/
int face_mem_init() {
	INT8U err;
	mem[0] = OSMemCreate(CommTxPart8, 512, 8, &err);
	if (err != 0){
		return err;
	}
	mem[1] = OSMemCreate(CommTxPart16, 256, 16, &err);
	if (err != 0){
		return err;
	}
	mem[2] = OSMemCreate(CommTxPart32, 256, 32, &err);
	if (err != 0){
		return err;
	}
	mem[3] = OSMemCreate(CommTxPart64, 256, 64, &err);
	if (err != 0){
		return err;
	}
	mem[4] = OSMemCreate(CommTxPart128, 5, 128, &err);
	if (err != 0){
		return err;
	}
	return 0;
}

void *face_mem_get(int size, INT8U *err) {
	if (size > 0 && size <= 8) {
		OSMemGet(mem[0], err);
		return *err ? err : mem[0]->OSMemFreeList;
	}
	else if (size > 8 && size <= 16) {
		OSMemGet(mem[1], err);
		return *err ? err : mem[1]->OSMemFreeList;
	}
	else if (size > 16 && size <= 32) {
		OSMemGet(mem[2], err);
		return *err ? err : mem[2]->OSMemFreeList;
	}
	else if (size > 32 && size <= 64) {
		OSMemGet(mem[3], err);
		return *err ? err : mem[3]->OSMemFreeList;
	}
	else if (size > 64 && size <= 128) {
		OSMemGet(mem[4], err);
		return *err ? err : mem[4]->OSMemFreeList;
	}
	else{
		*err = EINVAL;
		return err;
	}
}