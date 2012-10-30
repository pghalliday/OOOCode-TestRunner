#ifndef ISockets_H
#define ISockets_H

#include "OOOCode.h"
#include "optvxsocket.h"

#define OOOInterface ISockets
OOOVirtuals
	OOOVirtual(o_sock_handle, create, int nAf, int nType, int nProtocol)
	OOOVirtual(o_sock_error, bind, o_sock_handle hHandle, o_sockaddr * pAddress, size_t uAddressLength)
	OOOVirtual(o_sock_handle, accept, o_sock_handle hHandle)
	OOOVirtual(int, read, o_sock_handle hHandle, void * pBuffer, size_t uLength)
	OOOVirtual(o_sock_error, write, o_sock_handle hHandle, void * pBuffer, size_t uLength)
OOOVirtualsEnd
#undef OOOInterface

#endif
