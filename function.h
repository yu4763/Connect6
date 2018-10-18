#ifndef FUNCTION_H
#define FUNCTION_H

#include <board.h>
#include <stone.h>

int wheretoPut( board *b );
int checkEnd( stone *s[19][19], char state);
int check4( stone *s[19][19], char state, int& pos1, int& pos2);
int checkmine(stone *s[19][19], char state, int& pos1, int& pos2 );
int checkmineblank(stone *s[19][19], char state, int& pos1, int& pos2);

#endif
