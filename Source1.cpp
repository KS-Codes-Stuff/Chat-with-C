#pragma comment( lib, "ws2_32.lib" )
#include <iostream>
#include <stdio.h>
#include <winsock2.h>
#include <ctype.h>
#include <stdlib.h>

int main()
{																												
	char ip_addr[16];																							
	//printf("Bitte geben Sie die IP-Adresse ihres Chatpartners ein: \n");                                        //Abfrage, falls IP zu lang ist? Sonst evtl Fehler bei client2
	//fgets(ip_addr, 16, stdin);	
		
	char port_addr[5];
	int validInput = 0;
	int errorcount = 0;
	printf("Bitte geben Sie den Port ein, \x81 \bber den Sie chatten wollen: \n");
	do
	{
		fgets(port_addr, 5, stdin);																				 //Fordert iwie keine neue Eingabe an?!
		if (isdigit(port_addr[0]))
		{
			for (int i = 1; i < 5; i++)
			{
				if (!(isdigit(port_addr[i])))
				{
					errorcount++;
				}
			}
		}
		else
		{
			printf("Bitte geben Sie einen validen Wert ein: \n");
			errorcount++;
		}
		if (errorcount > 0)
		{
			printf("Bitte geben Sie einen validen Wert ein: \n");
			errorcount = 0;
		}
		else
		{
			validInput = 1;
		}
	} while (validInput == 0);

	/* ALTERNATIVE ZU DEM OBEN, OHNE FIX FÜR NEUE EINGABE :/
	char input[5];
	char * wrongInputStuff;	
	long port_addr;

	fgets(input, 5, stdin);

	port_addr = strtol(input,&wrongInputStuff,10);

	if (port_addr == NULL)
	{
	   printf("Bitte geben Sie einen validen Wert ein: \n");
	   fgets(input, 5, stdin);
	}
	
	*/

																																																							
	WORD wVersionRequested = MAKEWORD(2, 2);																	
	WSADATA wsaData;																							
	int err = WSAStartup(wVersionRequested, &wsaData);															
	if (err != 0)																									
	{																											
		printf("WSAStartup failed, could not find an usable Winsock\n");										
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
	}
	else
	{																											
		printf("Socket created!\n");
	}
																												
	SOCKADDR_IN client2;																
																												
	client2.sin_family = AF_INET;																				//Socketfamilie
	client2.sin_port = htons((u_short) port_addr);																//Port
	client2.sin_addr.s_addr = inet_addr(ip_addr);																//IP-Adresse



	memset(&client2, 0, sizeof(SOCKADDR_IN));																	//Sets the first num bytes of the block of memory pointed by ptr to 0 -> siehe cplusplus

	long rc = connect (client1, (SOCKADDR*)&client2, sizeof(sockaddr_in));

	if (rc==SOCKET_ERROR)
	{
		printf("Connection failed. Errorcode: %d\n", WSAGetLastError());
	} 
	else
	{
		printf("Connected with %s\n", ip_addr);
	}

	Sleep(3000);

	//WENN FERTIG MIT WINSOCK:
	closesocket(client1);
	WSACleanup(); 
}

