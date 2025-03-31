#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[]) {
        //hxutil
        //integrate rust formatter: https://parallel-rust-cpp.github.io/cpp_abi.html
        QApplication a(argc, argv);
        MainWindow w;
        w.show();
        return a.exec();
}
