#include "MockSocket.h"

#define OOOClass MockSocket

OOOPrivateData
	o_sock_handle hHandle;
	unsigned short uMessageClass;
	unsigned char * pReceivedData;
	size_t uReceivedDataLength;
	unsigned char * pSentData;
	size_t uSentDataLength;
OOOPrivateDataEnd

OOODestructor
{
	if (OOOF(pReceivedData))
	{
		O_free(OOOF(pReceivedData));
	}
	if (OOOF(pSentData))
	{
		O_free(OOOF(pSentData));
	}
}
OOODestructorEnd

OOOMethod(o_sock_handle, getHandle)
{
	return OOOF(hHandle);
}
OOOMethodEnd

OOOMethod(bool, send, unsigned char * pBuffer, size_t uLength)
{
	o_message tMessage;

	/* buffer the data */
	if (OOOF(pReceivedData))
	{
		O_free(OOOF(pReceivedData));
	}
	OOOF(pReceivedData) = O_malloc(uLength);
	O_memcpy(OOOF(pReceivedData), pBuffer, uLength);
	OOOF(uReceivedDataLength) = uLength;

	/* post the read notify message */
	tMessage.msg_class = MSG_CLASS_SOCKET;
	tMessage.type = MSG_TYPE_SOCK_READ_NFY;
	tMessage.INFO_SOCK_HANDLE = OOOF(hHandle);
	assert(O_post_message(&tMessage) == GOOD);
	return TRUE;
}
OOOMethodEnd

OOOMethod(int, read, unsigned char * pBuffer, size_t uLength)
{
	int nBytesRead = 0;
	if (OOOF(pReceivedData))
	{
		if (OOOF(uReceivedDataLength) < uLength)
		{
			o_message tMessage;
			O_memcpy(pBuffer, OOOF(pReceivedData), OOOF(uReceivedDataLength));
			O_free(OOOF(pReceivedData));
			OOOF(pReceivedData) = NULL;
			nBytesRead = OOOF(uReceivedDataLength);
			OOOF(uReceivedDataLength) = 0;

			/* post the send notify message */
			tMessage.msg_class = OOOF(uMessageClass);
			tMessage.type = MockSocket_MSG_TYPE_SEND_NFY;
			tMessage.info.vfp[0] = OOOThis;
			assert(O_post_message(&tMessage) == GOOD);
		}
		else
		{
			unsigned char * pCurrentData = OOOF(pReceivedData);
			O_memcpy(pBuffer, pCurrentData, uLength);
			OOOF(uReceivedDataLength) -= uLength;
			OOOF(pReceivedData) = O_malloc(OOOF(uReceivedDataLength));
			O_memcpy(OOOF(pReceivedData), pCurrentData + uLength, OOOF(uReceivedDataLength));
			O_free(pCurrentData);
			nBytesRead = uLength;
		}
	}
	return nBytesRead;
}
OOOMethodEnd

OOOMethod(o_sock_error, write, unsigned char * pBuffer, size_t uLength)
{
	o_message tMessage;

	/* buffer the data */
	if (OOOF(pSentData))
	{
		O_free(OOOF(pSentData));
	}
	OOOF(pSentData) = O_malloc(uLength);
	O_memcpy(OOOF(pSentData), pBuffer, uLength);
	OOOF(uSentDataLength) = uLength;

	/* post the receive notify message */
	tMessage.msg_class = OOOF(uMessageClass);
	tMessage.type = MockSocket_MSG_TYPE_RECEIVE_NFY;
	tMessage.info.vfp[0] = OOOThis;
	assert(O_post_message(&tMessage) == GOOD);
	return O_ERR_SOCK_NO_ERROR;
}
OOOMethodEnd

OOOMethod(int, receive, unsigned char * pBuffer, size_t uLength)
{
	int nBytesRead = 0;
	if (OOOF(pSentData))
	{
		if (OOOF(uSentDataLength) < uLength)
		{
			o_message tMessage;
			O_memcpy(pBuffer, OOOF(pSentData), OOOF(uSentDataLength));
			O_free(OOOF(pSentData));
			OOOF(pSentData) = NULL;
			nBytesRead = OOOF(uSentDataLength);
			OOOF(uSentDataLength) = 0;

			/* post the write notify message */
			tMessage.msg_class = MSG_CLASS_SOCKET;
			tMessage.type = MSG_TYPE_SOCK_WRITE_NFY;
			tMessage.INFO_SOCK_HANDLE = OOOF(hHandle);
			assert(O_post_message(&tMessage) == GOOD);
		}
		else
		{
			unsigned char * pCurrentData = OOOF(pSentData);
			O_memcpy(pBuffer, pCurrentData, uLength);
			OOOF(uSentDataLength) -= uLength;
			OOOF(pSentData) = O_malloc(OOOF(uSentDataLength));
			O_memcpy(OOOF(pSentData), pCurrentData + uLength, OOOF(uSentDataLength));
			O_free(pCurrentData);
			nBytesRead = uLength;
		}
	}
	return nBytesRead;
}
OOOMethodEnd

OOOConstructor(o_sock_handle hHandle, unsigned short uMessageClass)
{
	OOOMapMethods
		OOOMethodMapping(getHandle)
		OOOMethodMapping(send)
		OOOMethodMapping(read)
		OOOMethodMapping(write)
		OOOMethodMapping(receive)
	OOOMapMethodsEnd

	OOOF(hHandle) = hHandle;
	OOOF(uMessageClass) = uMessageClass;
}
OOOConstructorEnd

#undef OOOClass
