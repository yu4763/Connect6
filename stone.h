#ifndef STONE_H
#define STONE_H

#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QtGui>
#include <QObject>
#include <QPushButton>


class stone : public QWidget {

    Q_OBJECT

    public:
        stone(QWidget*, int i, int k);
        void paintEvent(QPaintEvent*);
        void mousePressEvent(QMouseEvent*);

    public:
        int i, k;
        char state;

};

#endif
