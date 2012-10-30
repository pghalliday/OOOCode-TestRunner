#include "OOOUnitTestDefines.h"
#include "MockSocket.h"

#define TEST_STRING_1		"This string is an exact multiple of the buffer size..."
#define TEST_STRING_2		"This string is not an exact multiple of the buffer size"
#define TEST_BUFFER_SIZE	5
#define	TEST_MSG_CLASS		10000

OOOTest(MockSocket)
{
	MockSocket * pMockSocket = OOOConstruct(MockSocket, 1, TEST_MSG_CLASS);

	/* Check stuff here */
	OOOCheck(pMockSocket != NULL);

	if (OOOCheck(OOOCall(pMockSocket, send, TEST_STRING_1, O_strlen(TEST_STRING_1) + 1)))
	{
		while (TRUE)
		{
			o_message tMessage;
			O_ui_get_event_wait(&tMessage);

			/* handle the read notify message */
			if (O_msg_class(&tMessage) == MSG_CLASS_SOCKET && O_msg_type(&tMessage) == MSG_TYPE_SOCK_READ_NFY)
			{
				if (tMessage.INFO_SOCK_HANDLE == OOOCall(pMockSocket, getHandle))
				{
					char * szTest = O_malloc(O_strlen(TEST_STRING_1) + 1);
					char szBuffer[TEST_BUFFER_SIZE] = {0};
					int nTotalBytesRead = 0;
					int nBytesRead = 0;
					do
					{
						nBytesRead = OOOCall(pMockSocket, read, szBuffer, TEST_BUFFER_SIZE);
						O_memcpy(szTest + nTotalBytesRead, szBuffer, nBytesRead);
						nTotalBytesRead += nBytesRead;
					}
					while (nBytesRead == TEST_BUFFER_SIZE);
					OOOCheck(nTotalBytesRead == O_strlen(TEST_STRING_1) + 1);
					OOOCheck(O_strcmp(TEST_STRING_1, szTest) == 0);
					O_free(szTest);
				}
			}

			/* handle the send notify message */
			if (O_msg_class(&tMessage) == TEST_MSG_CLASS && O_msg_type(&tMessage) == MockSocket_MSG_TYPE_SEND_NFY)
			{
				if (tMessage.info.vfp[0] == pMockSocket)
				{
					if (OOOCheck(OOOCall(pMockSocket, send, TEST_STRING_2, O_strlen(TEST_STRING_2) + 1)))
					{
						while (TRUE)
						{
							o_message tMessage;
							O_ui_get_event_wait(&tMessage);

							/* handle the read notify message */
							if (O_msg_class(&tMessage) == MSG_CLASS_SOCKET && O_msg_type(&tMessage) == MSG_TYPE_SOCK_READ_NFY)
							{
								if (tMessage.INFO_SOCK_HANDLE == OOOCall(pMockSocket, getHandle))
								{
									char * szTest = O_malloc(O_strlen(TEST_STRING_2) + 1);
									char szBuffer[TEST_BUFFER_SIZE] = {0};
									int nTotalBytesRead = 0;
									int nBytesRead = 0;
									do
									{
										nBytesRead = OOOCall(pMockSocket, read, szBuffer, TEST_BUFFER_SIZE);
										O_memcpy(szTest + nTotalBytesRead, szBuffer, nBytesRead);
										nTotalBytesRead += nBytesRead;
									}
									while (nBytesRead == TEST_BUFFER_SIZE);
									OOOCheck(nTotalBytesRead == O_strlen(TEST_STRING_2) + 1);
									OOOCheck(O_strcmp(TEST_STRING_2, szTest) == 0);
									O_free(szTest);
								}
							}

							/* handle the send notify message */
							if (O_msg_class(&tMessage) == TEST_MSG_CLASS && O_msg_type(&tMessage) == MockSocket_MSG_TYPE_SEND_NFY)
							{
								if (tMessage.info.vfp[0] == pMockSocket)
								{
									break;
								}
							}
						}
					}
					break;
				}
			}
		}
	}

	if (OOOCheck(OOOCall(pMockSocket, write, TEST_STRING_1, O_strlen(TEST_STRING_1) + 1) == O_ERR_SOCK_NO_ERROR))
	{
		while (TRUE)
		{
			o_message tMessage;
			O_ui_get_event_wait(&tMessage);

			/* handle the receive notify message */
			if (O_msg_class(&tMessage) == TEST_MSG_CLASS && O_msg_type(&tMessage) == MockSocket_MSG_TYPE_RECEIVE_NFY)
			{
				if (tMessage.info.vfp[0] == pMockSocket)
				{
					char * szTest = O_malloc(O_strlen(TEST_STRING_1) + 1);
					char szBuffer[TEST_BUFFER_SIZE] = {0};
					int nTotalBytesRead = 0;
					int nBytesRead = 0;
					do
					{
						nBytesRead = OOOCall(pMockSocket, receive, szBuffer, TEST_BUFFER_SIZE);
						O_memcpy(szTest + nTotalBytesRead, szBuffer, nBytesRead);
						nTotalBytesRead += nBytesRead;
					}
					while (nBytesRead == TEST_BUFFER_SIZE);
					OOOCheck(nTotalBytesRead == O_strlen(TEST_STRING_1) + 1);
					OOOCheck(O_strcmp(TEST_STRING_1, szTest) == 0);
					O_free(szTest);
				}
			}

			/* handle the write notify message */
			if (O_msg_class(&tMessage) == MSG_CLASS_SOCKET && O_msg_type(&tMessage) == MSG_TYPE_SOCK_WRITE_NFY)
			{
				if (tMessage.INFO_SOCK_HANDLE == OOOCall(pMockSocket, getHandle))
				{
					if (OOOCheck(OOOCall(pMockSocket, write, TEST_STRING_2, O_strlen(TEST_STRING_2) + 1) == O_ERR_SOCK_NO_ERROR))
					{
						while (TRUE)
						{
							o_message tMessage;
							O_ui_get_event_wait(&tMessage);

							/* handle the receive notify message */
							if (O_msg_class(&tMessage) == TEST_MSG_CLASS && O_msg_type(&tMessage) == MockSocket_MSG_TYPE_RECEIVE_NFY)
							{
								if (tMessage.info.vfp[0] == pMockSocket)
								{
									char * szTest = O_malloc(O_strlen(TEST_STRING_2) + 1);
									char szBuffer[TEST_BUFFER_SIZE] = {0};
									int nTotalBytesRead = 0;
									int nBytesRead = 0;
									do
									{
										nBytesRead = OOOCall(pMockSocket, receive, szBuffer, TEST_BUFFER_SIZE);
										O_memcpy(szTest + nTotalBytesRead, szBuffer, nBytesRead);
										nTotalBytesRead += nBytesRead;
									}
									while (nBytesRead == TEST_BUFFER_SIZE);
									OOOCheck(nTotalBytesRead == O_strlen(TEST_STRING_2) + 1);
									OOOCheck(O_strcmp(TEST_STRING_2, szTest) == 0);
									O_free(szTest);
								}
							}

							/* handle the write notify message */
							if (O_msg_class(&tMessage) == MSG_CLASS_SOCKET && O_msg_type(&tMessage) == MSG_TYPE_SOCK_WRITE_NFY)
							{
								if (tMessage.INFO_SOCK_HANDLE == OOOCall(pMockSocket, getHandle))
								{
									break;
								}
							}
						}
					}
					break;
				}
			}
		}
	}

	OOODestroy(pMockSocket);
}
