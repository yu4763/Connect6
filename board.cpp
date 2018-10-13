#include <function.h>
#include <stdio.h>
#include "monte_carlo_tree_search.h"

bool userStatus = false;
char userColor = 1;
extern int cnt;
extern int best_pos1;
extern int best_pos2;

extern float W1[110];
extern float b1[40];
extern float W2[10];
extern float b2[4];
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

    int result = checkEnd(stones, (-userColor+3));

    if(result == 1){
        userStatus = true;
        statusLabel->setText("White Computer Win!!");
        //GameEnd(DATA, LABEL, true);
    }
    else if(result == 2){
        userStatus = true;
        statusLabel->setText("Black Computer Win!!");
    }
    else if(result == 0 && cnt == 2 ){
        userStatus = true;
        loadWeight(W1, b1, W2, b2, false);
        statusLabel->setText("White Turn");

        MonteCarloTreeSearch();
        QTimer::singleShot(1, this, SLOT(WhiteHandleClick()));
        QTimer::singleShot(2, this, SLOT(WhiteHandleClick()));
        cnt = 0;
    }
}

void board::emptyLabel(){

    int result = checkEnd(stones, userColor);

    if(result == 1){
        userStatus = false;
        statusLabel->setText("Black Computer Win!!");
    }
    else if(result == 2){
        userStatus = false;
        statusLabel->setText("White Computer Win!!");
    }
    else if(result == 0 && cnt == 2){
        userStatus = false;
        loadWeight(W1, b1, W2, b2, true);
        statusLabel->setText("Black Turn");
        MonteCarloTreeSearch();
        QTimer::singleShot(1, this, SLOT(BlackHandleClick()));
        QTimer::singleShot(2, this, SLOT(BlackHandleClick()));
        cnt = 0;
    }

}
void board::FirstHandleClick(){

    int i = 9;
    int k = 9;
    cnt++;
    stones[i][k]->setUpdatesEnabled(true);
    stones[i][k]->update();
    stones[i][k]->state = -userColor+3;

}
void board::WhiteHandleClick(){

    int i, k;

    cnt++;

    if(cnt == 1){

        k = best_pos1%stoneNum;
        i = best_pos1/stoneNum;

    }

    else{

        k = best_pos2%stoneNum;
        i = best_pos2/stoneNum;

    }

    stones[i][k]->setUpdatesEnabled(true);
    stones[i][k]->update();
    stones[i][k]->state = -userColor+3;

}

void board::BlackHandleClick(){

    int i, k;

    cnt++;

    if(cnt == 1){

        k = best_pos1%stoneNum;
        i = best_pos1/stoneNum;

    }

    else{

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
