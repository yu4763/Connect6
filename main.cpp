#include <QApplication>
#include <QWidget>
#include <board.h>

board* window;

int main(int argc, char *argv[]) {


    QApplication app(argc, argv);
    board window_;

    window = &window_;
    window->resize(1000, 850);
    window->setWindowTitle("Connect6");
    window->show();

    return app.exec();

}
