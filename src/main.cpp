#include "mainwindow.h"

#include <QApplication>
#include <QPoint>
#include <QWindow>

int main(int argc, char *argv[]) {
        //hxutil
        //integrate rust formatter: https://parallel-rust-cpp.github.io/cpp_abi.html
        QApplication a(argc, argv);
        QCoreApplication::setOrganizationName("Artur Twardzik");
        QCoreApplication::setOrganizationDomain("https://github.com/atwardzik/hxutil");
        QCoreApplication::setApplicationName("hxutil");
        MainWindow w;
        QPoint center = w.windowHandle()->screen()->availableGeometry().center();
        w.move(center - QPoint(w.size().width() / 2, w.size().height() / 2));
        w.show();

        return a.exec();
}
