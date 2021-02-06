#pragma comment( lib, "ws2_32.lib" )
#include <iostream>
#include <stdio.h>
#include <winsock2.h>

int main()
{																												
	char ip_addr[16];																							
	printf("Bitte geben Sie die IP-Adresse ihres Chatpartners ein: \n");                                        //Abfrage, falls IP zu lang ist? Sonst evtl Fehler bei client2
	scanf_s("%s", ip_addr, 16);																					
																												
	/*short port_addr;																						    //Einlesen vom Port noch machen! Selber Fehler wie letztens bei der IP
	printf("Bitte geben Sie den Port ein, \x81 \bber den Sie chatten wollen: \n"); */							
																												
																												
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
	client2.sin_port = htons(12345);																			//Port -> BEISPIELPORT ZUM TESTEN, MUSS NOCH EINGEBBAR GEMACHT WERDEN!!!!
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

