#include "Listener.h"

#define OOOClass Listener

#define Listener_MAX_DATA_SIZE	4096

OOOPrivateData
	MessagePump * pMessagePump;
	ILink * iLink;
	ISockets * iSockets;
	unsigned short uPort;
	IRunner * iRunner;
	IListenerController * iListenerController;
	o_sock_handle hSocket;
	o_sock_handle hWaitingSocket;
OOOPrivateDataEnd

OOODestructor
{
}
OOODestructorEnd

OOOMethod(void, stop)
{
	/* TODO: clean up sockets and stuff */
	OOOICall(OOOF(iListenerController), stopped);
}
OOOMethodEnd

OOOMethod(bool, doMessage, o_message * pMessage)
{
	bool bHandled = FALSE;

	if (O_msg_class(pMessage) == MSG_CLASS_COMM && O_msg_type(pMessage) == MSG_TYPE_LINK_UP)
	{
		o_ip_addr tAddress =
				{
					AF_INET,
					OOOF(uPort),
					0
				};

		/* link is open so create the socket, etc and wait for connections */
		OOOF(hSocket) = OOOICall(OOOF(iSockets), create, AF_INET, SOCK_STREAM, 0);

		/* should be able to bind a socket now */
		if (OOOICall(OOOF(iSockets), bind, OOOF(hSocket), &tAddress, sizeof(tAddress)) == O_ERR_SOCK_NO_ERROR)
		{
			/* should be able to accept connections now */
			OOOF(hWaitingSocket) = OOOICall(OOOF(iSockets), accept, OOOF(hSocket));
			if (OOOF(hWaitingSocket) != ILLEGAL_HANDLE)
			{
				OOOICall(OOOF(iListenerController), started);
			}
			else
			{
				OOOICall(OOOF(iListenerController), error);
			}
		}
		else
		{
			OOOICall(OOOF(iListenerController), error);
		}
		bHandled = TRUE;
	}
	else if (O_msg_class(pMessage) == MSG_CLASS_SOCKET && O_msg_type(pMessage) == MSG_TYPE_SOCK_CONNECTED)
	{
		if (pMessage->INFO_SOCK_HANDLE == OOOF(hWaitingSocket))
		{
			/* TODO: do nothing we don't want to accept anymore connections until we've finished with this one */
		}
	}
	else if (O_msg_class(pMessage) == MSG_CLASS_SOCKET && O_msg_type(pMessage) == MSG_TYPE_SOCK_READ_NFY)
	{
		if (pMessage->INFO_SOCK_HANDLE == OOOF(hWaitingSocket))
		{
			/* read the data */
			unsigned char pData[Listener_MAX_DATA_SIZE + 1] = {0};
			int nBytesRead = OOOICall(OOOF(iSockets), read, OOOF(hWaitingSocket), pData, Listener_MAX_DATA_SIZE + 1);
			if (nBytesRead == Listener_MAX_DATA_SIZE + 1)
			{
				/* data is too big - flush the read buffer and respond with an error */
				do
				{
					nBytesRead = OOOICall(OOOF(iSockets), read, OOOF(hWaitingSocket), pData, Listener_MAX_DATA_SIZE + 1);
				}
				while (nBytesRead == Listener_MAX_DATA_SIZE + 1);

				/* TODO: write a response, close the connection and start accepting new connections */
			}
			else
			{
				OOOICall(OOOF(iRunner), run, pData, nBytesRead);
				/* TODO: write a response, close the connection and start accepting new connections */
			}
		}
	}

	return bHandled;
}
OOOMethodEnd

OOOMethod(void, start, IListenerController * iListenerController)
{
	o_link_type nLinkType = O_LINK_TYPE_DEFAULT;
	char * szModuleName = "Test";
	char * szLinkOptions = "Test";
	unsigned int uTimeout = 20;

	OOOF(iListenerController) = iListenerController;
	OOOICall(OOOF(iLink), open, nLinkType, szModuleName, szLinkOptions, uTimeout);
}
OOOMethodEnd

OOOConstructor(MessagePump * pMessagePump, ILink * iLink, ISockets * iSockets, unsigned short uPort, IRunner * iRunner)
{
#define OOOInterface IMessageHandler
	OOOMapVirtuals
		OOOVirtualMapping(doMessage)
	OOOMapVirtualsEnd
#undef OOOInterface

	OOOMapMethods
		OOOMethodMapping(start)
		OOOMethodMapping(stop)
	OOOMapMethodsEnd

	OOOF(pMessagePump) = pMessagePump;
	OOOF(iLink) = iLink;
	OOOF(iSockets) = iSockets;
	OOOF(uPort) = uPort;
	OOOF(iRunner) = iRunner;

	OOOCall(pMessagePump, addHandler, OOOCast(IMessageHandler, OOOThis));
}
OOOConstructorEnd

#undef OOOClass
