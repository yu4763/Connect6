#include<stdio.h>
#include "network.cpp"

int main(void){
	int inputsize = 2;
	int hiddensize = 4;
	int outputsize = 1;
	int batch = 4;
	
	int lines[48];
	int X[4*2]={0,0,0,1,1,0,1,1};
	float Y[4]= {0,1,1,0};
	float OUTPUT[4];
	
	float W1[inputsize * hiddensize] = {0};
	float b1[batch * hiddensize] = {0};
	float W2[hiddensize * outputsize] = {0};
	float b2[batch * outputsize] = {0};
	
	Network XNOR;
	
	for (int i=0; i<10000; i++){
		XNOR.backpropagation(X, W1, b1, W2, b2, batch, inputsize, hiddensize, outputsize, Y, 0.001);
			if (i % 1000 == 0){
				XNOR.getoutput(X, W1, b1, W2, b2, batch, inputsize, hiddensize, outputsize, OUTPUT);
				printf("%f %f %f %f\n",OUTPUT[0],OUTPUT[1],OUTPUT[2],OUTPUT[3]);	
			}
	}	
	return 0;
}

