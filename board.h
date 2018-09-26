#ifndef BOARD_H
#define BOARD_H

#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QtGui>
#include <QObject>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
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
            void changeLabel();
            void emptyLabel();
            void handleClick();

    private:
        int x, y;
        QPushButton *startButton;
        QLabel *statusLabel;
    public:
        stone *stones[boardNum][boardNum];

};

#endif
