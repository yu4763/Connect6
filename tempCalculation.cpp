#include <function.h>
#include <stdio.h>
#include "monte_carlo_tree_search.h"
#include "monte_carlo_tree_search.cpp"

// ====================
// To get two positions
extern int best_pos1;
extern int best_pos2;

bool is_first = true;
// ====================

int wheretoPut( board *b ){

    int i, k;
    // ========================================
    // Arbitrary playing
    //
    // for (i=0; i<stoneNum; i++){
    //    for(k=0; k<stoneNum; k++){
    //
    //        if(b->stones[i][k]->state == 0){
    //            return i*stoneNum +k;
    //        }
    //    }
    // }
    // =======================================
    // Using monte carlo search
    if (is_first) {
      MonteCarloTreeSearch();
      is_first = false;
      return best_pos1;
    } else {
      is_first = true;
      return best_pos2;
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
                for(j=k+1; j<k+6; j++){
                    if(j>=stoneNum)
                        break;
                    if(s[i][j]->state != state){
                        break;
                    }
                    cnt++;
                }
            }

            if(cnt == 6){
                if (k != stoneNum-6){
                    if(s[i][k+6]->state == state){
                        return 2;
                    }
                }
                return 1;
            }
        }
    }

    /*check column*/
    for(i=0; i<stoneNum; i++){

        j = -1;
        for(k=0; k<stoneNum-5; k++){

            if(k <= j){
                continue;
            }
            cnt = 1;
            if(s[k][i]->state==state){
                for(j=k+1; j<k+6; j++){
                    if(j>=stoneNum)
                        break;
                    if(s[j][i]->state != state){
                        break;
                    }
                    cnt++;
                }
            }

            if(cnt == 6){
                if (k != stoneNum-6){
                    if(s[k+6][i]->state == state){
                        return 2;
                    }
                }
                return 1;
            }
        }
    }

    /*check diagonal*/
    for(i=0; i<stoneNum-5; i++){

        j = -1;
        for(k=0; k<stoneNum-5; k++){

            if(k <= j){
                continue;
            }
            cnt = 1;
            if(s[i][k]->state==state){
                for(j=1; j<6; j++){
                    if(i+j >= stoneNum || k+j >= stoneNum)
                        break;
                    if(s[i+j][k+j]->state != state){
                        break;
                    }
                    cnt++;
                }
            }

            if(cnt == 6){
                if (i != stoneNum-6 && k!= stoneNum){
                    if(s[i+6][k+6]->state == state){
                        return 2;
                    }
                }
                return 1;
            }
        }
    }

    /*check diagonal*/
    for(i=0; i<stoneNum-5; i++){

        j = -1;
        for(k=5; k<stoneNum; k++){

            if(k <= j){
                continue;
            }
            cnt = 1;
            if(s[i][k]->state==state){
                for(j=1; j<6; j++){
                    if(i+j >= stoneNum || k-j>=stoneNum)
                        break;
                    if(s[i+j][k-j]->state != state){
                        break;
                    }
                    cnt++;
                }
            }

            if(cnt == 6){
                if (i != stoneNum-6 && k!= 5){
                    if(s[i+6][k-6]->state == state){
                        return 2;
                    }
                }
                return 1;
            }
        }
    }


    return 0;

}
