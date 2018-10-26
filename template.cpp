#include <WinSock2.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <ctime>

#define		BUF_SIZE	1024
#define		IPAddress	"127.0.0.1"
#define		PORT		8052	// white: 8052 black: 8053
#define		WIDTH		19
#define		HEIGHT		19
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

char black = 1;
char white = -1;
char board2[361] = {0,};
bool init = false;
char parameter = 100;
	//	ValueTable	  0		1	  2		3	  4		5	  6		7	  8		9	 10	   11	 12	   13	 14    15    16    17    18
float value[361] = {-0.8, -0.7, -0.6, -0.5, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.5, -0.6, -0.7, -0.8, // 0
					-0.7, -0.6, -0.5, -0.4, -0.3, -0.3, -0.3, -0.3, -0.3, -0.3, -0.3, -0.3, -0.3, -0.3, -0.3, -0.4, -0.5, -0.6, -0.7, // 1
					-0.6, -0.5, -0.4, -0.3, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.3, -0.4, -0.5, -0.6, // 2
					-0.5, -0.4, -0.3, -0.2, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.2, -0.3, -0.4, -0.5, // 3
					-0.4, -0.3, -0.2, -0.1, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.1, -0.2, -0.3, -0.4, // 4
					-0.4, -0.3, -0.2, -0.1, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.1, -0.2, -0.3, -0.4, // 5
					-0.4, -0.3, -0.2, -0.1, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.1, -0.2, -0.3, -0.4, // 6
					-0.4, -0.3, -0.2, -0.1, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.1, -0.2, -0.3, -0.4, // 7
					-0.4, -0.3, -0.2, -0.1, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.1, -0.2, -0.3, -0.4, // 8
					-0.4, -0.3, -0.2, -0.1, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.1, -0.2, -0.3, -0.4, // 9
					-0.4, -0.3, -0.2, -0.1, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.1, -0.2, -0.3, -0.4, //10
					-0.4, -0.3, -0.2, -0.1, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.1, -0.2, -0.3, -0.4, //11
					-0.4, -0.3, -0.2, -0.1, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.1, -0.2, -0.3, -0.4, //12
					-0.4, -0.3, -0.2, -0.1, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.1, -0.2, -0.3, -0.4, //13
					-0.4, -0.3, -0.2, -0.1, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.1, -0.2, -0.3, -0.4, //14
					-0.5, -0.4, -0.3, -0.2, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.2, -0.3, -0.4, -0.5, //15
					-0.6, -0.5, -0.4, -0.3, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.3, -0.4, -0.5, -0.6, //16
					-0.7, -0.6, -0.5, -0.4, -0.3, -0.3, -0.3, -0.3, -0.3, -0.3, -0.3, -0.3, -0.3, -0.3, -0.3, -0.4, -0.5, -0.6, -0.7, //17
					-0.8, -0.7, -0.6, -0.5, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.5, -0.6, -0.7, -0.8  //18
};
/*
TypeA x 5	+	TypeB x 5   = Total 10 patterns.
- O - -			- - O -
- X X O			O X X -
O X X -			- X X O
- - O -			- O - -
*/
// TypeA x5
char PATTERN0[5] = {2,0,3,1,4}; char PATTERN1[5] = {0,3,1,4,2}; char PATTERN2[5] = {3,1,4,2,0}; char PATTERN3[5] = {1,4,2,0,3}; char PATTERN4[5] = {4,2,0,3,1};
// TypeB x5
char PATTERN5[5] = {0,2,4,1,3}; char PATTERN6[5] = {2,4,1,3,0}; char PATTERN7[5] = {4,1,3,0,2}; char PATTERN8[5] = {1,3,0,2,5}; char PATTERN9[5] = {4,2,0,5,1};

char* PATTERNS[10] = {PATTERN0, PATTERN1, PATTERN2, PATTERN3, PATTERN4,
					  PATTERN5, PATTERN6, PATTERN7, PATTERN8, PATTERN9};

void update(int index, int gameLenth, bool Black);
void drawBoard(int gameLength);
void drawValue(int gameLength);
void updateValue(int index, bool Black);
int runDefenceAlgorithm(char parameter);
char initializeDefenceAlgorithm();

