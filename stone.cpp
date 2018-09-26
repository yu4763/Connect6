#include <stone.h>
#include <stdio.h>

stone::stone(QWidget *parent, int i, int k) : QWidget(parent){
    this->i = i;
    this->k = k;
    exist = 0;
}

void stone::paintEvent(QPaintEvent*){

    QPainter painter(this);

    if(stoneColor == 0){
        QPen myPen(Qt::black, Qt::SolidLine);
        painter.setPen(myPen);
        painter.setBrush(QBrush(Qt::black, Qt::SolidPattern));
    }
    else{
        QPen myPen(Qt::white, Qt::SolidLine);
        painter.setPen(myPen);
        painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
    }

    painter.drawEllipse(0, 0, 40, 40);

}

void stone::mousePressEvent(QMouseEvent*){

    if( exist == 0 )
        this->setUpdatesEnabled(true);

}

computerstone::computerstone(QWidget *parent, int i, int k) : QWidget(parent){
    this->i = i;
    this->k = k;
}
