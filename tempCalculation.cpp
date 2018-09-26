#include <function.h>

int wheretoPut( board *b ){

    int i, k;
    for (i=0; i<boardNum-1; i++){
        for(k=0; k<boardNum-1; k++){

            if(b->stones[i][k]->exist == 0){
                return i*18 +k;
            }
        }
    }
}
