#include<stdio.h>
#include <fstream>
using namespace std;

char black = 1;
char white = 2;
char board2[361] = {0,};

int index1;
int index2;
char parameter = 100;

extern int board[19][19];
extern int op_x[2];
extern int op_y[2];
	//	ValueTable	        0  	  1	    2 	  3	    4	  5	    6	 7	    8	  9	    10	  11    12	  13	14    15    16    17    18
float defaultValue[361] = {-0.8, -0.7, -0.6, -0.5, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.5, -0.6, -0.7, -0.8, // 0
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

float Value[361] = {0,};
char allowed[361] = {0, }; // 0 : now allowed, 1: allowed because it is pattern, 2 ~ 7 : allowed but not pattern,
/*

TypeA x 5	+	TypeB x 5   = Total 10 patterns.
- - O -			- O - -
O - - -			- - - O
- - - O			O - - -
- O - -			- - O -

*/

// TypeA x5
char PATTERN0[5] = {2,0,3,1,4}; char PATTERN1[5] = {0,3,1,4,2}; char PATTERN2[5] = {3,1,4,2,0}; char PATTERN3[5] = {1,4,2,0,3}; char PATTERN4[5] = {4,2,0,3,1}; // 아마 모르셔도 될 거 같은데 궁금하면 물어보세욤 

// TypeB x5
char PATTERN5[5] = {0,2,4,1,3}; char PATTERN6[5] = {2,4,1,3,0}; char PATTERN7[5] = {4,1,3,0,2}; char PATTERN8[5] = {1,3,0,2,4}; char PATTERN9[5] = {3,2,0,4,1};

char* PATTERNS[10] = {PATTERN0, PATTERN1, PATTERN2, PATTERN3, PATTERN4,
					  PATTERN5, PATTERN6, PATTERN7, PATTERN8, PATTERN9};

void update(int index, bool Black);
void clenaValue();
void updateValue(int index);
int runDefenceAlgorithm(char parameter);
char initializeDefenceAlgorithm(bool Black);
void getIndex();

void getIndex(void){
	for (int i = 0; i < 361; i++) {
		board2[i] = board[i/19][i%19];
	}
	if (parameter == 100) {
		parameter = initializeDefenceAlgorithm();
		update(180, true);
	} else{
		update(op_x[0] + (18 - op_y[0]) * 19, true);
		update(op_x[1] + (18 - op_y[1]) * 19, true);
	}

	index1 = runDefenceAlgorithm(parameter);
	update(index1, false);

	index2 = runDefenceAlgorithm(parameter);
	update(index2, false);
}
char initializeDefenceAlgorithm(bool Black){ //패턴의 각 자리의 가치의 총합이 가장 높은 패턴을 계산해 반환 
	float table[10] = {0,};
	int index;
	for (int i=0; i<10; i++){
		for (int j=0; j<19; j++){
			index = PATTERNS[i][j % 5];
			if ( index == 4 ){
				for (int k=0; k<3; k++){
					table[i] = table[i] + Value[19*j + index + 5 * k];
					if (Black){
						if ( board2[19*j + index + 5 * k] == black ){
							table[i] = table[i] + 100;
						} else if ( board2[19*j + index + 5 * k] == white ){
							table[i] = table[i] - 100;
						}
					} else{
						if ( board2[19*j + index + 5 * k] == black ){
							table[i] = table[i] - 100;
						} else if ( board2[19*j + index + 5 * k] == white ){
							table[i] = table[i] + 100;
						}
					}
				}
			}
			else{
				for (int k=0; k<4; k++){
					table[i] = table[i] + Value[19*j + index + 5 * k];
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
	

	for (int i=0; i<19; i++){
		if (PATTERNS[maxindex][i % 5] == 4){
			for (int j=0; j<3; j++){
				if ( board2[i*19 + PATTERNS[maxindex][i % 5] + 5*j] == 0 ){
					allowed[i*19 + PATTERNS[maxindex][i % 5] + 5*j ] = 1;
				}
			}
		}
		else{
			for (int j=0; j<4; j++){
				if ( board2[i*19 + PATTERNS[maxindex][i % 5] + 5*j] == 0 ){
					allowed[i*19 + PATTERNS[maxindex][i % 5] + 5*j ] = 1;
				}
			}
		}
	}
	
	return maxindex;
}
int runDefenceAlgorithm(char parameter){ // 이제 정해진 패턴에서 가장 가치가 높은 index를 계산해 반환 (패턴 중 가장 급하게 막아야 되는 곳) 
	float max = -1000;
	int maxindex;
	int orgx, orgy, x, y, dx, dy, direction;
	
	for (int i=0; i<361; i++){
		if (allowed[i] == 1 && board2[i] != 0){
			allowed[i] = 0;
			orgx = (i / 19);
			orgy = (i % 19);
			for (int j=0; j<4; j++){
				if (j==0){
					dx = 1;
					dy = 0;
				}
				else{
					dx = j - 2;
					dy = 1;
				}
				for (int k=-1; k<2; k = k + 2){
					direction = k;
					x = orgx;
					y = orgy;
					for (int l=0; l<6; l++){
						x = x + k * dx;
						y = y + k * dy;
						
						if ( (x<0) || (x>18) || (y<0) || (y>18) ){
							break;
						}
						else{
							allowed[19 * x +y] = l + 2;
						}
					}
				}
			}
		}
	}
	for (int i=0; i<361; i++){
		if (allowed[i] != 0 && Value[i] >= max ){
			max = Value[i];
			maxindex = i;
		}
	}
	allowed[maxindex] = 0;
	return maxindex;
}

void update(int index, int gameLength, bool Black){
	if (Black){
		board2[index] = black;
	}
	else{
		board2[index] = white;
	}
	clenaValue();
	for (int i=0; i<361; i++){
		updateValue(i);	
	}
}

void clenaValue(){
	for (int i=0; i<361; i++){
		Value[i] = defaultValue[i];
	}
}
void updateValue(int index){
	bool Black;
	if (board2[index] == black){
		Black = true;
	}
	else if (board2[index] == white){
		Black = false;
	}
	else{
		return;
	}
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
			while ( ( ( 0 <= (x + direction * dx) ) && ( (x + direction * dx) <= 18) )  && ( ( 0 <= (y + direction * dy) ) && ( (y + direction * dy) <= 18) ) ){
				x = x + direction * dx;
				y = y + direction * dy;
				distance++;
				if (Black){
					if (board2[19*x + y] != 0){
						Value[19*x + y] = -5;
					}
					else{
						Value[19*x + y] = Value[19*x + y] - (float)(4.0 / (distance) ) ;
					} 
				}
				else{
					if (board2[19*x + y] != 0){
						Value[19*x + y] = -5;
					}
					else{
						Value[19*x + y] = Value[19*x + y] + (float)(4.0 / (distance + 0.75)) ;
					} 
				}
			}
		}
	}
	
	for (int i=0; i<361; i++){
		if (allowed[i] >= 2){
			Value[i] = Value[i] +  (float)( 4.0 / (float)(allowed[i]) ) - 2 ;
		}
		if (board2[i] != 0){
			Value[i] = -5;
		}
	}
}

