#include <function.h>
#include <stdio.h>

int wheretoPut( board *b ){

    int i, k;
    for (i=0; i<stoneNum; i++){
        for(k=0; k<stoneNum; k++){

            if(b->stones[i][k]->state == 0){
                return i*stoneNum +k;
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
int checkEnd( stone *s[19][19], char state){

    int i, k, j;
    int cnt = 0;


    /* check row */
    for(i=0; i<stoneNum; i++){

        j = -1;
        for(k=0; k<stoneNum-5; k++){


            if(k <= j){
                continue;
            }

            cnt = 1;
            if(s[i][k]->state==state){
                for(j=k+1; j<=k+6; j++){
                    if(s[i][j]->state != state){
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
