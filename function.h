#ifndef FUNCTION_H
#define FUNCTION_H

#include <board.h>
#include <stone.h>

int wheretoPut( board *b );
int checkEnd( stone *s[19][19], char state);
int checkmine(stone *s[19][19], char state, int& pos1, int& pos2, int p, int q );
int checkmineblank(stone *s[19][19], char state, int& pos1, int& pos2, int p, int q);
int checkopponent(stone *s[19][19], char state, int& pos1, int& pos2, int p, int q );
int checkopponentblank(stone *s[19][19], char state, int& pos1, int& pos2, int p, int q );
#endif
