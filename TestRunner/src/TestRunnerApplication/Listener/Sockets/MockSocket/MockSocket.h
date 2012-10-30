#ifndef MockSocket_H
#define MockSocket_H

#include "OOOCode.h"
#include "optvxsocket.h"

typedef enum
{
	MockSocket_MSG_TYPE_SEND_NFY = 0,
	MockSocket_MSG_TYPE_RECEIVE_NFY
}
MockSocket_MSG_TYPE;

#define OOOClass MockSocket
OOODeclare(o_sock_handle hHandle, unsigned short uMessageClass)
	OOOImplements
	OOOImplementsEnd
	OOOExports
		OOOExport(o_sock_handle, getHandle)
		OOOExport(bool, send, unsigned char * pBuffer, size_t uLength)
		OOOExport(int, read, unsigned char * pBuffer, size_t uLength)
		OOOExport(o_sock_error, write, unsigned char * pBuffer, size_t uLength)
		OOOExport(int, receive, unsigned char * pBuffer, size_t uLength)
	OOOExportsEnd
OOODeclareEnd
#undef OOOClass

#endif
