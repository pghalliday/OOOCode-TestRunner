#include "OOOUnitTestDefines.h"
#include "MessagePump.h"

#define OOOClass MessagePumpTest
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
	unsigned int uHandledCount;
OOOPrivateDataEnd

OOODestructor
{
	OOODestroy(OOOF(pMessagePump));
}
OOODestructorEnd

OOOMethod(void, stopped)
{
	/*
	 * First 2 messages should have been handled
	 * twice, the next 3 only once. Thus OOOF(uHandledCount)
	 * should be 7
	 */
	OOOCheck(OOOF(uHandledCount) == 7);
}
OOOMethodEnd

OOOMethod(bool, doMessage, o_message * pMessage)
{
	bool bHandled = FALSE;
	if (O_msg_class(pMessage) == 10000)
	{
		OOOF(uHandledCount)++;
		if (O_msg_type(pMessage) == 2)
		{
			/* remove self from message handlers once */
			OOOCall(OOOF(pMessagePump), removeHandler, OOOCast(IMessageHandler, OOOThis));
			bHandled = TRUE;
		}
		else if (O_msg_type(pMessage) == 3)
		{
			/* stop the message pump once */
			OOOCall(OOOF(pMessagePump), stop);
			bHandled = TRUE;
		}
		else
		{
			/* allow other messages to be handled by other handlers */
			bHandled = FALSE;
		}
	}
	return bHandled;
}
OOOMethodEnd

OOOMethod(void, started)
{
	o_message tMessage;

	// Add self as message handler twice
	OOOCall(OOOF(pMessagePump), addHandler, OOOCast(IMessageHandler, OOOThis));
	OOOCall(OOOF(pMessagePump), addHandler, OOOCast(IMessageHandler, OOOThis));

	// Post some messages
	tMessage.msg_class = 10000;
	tMessage.type = 1;
	assert(O_post_message(&tMessage) == GOOD);
	tMessage.msg_class = 10000;
	tMessage.type = 1;
	assert(O_post_message(&tMessage) == GOOD);
	tMessage.msg_class = 10000;
	tMessage.type = 2;
	assert(O_post_message(&tMessage) == GOOD);
	tMessage.msg_class = 10000;
	tMessage.type = 1;
	assert(O_post_message(&tMessage) == GOOD);
	tMessage.msg_class = 10000;
	tMessage.type = 3;
	assert(O_post_message(&tMessage) == GOOD);
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
}
OOOConstructorEnd
#undef OOOClass

OOOTest(MessagePump)
{
	MessagePumpTest * pMessagePumpTest = OOOConstruct(MessagePumpTest);
	OOOCall(pMessagePumpTest, start);
	OOODestroy(pMessagePumpTest);
}
