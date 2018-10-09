void getline(char* state, char* line, int action);
void updateUCT(float* UCT, char* state, int action, Network CalUCT, float *W1, float *b1, float *W2, float *b2, char color);
void initUCT(float* UCT, char* state, Network CalUCT, float *W1, float *b1, float *W2, float *b2);
void addUCT(float* DeepUCT, float* UCT);
void best(char* state, float* UCT, int best, int* indexes);
void secondbest(char* state, float* UCT, int best, int* indexes, int first);
