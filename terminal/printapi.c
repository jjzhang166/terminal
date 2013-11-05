/*
 * printapi.cpp
 *
 *  Created on: 2013-9-11
 *      Author: zhangbo
 */

#include "printapi.h"

#include "putty.h"
#include "util/print.h"
#include <stdlib.h>

typedef struct printer {
	char type; //0 直接打印 1 驱动打印 2 共享打印 3 文件打印
	char wholedoc; //4i5i是否规整，如果规整，一个5i4i代表一个文档，否则，整个应用只打开一个文档。
	int buflen; //缓存大小
	char* buffer[32 * 1024 * 1024]; //缓存
	void* handle;
	char name[1024];
} PRINTER, *PPRINTER;

void* print_new(char *name, char type, char whole) {
	PPRINTER handle = (PPRINTER) malloc(sizeof(PRINTER));
	memset(handle, 0, sizeof(PRINTER));
	handle->type = type; //记录打印类型
	handle->wholedoc = whole; //农信5i-4i并不是规范。

	switch (handle->type) {
	case PRINT_LPT: //直接打印
		if (handle->wholedoc) {
			handle->handle = lptopen(0x378);
			handle->buflen = 0;
		}
		break;
	case PRINT_DRV: //驱动打印
		if (handle->wholedoc) {
			handle->handle = (void*)printer_start_job(name);
		} else {
			strcpy(handle->name, name);
		}
		break;
	case PRINT_SHR: //共享打印
		break;
	case PRINT_COM: //串口打印
		break;
	case PRINT_FIL: //文件打印
		if (handle->wholedoc) {
			handle->handle = fopen(name, "wb");
		} else {
			strcpy(handle->name, name);
		}
		break;
	}
	return handle;
}

void print_del(void* printer) {
	PPRINTER handle = (PPRINTER) printer;
	switch (handle->type) {
	case PRINT_LPT: //直接打印
		if (handle->handle != NULL) {
			lptclose(handle->handle);
			handle->handle = NULL;
			handle->buflen = 0;
		}
		break;
	case PRINT_DRV: //驱动打印
		if (handle->handle != NULL) {
			printer_finish_job(handle->handle);
			handle->handle = NULL;
		}
		break;
	case PRINT_SHR: //共享打印
		break;
	case PRINT_COM: //串口打印
		break;
	case PRINT_FIL: //文件打印
		if (handle->handle != NULL) {
			fclose(handle->handle);
			handle->handle = NULL;
		}
		break;
	}

	free(handle);
}

void print_open(void* printer) {
	PPRINTER handle = (PPRINTER) printer;
	switch (handle->type) {
	case PRINT_LPT: //直接打印
		if (handle->handle == NULL) {
			handle->handle = (void*) lptopen(0x378);
		}
		handle->buflen = 0;
		break;
	case PRINT_DRV: //驱动打印
		if (!handle->wholedoc) {
			if (handle->handle != NULL) {
				printer_finish_job(handle->handle);
			}
			handle->handle = (void*) printer_start_job(handle->name);
		}
		break;
	case PRINT_SHR: //共享打印
		break;
	case PRINT_COM: //串口打印
		break;
	case PRINT_FIL: //文件打印
		if (handle->handle == NULL) {
			handle->handle = (void*) fopen(handle->name, "wb");
		}
		break;
	}
}

void print_data(void* printer, void* data, int len) {
	PPRINTER handle = (PPRINTER) printer;
	switch (handle->type) {
	case PRINT_LPT: //直接打印
		memcpy(handle->buffer + handle->buflen, data, len);
		handle->buflen += len;
		break;
	case PRINT_DRV: //驱动打印
		printer_job_data((printer_job *) (handle->handle), data, len);
		break;
	case PRINT_SHR: //共享打印
		break;
	case PRINT_COM: //串口打印
		break;
	case PRINT_FIL: //文件打印
		if (handle->handle) {
			fwrite(data, len, 1, (FILE*) handle->handle);
			fflush((FILE*) handle->handle);
		}
		break;
	}
}

void print_close(void* printer) {
	PPRINTER handle = (PPRINTER) printer;
	switch (handle->type) {
	case PRINT_LPT: //直接打印
		if (handle->handle != NULL) {
			lptwrite(handle->handle, handle->buffer, handle->buflen);
			if (!handle->wholedoc) {
				lptclose(handle->handle);
				handle->handle = NULL;
			}
		}
		handle->buflen = 0;
		break;
	case PRINT_DRV: //驱动打印
		if (!handle->wholedoc && handle->handle != NULL) {
			printer_finish_job((printer_job*) handle->handle);
			handle->handle = NULL;
		}
		break;
	case PRINT_SHR: //共享打印
		break;
	case PRINT_COM: //串口打印
		break;
	case PRINT_FIL: //文件打印
		fflush(handle->handle);
		if (!handle->wholedoc) {
			fclose(handle->handle);
			handle->handle = NULL;
		}
		break;
	}
}
