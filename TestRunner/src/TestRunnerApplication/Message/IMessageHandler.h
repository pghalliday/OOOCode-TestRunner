#ifndef IMessageHandler_H
#define IMessageHandler_H

#include "OOOCode.h"

#define OOOInterface IMessageHandler
OOOVirtuals
	OOOVirtual(bool, doMessage, o_message * pMessage)
OOOVirtualsEnd
#undef OOOInterface

#endif
