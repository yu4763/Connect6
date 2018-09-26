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
        int stoneColor;  // 0:black 1:white

    private:
        int exist;

};

class computerstone : public QWidget {

    Q_OBJECT

public:
    computerstone(QWidget*, int i, int k);
    void painEvent(QPaintEvent*);

public:
    int i, k;

};
