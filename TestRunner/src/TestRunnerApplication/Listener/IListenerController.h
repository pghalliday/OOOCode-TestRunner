#ifndef IListenerController_H
#define IListenerController_H

#include "OOOCode.h"

#define OOOInterface IListenerController
OOOVirtuals
	OOOVirtual(void, started)
	OOOVirtual(void, error)
	OOOVirtual(void, stopped)
OOOVirtualsEnd
#undef OOOInterface

#endif
