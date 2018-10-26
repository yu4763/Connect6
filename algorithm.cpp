#include <function.h>
#include <stdio.h>
#include "monte_carlo_tree_search.h"


int check4( stone *s[19][19], char state, int& pos1, int& pos2){

    int i, k, j;
    int cnt = 0;
    int result;
    result = checkmine(s, state, pos1, pos2);

    return result;
}


//return 1: pos1 exist
//return 2: pos1 pos2 exist
//return 0: deosn't exist
int checkmine(stone *s[19][19], char state, int& pos1, int& pos2 ){

    int empty = 0;
    int emindex = -1;
    bool left = false;
    int cnt = 0;
    int i, j, k;

    /* check row */
    for(i=0; i<stoneNum; i++){

        j = -1;
        empty = 0;
        emindex = -1;
        left = false;
        cnt = 0;

        for(k=0; k<stoneNum-3; k++){

            if(k < j){
                continue;
            }

            if(s[i][k]->state == 0){

                empty = 1;
                emindex = k;

                if(cnt>0){
                    left = true;
                }
                else{
                    left = false;
                }

                for(j=k+1; ; j++){
                    if(j==stoneNum-1)
                        break;
                    if(s[i][j]->state != 0){
                        break;
                    }
                    if(empty < 2){
                        empty++;
                    }
                    emindex = j;
                }
            }

            else if(s[i][k]->state==state){
                cnt = 1;
                for(j=k+1; j<k+6; j++){
                    if(j>=stoneNum)
                        break;
                    if(s[i][j]->state != state){
                        break;
                    }
                    cnt++;
                }

                if(cnt == 5){
                    if (empty > 1 || (empty==1 && !left) ){
                        pos1 = i*stoneNum +emindex;
                        return 1;
                    }
                    else if( !(j==stoneNum) && s[i][j] ->state== 0){
                        if( j+1 == stoneNum  || s[i][j+1]-> state !=state){
                            pos1 = i*stoneNum+j;
                            return 1;
                        }
                    }
                }

                else if(cnt == 4){
                    printf("%d\n", empty);
                    printf("%d\n", emindex);
                    if (empty > 2 || (empty==2 && !left) ){
                        pos1 = i*stoneNum +emindex;
                        pos2 = i*stoneNum + (emindex-1);
                        return 2;
                    }
                    else if(empty == 1 && !left){
                        if( (j<=stoneNum-1) && s[i][j] ->state== 0){
                            if( j+1 == stoneNum  || s[i][j+1]-> state !=state){
                                pos1 = i*stoneNum+emindex;
                                pos2 = i*stoneNum+j;
                                return 2;
                            }
                        }
                    }
                    else if( (j<=stoneNum-2) && s[i][j] ->state== 0 && s[i][j+1]->state==0){
                        if( j+2 == stoneNum  || s[i][j+2]-> state !=state){
                            pos1 = i*stoneNum+j;
                            pos2 = i*stoneNum+j+1;
                            return 2;
                        }
                    }
                }
            }

            else{
                empty = 0;
                cnt = 0;
            }

        }
    }

    // /*check column*/
    for(i=0; i<stoneNum; i++){

        j = -1;
        empty = 0;
        emindex = -1;
        left = false;
        cnt = 0;

        for(k=0; k<stoneNum-3; k++){

            if(k < j){
                continue;
            }

            if(s[k][i]->state == 0){

                empty = 1;
                emindex = k;

                if(cnt>0){
                    left = true;
                }
                else{
                    left = false;
                }

                for(j=k+1; ; j++){
                    if(j==stoneNum-1)
                        break;
                    if(s[j][i]->state != 0){
                        break;
                    }
                    if(empty < 2){
                        empty++;
                    }
                    emindex = j;
                }
            }

            else if(s[k][i]->state==state){
                cnt = 1;
                for(j=k+1; j<k+6; j++){
                    if(j>=stoneNum)
                        break;
                    if(s[j][i]->state != state){
                        break;
                    }
                    cnt++;
                }

                if(cnt == 5){
                    if (empty > 1 || (empty==1 && !left) ){
                        pos1 = emindex*stoneNum +i;
                        return 1;
                    }
                    else if( !(j==stoneNum) && s[j][i] ->state== 0){
                        if( j+1 == stoneNum  || s[j+1][i]-> state !=state){
                            pos1 = j*stoneNum+i;
                            return 1;
                        }
                    }
                }

                else if(cnt == 4){
                    printf("%d\n", empty);
                    printf("%d\n", emindex);
                    if (empty > 2 || (empty==2 && !left) ){
                        pos1 = emindex*stoneNum + i;
                        pos2 = (emindex-1)*stoneNum + i;
                        return 2;
                    }
                    else if(empty == 1 && !left){
                        if( (j<=stoneNum-1) && s[j][i] ->state== 0){
                            if( j+1 == stoneNum  || s[j+1][i]-> state !=state){
                                pos1 = emindex*stoneNum+i;
                                pos2 = j*stoneNum+i;
                                return 2;
                            }
                        }
                    }
                    else if( (j<=stoneNum-2) && s[j][i] ->state== 0 && s[j+1][i]->state==0){
                        if( j+2 == stoneNum  || s[j+2][i]-> state !=state){
                            pos1 = j*stoneNum+i;
                            pos2 = (j+1)*stoneNum+i;
                            return 2;
                        }
                    }
                }
            }

            else{
                empty = 0;
                cnt = 0;
            }

        }
    }

    /* check diagonal*/

    for(i=0, k=0; k<stoneNum-5; k++){

        empty = 0;
        emindex = -1;
        left = false;
        cnt = 0;
        int t = -1;

        for(j=0; ; j++){

            if(i+j>=stoneNum || k+j>=stoneNum)
                break;

            if(j < t)
                continue;

            if(s[i+j][k+j]->state==0){

                empty = 1;
                emindex = j;

                if(cnt>0){
                    left = true;
                }
                else{
                    left = false;
                }

                for( t=j+1; ; t++){
                    if(i+t >= stoneNum-1 || k+t >= stoneNum-1)
                        break;
                    if(s[i+t][k+t]->state != 0){
                        break;
                    }
                    if(empty < 2){
                        empty++;
                    }
                    emindex = t;
                }

            }

            else if(s[i+j][k+j]->state==state){
                cnt = 1;
                for(t=j+1; t<j+6; t++){
                    if(i+t>=stoneNum || k+t>=stoneNum)
                        break;
                    if(s[i+t][k+t]->state != state){
                        break;
                    }
                    cnt++;
                }

                if(cnt == 5){
                    if (empty > 1 || (empty==1 && !left) ){
                        pos1 = (i+emindex)*stoneNum + k +emindex;
                        return 1;
                    }
                    else if( (i+t < stoneNum) && (k+t < stoneNum) && s[i+t][k+t] ->state== 0){
                        if( i+t+1 == stoneNum || k+t+1 == stoneNum || s[i+t+1][k+t+1]-> state !=state){
                            pos1 = (i+t)*stoneNum+(k+t);
                            return 1;
                        }
                    }
                }

                else if(cnt == 4){
                    printf("%d\n", empty);
                    printf("%d\n", emindex);
                    if (empty > 2 || (empty==2 && !left) ){
                        pos1 = (i+emindex)*stoneNum + k+emindex;
                        pos2 = (i+emindex-1)*stoneNum + k+emindex -1;
                        return 2;
                    }
                    else if(empty == 1 && !left){
                        if( (i+t < stoneNum) && (k+t < stoneNum) && s[i+t][k+t] ->state== 0){
                            if(  i+t+1 == stoneNum || k+t+1 == stoneNum || s[i+t+1][k+t+1]-> state !=state ){
                                pos1 = (i+emindex)*stoneNum+k+emindex;
                                pos2 = (i+t)*stoneNum+(k+t);
                                return 2;
                            }
                        }
                    }
                    else if( (i+t <= stoneNum-2) && (k+t <= stoneNum-2) && s[i+t][k+t] ->state== 0 && s[i+t+1][k+t+1]->state==0){
                        if( i+t+2 == stoneNum || k+t+2==stoneNum  || s[i+t+2][k+t+2]-> state !=state){
                            pos1 = (i+t)*stoneNum+k+t;
                            pos2 = (i+t+1)*stoneNum+k+t+1;
                            return 2;
                        }
                    }
                }
            }

            else{
                empty = 0;
                cnt = 0;
            }


        }


    }

    for(i=0, k=0; i<stoneNum-5; i++){

        empty = 0;
        emindex = -1;
        left = false;
        cnt = 0;
        int t = -1;

        for(j=0; ; j++){

            if(i+j>=stoneNum || k+j>=stoneNum)
                break;

            if(j < t)
                continue;

            if(s[i+j][k+j]->state==0){

                empty = 1;
                emindex = j;

                if(cnt>0){
                    left = true;
                }
                else{
                    left = false;
                }

                for( t=j+1; ; t++){
                    if(i+t >= stoneNum-1 || k+t >= stoneNum-1)
                        break;
                    if(s[i+t][k+t]->state != 0){
                        break;
                    }
                    if(empty < 2){
                        empty++;
                    }
                    emindex = t;
                }

            }

            else if(s[i+j][k+j]->state==state){
                cnt = 1;
                for(t=j+1; t<j+6; t++){
                    if(i+t>=stoneNum || k+t>=stoneNum)
                        break;
                    if(s[i+t][k+t]->state != state){
                        break;
                    }
                    cnt++;
                }

                if(cnt == 5){
                    if (empty > 1 || (empty==1 && !left) ){
                        pos1 = (i+emindex)*stoneNum + k +emindex;
                        return 1;
                    }
                    else if( (i+t < stoneNum) && (k+t < stoneNum) && s[i+t][k+t] ->state== 0){
                        if( i+t+1 == stoneNum || k+t+1 == stoneNum || s[i+t+1][k+t+1]-> state !=state){
                            pos1 = (i+t)*stoneNum+(k+t);
                            return 1;
                        }
                    }
                }

                else if(cnt == 4){
                    printf("%d\n", empty);
                    printf("%d\n", emindex);
                    if (empty > 2 || (empty==2 && !left) ){
                        pos1 = (i+emindex)*stoneNum + k+emindex;
                        pos2 = (i+emindex-1)*stoneNum + k+emindex -1;
                        return 2;
                    }
                    else if(empty == 1 && !left){
                        if( (i+t < stoneNum) && (k+t < stoneNum) && s[i+t][k+t] ->state== 0){
                            if(  i+t+1 == stoneNum || k+t+1 == stoneNum || s[i+t+1][k+t+1]-> state !=state ){
                                pos1 = (i+emindex)*stoneNum+k+emindex;
                                pos2 = (i+t)*stoneNum+(k+t);
                                return 2;
                            }
                        }
                    }
                    else if( (i+t <= stoneNum-2) && (k+t <= stoneNum-2) && s[i+t][k+t] ->state== 0 && s[i+t+1][k+t+1]->state==0){
                        if( i+t+2 == stoneNum || k+t+2==stoneNum  || s[i+t+2][k+t+2]-> state !=state){
                            pos1 = (i+t)*stoneNum+k+t;
                            pos2 = (i+t+1)*stoneNum+k+t+1;
                            return 2;
                        }
                    }
                }
            }

            else{
                empty = 0;
                cnt = 0;
            }


        }


    }


    /* check diagonal*/

    for(i=0, k=6; k<stoneNum; k++){

        empty = 0;
        emindex = -1;
        left = false;
        cnt = 0;
        int t = -1;

        for(j=0; ; j++){

            if(i+j>=stoneNum || k-j < 0)
                break;

            if(j < t)
                continue;

            if(s[i+j][k-j]->state==0){

                empty = 1;
                emindex = j;

                if(cnt>0){
                    left = true;
                }
                else{
                    left = false;
                }

                for( t=j+1; ; t++){
                    if(i+t >= stoneNum-1 || k-t <= 0)
                        break;
                    if(s[i+t][k-t]->state != 0){
                        break;
                    }
                    if(empty < 2){
                        empty++;
                    }
                    emindex = t;
                }

            }

            else if(s[i+j][k-j]->state==state){
                cnt = 1;
                for(t=j+1; t<j+6; t++){
                    if(i+t>=stoneNum || k-t<=0)
                        break;
                    if(s[i+t][k-t]->state != state){
                        break;
                    }
                    cnt++;
                }


                if(cnt == 5){
                    if (empty > 1 || (empty==1 && !left) ){
                        pos1 = (i+emindex)*stoneNum + k -emindex;
                        return 1;
                    }
                    else if( (i+t < stoneNum) && (k-t >= 0) && s[i+t][k-t] ->state== 0){
                        if( i+t+1 == stoneNum || k-t-1 < 0 || s[i+t+1][k-t-1]-> state !=state){
                            pos1 = (i+t)*stoneNum +(k-t);
                            return 1;
                        }
                    }
                }

                else if(cnt == 4){
                    printf("%d\n", empty);
                    printf("%d\n", emindex);
                    if (empty > 2 || (empty==2 && !left) ){
                        pos1 = (i+emindex)*stoneNum + k - emindex;
                        pos2 = (i+emindex-1)*stoneNum + k - emindex + 1;
                        return 2;
                    }
                    else if(empty == 1 && !left){
                        if( (i+t < stoneNum) && (k-t >= 0) && s[i+t][k-t] ->state== 0){
                            if(  i+t+1 == stoneNum || k-t-1 == stoneNum || s[i+t+1][k-t-1]-> state !=state ){
                                pos1 = (i+emindex)*stoneNum+k-emindex;
                                pos2 = (i+t)*stoneNum+(k-t);
                                return 2;
                            }
                        }
                    }

                    ////check here!!
                    else if( (i+t <= stoneNum-2) && (k-t >= 1 ) && s[i+t][k-t] ->state== 0 && s[i+t+1][k-t-1]->state==0){
                        if( i+t+2 == stoneNum || k-t-2 < 0 || s[i+t+2][k-t-2]-> state !=state){
                            pos1 = (i+t)*stoneNum+k-t;
                            pos2 = (i+t+1)*stoneNum+k-t-1;
                            return 2;
                        }
                    }
                }
            }

            else{
                empty = 0;
                cnt = 0;
            }


        }


    }

    for(k=stoneNum-1, i=0; i<stoneNum-5; i++){

        empty = 0;
        emindex = -1;
        left = false;
        cnt = 0;
        int t = -1;

        for(j=0; ; j++){

            if(i+j>=stoneNum || k-j < 0)
                break;

            if(j < t)
                continue;

            if(s[i+j][k-j]->state==0){

                empty = 1;
                emindex = j;

                if(cnt>0){
                    left = true;
                }
                else{
                    left = false;
                }

                for( t=j+1; ; t++){
                    if(i+t >= stoneNum-1 || k-t <= 0)
                        break;
                    if(s[i+t][k-t]->state != 0){
                        break;
                    }
                    if(empty < 2){
                        empty++;
                    }
                    emindex = t;
                }

            }

            else if(s[i+j][k-j]->state==state){
                cnt = 1;
                for(t=j+1; t<j+6; t++){
                    if(i+t>=stoneNum || k-t<=0)
                        break;
                    if(s[i+t][k-t]->state != state){
                        break;
                    }
                    cnt++;
                }


                if(cnt == 5){
                    if (empty > 1 || (empty==1 && !left) ){
                        pos1 = (i+emindex)*stoneNum + k -emindex;
                        return 1;
                    }
                    else if( (i+t < stoneNum) && (k-t >= 0) && s[i+t][k-t] ->state== 0){
                        if( i+t+1 == stoneNum || k-t-1 < 0 || s[i+t+1][k-t-1]-> state !=state){
                            pos1 = (i+t)*stoneNum +(k-t);
                            return 1;
                        }
                    }
                }

                else if(cnt == 4){
                    printf("%d\n", empty);
                    printf("%d\n", emindex);
                    if (empty > 2 || (empty==2 && !left) ){
                        pos1 = (i+emindex)*stoneNum + k - emindex;
                        pos2 = (i+emindex-1)*stoneNum + k - emindex + 1;
                        return 2;
                    }
                    else if(empty == 1 && !left){
                        if( (i+t < stoneNum) && (k-t >= 0) && s[i+t][k-t] ->state== 0){
                            if(  i+t+1 == stoneNum || k-t-1 == stoneNum || s[i+t+1][k-t-1]-> state !=state ){
                                pos1 = (i+emindex)*stoneNum+k-emindex;
                                pos2 = (i+t)*stoneNum+(k-t);
                                return 2;
                            }
                        }
                    }

                    ////check here!!
                    else if( (i+t <= stoneNum-2) && (k-t >= 1 ) && s[i+t][k-t] ->state== 0 && s[i+t+1][k-t-1]->state==0){
                        if( i+t+2 == stoneNum || k-t-2 < 0 || s[i+t+2][k-t-2]-> state !=state){
                            pos1 = (i+t)*stoneNum+k-t;
                            pos2 = (i+t+1)*stoneNum+k-t-1;
                            return 2;
                        }
                    }
                }
            }

            else{
                empty = 0;
                cnt = 0;
            }


        }


    }


    return 0;

}



