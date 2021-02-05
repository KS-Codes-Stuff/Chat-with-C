#include <winsock.h>
#include <netdb.h>
#include <netinet/in.h>

char ip_addr;
printf("Bitte geben Sie die IP-Adresse ihres Chatpartners ein: ")
scanf_s("%s", ipa_addr);

SOCKET socket(int af, int type, int protocol);

SOCKET client1 = socket(AF_INET, SOCK_STREAM, 0);
if (client1 < 0)
{
	//Fehler beim erstellen des Sockets
}

struct sockaddr_in client2;

client2.sin_family = AF_INET;							//Socketfamilie
client2.sin_port = htonl();								//Port
client2.in_addr.sin_addr = inet_addr(ip_addr);			//IP-Adresse

//int connect (SOCKET client1, const struct sockaddr_in FAR* addr, sizeof(sockaddr_in));

if (connect(client1, (struct client2), sizeof(client2)) < 0)
{
	//Verbindungsfehler
}