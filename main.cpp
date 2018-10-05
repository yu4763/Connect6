#include<stdio.h>
#include "network.cpp"
#include "eval.cpp"
int main(void){
	// 이렇게 3개를 받으면 
	int lastAction = 180;
	char state[361];
	float UCT[361] = {0};
	
	int inputsize = 11;
	int hiddensize = 10;
	int outputsize = 1;
	int batch = 4;
	
	char lines[44];
	float Y[4]= {5, 5, 5, 5};
	
	float W1[inputsize * hiddensize] = {0};	
	float b1[batch * hiddensize] = {0};
	float W2[hiddensize * outputsize] = {0};
	float b2[batch * outputsize] = {0};
	
	getline(state, lines, 0);
	
	
	Network CalUCT (batch, inputsize, hiddensize, outputsize);
	//Network Winrate (Q_batch, Q_inputsize, Q_hiddensize, Q_outputsize);
	
	for (int i=0; i<1000; i++){
		CalUCT.backpropagation(W1, b1, W2, b2, lines, Y, 0.001);
	}
	int index[361];
    for (int i=0; i<360; i++){
    	index[i] = i;
	}
	plusUCT(UCT, state, lastAction, CalUCT, W1, b1, W2, b2);

	quick_sort(UCT, index, 0, 360);
	
	//상위 10개를 드림 
	for (int i=0; i<10; i++){
		printf("%d : %d\n",i, index[i]);
	}
	return 0;
}

