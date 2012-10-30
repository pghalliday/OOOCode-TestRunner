#ifndef Listener_H
#define Listener_H

#include "OOOCode.h"
#include "ILink.h"
#include "ISockets.h"
#include "IRunner.h"
#include "IListenerController.h"
#include "MessagePump.h"

#define OOOClass Listener
OOODeclare(MessagePump * pMessagePump, ILink * iLink, ISockets * iSockets, unsigned short uPort, IRunner * iRunner)
	OOOImplements
		OOOImplement(IMessageHandler)
	OOOImplementsEnd
	OOOExports
		OOOExport(void, start, IListenerController * iListenerController)
		OOOExport(void, stop)
	OOOExportsEnd
OOODeclareEnd
#undef OOOClass

#endif
