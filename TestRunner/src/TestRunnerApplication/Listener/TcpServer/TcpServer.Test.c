#include "OOOUnitTestDefines.h"
#include "TcpServer.h"

OOOTest(TcpServer)
{
	TcpServer * pTcpServer = OOOConstruct(TcpServer);

	/* Check stuff here */
	OOOCheck(pTcpServer != NULL);

	OOODestroy(pTcpServer);
}