int checkmineblank(stone *s[19][19], char state, int& pos1, int& pos2, int p, int q){


    int cnt = 0;
    int empty = 0;
    int i, k;

    /*check row*/
    for(k=0; k<stoneNum; k++){

        if(s[p][k]->state == state){
            cnt++;
        }
        else
            cnt = 0;

        if(cnt == 1){

            if(k+5>=stoneNum)
                break;

            if(s[p][k+1]->state == 0){
                if(k+6>=stoneNum || s[p][k+6]->state != state){
                    if(s[p][k+2]->state==0 && s[p][k+3]->state==state && s[p][k+4]->state == state && s[p][k+5]->state==state){
                        pos1 = p*stoneNum + (k+1);
                        pos2 = p*stoneNum + (k+2);
                        return 2;
                    }
                    else if(s[p][k+2]->state==state && s[p][k+3]->state==state && s[p][k+4]->state == state && s[p][k+5]->state== 0){
                        pos1 = p*stoneNum + (k+1);
                        pos2 = p*stoneNum + (k+5);
                        return 2;
                    }
                    else if(s[p][k+2]->state==state && s[p][k+3]->state==state && s[p][k+4]->state == state && s[p][k+5]->state== state){
                        pos1 = p*stoneNum + (k+1);
                        return 1;
                    }
                }
            }

        }

        if(cnt==2 ){

            if(k+4 >= stoneNum)
                break;

            if(s[p][k+1]->state==0){

                if(k+5>=stoneNum || s[p][k+5]->state != state){
                    if(s[p][k+2]->state==0 && s[p][k+3]->state == state && s[p][k+4]->state == state){
                        pos1 = p*stoneNum + (k+1);
                        pos2 = p*stoneNum + (k+2);
                        return 2;
                    }
                    else if( s[p][k+2]->state == state && s[p][k+3]->state == state && s[p][k+4]->state == 0){
                        pos1 = p*stoneNum + (k+1);
                        pos2 = p*stoneNum + (k+4);
                        return 2;
                    }
                    else if( s[p][k+2]->state == state && s[p][k+3]->state == state && s[p][k+4]->state == state){
                        pos1 = p*stoneNum + (k+1);
                        return 1;
                    }

                }

            }

        }


        if(cnt == 3){

            if(k+3 >= stoneNum)
                break;

            else{

                if( s[p][k+1]->state == 0){

                    if(s[p][k+2]->state == 0 && s[p][k+3]->state == state){

                        if( k+4 >= stoneNum || s[p][k+4]->state != state){
                            pos1 = p*stoneNum + (k+1);
                            pos2 = p*stoneNum + (k+2);
                            return 2;
                        }
                    }
                    else if(s[p][k+2]->state == state){

                        if( k+4 >= stoneNum || s[p][k+4]->state != state){
                            if(s[p][k+3]->state == state){
                                pos1 = p*stoneNum +(k+1);
                                return 1;
                            }
                            else if(s[p][k+3]->state == 0){

                                pos1 = p*stoneNum + (k+1);
                                pos2 = p*stoneNum + (k+3);
                                return 2;

                            }
                        }

                    }

                }
                else if(s[p][k+1]->state == state){

                    if(s[p][k+2]->state==0 && s[p][k+3]->state == state){
                        if( k+4 >= stoneNum || s[p][k+4]->state != state){
                            pos1 = p*stoneNum + (k+2);
                            return 1;
                        }
                    }

                }

            }

        }
    }

    for(k=0; k<stoneNum; k++){

        if(s[p][k]->state == 0)
            empty++;
        else if(s[p][k]->state == state){
            if(empty>1 || (empty==1 && k-2 < 0)){
                if(k+4 >= stoneNum)
                    break;

                if(k+5>=stoneNum || s[p][k+5]->state != state){
                    if(s[p][k+1]->state==state && s[p][k+2]->state == 0 && s[p][k+3]->state==state && s[p][k+4]->state==state){
                        pos1 = p*stoneNum + (k-1);
                        pos2 = p*stoneNum + (k+2);
                        return 2;
                    }
                    else if(s[p][k+1]->state==state && s[p][k+2]->state == state && s[p][k+3]->state==0 && s[p][k+4]->state==state){
                        pos1 = p*stoneNum + (k-1);
                        pos2 = p*stoneNum + (k+3);
                        return 2;
                    }

                    else if(s[p][k+1]->state==0 && s[p][k+2]->state == state && s[p][k+3]->state==state && s[p][k+4]->state==state){
                        pos1 = p*stoneNum + (k-1);
                        pos2 = p*stoneNum + (k+1);
                        return 2;
                    }

                }

            }

            empty = 0;
        }
        else
            empty = 0;

    }

    /* check column */

    for(i=0; i<stoneNum; i++){

        if(s[i][q]->state == state){
            cnt++;
        }
        else
            cnt = 0;

        if(cnt == 1){

            if(i+5>=stoneNum)
                break;

            if(s[i+1][q]->state == 0){
                if(i+6>=stoneNum || s[i+6][q]->state != state){
                    if(s[i+2][q]->state==0 && s[i+3][q]->state==state && s[i+4][q]->state == state && s[i+5][q]->state==state){
                        pos1 = (i+1)*stoneNum + q;
                        pos2 = (i+2)*stoneNum + q;
                        return 2;
                    }
                    else if(s[i+2][q]->state==state && s[i+3][q]->state==state && s[i+4][q]->state == state && s[i+5][q]->state== 0){
                        pos1 = (i+1)*stoneNum + q;
                        pos2 = (i+5)*stoneNum + q;
                        return 2;
                    }
                    else if(s[i+2][q]->state==state && s[i+3][q]->state==state && s[i+4][q]->state == state && s[i+5][q]->state== state){
                        pos1 = (i+1)*stoneNum + q;
                        return 1;
                    }
                }
            }

        }

        if(cnt==2 ){

            if(i+4 >= stoneNum)
                break;

            if(s[i+1][q]->state==0){

                if(i+5>=stoneNum || s[i+5][q]->state != state){
                    if(s[i+2][q]->state==0 && s[i+3][q]->state == state && s[i+4][q]->state == state){
                        pos1 = (i+1)*stoneNum + q;
                        pos2 = (i+2)*stoneNum + q;
                        return 2;
                    }
                    else if( s[i+2][q]->state == state && s[i+3][q]->state == state && s[i+4][q]->state == 0){
                        pos1 = (i+1)*stoneNum + q;
                        pos2 = (i+4)*stoneNum + q;
                        return 2;
                    }
                    else if( s[i+2][q]->state == state && s[i+3][q]->state == state && s[i+4][q]->state == state){
                        pos1 = (i+1)*stoneNum + q;
                        return 1;
                    }

                }

            }

        }


        if(cnt == 3){

            if(i+3 >= stoneNum)
                break;

            else{

                if( s[i+1][q]->state == 0){

                    if(s[i+2][q]->state == 0 && s[i+3][q]->state == state){

                        if( i+4 >= stoneNum || s[i+4][q]->state != state){
                            pos1 = (i+1)*stoneNum + q;
                            pos2 = (i+2)*stoneNum + q;
                            return 2;
                        }
                    }
                    else if(s[i+2][q]->state == state){

                        if( i+4 >= stoneNum || s[i+4][q]->state != state){
                            if(s[i+3][q]->state == state){
                                pos1 = (i+1)*stoneNum + q;
                                return 1;
                            }
                            else if(s[i+3][q]->state == 0){

                                pos1 = (i+1)*stoneNum + q;
                                pos2 = (i+3)*stoneNum + q;
                                return 2;

                            }
                        }

                    }

                }
                else if(s[i+1][q]->state == state){

                    if(s[i+2][q]->state==0 && s[i+3][q]->state == state){
                        if( i+4 >= stoneNum || s[i+4][q]->state != state){
                            pos1 = (i+2)*stoneNum + q;
                            return 1;
                        }
                    }

                }

            }

        }
    }

    for(i=0; i<stoneNum; i++){

        if(s[i][q]->state == 0)
            empty++;
        else if(s[i][q]->state == state){
            if(empty>1 || (empty==1 && i-2 < 0)){
                if(i+4 >= stoneNum)
                    break;

                if(i+5>=stoneNum || s[i+5][q]->state != state){
                    if(s[i+1][q]->state==state && s[i+2][q]->state == 0 && s[i+3][q]->state==state && s[i+4][q]->state==state){
                        pos1 = (i-1)*stoneNum + q;
                        pos2 = (i+2)*stoneNum + q;
                        return 2;
                    }
                    else if(s[i+1][q]->state==state && s[i+2][q]->state == state && s[i+3][q]->state==0 && s[i+4][q]->state==state){
                        pos1 = (i-1)*stoneNum + q;
                        pos2 = (i+3)*stoneNum + q;
                        return 2;
                    }

                    else if(s[i+1][q]->state==0 && s[i+2][q]->state == state && s[i+3][q]->state==state && s[i+4][q]->state==state){
                        pos1 = (i-1)*stoneNum + q;
                        pos2 = (i+1)*stoneNum + q;
                        return 2;
                    }

                }

            }

            empty = 0;
        }
        else
            empty = 0;

    }

    /* check diagonal */

    int j;
    int max, min;

    if(p<=q){
        min = p;
        max = q;
    }
    else{
        min = q;
        max = p;
    }

    for(j=-min; j<stoneNum-max; j++){

        if(s[p+j][q+j]->state == state){
            cnt++;
        }
        else
            cnt = 0;

        if(cnt == 1){

            if(j+5>=stoneNum-max)
                break;

            if(s[p+j+1][q+j+1]->state == 0){
                if(j+6>=stoneNum-max || s[p+j+6][q+j+6]->state != state){
                    if(s[p+j+2][q+j+2]->state==0 && s[p+j+3][q+j+3]->state==state && s[p+j+4][q+j+4]->state == state && s[p+j+5][q+j+5]->state==state){
                        pos1 = (p+j+1)*stoneNum + q+j+1;
                        pos2 = (p+j+2)*stoneNum + q+j+2;
                        return 2;
                    }
                    else if(s[p+j+2][q+j+2]->state==state && s[p+j+3][q+j+3]->state==state && s[p+j+4][q+j+4]->state == state && s[p+j+5][q+j+5]->state== 0){
                        pos1 = (p+j+1)*stoneNum + q+j+1;
                        pos2 = (p+j+5)*stoneNum + q+j+5;
                        return 2;
                    }
                    else if(s[p+j+2][q+j+2]->state==state && s[p+j+3][q+j+3]->state==state && s[p+j+4][q+j+4]->state == state && s[p+j+5][q+j+5]->state== state){
                        pos1 = (p+j+1)*stoneNum + q+j+1;
                        return 1;
                    }
                }
            }

        }

        if(cnt==2 ){
            if(j+4 >= stoneNum-max)
                break;

            if(s[p+j+1][q+j+1]->state==0){

                if(j+5>=stoneNum-max || s[p+j+5][q+j+5]->state != state){
                    if(s[p+j+2][q+j+2]->state==0 && s[p+j+3][q+j+3]->state == state && s[p+j+4][q+j+4]->state == state){
                        pos1 = (p+j+1)*stoneNum + q+j+1;
                        pos2 = (p+j+2)*stoneNum + q+j+2;
                        return 2;
                    }
                    else if( s[p+j+2][q+j+2]->state == state && s[p+j+3][q+j+3]->state == state && s[p+j+4][q+j+4]->state == 0){
                        pos1 = (p+j+1)*stoneNum + q+j+1;
                        pos2 = (p+j+4)*stoneNum + q+j+4;
                        return 2;
                    }
                    else if( s[p+j+2][q+j+2]->state == state && s[p+j+3][q+j+3]->state == state && s[p+j+4][q+j+4]->state == state){
                        pos1 = (p+j+1)*stoneNum + q+j+1;
                        return 1;
                    }

                }

            }

        }


        if(cnt == 3){

            if(j+3 >= stoneNum-max)
                break;

            else{

                if( s[p+j+1][q+j+1]->state == 0){

                    if(s[p+j+2][q+j+2]->state == 0 && s[p+j+3][q+j+3]->state == state){

                        if( j+4 >= stoneNum-max || s[p+j+4][q+j+4]->state != state){
                            pos1 = (p+j+1)*stoneNum + q+j+1;
                            pos2 = (p+j+2)*stoneNum + q+j+2;
                            return 2;
                        }
                    }
                    else if(s[p+j+2][q+j+2]->state == state){

                        if( j+4 >= stoneNum-max || s[p+j+4][q+j+4]->state != state){
                            if(s[p+j+3][q+j+3]->state == state){
                                pos1 = (p+j+1)*stoneNum + q+j+1;
                                return 1;
                            }
                            else if(s[p+j+3][q+j+3]->state == 0){

                                pos1 = (p+j+1)*stoneNum + q+j+1;
                                pos2 = (p+j+3)*stoneNum + q+j+3;
                                return 2;

                            }
                        }

                    }

                }
                else if(s[p+j+1][q+j+1]->state == state){

                    if(s[p+j+2][q+j+2]->state==0 && s[p+j+3][q+j+3]->state == state){
                        if( j+4 >= stoneNum-max || s[p+j+4][q+j+4]->state != state){
                            pos1 = (p+j+2)*stoneNum + q+j+2;
                            return 1;
                        }
                    }

                }

            }

        }
    }

    for(j=-min; j<stoneNum-max; j++){

        if(s[p+j][q+j]->state == 0)
            empty++;
        else if(s[p+j][q+j]->state == state){
            if(empty>1 || (empty==1 && i-2 < 0)){
                if(j+4 >= stoneNum-max)
                    break;

                if( j+5>=stoneNum-max || s[p+j+5][q+j+5]->state != state){
                    if(s[p+j+1][q+j+1]->state==state && s[p+j+2][q+j+2]->state == 0 && s[p+j+3][q+j+3]->state==state && s[p+j+4][q+j+4]->state==state){
                        pos1 = (p+j-1)*stoneNum + q+j-1;
                        pos2 = (p+j+2)*stoneNum + q+j+2;
                        return 2;
                    }
                    else if(s[p+j+1][q+j+1]->state==state && s[p+j+2][q+j+2]->state == state && s[p+j+3][q+j+3]->state==0 && s[p+j+4][q+j+4]->state==state){
                        pos1 = (p+j-1)*stoneNum + q+j-1;
                        pos2 = (p+j+3)*stoneNum + q+j+3;
                        return 2;
                    }

                    else if(s[p+j+1][q+j+1]->state==0 && s[p+j+2][q+j+2]->state == state && s[p+j+3][q+j+3]->state==state && s[p+j+4][q+j+4]->state==state){
                        pos1 = (p+j-1)*stoneNum + q+j-1;
                        pos2 = (p+j+1)*stoneNum + q+j+1;
                        return 2;
                    }

                }

            }

            empty = 0;
        }
        else
            empty = 0;

    }

    /*check diagonal */
    if(p-0 < stoneNum-1-q){
        min = p;
    }
    else{
        min = stoneNum-1-q;
    }

    if(q-0 < stoneNum-1-p){
        max = q;
    }
    else{
        max = stoneNum-1-p;
    }


    for(j=-min; j <= max; j++){

        if(s[p+j][q-j]->state == state){
            cnt++;
        }
        else
            cnt = 0;

        if(cnt == 1){

            if(j+5> max)
                break;

            if(s[p+j+1][q-j-1]->state == 0){
                if(j+6 > max || s[p+j+6][q-j-6]->state != state){
                    if(s[p+j+2][q-j-2]->state==0 && s[p+j+3][q-j-3]->state==state && s[p+j+4][q-j-4]->state == state && s[p+j+5][q-j-5]->state==state){
                        pos1 = (p+j+1)*stoneNum + q-j-1;
                        pos2 = (p+j+2)*stoneNum + q-j-2;
                        return 2;
                    }
                    else if(s[p+j+2][q-j-2]->state==state && s[p+j+3][q-j-3]->state==state && s[p+j+4][q-j-4]->state == state && s[p+j+5][q-j-5]->state== 0){
                        pos1 = (p+j+1)*stoneNum + q-j-1;
                        pos2 = (p+j+5)*stoneNum + q-j-5;
                        return 2;
                    }
                    else if(s[p+j+2][q-j-2]->state==state && s[p+j+3][q-j-3]->state==state && s[p+j+4][q-j-4]->state == state && s[p+j+5][q-j-5]->state== state){
                        pos1 = (p+j+1)*stoneNum + q-j-1;
                        return 1;
                    }
                }
            }

        }

        if(cnt==2 ){
            if(j+4 > max)
                break;

            if(s[p+j+1][q-j-1]->state==0){

                if(j+5 > max || s[p+j+5][q-j-5]->state != state){
                    if(s[p+j+2][q-j-2]->state==0 && s[p+j+3][q-j-3]->state == state && s[p+j+4][q-j-4]->state == state){
                        pos1 = (p+j+1)*stoneNum + q-j-1;
                        pos2 = (p+j+2)*stoneNum + q-j-2;
                        return 2;
                    }
                    else if( s[p+j+2][q-j-2]->state == state && s[p+j+3][q-j-3]->state == state && s[p+j+4][q-j-4]->state == 0){
                        pos1 = (p+j+1)*stoneNum + q-j-1;
                        pos2 = (p+j+4)*stoneNum + q-j-4;
                        return 2;
                    }
                    else if( s[p+j+2][q-j-2]->state == state && s[p+j+3][q-j-3]->state == state && s[p+j+4][q-j-4]->state == state){
                        pos1 = (p+j+1)*stoneNum + q-j-1;
                        return 1;
                    }

                }

            }

        }


        if(cnt == 3){

            if(j+3 > max)
                break;

            else{

                if( s[p+j+1][q-j-1]->state == 0){

                    if(s[p+j+2][q-j-2]->state == 0 && s[p+j+3][q-j-3]->state == state){

                        if( j+4 > max || s[p+j+4][q-j-4]->state != state){
                            pos1 = (p+j+1)*stoneNum + q-j-1;
                            pos2 = (p+j+2)*stoneNum + q-j-2;
                            return 2;
                        }
                    }
                    else if(s[p+j+2][q-j-2]->state == state){

                        if( j+4 > max || s[p+j+4][q-j-4]->state != state){
                            if(s[p+j+3][q-j-3]->state == state){
                                pos1 = (p+j+1)*stoneNum + q-j-1;
                                return 1;
                            }
                            else if(s[p+j+3][q-j-3]->state == 0){

                                pos1 = (p+j+1)*stoneNum + q-j-1;
                                pos2 = (p+j+3)*stoneNum + q-j-3;
                                return 2;

                            }
                        }

                    }

                }
                else if(s[p+j+1][q-j-1]->state == state){

                    if(s[p+j+2][q-j-2]->state==0 && s[p+j+3][q-j-3]->state == state){
                        if( j+4 > max || s[p+j+4][q-j-4]->state != state){
                            pos1 = (p+j+2)*stoneNum + q-j-2;
                            return 1;
                        }
                    }

                }

            }

        }
    }

    for(j=-min; j<=max; j++){

        if(s[p+j][q-j]->state == 0)
            empty++;
        else if(s[p+j][q-j]->state == state){
            if(empty>1 || (empty==1 && i-2 < 0)){
                if(j+4 > max)
                    break;

                if( j+5> max || s[p+j+5][q-j-5]->state != state){
                    if(s[p+j+1][q-j-1]->state==state && s[p+j+2][q-j-2]->state == 0 && s[p+j+3][q-j-3]->state==state && s[p+j+4][q-j-4]->state==state){
                        pos1 = (p+j-1)*stoneNum + q-j-1;
                        pos2 = (p+j+2)*stoneNum + q-j-2;
                        return 2;
                    }
                    else if(s[p+j+1][q-j-1]->state==state && s[p+j+2][q-j-2]->state == state && s[p+j+3][q-j-3]->state==0 && s[p+j+4][q-j-4]->state==state){
                        pos1 = (p+j-1)*stoneNum + q-j-1;
                        pos2 = (p+j+3)*stoneNum + q-j-3;
                        return 2;
                    }

                    else if(s[p+j+1][q-j-1]->state==0 && s[p+j+2][q-j-2]->state == state && s[p+j+3][q-j-3]->state==state && s[p+j+4][q-j-4]->state==state){
                        pos1 = (p+j-1)*stoneNum + q-j-1;
                        pos2 = (p+j+1)*stoneNum + q-j-1;
                        return 2;
                    }

                }

            }

            empty = 0;
        }
        else
            empty = 0;

    }



    return 0;

}
