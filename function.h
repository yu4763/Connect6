#ifndef FUNCTION_H
#define FUNCTION_H

#include <board.h>
#include <stone.h>
#include "eval.h"
#include "network.h"


int wheretoPut( board *b );
int checkEnd( stone *s[19][19], char state);

#endif
