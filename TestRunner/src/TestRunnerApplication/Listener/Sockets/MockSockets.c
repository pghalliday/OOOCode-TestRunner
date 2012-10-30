#include "MockSockets.h"

#define OOOClass MockSockets

#define MockSockets_SOCKET_HANDLE			1
#define MockSockets_WAITING_SOCKET_HANDLE	2

OOOPrivateData
	MockLink * pMockLink;
	unsigned short uMessageClass;
	unsigned short uBoundPort;
	MockSocket * pConnectedSocket;
OOOPrivateDataEnd

OOODestructor
{
	if (OOOF(pConnectedSocket))
	{
		OOODestroy(OOOF(pConnectedSocket));
	}
}
OOODestructorEnd

OOOMethod(o_sock_handle, create, int nAf, int nType, int nProtocol)
{
	o_sock_handle hHandle = ILLEGAL_HANDLE;
	if (OOOCall(OOOF(pMockLink), isOpen))
	{
		hHandle = MockSockets_SOCKET_HANDLE;
	}
	return hHandle;
}
OOOMethodEnd

OOOMethod(o_sock_error, bind, o_sock_handle hHandle, o_sockaddr * pAddress, size_t uAddressLength)
{
	o_sock_error nError = O_ERR_SOCK_WRONG_STATE;
	if (hHandle == MockSockets_SOCKET_HANDLE)
	{
		if (OOOF(uBoundPort) == 0)
		{
			OOOF(uBoundPort) = ((o_ip_addr *) pAddress)->ip_port;
			nError = O_ERR_SOCK_NO_ERROR;
		}
	}
	return nError;
}
OOOMethodEnd

OOOMethod(o_sock_handle, accept, o_sock_handle hHandle)
{
	o_sock_handle hWaitingHandle = ILLEGAL_HANDLE;
	if (hHandle == MockSockets_SOCKET_HANDLE)
	{
		if (OOOF(uBoundPort) > 0)
		{
			hWaitingHandle = MockSockets_WAITING_SOCKET_HANDLE;
		}
	}
	return hWaitingHandle;
}
OOOMethodEnd

OOOMethod(MockSocket *, connect, unsigned short uPort)
{
	MockSocket * pSocket = NULL;
	if (uPort == OOOF(uBoundPort) && !OOOF(pConnectedSocket))
	{
		/* post the connected message */
		o_message tMessage;
		tMessage.msg_class = MSG_CLASS_SOCKET;
		tMessage.type = MSG_TYPE_SOCK_CONNECTED;
		tMessage.INFO_SOCK_HANDLE = MockSockets_WAITING_SOCKET_HANDLE;
		assert(O_post_message(&tMessage) == GOOD);

		OOOF(pConnectedSocket) = OOOConstruct(MockSocket, MockSockets_WAITING_SOCKET_HANDLE, OOOF(uMessageClass));
		pSocket = OOOF(pConnectedSocket);
	}
	return pSocket;
}
OOOMethodEnd

OOOMethod(int, read, o_sock_handle hHandle, void * pBuffer, size_t uLength)
{
	int nBytesRead = 0;
	if (OOOF(pConnectedSocket) && hHandle == OOOCall(OOOF(pConnectedSocket), getHandle))
	{
		nBytesRead = OOOCall(OOOF(pConnectedSocket), read, pBuffer, uLength);
	}
	return nBytesRead;
}
OOOMethodEnd

OOOMethod(o_sock_error, write, o_sock_handle hHandle, void * pBuffer, size_t uLength)
{
	o_sock_error nError = O_ERR_SOCK_WRONG_STATE;
	if (OOOF(pConnectedSocket) && hHandle == OOOCall(OOOF(pConnectedSocket), getHandle))
	{
		nError = OOOCall(OOOF(pConnectedSocket), write, pBuffer, uLength);
	}
	return nError;
}
OOOMethodEnd

OOOConstructor(MockLink * pMockLink, unsigned short uMessageClass)
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
		OOOMethodMapping(connect)
	OOOMapMethodsEnd

	OOOF(pMockLink) = pMockLink;
	OOOF(uMessageClass) = uMessageClass;
}
OOOConstructorEnd

#undef OOOClass
