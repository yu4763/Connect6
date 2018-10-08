#include "eval.h"

void getline(char* state, char* line, int action){
	int dx, dy, x, y, index;
	for (int i = 0; i < 4; i++){
		if (i ==0){
			dx = 1;
			dy = 0;
		}
		else{
			dx = i - 2;
			dy = 1;
		}
		x = action % 19;
		y = action / 19;
		for (int j=0; j<11; j++){
			x = action % 19 + (j - 5) * dx;
			y = action / 19 + (j - 5) * dy;
			index = x + y * 19;
			if ( (x < 0) || (x > 18) || (y < 0) || (y > 18) ){
				line[11 * i +j] = 2;
			}
			else{
				line[11 * i +j] = state[index];
			}
		}
	}
}

void updateUCT(float* UCT, char* state, int action, Network CalUCT, float *W1, float *b1, float *W2, float *b2){
	int dx, dy, x, y, index;
	char line[44];
	float OUTPUT[4];
	float temp;
	float temp2 = 0;
	for (int i = 0; i < 4; i++){
		if (i ==0){
			dx = 1;
			dy = 0;
		}
		else{
			dx = i - 2;
			dy = 1;
		}
		for (int j=0; j<11; j++){
			x = action % 19 + (j - 5) * dx;
			y = action / 19 + (j - 5) * dy;
			//printf("%d %d\n", x,y);
			index = x + y * 19;
			if ( !(x < 0) || (x > 18) || (y < 0) || (y > 18) ){
				getline(state, line, index);
				CalUCT.getoutput(W1, b1, W2, b2, line, OUTPUT);
				temp = OUTPUT[0] + OUTPUT[1] + OUTPUT[2] + OUTPUT[3];
				if (index == action) {
					temp2 += temp;
				} else {
					UCT[index] = temp;
				}
			}
		}
		UCT[action] = temp2 * 0.25;
	}
}

void initUCT(float* UCT, char* state, int action, Network CalUCT, float *W1, float *b1, float *W2, float *b2){
	char line[44];
	float OUTPUT[4];
	float temp;
	for (int i = 0; i < 361; i++){
		getline(state, line, i);
		CalUCT.getoutput(W1, b1, W2, b2, line, OUTPUT);
		temp = OUTPUT[0] + OUTPUT[1] + OUTPUT[2] + OUTPUT[3];
		
		UCT[i] = temp;
		
	}
}

void addUCT(float* DeepUCT, float* UCT){
	for (int i = 0; i < 361; i++){
		DeepUCT[i] += UCT[i];
	}
}

void best10(float* UCT, int best, int* indexes){
	float max;
	int maxindex;
	float recentmax = 1000;
	for (int j=0; j<best; j++){
		max = -100;
		for (int i=0; i<361; i++){
			if (max <= UCT[i] && UCT[i] < recentmax){
				max = UCT[i];
				maxindex = i;
			}
		}
		recentmax = max;
		indexes[j] = maxindex;
		//printf("%d %d", max, maxindex);
	}
}

