#include "MessagePump.h"

#define OOOClass MessagePump

typedef struct _MessageHandlers MessageHandlers;
struct _MessageHandlers
{
	IMessageHandler * iMessageHandler;
	MessageHandlers * pNext;
};

OOOPrivateData
	bool bCanContinue;
	MessageHandlers * pMessageHandlers;
	MessageHandlers * pIteratorNext;
OOOPrivateDataEnd

OOODestructor
{
	MessageHandlers * pNext = OOOF(pMessageHandlers);
	MessageHandlers * pPrevious = NULL;
	while (pNext)
	{
		pPrevious = pNext;
		pNext = pNext->pNext;
		O_free(pPrevious);
	}
}
OOODestructorEnd

OOOMethod(void, stop)
{
	OOOF(bCanContinue) = FALSE;
}
OOOMethodEnd

OOOMethod(void, addHandler, IMessageHandler * iMessageHandler)
{
	MessageHandlers * pNew = O_malloc(sizeof(MessageHandlers));
	pNew->pNext = OOOF(pMessageHandlers);
	pNew->iMessageHandler = iMessageHandler;
	OOOF(pMessageHandlers) = pNew;
}
OOOMethodEnd

OOOMethod(void, removeHandler, IMessageHandler * iMessageHandler)
{
	MessageHandlers * pNext = OOOF(pMessageHandlers);
	MessageHandlers * pPrevious = NULL;
	while (pNext)
	{
		if (pNext->iMessageHandler == iMessageHandler)
		{
			/* If the handler being removed is next in the iteration then move past it */
			if (pNext == OOOF(pIteratorNext))
			{
				OOOF(pIteratorNext) = OOOF(pIteratorNext)->pNext;
			}

			if (pPrevious)
			{
				pPrevious->pNext = pNext->pNext;
			}
			else
			{
				OOOF(pMessageHandlers) = pNext->pNext;
			}
			O_free(pNext);
			break;
		}
		pPrevious = pNext;
		pNext = pNext->pNext;
	}
}
OOOMethodEnd

OOOMethod(void, start, IMessagePumpController * iMessagePumpController)
{
	o_message tMessage;

	OOOF(bCanContinue) = TRUE;

	/* Notify controller that we're ready immediately */
	OOOICall(iMessagePumpController, started);

	/* Wait for events until stopped */
	while (OOOF(bCanContinue))
	{
		MessageHandlers * pPrevious = NULL;
		OOOF(pIteratorNext) = OOOF(pMessageHandlers);
		O_ui_get_event_wait(&tMessage);
		while (OOOF(pIteratorNext))
		{
			pPrevious = OOOF(pIteratorNext);
			OOOF(pIteratorNext) = OOOF(pIteratorNext)->pNext;
			if (OOOICall(pPrevious->iMessageHandler, doMessage, &tMessage))
			{
				break;
			}
		}
	}

	/* Notify controller that we stopped */
	OOOICall(iMessagePumpController, stopped);
}
OOOMethodEnd

OOOConstructor()
{
	OOOMapMethods
		OOOMethodMapping(start)
		OOOMethodMapping(stop)
		OOOMethodMapping(addHandler)
		OOOMethodMapping(removeHandler)
	OOOMapMethodsEnd
}
OOOConstructorEnd

#undef OOOClass
