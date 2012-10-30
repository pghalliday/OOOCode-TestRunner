#ifndef MessagePump_H
#define MessagePump_H

#include "OOOCode.h"
#include "IMessagePumpController.h"
#include "IMessageHandler.h"

#define OOOClass MessagePump
OOODeclare()
	OOOImplements
	OOOImplementsEnd
	OOOExports
		OOOExport(void, start, IMessagePumpController * iMessagePumpController)
		OOOExport(void, stop)
		OOOExport(void, addHandler, IMessageHandler * iMessageHandler)
		OOOExport(void, removeHandler, IMessageHandler * iMessageHandler)
	OOOExportsEnd
OOODeclareEnd
#undef OOOClass

#endif
