#include <QApplication>
#include <QWidget>
#include <board.h>

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);

    board window;

    window.resize(1000, 850);
    window.setWindowTitle("Connect6");
    window.show();

    return app.exec();

}
