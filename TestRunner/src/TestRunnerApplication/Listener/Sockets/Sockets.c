#include "Sockets.h"

#define OOOClass Sockets

OOOPrivateData
OOOPrivateDataEnd

OOODestructor
{
}
OOODestructorEnd

OOOMethod(o_sock_handle, create, int nAf, int nType, int nProtocol)
{
	return O_sock_create(nAf, nType, nProtocol);
}
OOOMethodEnd

OOOMethod(o_sock_error, bind, o_sock_handle hHandle, o_sockaddr * pAddress, size_t uAddressLength)
{
	return O_sock_bind(hHandle, pAddress, uAddressLength);
}
OOOMethodEnd

OOOMethod(o_sock_handle, accept, o_sock_handle hHandle)
{
	return O_sock_accept(hHandle);
}
OOOMethodEnd

OOOMethod(int, read, o_sock_handle hHandle, void * pBuffer, size_t uLength)
{
	return O_sock_read(hHandle, pBuffer, uLength);
}
OOOMethodEnd

OOOMethod(o_sock_error, write, o_sock_handle hHandle, void * pBuffer, size_t uLength)
{
	return O_sock_write(hHandle, pBuffer, uLength);
}
OOOMethodEnd

OOOConstructor()
{
#define OOOInterface ISockets
	OOOMapVirtuals
		OOOVirtualMapping(create)
		OOOVirtualMapping(bind)
		OOOVirtualMapping(accept)
		OOOVirtualMapping(read)
		OOOVirtualMapping(write)
	OOOMapVirtualsEnd
#undef OOOInterface

	OOOMapMethods
	OOOMapMethodsEnd
}
OOOConstructorEnd

#undef OOOClass
