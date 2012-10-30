#include "OOOUnitTestDefines.h"
#include "MockLink.h"

#define OOOClass MockLinkTest
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
OOOPrivateDataEnd

OOODestructor
{
	OOODestroy(OOOF(pMessagePump));
}
OOODestructorEnd

OOOMethod(void, stopped)
{
	OOODestroy(OOOF(pMockLink));
}
OOOMethodEnd

OOOMethod(bool, doMessage, o_message * pMessage)
{
	bool bHandled = FALSE;

	/* wait for the link up message */
	if (O_msg_class(pMessage) == MSG_CLASS_COMM && O_msg_type(pMessage) == MSG_TYPE_LINK_UP)
	{
		/* should be open now */
		OOOCheck(OOOCall(OOOF(pMockLink), isOpen));
		OOOCall(OOOF(pMessagePump), stop);
		bHandled = TRUE;
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

	/* link should not be open */
	OOOCheck(!OOOCall(OOOF(pMockLink), isOpen));

	nError = OOOICall(OOOCast(ILink, OOOF(pMockLink)), open, nLinkType, szModuleName, szLinkOptions, uTimeout);
	if (OOOCheck(nError == O_LINK_ERR_NO_ERROR))
	{
		/* should still not be open */
		OOOCheck(!OOOCall(OOOF(pMockLink), isOpen));
	}
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

OOOTest(MockLink)
{
	MockLinkTest * pMockLinkTest = OOOConstruct(MockLinkTest);
	OOOCall(pMockLinkTest, start);
	OOODestroy(pMockLinkTest);
}
