/*	AG0907 Lab 4 UDP server example - by Henry Fortuna and Adam Sampson

	When the user types a message, the client sends it to the server
	as a UDP packet. The server then sends a packet back to the
	client, and the client prints it out.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")


// The IP address for the server
#define SERVERIP "127.0.0.1"

// The UDP port number for the server
#define SERVERPORT 4444

// The (fixed) size of message that we send between the two programs
#define MESSAGESIZE 3


// Prototypes
void die(const char *message);

// Define struct HERE!
struct PlayerData
{
	char playerID[4] = { '-', '-', '-', '-', };
	int playerXpos = 0;
	int playerYpos = 0;
};

int main()
{
	printf("Echo Server\n");

	// Initialise the WinSock library -- we want version 2.2.
	WSADATA w;
	int error = WSAStartup(0x0202, &w);
	if (error != 0)
	{
		die("WSAStartup failed");
	}
	if (w.wVersion != 0x0202)
	{
		die("Wrong WinSock version");
	}

	// Create a UDP socket.
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET)
	{
		die("socket failed");
	}

	// Fill out a sockaddr_in structure to describe the address we'll listen on.
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(SERVERIP);
	// htons converts the port number to network byte order (big-endian).
	serverAddr.sin_port = htons(SERVERPORT);

	// Bind the socket to that address.
	if (bind(sock, (const sockaddr *) &serverAddr, sizeof(serverAddr)) != 0)
	{
		die("bind failed");
	}

	// ntohs does the opposite of htons.
	printf("Server socket bound to address %s, port %d\n", inet_ntoa(serverAddr.sin_addr), ntohs(serverAddr.sin_port));

	// We'll use this array to hold the messages we exchange with the client.
	char buffer[MESSAGESIZE];

	while (true)
	{
		printf("Waiting for a message...\n");

		sockaddr_in fromAddr;
		int fromAddrSize = sizeof(fromAddr);

		PlayerData player_1_data;
		/*for (int i = 0; i < sizeof(player_1_data.playerID); ++i)
		{
			player_1_data.playerID[i] = '-';
		}*/
		//player_1_data.playerID = "";
		/*player_1_data.playerXpos = 0;
		player_1_data.playerYpos = 0;*/

		int count = recvfrom(sock, (char*) &player_1_data, sizeof(PlayerData), 0, (sockaddr *) &fromAddr, &fromAddrSize);

		if (count < 0)
		{
			die("recvfrom failed");
		}
		if (count != sizeof(PlayerData))
		{
			die("received odd-sized message");
		}

		printf("Received %d bytes from address %s port %d: '",
			   count, inet_ntoa(fromAddr.sin_addr), ntohs(fromAddr.sin_port));
		//fwrite(buffer, 1, count, stdout);
		printf("'\n");

		std::cout << "Player ID: " << player_1_data.playerID <<
			", Player X Pos: " << player_1_data.playerXpos <<
			", Player Y Pos: " << player_1_data.playerYpos << '\n';

		/*printf("Player ID: %s, Player X Pos: %d, Player Y Pos: %d\n",
			player_1_data.playerID,
			player_1_data.playerXpos,
			player_1_data.playerYpos);*/

		// Send the same data back to the address it came from.
		/*if (sendto(sock, buffer, MESSAGESIZE, 0, (const sockaddr *) &fromAddr, sizeof(fromAddr)) != MESSAGESIZE)
		{
			die("sendto failed");
		}*/
	}

	// We won't actually get here, but if we did then we'd want to clean up...
	printf("Quitting\n");
	closesocket(sock);
	WSACleanup();
	return 0;
}


// Print an error message and exit.
void die(const char *message)
{
	fprintf(stderr, "Error: %s (WSAGetLastError() = %d)\n", message, WSAGetLastError());

#ifdef _DEBUG
	// Debug build -- drop the program into the debugger.
	abort();
#else
	exit(1);
#endif
}