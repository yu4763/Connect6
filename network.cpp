#include "network.h"
Network::Network(int _batch, int _inputsize, int _hiddensize, int _outputsize){
	batch = _batch;
	inputsize = _inputsize;
	hiddensize = _hiddensize;
	outputsize = _outputsize;
}

void saveWeight(float* W1, float* b1, float* W2, float* b2, bool black){
	FILE *file;
	if (black){
		file = fopen("BlackWeights", "w");

		for(int i = 0; i<110; i++){
			fprintf(file, "%f ", W1[i]);
		}
		fprintf(file, "\n");
		for(int i = 0; i<40; i++){
			fprintf(file, "%f ", b1[i]);
		}
		fprintf(file, "\n");
		for(int i = 0; i<10; i++){
			fprintf(file, "%f ", W2[i]);
		}
		fprintf(file, "\n");
		for(int i = 0; i<4; i++){
			fprintf(file, "%f ", b2[i]);
		}
	}
	else{
		file = fopen("WhiteWeights", "w");

		for(int i = 0; i<110; i++){
			fprintf(file, "%f ", W1[i]);
		}
		fprintf(file, "\n");
		for(int i = 0; i<40; i++){
			fprintf(file, "%f ", b1[i]);
		}
		fprintf(file, "\n");
		for(int i = 0; i<10; i++){
			fprintf(file, "%f ", W2[i]);
		}
		fprintf(file, "\n");
		for(int i = 0; i<4; i++){
			fprintf(file, "%f ", b2[i]);
		}		
	}
}

void loadWeight(float* W1, float* b1, float* W2, float* b2,  bool black){
	if (black){
		FILE *file;
		file = fopen("BlackWeights", "r");

		for(int i = 0; i<110; i++){
			fscanf(file, "%f ", &W1[i]);
		}
		fprintf(file, "\n");
		for(int i = 0; i<40; i++){
			fscanf(file, "%f ", &b1[i]);
		}
		fprintf(file, "\n");
		for(int i = 0; i<10; i++){
			fscanf(file, "%f ", &W2[i]);
		}
		fprintf(file, "\n");
		for(int i = 0; i<4; i++){
			fscanf(file, "%f ", &b2[i]);
		}
	}
	else{
		FILE *file;
		file = fopen("BlackWeights", "r");

		for(int i = 0; i<110; i++){
			fscanf(file, "%f ", &W1[i]);
		}
		fprintf(file, "\n");
		for(int i = 0; i<40; i++){
			fscanf(file, "%f ", &b1[i]);
		}
		fprintf(file, "\n");
		for(int i = 0; i<10; i++){
			fscanf(file, "%f ", &W2[i]);
		}
		fprintf(file, "\n");
		for(int i = 0; i<4; i++){
			fscanf(file, "%f ", &b2[i]);
		}	
	}
}

void Network::getoutput(float *W1, float *b1, float *W2, float *b2, char* DATA, float* OUTPUT){
	float Y1[batch * hiddensize];
	float L1[batch * hiddensize];
	//float L2[_batch * _outputsize];			
	//Layer1
	for (int i = 0; i<batch; i++){
		for (int j = 0; j<hiddensize; j++){
			Y1[hiddensize * i + j] = 0;
			for (int k=0; k<inputsize; k++){
				Y1[hiddensize * i + j] += DATA[inputsize * i + k] * W1[hiddensize * k + j];
			}
			Y1[hiddensize * i + j] += b1[hiddensize * i + j];
			L1[hiddensize * i + j] = sigmoid(Y1[hiddensize * i + j]);
		}
	}
	
	//Layer2
	for (int i = 0; i<batch; i++){
		for (int j = 0; j<outputsize; j++){
			OUTPUT[outputsize * i + j] = 0;
			for (int k=0; k<hiddensize; k++){
				OUTPUT[outputsize * i + j] += L1[hiddensize * i + k] * W2[outputsize * k + j];
			}
			OUTPUT[outputsize * i + j] += b2[outputsize * i + j];
		}
	}
}
	
void Network::backpropagation(float *W1, float *b1, float *W2, float *b2, char* DATA, float* LABEL, float lr){
	float Y1[batch * hiddensize] = {0};
	float L1[batch * hiddensize];
	float L2[batch * outputsize];
	float dL1[batch * hiddensize];
	float dL2[batch * outputsize];
	float dW1[inputsize * hiddensize];
	float dW2[hiddensize * outputsize];
	float Cost[batch * outputsize];
	float temp;
	//printf("%d", W1[0]);
	//printf("%d", 2);
	//Layer1
	for (int i = 0; i<batch; i++){
		for (int j = 0; j<hiddensize; j++){
			Y1[hiddensize * i + j] = 0;
			for (int k=0; k<inputsize; k++){
				Y1[hiddensize * i + j] += DATA[inputsize * i + k] * W1[hiddensize * k + j];
			}
			Y1[hiddensize * i + j] += b1[hiddensize * i + j];
			L1[hiddensize * i + j] = sigmoid(Y1[hiddensize * i + j]);
			//printf("%d", W1[0]);
		}
	}

	//Layer2
	for (int i = 0; i<batch; i++){
		for (int j = 0; j<outputsize; j++){
			L2[outputsize * i + j] = 0;
			for (int k=0; k<hiddensize; k++){
				L2[outputsize * i + j] += L1[hiddensize * i + k] * W2[outputsize * k + j];
			}
			L2[outputsize * i + j] += b2[outputsize * i + j];
		}
	}
	//Layer2
	// dL2 = db2 = error, dW2 = np.matmul(L1.T, dL2)
	for (int i=0; i<batch * outputsize; i++){
		temp =  (L2[i] - LABEL[i]);
		Cost[i] = temp*temp;
		dL2[i] = temp;
		b2[i] -= lr * dL2[i];
		//printf("%f ", L2[i]);
	}
	
	//printf("\n");
	for (int i = 0; i<hiddensize; i++){
		for (int j = 0; j<outputsize; j++){
			dW2[outputsize * i + j] = 0;
			for (int k=0; k<batch; k++){
				dW2[outputsize * i + j] += L1[i + k * hiddensize] * dL2[outputsize * k + j];
			}
			W2[outputsize * i + j] -= lr *dW2[outputsize * i + j];
		}
	}
	//Layer1
	// dL1 = db1 = np.matmul(dL2 , W2.T) * diff_sigmoid(Y1), dW1 = np.matmul(x.T, dL1)
	for (int i = 0; i<batch; i++){
		for (int j = 0; j<hiddensize; j++){
			dL1[hiddensize * i + j] = 0;
			for (int k=0; k<outputsize; k++){
				dL1[hiddensize * i + j] += dL2[outputsize * i + k] * W2[ outputsize * j  + k];
			}
			dL1[hiddensize * i + j] *= diffsigmoid(Y1[hiddensize * i + j]);
			b1[hiddensize * i + j] -= lr *dL1[hiddensize * i + j];
		}
	}
	for (int i = 0; i<inputsize; i++){
		for (int j = 0; j<hiddensize; j++){
			dW1[hiddensize * i + j] = 0;
			for (int k=0; k<batch; k++){
				dW1[hiddensize * i + j] += DATA[i + k * inputsize] * dL1[hiddensize * k + j];
			}
			W1[hiddensize * i + j] -= lr *dW1[hiddensize * i + j];
		}
	}
}

float diffsigmoid(float x){
	return sigmoid(x) *(1 - sigmoid(x));
}
float sigmoid(float x){
	return 1/(1+ exp(-x));
}
