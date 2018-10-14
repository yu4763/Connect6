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
#include "network.h"


#define stoneNum 19
#define stoneSize 40


class board : public QWidget {

    Q_OBJECT

    public:
        board();
        void paintEvent(QPaintEvent*);
        public slots:
            void changeLabel();
            void emptyLabel();
            void FirstHandleClick();
            void BlackHandleClick();
            void WhiteHandleClick();

    private:
        int x, y;
        QLabel *statusLabel;
    public:
        stone *stones[stoneNum][stoneNum];

};

void saveDATASET(char* DATASET, char* state, int index, int len);
void GameEnd(char* DATASET1, char* DATASET2, Network score_board, int len1, int len2, bool win);

#endif
