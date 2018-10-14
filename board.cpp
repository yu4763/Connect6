#include <function.h>
#include <stdio.h>
#include "monte_carlo_tree_search.h"
#include "eval.h"
#include "network.h"


bool userStatus = false;
char userColor = 1;
extern int cnt;
extern int best_pos1;
extern int best_pos2;
int black_len = 0;
int white_len = 0;
char BLACK_DATASET[361*44];
char WHITE_DATASET[361*44];

int indexes[1];
extern float W1[110];
extern float b1[40];
extern float W2[10];
extern float b2[4];
char state[361];
extern Network score_board;


board::board() : QWidget(){

    this->x = 200;
    this->y = 60;

    statusLabel = new QLabel(this);
    statusLabel->setGeometry(QRect(QPoint(40, 350), QSize(150,80)));

    update();

    int i, k;
    for(i=0; i<stoneNum; i++){
        for(k=0; k<stoneNum; k++){
            stones[i][k] = new stone(this, i, k);
            stones[i][k]->setUpdatesEnabled(false);
            stones[i][k]->setGeometry(x+40*k-20,y+40*i-20, 41, 41);
        }
    }

    userColor = 1;
    userStatus = true;
    statusLabel->setText("Black Turn");

    QTimer::singleShot(1, this, SLOT(FirstHandleClick()));

    statusLabel->setText("White Turn");


}

void board::changeLabel(){

    int result = checkEnd(stones, (-userColor));

    if(result == 1){
        userStatus = true;
        statusLabel->setText("White Computer Win!!");
        GameEnd(BLACK_DATASET, WHITE_DATASET, score_board, black_len, white_len, false);
    }
    else if(result == 2){
        userStatus = true;
        statusLabel->setText("Black Computer Win!!");
        GameEnd(BLACK_DATASET, WHITE_DATASET, score_board, black_len, white_len, true);
    }
    else if(result == 0 && cnt == 2 ){
        userStatus = true;
        loadWeight(W1, b1, W2, b2, false);
        statusLabel->setText("White Turn");

        for (int i=0; i<361; i++){
            state[i] = stones[i/19][i%19]->state;
        }
        GetBestIndex(state, indexes, W1, b1, W2,b2, score_board, 1);
        best_pos2 = indexes[0];
        QTimer::singleShot(1, this, SLOT(WhiteHandleClick()));
        GetBestIndex(state, indexes, W1, b1, W2,b2, score_board, 1);
        best_pos2 = indexes[0];
        QTimer::singleShot(2, this, SLOT(WhiteHandleClick()));
        cnt = 0;
    }
}

void board::emptyLabel(){

    int result = checkEnd(stones, userColor);

    if(result == 1){
        userStatus = false;
        statusLabel->setText("Black Computer Win!!");
        GameEnd(BLACK_DATASET, WHITE_DATASET, score_board, black_len, white_len, true);
    }
    else if(result == 2){
        userStatus = false;
        statusLabel->setText("White Computer Win!!");
        GameEnd(BLACK_DATASET, WHITE_DATASET, score_board, black_len, white_len, false);
    }
    else if(result == 0 && cnt == 2){
        userStatus = false;
        loadWeight(W1, b1, W2, b2, true);
        statusLabel->setText("Black Turn");
        for (int i=0; i<361; i++){
            state[i] = stones[i/19][i%19]->state;
        }
        GetBestIndex(state, indexes, W1, b1, W2,b2, score_board, 1);
        best_pos1 = indexes[0];
        QTimer::singleShot(1, this, SLOT(BlackHandleClick()));
        GetBestIndex(state, indexes, W1, b1, W2,b2, score_board, 1);
        best_pos2 = indexes[0];
        QTimer::singleShot(2, this, SLOT(BlackHandleClick()));
        cnt = 0;
    }

}
void board::FirstHandleClick(){

    int i = 9;
    int k = 9;
    cnt++;
    for (int i=0; i<361; i++){
        state[i] = stones[i/19][i%19]->state;
    }  
    saveDATASET(BLACK_DATASET, state, 180, 0);
    black_len++;
    stones[i][k]->setUpdatesEnabled(true);
    stones[i][k]->update();
    stones[i][k]->state = -userColor;


}
void board::WhiteHandleClick(){

    int i, k;

    cnt++;

    if(cnt == 1){
        for (int i=0; i<361; i++){
            state[i] = stones[i/19][i%19]->state;
        }
        saveDATASET(WHITE_DATASET, state, best_pos1, white_len);
        white_len++;
        k = best_pos1%stoneNum;
        i = best_pos1/stoneNum;

    }

    else{
        for (int i=0; i<361; i++){
            state[i] = stones[i/19][i%19]->state;
        }
        saveDATASET(WHITE_DATASET, state, best_pos2, white_len);
        white_len++;
        k = best_pos2%stoneNum;
        i = best_pos2/stoneNum;

    }

    stones[i][k]->setUpdatesEnabled(true);
    stones[i][k]->update();
    stones[i][k]->state = -userColor;

}

