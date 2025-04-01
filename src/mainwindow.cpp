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

        this->setWindowIcon(QIcon(":/Contents/Resources/icon.icns"));

        setWindowTitle("hxutil");

        changePalette();

        setupInfoStrings();

        setupMainMenu();

        setupTextWindows();

        tabWidget = this->ui->tabWidget;

        //TODO: this must be added dynamically as the user opens the file
        CodeEditor *secondPageEditor = new CodeEditor(this);
        tabWidget->setTabsClosable(true);
        tabWidget->removeTab(1);


        QWidget *tab1 = new QWidget();
        QVBoxLayout *tab1Layout = new QVBoxLayout(tab1);
        tab1Layout->setContentsMargins(0, 0, 0, 0);
        tab1Layout->addWidget(secondPageEditor);
        tabWidget->addTab(tab1, "File 2");

        connect(tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::onTabCloseRequested);
}

MainWindow::~MainWindow() {
        delete ui;
}


void MainWindow::setupInfoStrings() {
        modeLabel = new QLabel(this);
        modeLabel->setText("Mode: <none>");
        modeLabel->setStyleSheet("margin-right: 10px;");
        ui->statusbar->addPermanentWidget(modeLabel);

        QLabel *authorLabel = new QLabel(this);
        authorLabel->setText("Author: Artur Twardzik");
        ui->statusbar->addPermanentWidget(authorLabel);
}

void MainWindow::setupMainMenu() {
        QMenu *actionMenu = ui->menubar->addMenu("Action");

        QAction *preferencesAction = new QAction("Preferences", this);
        preferencesAction->setMenuRole(QAction::PreferencesRole);
        connect(preferencesAction, &QAction::triggered, this, &MainWindow::showPreferencesDialog);
        actionMenu->addAction(preferencesAction);

        QAction *quitAction = new QAction("Quit", this);
        quitAction->setShortcut(QKeySequence::Quit);
        connect(quitAction, &QAction::triggered, this, &QApplication::quit);
        actionMenu->addAction(quitAction);
}

void MainWindow::setupTextWindows() {
        codeEditor = ui->plainTextEdit;
        codeEditor->setLineWrapMode(QPlainTextEdit::NoWrap);
        ui->textBrowserUp->setLineWrapMode(QTextEdit::NoWrap);
        ui->textBrowserDown->setLineWrapMode(QTextEdit::NoWrap);

        setHighlighter(Language::ARMv6_ASM);
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

        QPalette palette = ui->textBrowserUp->palette();
        palette.setColor(QPalette::Inactive, QPalette::Base, backgroundColor);
        palette.setColor(QPalette::Active, QPalette::Base, backgroundColor);
        ui->textBrowserUp->setPalette(palette);
        ui->textBrowserDown->setPalette(palette);
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

void MainWindow::setHighlighter(Language language) {
        delete highlighter;

        switch (language) {
                        using enum Language;
                case ARMv6_ASM:
                        highlighter = new ARMv6_ASM_Highlighter(codeEditor->document());
                        break;
                case x86_ASM:
                case C:
                case HEX:
                default:
                        highlighter = nullptr;
                        break;
        }
}


void MainWindow::on_actionAssemblyOpenFile_triggered(bool checked) {
        QString fileName = QFileDialog::getOpenFileName(this, "Open the file",
                                                        QDir::homePath(), "Assembly files (*.s)");
        if (fileName.isEmpty()) {
                return;
        }
        currentFile = fileName;

        const QString text = QString::fromStdString(readPlainText(fileName.toStdString()));

        codeEditor->setPlainText(text);
        setHighlighter(Language::ARMv6_ASM);
        modeLabel->setText("Mode: Assembly → HEX");
}

void MainWindow::on_actionAssemblyNewFile_triggered(bool checked) {}


void MainWindow::on_actionHEXOpenFile_triggered(bool checked) {
        QString fileName = QFileDialog::getOpenFileName(this, "Open the file",
                                                        QDir::homePath(), "Object files (*.o *.obj *.elf *.so *.out)");
        if (fileName.isEmpty()) {
                return;
        }
        currentFile = fileName;

        const QString text = QString::fromStdString(
                readPlainText(fileName.toStdString(), std::ios::in | std::ios::binary));

        codeEditor->setPlainText(text);
        modeLabel->setText("Mode: HEX → Dump/Disassembly");
}


void MainWindow::on_actionCompileButton_triggered(bool checked) {
        QString contents = ui->plainTextEdit->toPlainText();

        ui->textBrowserDown->setText(contents);
}
