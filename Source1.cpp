#include <iostream>
#include <stdio.h>
#include <winsock2.h>

int main()
{
	/*char ip_addr;
	printf("Bitte geben Sie die IP-Adresse ihres Chatpartners ein: \n");
	scanf_s("%s", ip_addr);*/

	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsaData;
	int err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		printf("WSAStartup failed, could not find an usable Winsock\n");
		return 1;
	}

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{                             
		printf("Could not find a usable version of Winsock.dll\n");
		WSACleanup();
		return 1;
	}
	else
	{
	    printf("The Winsock 2.2 dll was found okay\n");
	}
		

	SOCKET client1 = socket(AF_INET, SOCK_STREAM, 0);
	if (client1 == INVALID_SOCKET)
	{
		//Fehler beim erstellen des Sockets
		printf("Socket could not be created. Error-Code: %d\n", WSAGetLastError());
	}
	else
	{
		printf("Socket created!\n");
	}

	/*struct sockaddr_in client2;

	client2.sin_family = AF_INET;							//Socketfamilie
	client2.sin_port = htonl();								//Port
	client2.in_addr.sin_addr = inet_addr(ip_addr);			//IP-Adresse

	//int connect (SOCKET client1, const struct sockaddr_in FAR* addr, sizeof(sockaddr_in));

	if (connect(client1, (struct client2), sizeof(client2)) < 0)
	{
		//Verbindungsfehler
	} */





	//WENN FERTIG MIT WINSOCK:
	WSACleanup(); 
}

