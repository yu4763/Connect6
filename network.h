#include<math.h>

float sigmoid(float x);
float diffsigmoid(float x);

class Network{
	public:
		void getoutput(int* DATA, float* W1, float* b1, float* W2, float* b2, int batch, int inputsize, int hiddensize, int outputsize, float* OUTPUT);
		void backpropagation(int* DATA, float* W1, float* b1, float* W2, float* b2, int batch, int inputsize, int hiddensize, int outputsize, float* LABEL, float lr);	
};

