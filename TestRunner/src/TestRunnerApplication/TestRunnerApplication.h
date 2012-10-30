#ifndef TestRunnerApplication_H
#define TestRunnerApplication_H

#include "OOOCode.h"
#include "IMessagePumpController.h"
#include "IMessageHandler.h"

#define OOOClass TestRunnerApplication
OOODeclare()
	OOOImplements
		OOOImplement(IMessageHandler)
		OOOImplement(IMessagePumpController)
	OOOImplementsEnd
	OOOExports
		OOOExport(void, start)
	OOOExportsEnd
OOODeclareEnd
#undef OOOClass

#endif
