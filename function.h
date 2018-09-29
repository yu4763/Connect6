#ifndef FUNCTION_H
#define FUNCTION_H

#include <board.h>
#include <stone.h>

int wheretoPut( board *b );
int checkEnd( stone *s[19][19], int color);

#endif