void board::BlackHandleClick(){

    int i, k;

    cnt++;

    if(cnt == 1){

        for (int i=0; i<361; i++){
            state[i] = stones[i/19][i%19]->state;
        }
        saveDATASET(BLACK_DATASET, state, best_pos1, black_len);
        black_len++;

        k = best_pos1%stoneNum;
        i = best_pos1/stoneNum;

    }

    else{
        for (int i=0; i<361; i++){
            state[i] = stones[i/19][i%19]->state;
        }
        saveDATASET(BLACK_DATASET, state, best_pos2, black_len);
        black_len++;

        k = best_pos2%stoneNum;
        i = best_pos2/stoneNum;

    }

    stones[i][k]->setUpdatesEnabled(true);
    stones[i][k]->update();
    stones[i][k]->state = userColor;


}



void board::paintEvent(QPaintEvent*) {

    int i;
    int size = (stoneNum-1)*stoneSize;

    QPainter painter(this);

    QPen myPen(Qt::black, 1, Qt::SolidLine);
    painter.setPen(myPen);

    painter.setBrush(QBrush(Qt::lightGray, Qt::SolidPattern));
    painter.drawRect(x, y, size, size);
    painter.setBrush(Qt::NoBrush);

    for(i=1; i<stoneNum-1; i++){
        painter.drawLine(x, y + i*stoneSize, x+size, y+i*stoneSize);
        painter.drawLine(x + i*stoneSize, y, x + i*stoneSize, y+size);

    }

}

void saveDATASET(char* DATASET, char* state, int index, int len){
    char line[44];
    getline(state, line, index);
    for (int i=0; i<44; i++){
        DATASET[len*44 + i] = line[i]; 
    }
}

void GameEnd(char* DATASET1, char* DATASET2, Network score_board, int len1, int len2, bool win){
    char DATA[44];
    float LABEL[4];
    loadWeight(W1, b1, W2, b2, true);
    for (int i=0; i<len1; i++){
        for (int j=0; j<44; j++){
            DATA[j] = DATASET1[i*44 +j];
        }

        score_board.getoutput(W1, b1, W2, b2, DATA, LABEL);

        if (win){
            for (int k=0; k<4; k++){
                LABEL[i] = LABEL[i] + 0.01 * i;
            }
        }
        else{
            for (int k=0; k<4; k++){
                LABEL[i] = LABEL[i] - 0.01 * i;
            }            
        }
        for (int j=0; j<100; j++){
            score_board.backpropagation(W1, b1, W2, b2, DATA, LABEL, 0.001);
        }
        
    }
    saveWeight(W1, b1, W2, b2, true);
    
    loadWeight(W1, b1, W2, b2, false);
    for (int i=0; i<len2; i++){
        for (int j=0; j<44; j++){
            DATA[j] = DATASET2[i*44 +j];
        }

        score_board.getoutput(W1, b1, W2, b2, DATA, LABEL);

        if (win){
            for (int k=0; k<4; k++){
                LABEL[i]--;
            }
        }
        else{
            for (int k=0; k<4; k++){
                LABEL[i]++;
            }            
        }
        for (int j=0; j<1000; j++){
            score_board.backpropagation(W1, b1, W2, b2, DATA, LABEL, 0.001);
        }
        
    }
    saveWeight(W1, b1, W2, b2, false);
    
    return;
}
