#include <stone.h>

stone::stone(QWidget *parent, int i, int k) : QWidget(parent){
    this->i = i;
    this->k = k;
}

void stone::paintEvent(QPaintEvent*){

    QPainter painter(this);

    QPen myPen(Qt::black, 1, Qt::SolidLine);
    painter.setPen(myPen);
    painter.setBrush(QBrush(Qt::black, Qt::SolidPattern));

    painter.drawEllipse(20, 20, 40, 40);


}
