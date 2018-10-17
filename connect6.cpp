#include<stdio.h>
char black = 1;
char white = -1;
char board[361] = {0,};
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
char PATTERN0[5] = {2,0,3,1,4}; char PATTERN1[5] = {0,3,1,4,2}; char PATTERN2[5] = {3,1,4,2,0}; char PATTERN3[5] = {1,4,2,0,3}; char PATTERN4[5] = {4,2,0,3,1}; // 아마 모르셔도 될 거 같은데 궁금하면 물어보세욤 

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

int main(void){

	int gameLength = 0;
	char parameter;
	int index;
	
	// Initial point Black, (9,9)
	update(180, gameLength, true);
	gameLength++;
	
	// First White turn
	update(181, gameLength, false);
	gameLength++;
	update(184, gameLength, false);
	gameLength++;
	
	// Get parameter which is required for DefenceAlgorithm. (Run it once, First Turn of Black!)
	parameter = initializeDefenceAlgorithm();
	
	// First turn of Black
	index = runDefenceAlgorithm(parameter);
	update(index, gameLength, true);
	gameLength++;
	
	index = runDefenceAlgorithm(parameter);
	update(index, gameLength, true);
	gameLength++;
	
	return 0;
}
char initializeDefenceAlgorithm(){ //패턴의 각 자리의 가치의 총합이 가장 높은 패턴을 계산해 반환 
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
int runDefenceAlgorithm(char parameter){ // 이제 정해진 패턴에서 가장 가치가 높은 index를 계산해 반환 (패턴 중 가장 급하게 막아야 되는 곳) 
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
		board[index] = black;
	}
	else{
		board[index] = white;
	}
	drawBoard(gameLength);
	updateValue(index, Black);
	drawValue(gameLength);
}

void drawBoard(int gameLength){
	printf("\nThe board of %d th Turn\n", gameLength);
	for (int i=0; i<19; i++){
		for (int j=0; j<19; j++){
			if (board[19*i+j] == black){
				printf("O ");
			}
			else if (board[19*i+j] == white){
				printf("X ");
			}
			else{
				printf("- ");
			}
		}
		printf("\n");
	}
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
					if (board[19*x + y] != 0){
						value[19*x + y] = 0;
					}
					else{
						value[19*x + y] = value[19*x + y] - (float)(5.0 / distance) ;
					} 
				}
				else{
					if (board[19*x + y] != 0){
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

void drawValue(int gameLength){
	printf("\nThe Value of %d th Turn\n", gameLength);
	for (int i=0; i<19; i++){
		for (int j=0; j<19; j++){
			if (value[19*i +j] > 0){
				printf(" %1.1f ", value[19*i +j]);	
			}
			else if (value[19*i +j] == 0){
				printf(" 0.0 ");	
			}
			else{
				printf("%1.1f ", value[19*i +j]);	
			}
		}
		printf("\n");
	}
}
