#ifndef IRunner_H
#define IRunner_H

#include "OOOCode.h"

#define OOOInterface IRunner
OOOVirtuals
	OOOVirtual(void, run, unsigned char * pData, size_t uSize)
OOOVirtualsEnd
#undef OOOInterface

#endif
