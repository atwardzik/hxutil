#include "mainwindow.h"
#include "mainwindow.h"
#include "mainwindow.h"
#include "../ui/ui_mainwindow.h"

#include <iostream>
#include <fstream>
#include <QPushButton>
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


        QMenu *fileMenu = ui->menubar->addMenu("App");

        QAction *preferencesAction = new QAction("Preferences", this);
        preferencesAction->setMenuRole(QAction::PreferencesRole);
        connect(preferencesAction, &QAction::triggered, this, &MainWindow::showPreferencesDialog);
        fileMenu->addAction(preferencesAction);

        QAction *quitAction = new QAction("Quit", this);
        quitAction->setShortcut(QKeySequence::Quit);
        QObject::connect(quitAction, &QAction::triggered, this, &QApplication::quit);
        fileMenu->addAction(quitAction);

        changePalette();

        ui->plainTextEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
        ui->textBrowser->setLineWrapMode(QTextEdit::NoWrap);
        ui->textBrowser_2->setLineWrapMode(QTextEdit::NoWrap);

        highlighter = new Highlighter(ui->plainTextEdit->document());
}

MainWindow::~MainWindow() {
        delete ui;
}


void MainWindow::changePalette() {
        QColor backgroundColor;

        if (QStyleHints const *style = QGuiApplication::styleHints();
                style->colorScheme() == Qt::ColorScheme::Dark) {
                backgroundColor = DarkTheme::backgroundColor;
        }
        else {
                backgroundColor = LightTheme::backgroundColor;
        }

        QPalette palette = ui->textBrowser->palette();
        palette.setColor(QPalette::Inactive, QPalette::Base, backgroundColor);
        palette.setColor(QPalette::Active, QPalette::Base, backgroundColor);
        ui->textBrowser->setPalette(palette);
        ui->textBrowser_2->setPalette(palette);
}


void MainWindow::changeEvent(QEvent *event) {
        if (event->type() == QEvent::ThemeChange ||
            event->type() == QEvent::StyleChange ||
            event->type() == QEvent::PaletteChange) {
                changePalette();
        }

        QMainWindow::changeEvent(event);
}

void MainWindow::closeEvent(QCloseEvent *event) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Confirm Exit");
        msgBox.setText("Are you sure you want to exit?");
        msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        int ret = msgBox.exec();

        switch (ret) {
                case QMessageBox::Cancel:
                        event->ignore();
                        break;
                case QMessageBox::Ok:
                        event->accept();
                        break;
                default:
                        break;
        }
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
