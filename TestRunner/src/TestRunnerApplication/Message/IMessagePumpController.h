#ifndef IMessagePumpController_H
#define IMessagePumpController_H

#include "OOOCode.h"

#define OOOInterface IMessagePumpController
OOOVirtuals
	OOOVirtual(void, started)
	OOOVirtual(void, stopped)
OOOVirtualsEnd
#undef OOOInterface

#endif
