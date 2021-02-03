#include <winsock.h>

SOCKET socket(int af, int type, int protocol);

struct sockaddr_in {
   sa_family sin_family;          // Adressfamilie AF_XXX
   unsigned short int sin_port;   // Portnummer
   struct in_addr sin_addr;       // IP-Adresse
   unsigned char pad[8];          // Auffüllbytes für sockaddr
 };

client1 = socket(AF_INET, SOCK_STREAM, 0);
if (client1 < 0)
{
	//Fehlermeldung
}

struct sockaddr_in client2;

struct sockaddr_in client2 =
{
	AF_INET;
	...
	...
	...
}

int connect (SOCKET client1, const struct sockaddr_in FAR* addr, sizeof(sockaddr_in));