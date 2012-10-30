#include "OOOUnitTestDefines.h"
#include "MockSockets.h"
#include "MockLink.h"

#define TEST_STRING_1		"This string is an exact multiple of the buffer size..."
#define TEST_STRING_2		"This string is not an exact multiple of the buffer size"
#define TEST_BUFFER_SIZE	5
#define	TEST_MSG_CLASS		10001

#define OOOClass MockSocketsTest
OOODeclare()
	OOOImplements
		OOOImplement(IMessagePumpController)
		OOOImplement(IMessageHandler)
	OOOImplementsEnd
	OOOExports
		OOOExport(void, start)
	OOOExportsEnd
OOODeclareEnd

OOOPrivateData
	MessagePump * pMessagePump;
	MockLink * pMockLink;
	MockSockets * pMockSockets;
	o_sock_handle hSocket;
	o_sock_handle hWaitingSocket;
	MockSocket * pMockSocket;
OOOPrivateDataEnd

OOODestructor
{
	OOODestroy(OOOF(pMessagePump));
}
OOODestructorEnd

OOOMethod(void, stopped)
{
	OOODestroy(OOOF(pMockLink));
	OOODestroy(OOOF(pMockSockets));
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
					80,
					0
				};

		/* should be able to create a TCP socket */
		OOOF(hSocket) = OOOICall(OOOCast(ISockets, OOOF(pMockSockets)), create, AF_INET, SOCK_STREAM, 0);
		OOOCheck(OOOF(hSocket) != ILLEGAL_HANDLE);

		/* should be able to bind a socket now */
		if (OOOCheck(OOOICall(OOOCast(ISockets, OOOF(pMockSockets)), bind, OOOF(hSocket), &tAddress, sizeof(tAddress)) == O_ERR_SOCK_NO_ERROR))
		{
			/* should be able to accept connections now */
			OOOF(hWaitingSocket) = OOOICall(OOOCast(ISockets, OOOF(pMockSockets)), accept, OOOF(hSocket));
			if (OOOCheck(OOOF(hWaitingSocket) != ILLEGAL_HANDLE))
			{
				/* connect to the waiting socket */
				OOOF(pMockSocket) = OOOCall(OOOF(pMockSockets), connect, 80);
				if (!OOOCheck(OOOF(pMockSocket) != NULL))
				{
					OOOCall(OOOF(pMessagePump), stop);
				}
			}
			else
			{
				OOOCall(OOOF(pMessagePump), stop);
			}
		}
		else
		{
			OOOCall(OOOF(pMessagePump), stop);
		}
		bHandled = TRUE;
	}
	else if (O_msg_class(pMessage) == MSG_CLASS_SOCKET && O_msg_type(pMessage) == MSG_TYPE_SOCK_CONNECTED)
	{
		if (pMessage->INFO_SOCK_HANDLE == OOOF(hWaitingSocket))
		{
			/* send some data to the socket */
			OOOCheck(OOOCall(OOOF(pMockSocket), send, TEST_STRING_1, O_strlen(TEST_STRING_1) + 1));
			bHandled = TRUE;
		}
	}
	else if (O_msg_class(pMessage) == MSG_CLASS_SOCKET && O_msg_type(pMessage) == MSG_TYPE_SOCK_READ_NFY)
	{
		if (pMessage->INFO_SOCK_HANDLE == OOOF(hWaitingSocket))
		{
			o_sock_error nError;

			/* read the data back */
			char * szTest = O_malloc(O_strlen(TEST_STRING_1) + 1);
			char szBuffer[TEST_BUFFER_SIZE] = {0};
			int nTotalBytesRead = 0;
			int nBytesRead = 0;
			do
			{
				nBytesRead = OOOICall(OOOCast(ISockets, OOOF(pMockSockets)), read, OOOF(hWaitingSocket), szBuffer, TEST_BUFFER_SIZE);
				O_memcpy(szTest + nTotalBytesRead, szBuffer, nBytesRead);
				nTotalBytesRead += nBytesRead;
			}
			while (nBytesRead == TEST_BUFFER_SIZE);
			OOOCheck(nTotalBytesRead == O_strlen(TEST_STRING_1) + 1);
			OOOCheck(O_strcmp(TEST_STRING_1, szTest) == 0);
			O_free(szTest);

			/* write a response */
			nError = OOOICall(OOOCast(ISockets, OOOF(pMockSockets)), write, OOOF(hWaitingSocket), TEST_STRING_2, O_strlen(TEST_STRING_2) + 1);
			OOOCheck(O_ERR_SOCK_NO_ERROR == nError);
			bHandled = TRUE;
		}
	}
	else if (O_msg_class(pMessage) == TEST_MSG_CLASS && O_msg_type(pMessage) == MockSocket_MSG_TYPE_SEND_NFY)
	{
		if (pMessage->info.vfp[0] == OOOF(pMockSocket))
		{
			// do nothing
			bHandled = TRUE;
		}
	}
	else if (O_msg_class(pMessage) == TEST_MSG_CLASS && O_msg_type(pMessage) == MockSocket_MSG_TYPE_RECEIVE_NFY)
	{
		if (pMessage->info.vfp[0] == OOOF(pMockSocket))
		{
			char * szTest = O_malloc(O_strlen(TEST_STRING_2) + 1);
			char szBuffer[TEST_BUFFER_SIZE] = {0};
			int nTotalBytesRead = 0;
			int nBytesRead = 0;
			do
			{
				nBytesRead = OOOCall(OOOF(pMockSocket), receive, szBuffer, TEST_BUFFER_SIZE);
				O_memcpy(szTest + nTotalBytesRead, szBuffer, nBytesRead);
				nTotalBytesRead += nBytesRead;
			}
			while (nBytesRead == TEST_BUFFER_SIZE);
			OOOCheck(nTotalBytesRead == O_strlen(TEST_STRING_2) + 1);
			OOOCheck(O_strcmp(TEST_STRING_2, szTest) == 0);
			O_free(szTest);
			bHandled = TRUE;
		}
	}
	else if (O_msg_class(pMessage) == MSG_CLASS_SOCKET && O_msg_type(pMessage) == MSG_TYPE_SOCK_WRITE_NFY)
	{
		if (pMessage->INFO_SOCK_HANDLE == OOOF(hWaitingSocket))
		{
			OOOCall(OOOF(pMessagePump), stop);
			bHandled = TRUE;
		}
	}

	return bHandled;
}
OOOMethodEnd

