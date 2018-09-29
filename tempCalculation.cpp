#include <function.h>
#include <stdio.h>

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


/*
return value : reuslt
0 : not end
1 : end and win
2 : end and lose
*/
int checkEnd( stone *s[19][19], int color){

    int i, k, j;
    int cnt = 0;


    /* check row */
    for(i=0; i<boardNum-1; i++){

        j = -1;
        for(k=0; k<boardNum-6; k++){


            if(k <= j){
                continue;
            }

            cnt = 1;
            if(s[i][k]->exist==1 && s[i][k]->color == color){
                for(j=k+1; j<=k+6; j++){
                    if(s[i][j]->exist != 1 || s[i][j]->color != color){
                        break;
                    }
                    cnt++;
                }
            }

            if(cnt == 6){
                return 1;
            }
            else if (cnt > 6){
                return 2;
            }
        }
    }

    return 0;
    /*check column*/

}
