#pragma comment( lib, "ws2_32.lib" )
#include <iostream>
#include <stdio.h>
#include <winsock2.h>
#include <ctype.h>
#include <stdlib.h>
#define MAX_IN 500

void messaging(SOCKET s, int mode);
int isNumber(char* str);
void getValidInput(int validationMode, char *inputAddress, int size);

int main()
{	
	//--------------------Einlesen von IP--------------------	
	char ip_address;
	char* ip_ptr = (char*) malloc(30);
	ip_ptr = &ip_address;							  
	getValidInput(1, ip_ptr, 16);

	//--------------------Einlesen vom Port--------------------
	char port_addr;
	char* port_ptr = (char*)malloc(30);
	port_ptr = &port_addr;
	getValidInput(2, port_ptr, 5);
	
	//--------------------Einlesen vom Socketmode--------------------
	char socketMode;
	char *launch_ptr = (char*) malloc(3);
	launch_ptr = &socketMode;
	getValidInput(3, launch_ptr, 2);

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
	if (*launch_ptr == '1')
	{
		SOCKADDR_IN client2;																						//Speicher fuer Infos fuer Zielclient 

		memset(&client2, 0, sizeof(SOCKADDR_IN));																	//Sets the first num bytes of the block of memory pointed by ptr to 0 -> siehe cplusplus

		client2.sin_family = AF_INET;																				//Socketfamilie
		client2.sin_port = htons((u_short)port_addr);																//Port
		client2.sin_addr.s_addr = inet_addr(ip_ptr);															//IP-Adresse
															

		long connection = connect(client1, (SOCKADDR*)&client2, sizeof(sockaddr_in));

		if (connection == SOCKET_ERROR)
		{
			printf("Connection failed. Errorcode: %d\n", WSAGetLastError());
			closesocket(client1);
			WSACleanup();
		}
		else
		{
			printf("Connected with %s\n", ip_ptr);
			messaging(client1, 1);
		}
	}

	//--------------------Socket wartet auf connection--------------------
	if (*launch_ptr == '2')
	{
		SOCKADDR_IN me;

		memset(&me, 0, sizeof(SOCKADDR_IN));

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
				printf("Socket is now in listenig mode and is waiting for an incoming connection. \n");
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
			printf("New Connection accepted! \n");
			messaging(enableConnection, 2);
		}
	}

	//--------------------Beenden der Sockets--------------------
	closesocket(client1);
	WSACleanup();
	return 1;
}

void messaging(SOCKET s, int mode)
{
	char bufOutput[500] = "";
	char bufInput[500] = "";
	int end = 0;

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;

	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;

	while (true)
	{
		if (mode == 1)
		{
			if (!strchr(bufOutput, '\n'))
			{
				int ch;
				while (((ch = getchar()) != EOF) && (ch != '\n'));
			}
			SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
			printf("Geben Sie die Nachricht ein, die Sie senden wollen: \n");
			SetConsoleTextAttribute(hConsole, saved_attributes);
			printf("\n");
			fgets(bufOutput, 500, stdin);
			if (bufOutput[0] == 'b')
			{
				printf("Chat wurde beendet.");
				Sleep(2000);
				break;
			}
			int sendError = send(s, bufOutput, sizeof(bufOutput), 0);
			if (sendError == SOCKET_ERROR)
			{
				printf("send failed with error: %d\n", WSAGetLastError());
			}
			else
			{
				Sleep(500);
				mode = 2;
			}
		}

		if (mode == 2)
		{
			printf("\n");
			int recvError = recv(s, bufInput, sizeof(bufInput), 0);
			if (recvError > 0)
			{
				Beep(430, 100);
				SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
				printf("Ihr Chatpartner hat Ihnen folgende Nachricht geschickt: \n");
				SetConsoleTextAttribute(hConsole, saved_attributes);
				printf("\n");
				printf("%s \n", bufInput);
				Sleep(500);
				mode = 1;
			}
			else
			{
				printf("recv failed with error: %d\n", WSAGetLastError());
				Sleep(2000);
				break;
			}
		}
	}
	
}
int isNumber(char* str) 
{
	while (*str) {
		if (!isdigit(*str)) 
		{ 
			return 0;
		}
		str++; 
	}
	return 1;
}

void clearBuff(int size)
{
	int ch;
	while ((ch = getc(stdin)) != '\n' && ch != EOF);
}

void getValidInput(int validationMode, char *inputAddress, int size)				
{
	//While-Schleifen
	int validInput = 0;

	//Ip-Validierung
	char ip_addr[MAX_IN] = "";
	int num = 0;
	int dots = 0;

	//Port-validierung
	char port_address[MAX_IN] = "";
	int errorcount = 0;

	//Launch-Mode-Validierung
	char launch_mode[MAX_IN] = "";

	switch (validationMode)
	{
		case 1: //IP-Adresse
			printf("Bitte geben Sie die IP-Adresse ihres Chatpartners ein:\n");
			while (validInput == 0)
			{	
				fgets(ip_addr, MAX_IN, stdin);
				char* ptr;
				if (ip_addr == NULL || ip_addr[0] == '\0')
				{
					break;
				}
				ptr = strtok(ip_addr, ".");
				if (ptr == NULL)
				{
					break;
				}
				while (ptr)
				{
					if (!isNumber(ptr))
					{
						break;
					}
					num = atoi(ptr);
					if (num >= 0 && num <= 255)
					{
						ptr = strtok(NULL, ".");
						if (ptr != NULL)
						{
							dots++;
						}
					}
					else
					{
						break;
					}
				}
				if (dots != 3)
				{
					printf("Eingabe war nicht valide!\n");
					dots = 0;
				}
				else
				{
					for (int i = 0; i < size; i++)
					{
						inputAddress[i] = ip_addr[i];
					}
					validInput = 1;
				}
			}
			break;			

		case 2: //Port
			printf("Bitte geben Sie den Port ein, \x81 \bber den Sie chatten wollen: \n");
			printf("Hinweis: Es werden nur die ersten vier Stellen beachtet!\n");
			while (validInput == 0)
			{	
				fgets(port_address, size, stdin);
				if (isdigit(port_address[0]))
				{
					for (int i = 1; i <= 4; i++)
					{
						if (!(isdigit(port_address[i])))
						{
							if (port_address[i] == '\0')
							{
								clearBuff(size);
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
					for (int i = 0; i < size; i++)
					{
						inputAddress[i] = port_address[i];
					}
					validInput = 1;
				}
				else
				{
					printf("Eingabe war nicht valide!\n");
					errorcount = 0;
					clearBuff(size);
				}
			}
			break;
		case 3: //Socketmode
			printf("Wenn Sie eine Verbindung zu einem Chatpartner herstellen wollen, dr\x81 \bcken Sie die 1.\n");
			printf("Wenn sie warten m\x94 \bchten, bis ihr Chatpartner eine Verbindung zu Ihnen herstellt, dr\x81 \bcken Sie die 2.\n");
			while (validInput == 0)
			{
				fgets(launch_mode, size, stdin);
				if (launch_mode[0] == '1' || launch_mode[0] == '2')
				{
					inputAddress[0] = launch_mode[0];
					validInput = 1;
				}
				else
				{
					printf("Eingabe war nicht valide!\n");
					clearBuff(size);
				}
			}
			break;
		case 4: //message

		default: break;
	}
}
