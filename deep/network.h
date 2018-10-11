#ifndef __NETWORK_H__
#define __NETWORK_H__

#include<math.h>

float sigmoid(float x);
float diffsigmoid(float x);

class Network{
	int batch, inputsize, hiddensize, outputsize;
	
	public:
		Network(int, int, int, int);
		void getoutput(float *W1, float *b1, float *W2, float *b2, char* DATA, float* OUTPUT);
		void backpropagation(float *W1, float *b1, float *W2, float *b2, char* DATA, float* LABEL, float lr);	
};

#endif
