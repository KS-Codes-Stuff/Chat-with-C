#include <winsock.h>

SOCKET socket(int af, int type, int protocol);

sock = socket(AF_INET, SOCK_STREAM, 0);
if (sock < 0)
{
	//Fehlermeldung
}
