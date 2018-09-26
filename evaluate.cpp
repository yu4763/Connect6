#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define LENGTH 19
float sigmoid(float x);
float diffsigmoid(float x);
void getline(int* state, int* line, int num, int action);
int argmax(float *UCT, int num);
int checkline(float *address);

class Network{
	public:
		void getoutput(int* DATA, float* W1, float* b1, float* W2, float* b2, int batch, int inputsize, int hiddensize, int outputsize, float* OUTPUT){
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
		
		void backpropagation(int* DATA, float* W1, float* b1, float* W2, float* b2, int batch, int inputsize, int hiddensize, int outputsize, float* LABEL, float lr){
			float Y1[batch * hiddensize];
			float L1[batch * hiddensize];
			float L2[batch * outputsize];
			float dL1[batch * hiddensize];
			float dL2[batch * outputsize];
			float dW1[inputsize * hiddensize];
			float dW2[hiddensize * outputsize];
			float Cost[batch * outputsize];
			float temp;
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
		
};
int main(void){
	int inputsize = 2;
	int hiddensize = 4;
	int outputsize = 1;
	int batch = 4;
	
	int state[LENGTH * LENGTH] = {0};
	int lines[48];
	int X[4*2]={0,0,0,1,1,0,1,1};
	float Y[4]= {0,1,1,0};
	float OUTPUT[4];
	
	//getline(state, lines, LENGTH, 13);
	
	float W1[inputsize * hiddensize] = {0};
	float b1[batch * hiddensize] = {0};
	float W2[hiddensize * outputsize] = {0};
	float b2[batch * outputsize] = {0};
	
	Network XNOR;
	
	for (int i=0; i<10000; i++){
		XNOR.backpropagation(X, W1, b1, W2, b2, batch, inputsize, hiddensize, outputsize, Y, 0.001);
		XNOR.getoutput(X, W1, b1, W2, b2, batch, inputsize, hiddensize, outputsize, OUTPUT);
			if (i % 1000 == 0){
				printf("%f %f %f %f\n",OUTPUT[0],OUTPUT[1],OUTPUT[2],OUTPUT[3]);	
			}
	}
	

	
	return 0;
}
float diffsigmoid(float x){
	return sigmoid(x) *(1 - sigmoid(x));
}
float sigmoid(float x){
	return 1/(1+ exp(-x));
}
int checkline(int *address, int value){
	int count = 0;
	int temp = 0;
	for (int i=0; i<6; i++){
		if (address[5 + i] == value){
			temp = 0;
			count +=1;
		}
		else if (address[5 + i] == 0){
			temp +=1;			
		}
		else{
			break;
		}
		
		if (temp ==2){
			break;
		}
	}
	temp = 0;
	for (int i=0; i<6; i++){
		if (address[5 - i] == value){
			temp = 0;
			count +=1;
		}
		else if (address[5 - i] == 0){
			temp +=1;			
		}
		else{
			break;
		}
		
		if (temp ==2){
			break;
		}
	}
	return count;
}
void getline(int* state, int* line, int num, int action){
	int interval, index;
	for (int i = 0; i < 4; i++){
		if (i ==0){
			interval = 1;
		}
		else{
			interval = num-2 + i;
		}
		for (int j=0; j<11; j++){
			index = action + (j - 5) * interval;
			if ((index < 0) || (index > (num*num -1))){
				line[13 * i +j] = 2;
			}
			else{
				line[13 * i +j] = state[index];
			}
		}
		line[13 * i + 11] = checkline(&line[12 * i], 1);
		line[13 * i + 12] = checkline(&line[12 * i], -1);
	}
}
int argmax(float *UCT, int num){
	int max = 0;
	for (int i=0; i<num; i++){
		if (UCT[max] <= UCT[i]){
			max = i;
		}
	}
	return max;
}
	
