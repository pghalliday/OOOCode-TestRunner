#ifndef MockLink_H
#define MockLink_H

#include "OOOCode.h"
#include "ILink.h"
#include "MessagePump.h"

#define OOOClass MockLink
OOODeclare(MessagePump * pMessagePump, unsigned short uMessageClass)
	OOOImplements
		OOOImplement(ILink)
		OOOImplement(IMessageHandler)
	OOOImplementsEnd
	OOOExports
		OOOExport(bool, isOpen)
	OOOExportsEnd
OOODeclareEnd
#undef OOOClass

#endif
