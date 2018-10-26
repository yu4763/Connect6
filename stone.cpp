#include <board.h>
#include <stdio.h>

extern bool userStatus;
extern int userColor;
int cnt = 1;
extern int minei1, minek1;
extern int minei2, minek2;

stone::stone(QWidget *parent, int i, int k) : QWidget(parent){
    this->i = i;
    this->k = k;
    state = 0;
}

void stone::paintEvent(QPaintEvent*){

    QPainter painter(this);

    if(userStatus){
        if(userColor == 1){
            QPen myPen(Qt::black, Qt::SolidLine);
            painter.setPen(myPen);
            painter.setBrush(QBrush(Qt::black, Qt::SolidPattern));
            state = 1;
        }
        else{
            QPen myPen(Qt::white, Qt::SolidLine);
            painter.setPen(myPen);
            painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
            state = 2;
        }

        this->setUpdatesEnabled(false);
        QTimer::singleShot(0, (board*)this->parentWidget(), SLOT(emptyLabel()));

    }
    else{
        if(userColor == 2){
            QPen myPen(Qt::black, Qt::SolidLine);
            painter.setPen(myPen);
            painter.setBrush(QBrush(Qt::black, Qt::SolidPattern));
            state = 1;
        }
        else{
            QPen myPen(Qt::white, Qt::SolidLine);
            painter.setPen(myPen);
            painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
            state = 2;
        }

        this->setUpdatesEnabled(false);
        QTimer::singleShot(0, (board*)this->parentWidget(), SLOT(changeLabel()));

    }

    painter.drawEllipse(0, 0, 40, 40);

}

void stone::mousePressEvent(QMouseEvent*){


    if( userStatus && state == 0 ){
        cnt++;
        if(cnt==1){
            minei1 = i;
            minek1 = k;
        }
        else if(cnt==2){
            minei2 = i;
            minek2 = k;
        }
        this->setUpdatesEnabled(true);

    }


}
