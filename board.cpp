#include <function.h>
#include <stdio.h>

bool userStatus = false;
int userColor = 0;

board::board() : QWidget(){

    this->x = 200;
    this->y = 50;
    startButton = new QPushButton("Game Start", this);
    startButton->setGeometry(QRect(QPoint(50, 340), QSize(100,50)));
    connect(startButton, SIGNAL (released()), this, SLOT (handleButton()));

    statusLabel = new QLabel(this);
    statusLabel->setGeometry(QRect(QPoint(65, 390), QSize(100,50)));

    update();

    int i, k;
    for(i=0; i<boardNum-1; i++){
        for(k=0; k<boardNum-1; k++){
            stones[i][k] = new stone(this, i, k);
            stones[i][k]->setUpdatesEnabled(false);
            stones[i][k]->setGeometry(x+40*k+20,y+40*i+20, 41, 41);
        }
    }
}

void board::handleButton(){

    userColor = 0;
    userStatus = true;
    statusLabel->setText("Your Turn");

}

void board::changeLabel(){

    userStatus = true;
    statusLabel->setText("Your Turn");
}

void board::emptyLabel(){

    userStatus = false;
    statusLabel->setText("");
}

void board::handleClick(){

    int i, k;
    i = wheretoPut(this);
    k = i%18;
    i = i/18;

    stones[i][k]->setUpdatesEnabled(true);
    stones[i][k]->exist = 1;
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
