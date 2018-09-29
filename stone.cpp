#include <board.h>
#include <stdio.h>

extern bool userStatus;
extern int userColor;

stone::stone(QWidget *parent, int i, int k) : QWidget(parent){
    this->i = i;
    this->k = k;
    exist = 0;
}

void stone::paintEvent(QPaintEvent*){

    QPainter painter(this);

    if(userStatus){
        if(userColor == 0){
            QPen myPen(Qt::black, Qt::SolidLine);
            painter.setPen(myPen);
            painter.setBrush(QBrush(Qt::black, Qt::SolidPattern));
            color = 0;
        }
        else{
            QPen myPen(Qt::white, Qt::SolidLine);
            painter.setPen(myPen);
            painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
            color = 1;
        }

        this->setUpdatesEnabled(false);
        QTimer::singleShot(0, (board*)this->parentWidget(), SLOT(emptyLabel()));

    }
    else{
        if(userColor == 1){
            QPen myPen(Qt::black, Qt::SolidLine);
            painter.setPen(myPen);
            painter.setBrush(QBrush(Qt::black, Qt::SolidPattern));
            color = 0;
        }
        else{
            QPen myPen(Qt::white, Qt::SolidLine);
            painter.setPen(myPen);
            painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
            color = 1;
        }
        
        this->setUpdatesEnabled(false);
        QTimer::singleShot(0, (board*)this->parentWidget(), SLOT(changeLabel()));
    }

    painter.drawEllipse(0, 0, 40, 40);

}

void stone::mousePressEvent(QMouseEvent*){

    if( userStatus && exist == 0 ){
        this->setUpdatesEnabled(true);
        exist = 1;

    }

}
