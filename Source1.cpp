#include <winsock.h>
#inlcude <netdb.h>

char ip_addr;
printf("Bitte geben Sie die IP-Adresse ihres Chatpartners ein: ")
scanf("%s", ipa_addr);

SOCKET socket(int af, int type, int protocol);

client1 = socket(AF_INET, SOCK_STREAM, 0);
if (client1 < 0)
{
	//Fehlermeldung
}

struct sockaddr_in client2;

struct sockaddr_in client2 =
{
	AF_INET,
	...								//Portnummer
	inet_aton(ip_addr),				//IP-Adresse
	...								//Auffüllbytes für sockaddr
};

int connect (SOCKET client1, const struct sockaddr_in FAR* addr, sizeof(sockaddr_in));