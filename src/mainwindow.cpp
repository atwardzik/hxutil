#include "mainwindow.h"
#include "mainwindow.h"
#include "mainwindow.h"
#include "../ui/ui_mainwindow.h"

#include <iostream>
#include <fstream>
#include <sstream>
// import binary_file_handler;


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
        ui->setupUi(this);

        setWindowTitle("hxdump");

        mode_label = new QLabel(this);
        mode_label->setText("Mode: <none>");
        mode_label->setStyleSheet("margin-right: 10px;");
        ui->statusbar->addPermanentWidget(mode_label);

        QLabel *author_label = new QLabel(this);
        author_label->setText("Author: Artur Twardzik");
        ui->statusbar->addPermanentWidget(author_label);


        QAction *preferencesAction = new QAction("Preferences", this);
        preferencesAction->setMenuRole(QAction::PreferencesRole);
        connect(preferencesAction, &QAction::triggered, this, &MainWindow::showPreferencesDialog);

        QMenu *fileMenu = ui->menubar->addMenu("File");
        fileMenu->addAction(preferencesAction);

        highlighter = new Highlighter(ui->plainTextEdit->document());
}

MainWindow::~MainWindow() {
        delete ui;
}


void MainWindow::on_actionAssembly_File_2_triggered(bool checked) {
        QString file_name = QFileDialog::getOpenFileName(this, "Open the file",
                                                         QDir::homePath(), "Assembly files (*.s)");
        if (file_name.isEmpty()) {
                return;
        }

        QFile file(file_name);
        current_file = file_name;
        if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
                QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
                return;
        }
        // setWindowTitle(file_name);
        QTextStream in(&file);
        QString text = in.readAll();
        file.close();

        ui->plainTextEdit->setPlainText(text);
        mode_label->setText("Mode: Assembly → HEX");
}

void MainWindow::on_actionAssembly_File_1_triggered(bool checked) {}


void MainWindow::on_actionHEX_File_2_triggered(bool checked) {
        QString file_name = QFileDialog::getOpenFileName(this, "Open the file",
                                                         QDir::homePath(), "Object files (*.o *.obj *.elf *.so *.out)");
        if (file_name.isEmpty()) {
                return;
        }

        std::ifstream file(file_name.toStdString(), std::ios::in | std::ios::binary);
        if (!file.is_open()) {
                QMessageBox::warning(this, "Warning", "Cannot open file: " + file_name);
        }

        std::vector<uint8_t> bytes(
                (std::istreambuf_iterator<char>(file)),
                (std::istreambuf_iterator<char>()));

        file.close();

        current_file = file_name;

        hex_bytes = bytes;

        QString text = QString::fromStdString(bytes_to_printable(bytes));

        ui->plainTextEdit->setPlainText(text);
        mode_label->setText("Mode: HEX → Dump/Disassembly");
}


void MainWindow::on_actionCompileButton_triggered(bool checked) {
        QString contents = ui->plainTextEdit->toPlainText();

        ui->textBrowser->setText(contents);
}
