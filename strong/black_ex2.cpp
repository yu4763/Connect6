#include <WinSock2.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <ctime>

#include "monte_carlo_tree_search_strong.h"
extern int best_pos1, best_pos2;

#define		BUF_SIZE	1024
#define		IPAddress	"127.0.0.1"
#define		PORT		8053	// white: 8052 black: 8053
#define		WIDTH		19
#define		HEIGHT	19
#define		CNT			2
#define		CORD_X(X)	((WIDTH - 1) - X)
#define		CORD_Y(Y)	(Y)
#define		CORD(X,Y)	{(X)^=(Y); (Y)^=(X); (X)^=(Y);}

#pragma warning(disable: 4996)

using namespace std;

int board[WIDTH][HEIGHT];
int op_x[2];
int op_y[2];
int x[2];
int y[2];

void print_board() {

	printf("\n    ---------------------------------------------------------\n");

	for (int i = 0; i < WIDTH; i++) {
		printf("%3d", 18-i);
		for (int j = 0; j < HEIGHT; j++) {
			printf("|%c", board[i][j] == 1 ? 'O' : board[i][j] == 2 ? 'X' : ' ');
		}
		printf("|\n");
	}
	printf("   ----------------------------------------------------------\n   ");
	for (int i = 0; i < WIDTH; i++) printf("%3d", i);

}

int isFree(int X, int Y) {
	return X >= 0 && Y >= 0 && X < WIDTH && Y < HEIGHT && !board[CORD_X(X)][CORD_Y(Y)];
}

void put_stone() {
	int index[2];
	char board_[361];
	for (int i = 0; i < 19; i++) {
		int iter = 19 * i;
		for (int j = 0; j < 19; j++) {
			board_[iter + j] = board[i][j];
		}
	}
	GetBestPositions(board_, index, 2, 1);
	// YOUT ALGORITHM
	for (int i = 0; i < CNT; i++) {
		do {
			if (i == 0) {
				x[i] = index[0] % 19;
				y[i] = 18 - (index[0] / 19);
			} else if (i == 1) {
				x[i] = index[1] % 19;
				y[i] = 18 - (index[1] / 19);
			}
			CORD(x[i], y[i]);
		} while (!isFree(x[i], y[i]));
		CORD(x[i], y[i]);

		if (x[1] == x[0] && y[1] == y[0]) i--;
	}
}

int main() {

	WORD		wVersionRequested;
	WSADATA		wsaData;
	SOCKADDR_IN servAddr, cliAddr; //Socket address information
	int			err;
	char        buf[BUF_SIZE];

	srand((unsigned int)time(0));

	wVersionRequested = MAKEWORD(1, 1);
	err = WSAStartup(wVersionRequested, &wsaData);

	if (err != 0) {
		cout << "WSAStartup error " << WSAGetLastError() << endl;
		WSACleanup();
		return false;
	}

	servAddr.sin_family = AF_INET; // address family Internet
	servAddr.sin_port = htons(PORT); //Port to connect on
	servAddr.sin_addr.s_addr = inet_addr(IPAddress); //Target IP

	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //Create socket
	if (s == INVALID_SOCKET)
	{
		cout << "Socket error " << WSAGetLastError() << endl;
		WSACleanup();
		return false; //Couldn't create the socket
	}

	if (bind(s, reinterpret_cast<SOCKADDR *>(&servAddr), sizeof(servAddr)) == SOCKET_ERROR){
		cout << "Binding failed. Error code: " << WSAGetLastError() << endl;
		WSACleanup();
		return false; //Couldn't connect
	}

	cout << "Waiting for client..." << endl;

	while (true) {

		listen(s, 5);
		int cSize = sizeof(cliAddr);
		SOCKET s2 = accept(s, reinterpret_cast<SOCKADDR *>(&cliAddr), &cSize);
		//cout << "Connection established. New socket num is " << s2 << endl << endl;

		int n;
		n = recv(s2, buf, sizeof(buf), NULL);
		if (n <= 0) { cout << "Got nothing" << endl; break; }
		buf[n] = 0;

		for (int i = 0; i < 19; i++)
			for (int j = 0; j < 19; j++)
				board[18-i][j] = buf[i * 19 + j] - '0';

		op_x[0] = (buf[361]-'0')*10 + (buf[362] - '0');
		op_y[0] = (buf[363] - '0') * 10 + (buf[364] - '0');
		op_x[1] = (buf[365] - '0') * 10 + (buf[366] - '0');
		op_y[1] = (buf[367] - '0') * 10 + (buf[368] - '0');

		printf("\n\nOpposite: %d %d / %d %d", op_x[0], op_y[0], op_x[1], op_y[1]);

		put_stone();
		print_board();

		char sbuf[8];
		sbuf[0] = x[0] / 10 + '0';
		sbuf[1] = x[0] % 10 + '0';
		sbuf[2] = y[0] / 10 + '0';
		sbuf[3] = y[0] % 10 + '0';
		sbuf[4] = x[1] / 10 + '0';
		sbuf[5] = x[1] % 10 + '0';
		sbuf[6] = y[1] / 10 + '0';
		sbuf[7] = y[1] % 10 + '0';

		send(s2, sbuf, sizeof(sbuf), NULL);
		closesocket(s2);
	}

	closesocket(s);
	WSACleanup();

	return 0;
}
