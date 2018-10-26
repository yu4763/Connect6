/*check row*/

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
