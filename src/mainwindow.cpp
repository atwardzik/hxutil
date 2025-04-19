#include "mainwindow.h"
#include "../ui/ui_mainwindow.h"
#include "file_viewer.h"

#include <iostream>
#include <fstream>
#include <QFileSystemModel>
// import binary_file_handler;


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
        ui->setupUi(this);

        this->setWindowIcon(QIcon(":/Contents/Resources/icon.icns"));

        setWindowTitle("hxutil");

        setupInfoStrings();

        setupMainMenu();

        setupTabWidget();

        setupTextWindows();

        //fileTreeView
        // QFileSystemModel *model = new QFileSystemModel;
        // model->setRootPath(QDir::currentPath());
        FileViewerModel *model = new FileViewerModel;
        ui->fileTreeView->setModel(model);

        // command_tab
        ui->file_text_splitter->setSizes({0, 65535});
        ui->command_tab_splitter->setSizes({65535, 0});
        QToolButton *btn = new QToolButton;
        btn->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::ListRemove));
        btn->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonFollowStyle);
        btn->setIconSize(QSize(13, 16));
        ui->commandTabWidget->setCornerWidget(btn, Qt::TopRightCorner);


        connect(btn, &QToolButton::clicked, this, [this](bool checked) {
                ui->command_tab_splitter->setSizes({65535, 0});
        });

        //commands

        QGridLayout *layout = new QGridLayout(ui->command_tab);
        executedCommands = new QTextEdit(ui->command_tab);
        executedCommands->setReadOnly(true);
        executedCommands->setLineWrapMode(QTextEdit::NoWrap);

        layout->addWidget(executedCommands);
        layout->setContentsMargins(0, 0, 0, 0);
        ui->command_tab->setLayout(layout);

        //terminal

        changePalette();
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


        QAction *compileAction = new QAction("Compile", this);
        compileAction->setShortcut(QKeySequence(settings.value("CompileShortcut").toString()));
        connect(compileAction, &QAction::triggered, this, &MainWindow::on_actionCompileButton_triggered);
        compileAction->setIcon(createColoredIcon(":/icons/hammer.png", UtilColors::iconColor));
        ui->toolBar->addAction(compileAction);

        QAction *formatterAction = new QAction("Format code", this);
        formatterAction->setShortcut(QKeySequence(settings.value("FormatShortcut").toString()));
        connect(formatterAction, &QAction::triggered, this, [this]() {
                CodeEditor *editor = this->tabWidget->getCurrentEditor();
                if (editor) {
                        editor->formatCode();
                }
        });
        formatterAction->setIcon(createColoredIcon(":/icons/wand.png", UtilColors::iconColor));
        ui->toolBar->addAction(formatterAction);
}

void MainWindow::setupTextWindows() {
        CodeEditor *defaultEditor = new CodeEditor(this, "", Language::ARMv6_ASM);
        defaultEditor->setPlainText(ui->plainTextEdit->toPlainText());
        tabWidget->removeTab(0);
        tabWidget->addEditor(defaultEditor, QIcon::fromTheme(QIcon::ThemeIcon::Computer), "example");

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
        ui->fileTreeView->setPalette(palette);
        executedCommands->setPalette(palette);
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


void MainWindow::on_actionAssemblyOpenFile_triggered(bool checked) {
        const auto [fileName, plaintext] = openFileGetPlaintext(Language::Generic_ASM);

        const Language assembly_dialect = detectAssemblyDialect(fileName);
        CodeEditor *editor = new CodeEditor(this, fileName, assembly_dialect);
        editor->setPlainText(plaintext);

        tabWidget->addEditor(editor, QIcon::fromTheme(QIcon::ThemeIcon::Computer), fileName);
        modeLabel->setText("Mode: Assembly → HEX");
}

void MainWindow::on_actionAssemblyNewFile_triggered(bool checked) {}


void MainWindow::on_actionHEXOpenFile_triggered(bool checked) {
        const OpenFile file = openFileGetPlaintext(Language::HEX);

        CodeEditor *editor = new CodeEditor(this);
        editor->setPlainText(file.plaintext);

        tabWidget->addEditor(editor, QIcon::fromTheme(QIcon::ThemeIcon::DriveHarddisk), file.fileName);
        modeLabel->setText("Mode: HEX → Dump/Disassembly");
}


void MainWindow::on_actionCompileButton_triggered(bool checked) {
        //TODO: save all opened files
        CodeEditor *currentEditor = tabWidget->getCurrentEditor();
        const QString plainTextFile = currentEditor->saveFile();

        if (plainTextFile.isEmpty()) {
                return;
        }

        const QString objectFile = currentEditor->getObjectFileName();

        const QString command = settings.value("compiler_path").toString();

        QStringList params;
        params << "-g" << "-v"
                        << plainTextFile
                        << "-o" << objectFile;

        QProcess compilation;
        compilation.start(command, params);
        compilation.waitForFinished(); // sets current thread to sleep and waits for compilation to end
        QString output(compilation.readAllStandardOutput());
        QString errors(compilation.readAllStandardError());

        QTextCharFormat format;
        executedCommands->insertPlainText("Compilation ");
        format.setFontItalic(true);
        format.setForeground(OneDarkTheme::red);
        executedCommands->setCurrentCharFormat(format);
        executedCommands->insertPlainText(QDateTime::currentDateTime().toString() + "\n");
        format.setFontItalic(false);
        format.clearForeground();
        executedCommands->setCurrentCharFormat(format);
        executedCommands->append(output + errors);

        ui->commandTabWidget->setCurrentIndex(0);
        ui->command_tab_splitter->setSizes({65535, 200});


        try {
                QString contents = QString::fromStdString(
                        readPlainText(objectFile.toStdString(), std::ios::in | std::ios::binary));
                ui->textBrowserDown->setText(contents);

                QProcess disassembly;
                QStringList params;
                params << "-d" << objectFile;

                disassembly.start("objdump", params);
                disassembly.waitForFinished();
                QString output(disassembly.readAllStandardOutput());
                QString errors(disassembly.readAllStandardError());

                ui->textBrowserUp->setText(output + errors);
        } catch (const std::runtime_error &e) {
                qDebug() << "[!] file could not be opened due to compilation error";
        }
}
