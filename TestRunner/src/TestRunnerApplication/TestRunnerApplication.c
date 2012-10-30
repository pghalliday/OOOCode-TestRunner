#include "TestRunnerApplication.h"
#include "MessagePump.h"

#define OOOClass TestRunnerApplication

OOOPrivateData
	MessagePump * pMessagePump;
OOOPrivateDataEnd

OOODestructor
{
	// clean up stuff that was done on construction
	OOODestroy(OOOF(pMessagePump));
}
OOODestructorEnd

OOOMethod(void, stopped)
{
	// clean up stuff that was done on started
	OOOCall(OOOF(pMessagePump), removeHandler, OOOCast(IMessageHandler, OOOThis));
}
OOOMethodEnd

OOOMethod(bool, doMessage, o_message * pMessage)
{
	bool bHandled = FALSE;
	if (O_msg_class(pMessage) == MSG_CLASS_CONTROL && O_msg_type(pMessage) == MSG_TYPE_QUIT)
	{
		// TODO: asynchronous cleanup before stopping the message pump?

		OOOCall(OOOF(pMessagePump), stop);
		bHandled = TRUE;
	}
	return bHandled;
}
OOOMethodEnd

OOOMethod(void, started)
{
	// TODO: now the message pump is ready we can really start constructing the application

	// Add the application itself as a message handler (for quit messages)
	OOOCall(OOOF(pMessagePump), addHandler, OOOCast(IMessageHandler, OOOThis));
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
