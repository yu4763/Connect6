#ifndef __NETWORK_H__
#define __NETWORK_H__

#include<stdio.h>
#include<math.h>

float sigmoid(float x);
float diffsigmoid(float x);
void saveWeight(float* W1, float* b1, float* W2, float* b2, bool black);
void loadWeight(float* W1, float* b1, float* W2, float* b2, bool black);

class Network{
	int batch, inputsize, hiddensize, outputsize;
	
	public:
		Network(int, int, int, int);
		void getoutput(float *W1, float *b1, float *W2, float *b2, char* DATA, float* OUTPUT);
		void backpropagation(float *W1, float *b1, float *W2, float *b2, char* DATA, float* LABEL, float lr);	
};

#endif