OOOMethod(void, started)
{
	o_link_error nError;
	o_link_type nLinkType = O_LINK_TYPE_DEFAULT;
	char * szModuleName = "Test";
	char * szLinkOptions = "Test";
	unsigned int uTimeout = 20;

	OOOF(pMockLink) = OOOConstruct(MockLink, OOOF(pMessagePump), 10000);
	OOOF(pMockSockets) = OOOConstruct(MockSockets, OOOF(pMockLink), TEST_MSG_CLASS);

	nError = OOOICall(OOOCast(ILink, OOOF(pMockLink)), open, nLinkType, szModuleName, szLinkOptions, uTimeout);
	OOOCheck(nError == O_LINK_ERR_NO_ERROR);
}
OOOMethodEnd

OOOMethod(void, start)
{
	OOOCall(OOOF(pMessagePump), start, OOOCast(IMessagePumpController, OOOThis));
}
OOOMethodEnd

OOOConstructor()
{
#define OOOInterface IMessagePumpController
	OOOMapVirtuals
		OOOVirtualMapping(started)
		OOOVirtualMapping(stopped)
	OOOMapVirtualsEnd
#undef OOOInterface

#define OOOInterface IMessageHandler
	OOOMapVirtuals
		OOOVirtualMapping(doMessage)
	OOOMapVirtualsEnd
#undef OOOInterface

	OOOMapMethods
		OOOMethodMapping(start)
	OOOMapMethodsEnd

	OOOF(pMessagePump) = OOOConstruct(MessagePump);
	OOOCall(OOOF(pMessagePump), addHandler, OOOCast(IMessageHandler, OOOThis));
}
OOOConstructorEnd
#undef OOOClass

OOOTest(MockSockets)
{
	MockSocketsTest * pMockSocketsTest = OOOConstruct(MockSocketsTest);
	OOOCall(pMockSocketsTest, start);
	OOODestroy(pMockSocketsTest);
}
