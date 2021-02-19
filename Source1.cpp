#pragma comment( lib, "ws2_32.lib" )
#include <iostream>
#include <stdio.h>
#include <winsock2.h>
#include <ctype.h>
#include <stdlib.h>
#define MAX_IN 500

void messaging(SOCKET s, int mode);
int isNumber(char* str);
void clearBuff();
void getValidInput(int validationMode, char *inputAddress, int size);

int main()
{	

	//--------------------Einlesen vom Socketmode--------------------
	char* launch_ptr = (char*)malloc(3);
	getValidInput(3, launch_ptr, 2);

	//--------------------Einlesen von IP--------------------
	char* ip_ptr = (char*)malloc(30);
	if (*launch_ptr == '1') 
	{
		getValidInput(1, ip_ptr, 16);
	}

	//--------------------Einlesen vom Port--------------------
	char* port_ptr = (char*)malloc(30);
	getValidInput(2, port_ptr, 5);
	char port_address = *port_ptr;
	
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
		client2.sin_port = htons((u_short)port_address);																        //Port
		client2.sin_addr.s_addr = inet_addr(ip_ptr);															    //IP-Adresse
															

		long connection = connect(client1, (SOCKADDR*)&client2, sizeof(sockaddr_in));

		if (connection == SOCKET_ERROR)
		{
			printf("Connection failed. Errorcode: %d\n", WSAGetLastError());
			closesocket(client1);
			WSACleanup();
		}
		else
		{
			printf("Connected with %s \n", ip_ptr);
			printf("If your input-portnumber was less then 4 Chars, then press Enter to continue....");
			messaging(client1, 1);
		}
	}

	//--------------------Socket wartet auf connection--------------------
	if (*launch_ptr == '2')
	{
		SOCKADDR_IN me;

		memset(&me, 0, sizeof(SOCKADDR_IN));

		me.sin_family = AF_INET;
		me.sin_port = htons((u_short)port_address);
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
			printf("If your input-portnumber was less then 4 Chars, then press Enter to continue....");
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
	clearBuff();
	while (true)
	{
		if (mode == 1)
		{
			SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
			printf("Enter your message: \n");
			SetConsoleTextAttribute(hConsole, saved_attributes);
			printf("\n");
			fgets(bufOutput, 500, stdin);
			if (bufOutput[0] == 'y')
			{
				printf("Chat has been closed.");
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
				Beep(860, 200);
				SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
				printf("They said: \n");
				SetConsoleTextAttribute(hConsole, saved_attributes);
				printf("\n");
				printf("%s \n", bufInput);
				Sleep(500);
				mode = 1;
			}
			else if (WSAGetLastError() == 10054 || recvError == 0)
			{
				printf("The chat has been closed by your partner.");
				Sleep(2000);
				break;
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

void clearBuff()
{
	int ch;
	while ((ch = getc(stdin)) != '\n' && ch != EOF);
}

void print_err() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;

	SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
	printf("\nInvalid entry, please try again...\n");
	SetConsoleTextAttribute(hConsole, saved_attributes);
}

void getValidInput(int validationMode, char *inputAddress, int size)			
{

	//---------------------Configure Display-settings-----------------------

	//While-Schleifen
	int validInput = 0;

	//Ip-Validierung
	char ip_addr[MAX_IN] = "";
	int num = 0;
	int dots = 0;

	//Port-validierung
	char port_addr[MAX_IN] = "";
	int errorcount = 0;

	//Launch-Mode-Validierung
	char launch_mode[MAX_IN] = "";

	switch (validationMode)
	{
		case 1: //IP-Adresse
			printf("\nPlease enter your partner's IP-adress:\n");
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
					print_err();
					dots = 0;
				}
				else
				{
					for (int i = 0; i < size; i++)
					{
						if (ip_addr[i] == '\0')
						{
							inputAddress[i] = '.';
						}
						else if (ip_addr[i] == '\n')
						{
							inputAddress[i] = '\0';
							break;
						}
						else
						{
							inputAddress[i] = ip_addr[i];
						}
					}
					validInput = 1;
				}
			}
			break;			

		case 2: //Port
			printf("\nPlease enter the port-number you want to use to chat: \n");
			printf("(Hint: Only the first 4 characters entered are interpreted)\n");
			while (validInput == 0)
			{	
				fgets(port_addr, size, stdin);
				if (isdigit(port_addr[0]))
				{
					for (int i = 1; i <= 4; i++)
					{
						if (!(isdigit(port_addr[i])))
						{
							if (port_addr[i] == '\0')
							{
								//clearBuff();
								break;
							}
							else if (port_addr[i] == '\n')
							{
								port_addr[i] = '\0';
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
						inputAddress[i] = port_addr[i];
					}
					validInput = 1;
				}
				else
				{
					print_err();
					errorcount = 0;
					clearBuff();
				}
			}
			break;
		case 3: //Socketmode
			printf("To connect to someone else's machine, enter 1.\n");
			printf("To wait for an incoming connection, enter 2.\n");
			while (validInput == 0)
			{
				fgets(launch_mode, size, stdin);
				if (launch_mode[0] == '1' || launch_mode[0] == '2')
				{
					inputAddress[0] = launch_mode[0];
					validInput = 1;
					clearBuff();
				}
				else
				{
					print_err();
					clearBuff();
				}
			}
			break;
		case 4: //message

		default: break;
	}
}
