#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QToolButton>
#include <QShortcut>
#include <QCursor>
#include <qwindowdefs.h>

#include <vector>

#include "file_modifier.h"
#include "highlighter.h"
#include "code_editor.h"

QT_BEGIN_NAMESPACE

namespace Ui {
        class MainWindow;
}

QT_END_NAMESPACE

enum class Language {
        ARMv6_ASM,
        x86_ASM,
        C,
        HEX,
};

class MainWindow : public QMainWindow {
        Q_OBJECT

public:
        explicit MainWindow(QWidget *parent = nullptr);

        ~MainWindow();

private slots:
        void on_actionAssemblyNewFile_triggered(bool checked);

        void on_actionAssemblyOpenFile_triggered(bool checked);

        void on_actionHEXOpenFile_triggered(bool checked);

        void on_actionCompileButton_triggered(bool checked);

protected:
        void changeEvent(QEvent *) override;

        void closeEvent(QCloseEvent *event) override;

private:
        void setupInfoStrings();

        void setupMainMenu();

        void setupTextWindows();

        void changePalette();

        void setHighlighter(Language language);

private:
        Ui::MainWindow *ui;
        QString currentFile;
        QMenu *preferences;
        QMenuBar *menuBar;
        QAction *menuAction;
        CodeEditor *codeEditor;
        Highlighter *highlighter = nullptr;
        QLabel *modeLabel;

        // std::vector<uint8_t> hex_bytes;

        void showPreferencesDialog() {
                QDialog dialog(this);
                dialog.setWindowTitle("Tutaj dokona się akt stworzenia!");
                dialog.exec();
                // QMessageBox::information(this, "Preferences", "Open Preferences Dialog Here!");
        }
};
#endif // MAINWINDOW_H
