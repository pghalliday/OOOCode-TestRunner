#include "MockLink.h"

#define OOOClass MockLink

#define MockLink_MSG_TYPE_LINK_UP	1

OOOPrivateData
	MessagePump * pMessagePump;
	unsigned short uMessageClass;
	bool bIsOpen;
OOOPrivateDataEnd

OOODestructor
{
	OOOCall(OOOF(pMessagePump), removeHandler, OOOCast(IMessageHandler, OOOThis));
}
OOODestructorEnd

OOOMethod(o_link_error, open, o_link_type nLinkType, char * szModuleName, char * szLinkOptions, unsigned int uTimeout)
{
	/* post the internal message */
	o_message tMessage;
	tMessage.msg_class = OOOF(uMessageClass);
	tMessage.type = MockLink_MSG_TYPE_LINK_UP;
	assert(O_post_message(&tMessage) == GOOD);

	/* return no error */
	return O_LINK_ERR_NO_ERROR;
}
OOOMethodEnd

OOOMethod(bool, doMessage, o_message * pMessage)
{
	bool bHandled = FALSE;
	if (O_msg_class(pMessage) == OOOF(uMessageClass) && O_msg_type(pMessage) == MockLink_MSG_TYPE_LINK_UP)
	{
		/* post the link up message */
		o_message tMessage;
		tMessage.msg_class = MSG_CLASS_COMM;
		tMessage.type = MSG_TYPE_LINK_UP;
		tMessage.O_LINK_CONDITION = 0;
		assert(O_post_message(&tMessage) == GOOD);

		/* flag the link as open */
		OOOF(bIsOpen) = TRUE;
	}
	return bHandled;
}
OOOMethodEnd

OOOMethod(bool, isOpen)
{
	return OOOF(bIsOpen);
}
OOOMethodEnd


OOOConstructor(MessagePump * pMessagePump, unsigned short uMessageClass)
{
#define OOOInterface ILink
	OOOMapVirtuals
		OOOVirtualMapping(open)
	OOOMapVirtualsEnd
#undef OOOInterface

#define OOOInterface IMessageHandler
	OOOMapVirtuals
		OOOVirtualMapping(doMessage)
	OOOMapVirtualsEnd
#undef OOOInterface

	OOOMapMethods
		OOOMethodMapping(isOpen)
	OOOMapMethodsEnd

	OOOF(pMessagePump) = pMessagePump;
	OOOF(uMessageClass) = uMessageClass;

	OOOCall(pMessagePump, addHandler, OOOCast(IMessageHandler, OOOThis));
}
OOOConstructorEnd

#undef OOOClass
