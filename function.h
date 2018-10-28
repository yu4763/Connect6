#ifndef FUNCTION_H
#define FUNCTION_H


int checkmine(int s[19][19], char state, int& pos1, int& pos2, int p, int q);
int checkmineblank(int s[19][19], char state, int& pos1, int& pos2, int p, int q);
int checkopponent(int s[19][19], char state, int& pos1, int& pos2, int p, int q);
int checkopponentblank(int s[19][19], char state, int& pos1, int& pos2, int p, int q);

#endif