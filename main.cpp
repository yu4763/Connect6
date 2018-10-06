#include<stdio.h>
#include "network.cpp"
#include "eval.cpp"
int main(void){
	// 이렇게 3개를 받으면 
	int lastAction = 180;
	char state[361];
	float UCT[361] = {0};
	
	float temp;
	float DeepUCT[361] = {0};
	int inputsize = 11;
	int hiddensize = 10;
	int outputsize = 1;
	int batch = 4;
	
	char lines[44];
	int indexes[10];
	float Y[4]= {10, 10, 10, 10};
	
	float W1[inputsize * hiddensize] = {1};	
	float b1[batch * hiddensize] = {2};
	float W2[hiddensize * outputsize] = {3};
	float b2[batch * outputsize] = {4};
	
	getline(state, lines, 180);
	
	
	Network CalDeepUCT (batch, inputsize, hiddensize, outputsize);
	//Network Winrate (Q_batch, Q_inputsize, Q_hiddensize, Q_outputsize);
	
	//train CALDeepUCT 
	for (int i=0; i<100000; i++){
		CalDeepUCT.backpropagation(W1, b1, W2, b2, lines, Y, 0.001);
	}
	
	//init Deep UCT
	initUCT(DeepUCT, state, lastAction, CalDeepUCT, W1, b1, W2, b2);
	
	//update Deep UCT
	updateUCT(DeepUCT, state, lastAction, CalDeepUCT, W1, b1, W2, b2);
	
	//merge Deep UCT and UCT
	addUCT(DeepUCT, UCT);

	best10(UCT, 10, indexes);
	
	for(int i =0; i<10; i++){
		printf("%d index : %d, value : %f\n",i, indexes[i], UCT[indexes[i]]);	
	}
	
	return 0;
}

