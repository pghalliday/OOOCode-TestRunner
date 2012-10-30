#ifndef ILink_H
#define ILink_H

#include "OOOCode.h"
#include "optvxsocket.h"

#define OOOInterface ILink
OOOVirtuals
	OOOVirtual(o_link_error, open, 	o_link_type nLinkType, char * szModuleName, char * szLinkOptions, unsigned int uTimeout)
OOOVirtualsEnd
#undef OOOInterface

#endif
