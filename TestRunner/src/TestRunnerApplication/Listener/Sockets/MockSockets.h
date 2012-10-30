#ifndef MockSockets_H
#define MockSockets_H

#include "OOOCode.h"
#include "ISockets.h"
#include "MockLink.h"
#include "MockSocket.h"

#define OOOClass MockSockets
OOODeclare(MockLink * pMockLink, unsigned short uMessageClass)
	OOOImplements
		OOOImplement(ISockets)
	OOOImplementsEnd
	OOOExports
		OOOExport(MockSocket *, connect, unsigned short uPort)
	OOOExportsEnd
OOODeclareEnd
#undef OOOClass

#endif
