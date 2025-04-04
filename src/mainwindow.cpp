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

        tabWidget->setTabsClosable(true);
        tabWidget->setUsesScrollButtons(true);
        tabWidget->removeTab(1);
        tabWidget->setElideMode(Qt::TextElideMode::ElideNone);
        ///////////////////////////////////////////////
        /// THIS LINE IS VERY IMPORTANT, DUE TO THE  //
        ///    MACOS BUG (TRUNCATING MIDDLE TEXT)    //
        ///    FIX IN FUTURE VERSIONS OF SYSTEM      //
        tabWidget->setIconSize(QSize(10, 10)); //
        ///////////////////////////////////////////////
        ///////////////////////////////////////////////

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

        setHighlighter(codeEditor, Language::ARMv6_ASM);
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

void MainWindow::setHighlighter(QPlainTextEdit *parent, Language language) {
        // delete highlighter;

        switch (language) {
                        using enum Language;
                case ARMv6_ASM:
                        new ARMv6_ASM_Highlighter(parent->document());
                        break;
                case x86_ASM:
                case C:
                case HEX:
                default:
                        // highlighter = nullptr;
                        break;
        }
}

void MainWindow::createTab(CodeEditor *editor, const QIcon &icon, const QString &name) {
        QWidget *tab = new QWidget();
        QVBoxLayout *tabLayout = new QVBoxLayout(tab);

        tabLayout->setContentsMargins(0, 0, 0, 0);
        tabLayout->addWidget(editor);

        tabWidget->addTab(tab, icon, name);
}

void MainWindow::on_actionAssemblyOpenFile_triggered(bool checked) {
        const OpenFile file = openFileGetPlaintext(Language::ARMv6_ASM);

        CodeEditor *editor = new CodeEditor(this);
        editor->setPlainText(file.plaintext);
        setHighlighter(editor, Language::ARMv6_ASM);

        createTab(editor, QIcon::fromTheme(QIcon::ThemeIcon::Computer), file.fileName);
        modeLabel->setText("Mode: Assembly → HEX");
}

void MainWindow::on_actionAssemblyNewFile_triggered(bool checked) {}


void MainWindow::on_actionHEXOpenFile_triggered(bool checked) {
        const OpenFile file = openFileGetPlaintext(Language::HEX);

        CodeEditor *editor = new CodeEditor(this);
        editor->setPlainText(file.plaintext);

        createTab(editor, QIcon::fromTheme(QIcon::ThemeIcon::DriveHarddisk), file.fileName);
        modeLabel->setText("Mode: HEX → Dump/Disassembly");
}


void MainWindow::on_actionCompileButton_triggered(bool checked) {
        QString contents = ui->plainTextEdit->toPlainText();

        ui->textBrowserDown->setText(contents);
}
