#include "mainwindow.h"
#include "../ui/ui_mainwindow.h"

#include <iostream>
#include <fstream>
// import binary_file_handler;


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
        ui->setupUi(this);

        this->setWindowIcon(QIcon(":/Contents/Resources/icon.icns"));

        setWindowTitle("hxutil");

        changePalette();

        setupInfoStrings();

        setupMainMenu();

        setupTextWindows();

        setupTabWidget();
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
        new ARMv6_ASM_Highlighter(codeEditor->document());

        ui->textBrowserUp->setLineWrapMode(QTextEdit::NoWrap);
        ui->textBrowserDown->setLineWrapMode(QTextEdit::NoWrap);
}

void MainWindow::setupTabWidget() {
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

void MainWindow::createTab(CodeEditor *editor, const QIcon &icon, const QString &name) {
        QWidget *tab = new QWidget();
        QVBoxLayout *tabLayout = new QVBoxLayout(tab);

        tabLayout->setContentsMargins(0, 0, 0, 0);
        tabLayout->addWidget(editor);

        tabWidget->addTab(tab, icon, name);
}

void MainWindow::on_actionAssemblyOpenFile_triggered(bool checked) {
        const auto [fileName, plaintext] = openFileGetPlaintext(Language::Generic_ASM);

        const Language assembly_dialect = detectAssemblyDialect(fileName);
        CodeEditor *editor = new CodeEditor(this, assembly_dialect);
        editor->setPlainText(plaintext);

        createTab(editor, QIcon::fromTheme(QIcon::ThemeIcon::Computer), fileName);
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
        savePlaintextFile("debug_file.s", ui->plainTextEdit->toPlainText().toStdString());

        const QString command = settings.value("compiler_path").toString();

        QStringList params;
        params << "-g" << "-v"
                        << "debug_file.s"
                        << "-o"
                        << "debug_file.o";

        QProcess compilation;
        compilation.start(command, params);
        compilation.waitForFinished(); // sets current thread to sleep and waits for compilation to end
        QString output(compilation.readAllStandardOutput());
        QString errors(compilation.readAllStandardError());

        QDialog *dialog = new QDialog(this);
        dialog->setWindowTitle("Compilation result");
        QVBoxLayout *layout = new QVBoxLayout;
        QLabel *label = new QLabel(output + errors);
        layout->addWidget(label);
        dialog->setLayout(layout);
        dialog->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

        dialog->show();

        try {
                QString contents = QString::fromStdString(
                        readPlainText("debug_file.o", std::ios::in | std::ios::binary));
                ui->textBrowserDown->setText(contents);
        } catch (const std::runtime_error &e) {
                qDebug() << "[!] file could not be opened due to compilation error";
        }
}
