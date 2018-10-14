#include "network.cpp"
#include "eval.cpp"
void getbestindex(char* state, int* indexes, float* W1, float* b1, float* W2, float* b2, Network CalDeepUCT, int number){
	float DeepUCT[361] = {0};
	
	initUCT(DeepUCT, state, CalDeepUCT, W1, b1, W2, b2);
	best(state, DeepUCT, number, indexes);
	/*
	for(int i =0; i<number; i++){
		printf("%d index : %d, value : %f\n",i, indexes[i], DeepUCT[indexes[i]]);	
	}
	*/
}


void getbestpair(char* state, int* index1, int* index2, float* W1, float* b1, float* W2, float* b2, Network CalDeepUCT, int first, int second, char color){
	float DeepUCT[361] = {0};
	
	initUCT(DeepUCT, state, CalDeepUCT, W1, b1, W2, b2);
	best(state, DeepUCT, first, index1);
	for(int i =0; i<first; i++){
		updateUCT(DeepUCT, state, index1[i], CalDeepUCT, W1, b1, W2, b2, color);
		secondbest(state, DeepUCT, second, index2, index1[i]);
	}
	/*
	for(int i =0; i<first; i++){
		for(int j =0; j<second; j++){
			printf("%d index1 : %d index2 : %d \n",i*first + j, index1[i], index2[j]);	
		}
	}
	*/
}
	