char initializeDefenceAlgorithm(){
	
	float table[10] = {0,};
	int index;
	for (int i=0; i<10; i++){
		for (int j=0; j<19; j++){
			index = PATTERNS[i][j % 5];
			if ( index == 4 ){
				for (int k=0; k<3; k++){
					table[i] = table[i] + value[19*j + index + 5 * k];
				}
			}
			else{
				for (int k=0; k<4; k++){
					table[i] = table[i] + value[19*j + index + 5 * k];
				}
			}
		}	
	}
	
	float max = table[0];
	int maxindex = 0;
	
	for (int i=0; i<10; i++){
		if ( table[i] > max){
			max = table[i];
			maxindex = i;
		}
	}
	return maxindex;
}
int runDefenceAlgorithm(char parameter){
	
	float max = value[PATTERNS[parameter][0]];
	int maxindex = PATTERNS[parameter][0];
	for (int i=0; i<19; i++){
		if (PATTERNS[parameter][i % 5] == 4){
			for (int j=0; j<3; j++){
				if ( (value[i*19 + PATTERNS[parameter][i % 5] + 5*j] > max) && (board[i*19 + PATTERNS[parameter][i % 5] + 5*j] == 0) ){
					max = value[i*19 + PATTERNS[parameter][i % 5] + 5*j];
					maxindex = i*19 + PATTERNS[parameter][i % 5] + 5*j;
				}
			}
		}
		else{
			for (int j=0; j<4; j++){
				if ( (value[i*19 + PATTERNS[parameter][i % 5] + 5*j] > max) && (board[i*19 + PATTERNS[parameter][i % 5] + 5*j] == 0) ){
					max = value[i*19 + PATTERNS[parameter][i % 5] + 5*j];
					maxindex = i*19 + PATTERNS[parameter][i % 5] + 5*j;
				}
			}
		}
	}

	return maxindex;
}

void update(int index, int gameLength, bool Black){
	if (Black){
		board2[index] = black;
	}
	else{
		board2[index] = white;
	}
	updateValue(index, Black);
}


void updateValue(int index, bool Black){
	int orgx = index / 19;
	int orgy = index % 19;
	
	int x, y, dx, dy, direction;
	float distance;
	for (int i = 0; i<4; i++){
		if (i==0){
			dx = 1;
			dy = 0;
		}
		else{
			dx = i-2;
			dy = 1;
		}
		for (int j=-1; j<2; j = j+2){
			direction = j;
			distance = 0;
			x = orgx;
			y = orgy;
			value[19*x + y] = 0;
			while ( ( ( 0 <= (x + direction * dx) ) && ( (x + direction * dx) <= 18) )  && ( ( 0 <= (y + direction * dy) ) && ( (y + direction * dy) <= 18) ) ){
				x = x + direction * dx;
				y = y + direction * dy;
				distance++;
				if (Black){
					if (board2[19*x + y] != 0){
						value[19*x + y] = 0;
					}
					else{
						value[19*x + y] = value[19*x + y] - (float)(5.0 / distance) ;
					} 
				}
				else{
					if (board2[19*x + y] != 0){
						value[19*x + y] = 0;
					}
					else{
						value[19*x + y] = value[19*x + y] + (float)(5.0 / distance) ;
					} 
				}
			}
		}
	}
}

void put_stone() {
	if (parameter == 100){
		initializeDefenceAlgorithm();
	}
	for (int i = 0; i<361; i++){
		board2[i] = board[i/19][i%19];
	}
	for (int i = 0; i < CNT; i++) {
		do {
			index = runDefenceAlgorithm(parameter);
			x[i] = index / WIDTH;
			y[i] = index % HEIGHT;
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
				board2[18-i][j] = buf[i * 19 + j] - '0';

		op_x[0] = (buf[361]-'0')*10 + (buf[362] - '0');
		op_y[0] = (buf[363] - '0') * 10 + (buf[364] - '0');
		op_x[1] = (buf[365] - '0') * 10 + (buf[366] - '0');
		op_y[1] = (buf[367] - '0') * 10 + (buf[368] - '0');

		printf("\n\nOpposite: %d %d / %d %d", op_x[0], op_y[0], op_x[1], op_y[1]);

		put_stone();
		//print_board();

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
