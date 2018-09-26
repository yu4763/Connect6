#include <board.h>
#include <stdio.h>
#include <iostream>
using namespace std;

board::board() : QWidget(){

    this->x = 200;
    this->y = 50;
    startButton = new QPushButton("Game Start", this);
    startButton->setGeometry(QRect(QPoint(60, 400), QSize(100,50)));
    connect(startButton, SIGNAL (released()), this, SLOT (handleButton()));

    update();

    int i, k;
    for(i=0; i<boardNum-1; i++){
        for(k=0; k<boardNum-1; k++){
            stones[i][k] = new stone(this, i, k);
            stones[i][k]->setUpdatesEnabled(false);
            stones[i][k]->setGeometry(x+40*i+20,y+40*k+20, 45, 45);
            stones[i][k]->stoneColor = 1;
        }
    }
}

void board::handleButton(){

}

void board::paintEvent(QPaintEvent*) {

    int i;
    int size = boardNum*stoneSize;

    QPainter painter(this);

    QPen myPen(Qt::black, 1, Qt::SolidLine);
    painter.setPen(myPen);

    painter.setBrush(QBrush(Qt::lightGray, Qt::SolidPattern));
    painter.drawRect(x, y, size, size);
    painter.setBrush(Qt::NoBrush);

    for(i=1; i<boardNum; i++){
        painter.drawLine(x, y + i*stoneSize, x+size, y+i*stoneSize);
        painter.drawLine(x + i*stoneSize, y, x + i*stoneSize, y+size);

    }

}
