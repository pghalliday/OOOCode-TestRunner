#ifndef Link_H
#define Link_H

#include "OOOCode.h"
#include "ILink.h"

#define OOOClass Link
OOODeclare()
	OOOImplements
		OOOImplement(ILink)
	OOOImplementsEnd
	OOOExports
	OOOExportsEnd
OOODeclareEnd
#undef OOOClass

#endif
