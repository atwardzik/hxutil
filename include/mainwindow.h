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
#include <qwindowdefs.h>

#include <vector>

#include "hex_mod.h"
#include "highlighter.h"
#include "code_editor.h"

QT_BEGIN_NAMESPACE

namespace Ui {
        class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow {
        Q_OBJECT

public:
        explicit MainWindow(QWidget *parent = nullptr);

        ~MainWindow();

private slots:
        void on_actionAssembly_File_2_triggered(bool checked);

        void on_actionAssembly_File_1_triggered(bool checked);

        void on_actionHEX_File_2_triggered(bool checked);

        void on_actionCompileButton_triggered(bool checked);

protected:
        void changeEvent(QEvent *) override;

private:
        void changePalette();

private:
        Ui::MainWindow *ui;
        QString current_file;
        QMenu *preferences;
        QMenuBar *menu_bar;
        QAction *menu_action;
        CodeEditor *code_editor;
        Highlighter *highlighter;
        QLabel *mode_label;

        std::vector<uint8_t> hex_bytes;

        void showPreferencesDialog() {
                QMessageBox::information(this, "Preferences", "Open Preferences Dialog Here!");
        }
};
#endif // MAINWINDOW_H
