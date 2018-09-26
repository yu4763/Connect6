#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QtGui>
#include <QObject>
#include <QPushButton>
#include <stone.h>

#define boardNum 19
#define stoneSize 40


class board : public QWidget {

    Q_OBJECT

    public:
        board();
        void paintEvent(QPaintEvent*);
        public slots:
            void handleButton();
    private:
        int x, y;
        int stoneColor;  // 0:black 1:white
        QPushButton *startButton;
        stone *stones[boardNum][boardNum];

};
