#ifndef Sockets_H
#define Sockets_H

#include "OOOCode.h"
#include "ISockets.h"

#define OOOClass Sockets
OOODeclare()
	OOOImplements
		OOOImplement(ISockets)
	OOOImplementsEnd
	OOOExports
	OOOExportsEnd
OOODeclareEnd
#undef OOOClass

#endif
