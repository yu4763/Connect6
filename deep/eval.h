#ifndef __EVAL_H__
#define __EVAL_H__

#include "network.h"

void getline(char* state, char* line, int action);
void updateUCT(float* UCT, char* state, int action, Network CalUCT, float *W1, float *b1, float *W2, float *b2, char color);
void initUCT(float* UCT, char* state, Network CalUCT, float *W1, float *b1, float *W2, float *b2);
void addUCT(float* DeepUCT, float* UCT);
void best(char* state, float* UCT, int best, int* indexes);
void secondbest(char* state, float* UCT, int best, int* indexes, int first);

// Move Getfunctions to here
void GetBestIndex(char* state, int* indexes, float* W1, float* b1, float* W2, float* b2, Network CalDeepUct, int number);
void GetBestPair(char* state, int* index1, int* index2, float* W1, float* b1, float* W2, float* b2, Network CalDeepUCT, int first, int second, char color);

#endif
