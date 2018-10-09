#include<stdio.h>
#include "network.cpp"
#include "eval.cpp"
void getbestindex(char* state, int* indexes, float* W1, float* b1, float* W2, float* b2, Network network, int number);
void getbestpair(char* state, int* index1, int* index2, float* W1, float* b1, float* W2, float* b2, Network network, int first, int second, char color);

int main(void){
	
	int inputsize = 11;
	int hiddensize = 10;
	int outputsize = 1;
	int batch = 4;
	
	int lastAction = 180;
	char state[361] = {0};
	
	for(int i =0; i<361; i++){
		state[i] = i % 3;
	}
	float UCT[361] = {0};  
	
	Network CalDeepUCT(batch, inputsize, hiddensize, outputsize); 
	float W1[inputsize * hiddensize] = {1,2,3,4,5,6,};	
	float b1[batch * hiddensize] = {2,3,4,4,5,6,7,7,2,1,2,3,4,4,5,6,7,7,2,1,2,3,4,4,5,6,7,7,2,1,2,3,4,4,5,6,7,7,2,1};
	float W2[hiddensize * outputsize] = {2,3,4,4,5,6,7,7,2,1};
	float b2[batch * outputsize] = {4,2,1,4};
	int index1[45]; 
	int index2[45]; 
	
	getbestindex(state, index1, W1, b1, W2, b2, CalDeepUCT, 45);
	
	getbestpair(state, index1, index2, W1, b1, W2, b2, CalDeepUCT, 5, 5, 1);
	
	
	return 0;
}
 
void getbestindex(char* state, int* indexes, float* W1, float* b1, float* W2, float* b2, Network CalDeepUCT, int number){
	float DeepUCT[361] = {0};
	
	initUCT(DeepUCT, state, CalDeepUCT, W1, b1, W2, b2);
	best(state, DeepUCT, number, indexes);
	
	for(int i =0; i<number; i++){
		printf("%d index : %d, value : %f\n",i, indexes[i], DeepUCT[indexes[i]]);	
	}
}


void getbestpair(char* state, int* index1, int* index2, float* W1, float* b1, float* W2, float* b2, Network CalDeepUCT, int first, int second, char color){
	float DeepUCT[361] = {0};
	
	initUCT(DeepUCT, state, CalDeepUCT, W1, b1, W2, b2);
	best(state, DeepUCT, first, index1);
	for(int i =0; i<first; i++){
		updateUCT(DeepUCT, state, index1[i], CalDeepUCT, W1, b1, W2, b2, color);
		secondbest(state, DeepUCT, second, index2, index1[i]);
	}
	
	for(int i =0; i<first; i++){
		for(int j =0; j<second; j++){
			printf("%d index1 : %d index2 : %d \n",i*first + j, index1[i], index2[j]);	
		}
	}
	
}

