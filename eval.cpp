#include "eval.h"
void getline(char* state, char* line, int action){
	int interval, index;
	for (int i = 0; i < 4; i++){
		if (i ==0){
			interval = 1;
		}
		else{
			interval = 19-2 + i;
		}
		for (int j=0; j<11; j++){
			index = action + (j - 5) * interval;
			if ( (index / 19 > 0) || (index / 19 < 19) || (index % 19 > 0) || (index % 19 < 19) ){
				line[11 * i +j] = 2;
			}
			else{
				line[11 * i +j] = state[index];
			}
		}
	}
}

void plusUCT(float* UCT, char* state, int action, Network CalUCT, float *W1, float *b1, float *W2, float *b2){
	int interval, index;
	char line[44];
	float OUTPUT[4];
	float temp;
	for (int i = 0; i < 4; i++){
		if (i ==0){
			interval = 1;
		}
		else{
			interval = 19-2 + i;
		}
		for (int j=0; j<11; j++){
			index = action + (j - 5) * interval;
			if ( !(index / 19 > 0) || (index / 19 < 19) || (index % 19 > 0) || (index % 19 < 19) ){
				getline(state, line, index);
				CalUCT.getoutput(W1, b1, W2, b2, line, OUTPUT);
				temp = OUTPUT[0] + OUTPUT[1] + OUTPUT[2] + OUTPUT[3];
				UCT[11 * i +j] += temp;
			}
		}
	}
}

void floatswap(float* a, float* b){
    float tmp=*a;
    *a=*b;
    *b=tmp;
}

void intswap(int* a, int* b){
    int tmp=*a;
    *a=*b;
    *b=tmp;
}
 
void quick_sort(float* UCT, int* index, int start, int end){
 
    if(start>=end) return;

    int mid=(start+end)/2;
    int pivot=UCT[mid];
 
    floatswap(&UCT[start],&UCT[mid]);
    intswap(&index[start],&index[mid]);
 
    int p=start+1,q=end;
 	
    while(1){
        while(UCT[p]<=pivot){ p++; }
        while(UCT[q]>pivot){ q--; }
        //printf("%d", UCT[p]);
 
        if(p>q) break;
 
        floatswap(&UCT[p],&UCT[q]);
        intswap(&index[p],&index[q]);
    }
 
    floatswap(&UCT[start],&UCT[q]);
    intswap(&index[start],&index[q]);
 
    quick_sort(UCT,index,start,q-1);
    quick_sort(UCT,index,q+1,end);
 
}

