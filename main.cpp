#include<stdio.h>
#include "network.cpp"
#include "eval.cpp"
void get10index(char* state, float* UCT, int lastAction, int* indexes, float* W1, float* b1, float* W2, float* b2, Network network); //판,  UCT = R + sqrt(log(n)/n_action), 최근 액션, best 10을 받을 array pointer 

int main(void){
	
	int inputsize = 11;
	int hiddensize = 10;
	int outputsize = 1;
	int batch = 4;
	// 이렇게 9개를 받으면  됩니다. 
	int lastAction = 180;
	char state[361];
	float UCT[361] = {0};  //여기까지는 겜중에 나오는 정보 
	
	Network CalDeepUCT(batch, inputsize, hiddensize, outputsize); 
	float W1[inputsize * hiddensize] = {1,2,3,4,5,6,};	
	float b1[batch * hiddensize] = {2,3,4,4,5,6,7,7,2,1,2,3,4,4,5,6,7,7,2,1,2,3,4,4,5,6,7,7,2,1,2,3,4,4,5,6,7,7,2,1};
	float W2[hiddensize * outputsize] = {2,3,4,4,5,6,7,7,2,1};
	float b2[batch * outputsize] = {4,2,1,4};        // 이거는 CalDeepUCT 겜 시작할때  객체 하나 만들어주면되고, W1, b1, W2, b2는 겜이 끝날때마다 update 후  저장, 실제 대회에선 학습된 파라미터를 로컬에 저장된거 불러와서 초기화할거임                                      

	int indexes[10]; // 아건 best 10 받을 array 
	
	get10index(state, UCT, lastAction, indexes, W1, b1, W2, b2, CalDeepUCT);
	
	
	return 0;
}
 
void get10index(char* state, float* UCT, int lastAction, int* indexes, float* W1, float* b1, float* W2, float* b2, Network CalDeepUCT){
	float temp;
	float DeepUCT[361] = {0};
	
	initUCT(DeepUCT, state, lastAction, CalDeepUCT, W1, b1, W2, b2);
	
	updateUCT(DeepUCT, state, lastAction, CalDeepUCT, W1, b1, W2, b2);
	
	addUCT(DeepUCT, UCT);
	
	best10(DeepUCT, 10, indexes);
	/*
	for(int i =0; i<10; i++){
		printf("%d index : %d, value : %f\n",i, indexes[i], DeepUCT[indexes[i]]);	
	}
	*/	 
}

