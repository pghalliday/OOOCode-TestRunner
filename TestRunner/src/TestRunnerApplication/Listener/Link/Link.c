#include "Link.h"

#define OOOClass Link

OOOPrivateData
OOOPrivateDataEnd

OOODestructor
{
}
OOODestructorEnd

OOOMethod(o_link_error, open, o_link_type nLinkType, char * szModuleName, char * szLinkOptions, unsigned int uTimeout)
{
	return O_link_open(nLinkType, szModuleName, szLinkOptions, uTimeout);
}
OOOMethodEnd

OOOConstructor()
{
#define OOOInterface ILink
	OOOMapVirtuals
		OOOVirtualMapping(open)
	OOOMapVirtualsEnd
#undef OOOInterface

	OOOMapMethods
	OOOMapMethodsEnd
}
OOOConstructorEnd

#undef OOOClass
