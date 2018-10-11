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
	
	float DeepUCT[361];
	initUCT(DeepUCT, state, CalDeepUCT, W1, b1, W2, b2);  
	int temp1[first*second];
	int temp2[first*second];
	
	best(state, DeepUCT, first, temp1);
	for(int i =0; i<first; i++){
		updateUCT(DeepUCT, state, temp1[i], CalDeepUCT, W1, b1, W2, b2, color);
		secondbest(state, DeepUCT, second, temp2, temp1[i]);
		for(int j =0; j<second; j++){
			index1[first*i +j] = temp1[i];
			index2[first*i +j] = temp2[j];
		}
	}	
	for(int i =0; i<first; i++){
		for(int j =0; j<second; j++){
			for(int i1 =0; i1<first; i1++){
				for(int j1 =0; j1<second; j1++){
					if ( (index1[first*i +j] == index2[first*i1 +j1]) && (index2[first*i +j] == index1[first*i1 +j1]) ){
						index1[first*i +j] = -1;
						index2[first*i +j] = -2;
					}
				}
			}
		}
	}
}
	
