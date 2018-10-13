#include "eval.h"
#include "network.h"
using namespace std;

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

void updateUCT(float* UCT, char* state, int action, Network CalUCT, float *W1, float *b1, float *W2, float *b2, char color){
	int dx, dy, x, y, index;
	char line[44];
	float OUTPUT[4];
	float temp;
	float temp2 = 0;
	state[action] = color;
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
			if ( !((x < 0) || (x > 18) || (y < 0) || (y > 18)) ){
				getline(state, line, index);
				CalUCT.getoutput(W1, b1, W2, b2, line, OUTPUT);
				//printf("%d %d %d %d\n", i, j, x,y);
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
	
	state[action] = 0;
}

void initUCT(float* UCT, char* state, Network CalUCT, float *W1, float *b1, float *W2, float *b2){
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

void best(char* state, float* UCT, int best, int* indexes){
	float max;
	int maxindex;
	float recentmax = 1000;
	for (int j=0; j<best; j++){
		max = -100;
		for (int i=0; i<361; i++){
			if (max <= UCT[i] && UCT[i] < recentmax && state[i] == 0){
				max = UCT[i];
				maxindex = i;
				//printf("%d", best);
			}
		}
		recentmax = max;
		indexes[j] = maxindex;
		//printf("%d %d", max, maxindex);
	}
}

void secondbest(char* state, float* UCT, int best, int* indexes, int first){
	float max;
	int maxindex;
	float recentmax = 1000;
	for (int j=0; j<best; j++){
		max = -100;
		for (int i=0; i<361; i++){
			if (max <= UCT[i] && UCT[i] < recentmax && state[i] == 0 && i!=first){
				max = UCT[i];
				maxindex = i;
				//printf("%d", best);
			}
		}
		recentmax = max;
		indexes[j] = maxindex;
		//printf("%d %d\n", first, maxindex);
	}
}

void GetBestIndex(char* state, int* indexes, float* W1, float* b1, float* W2, float* b2, Network CalDeepUCT, int number){
	float DeepUCT[361] = {0};
	
	initUCT(DeepUCT, state, CalDeepUCT, W1, b1, W2, b2);
	best(state, DeepUCT, number, indexes);
	/*
	for(int i =0; i<number; i++){
		printf("%d index : %d, value : %f\n",i, indexes[i], DeepUCT[indexes[i]]);	
	}
	*/
}


void GetBestPair(char* state, int* index1, int* index2, float* W1, float* b1, float* W2, float* b2, Network CalDeepUCT, int first, int second, char color){
	
	float DeepUCT[361];
	initUCT(DeepUCT, state, CalDeepUCT, W1, b1, W2, b2);  
	int temp1[first*second];
	int temp2[first*second];
	
	best(state, DeepUCT, first, temp1);
	for(int i =0; i<first; i++){
		updateUCT(DeepUCT, state, temp1[i], CalDeepUCT, W1, b1, W2, b2, color);
		secondbest(state, DeepUCT, second, temp2, temp1[i]);
		for(int j =0; j<second; j++){
			index1[second*i +j] = temp1[i];
			index2[second*i +j] = temp2[j];
		}
	}	
	for(int i =0; i<first; i++){
		for(int j =0; j<second; j++){
			for(int i1 =0; i1<first; i1++){
				for(int j1 =0; j1<second; j1++){
					if ( (index1[second*i +j] == index2[second*i1 +j1]) && (index2[second*i +j] == index1[second*i1 +j1]) ){
						index1[second*i +j] = -1;
					}
				}
			}
		}
	}
  for (int i = 0; i < first; i++) {
    for (int j = 0; j < second; j++) {
      cout << index1[second*i + j] << " " << index2[second*i + j] << endl;
    }
  }
  cout << "End" << endl;
}
