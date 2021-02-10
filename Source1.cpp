#pragma comment( lib, "ws2_32.lib" )
#include <iostream>
#include <stdio.h>
#include <winsock2.h>
#include <ctype.h>
#include <stdlib.h>

int main()
{	
	//--------------------Einlesen von IP, Port und Socketmodus--------------------								  
	char ip_addr[16];																							
	//printf("Bitte geben Sie die IP-Adresse ihres Chatpartners ein: \n");                                        
	//fgets(ip_addr, 16, stdin);	
		
	char port_addr[5];
	int validInput = 0;
	int errorcount = 0;
	printf("Bitte geben Sie den Port ein, \x81 \bber den Sie chatten wollen: \n");
	printf("Hinweis: Es werden nur die ersten vier Stellen beachtet!\n");
	do
	{
		fgets(port_addr, 5, stdin);																				 
		if (isdigit(port_addr[0]))
		{
			for (int i = 1; i < 5; i++)
			{
				if (!(isdigit(port_addr[i])))																	 
				{
					if ((port_addr[i] == '\n') || (port_addr[i] == '\0'))
					{
						break;
					}
					else
					{
						errorcount++; 
					}
					
				}
			}
		}
		else
		{
			errorcount++;
		}
		if (errorcount == 0)
		{
			validInput = 1;
			
		}
		else
		{
			printf("Bitte geben Sie einen validen Wert ein: \n");
			errorcount = 0;
		}
	} while (validInput == 0);

	char mode[2];
	printf("Wenn Sie eine Verbindung zu einem Chatpartner herstellen wollen, dr\x81 \bcken Sie die 1.\n");
	printf("Wenn sie warten m\x94 \bchten, bis ihr Chatpartner eine Verbindung zu Ihnen herstellt, dr\x81 \bcken Sie die 2.\n");
	fgets(mode, 2, stdin);

	//--------------------Socket erstellen--------------------																																																						
	WORD wVersionRequested = MAKEWORD(2, 2);																	
	WSADATA wsaData;																							
	int err = WSAStartup(wVersionRequested, &wsaData);															
	if (err != 0)																									
	{																											
		printf("WSAStartup failed, could not find an usable Winsock\n");
		WSACleanup();
	}																											
																												
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)											
	{																											
		printf("Could not find a usable version of Winsock.dll\n");												
		WSACleanup();																							
	}																											
	else																										
	{
	    printf("The Winsock 2.2 dll was found okay\n");															
	}
																												
	SOCKET client1 = socket(AF_INET, SOCK_STREAM, 0);
	if (client1 == INVALID_SOCKET)																				
	{
		printf("Socket could not be created. Error-Code: %d\n", WSAGetLastError());
		closesocket(client1);
		WSACleanup();
	}
	else
	{																											
		printf("Socket created!\n");
	}
		
	//--------------------Socket connecten--------------------
	if (mode[0] == '1')
	{
		SOCKADDR_IN client2;																						//Speicher für Infos für Zielclient 

		client2.sin_family = AF_INET;																				//Socketfamilie
		client2.sin_port = htons((u_short)port_addr);																//Port
		client2.sin_addr.s_addr = inet_addr(ip_addr);																//IP-Adresse

		memset(&client2, 0, sizeof(SOCKADDR_IN));																	//Sets the first num bytes of the block of memory pointed by ptr to 0 -> siehe cplusplus

		long connection = connect(client1, (SOCKADDR*)&client2, sizeof(sockaddr_in));

		if (connection == SOCKET_ERROR)
		{
			printf("Connection failed. Errorcode: %d\n", WSAGetLastError());
			closesocket(client1);
			WSACleanup();
		}
		else
		{
			printf("Connected with %s\n", ip_addr);
		}
	}

	//--------------------Socket wartet auf connection--------------------
	if (mode[0] == '2')
	{
		SOCKADDR_IN me;

		me.sin_family = AF_INET;
		me.sin_port = htons((u_short)port_addr);																					
		me.sin_addr.s_addr = ADDR_ANY;

		long binding = bind(client1, (SOCKADDR*)&me, sizeof(SOCKADDR_IN));

		if (binding != 0)
		{
			printf("An Error occured, when trying to bind the Socket. Errorcode: %d\n", WSAGetLastError());
			closesocket(client1);
			WSACleanup();
		}
		else
		{
			long listening = listen(client1, 10);

			if (listening == SOCKET_ERROR)
			{
				printf("Could not put Socket in listening Mode. Errorcode: %d\n", WSAGetLastError());	
				closesocket(client1);
				WSACleanup();
			}
			else
			{
				printf("Socket is now in listenig mode an waits for a connection. \n");
			}
		}

		SOCKET enableConnection = accept(client1, NULL, NULL);
		if (enableConnection == INVALID_SOCKET)
		{
			printf("Socket could not accept a connection. Errorcode: %d\n", WSAGetLastError());
			closesocket(client1);
			WSACleanup();
		}
		else
		{
			printf("New Connection accepted!");
		}
	}


	//--------------------Wait, damit Ausgabe sichtbar--------------------
	Sleep(5000);

	//--------------------Beenden der Sockets--------------------
	closesocket(client1);
	WSACleanup(); 
}

