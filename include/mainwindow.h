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
#include <QComboBox>
#include <QListWidget>
#include <QListView>
#include <QStringListModel>
#include <QStandardItemModel>

#include <vector>

#include "file_modifier.h"
#include "highlighter.h"
#include "code_editor.h"
#include "open_files_list.h"
#include "../ui/ui_mainwindow.h"

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

        void onTabCloseRequested(int index) {
                qDebug() << "Requested removal: " << index;
                QWidget *widget = tabWidget->widget(index);
                tabWidget->removeTab(index);
                delete widget;
        }

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
        QTabWidget *tabWidget;
        CodeEditor *codeEditor;
        Highlighter *highlighter = nullptr; //todo: should be property of codeEditor!!!
        QLabel *modeLabel;

        void showPreferencesDialog() {
                QDialog dialog(this);
                dialog.setWindowTitle("Tutaj dokona się akt stworzenia!");
                dialog.exec();
                // QMessageBox::information(this, "Preferences", "Open Preferences Dialog Here!");
        }
};
#endif // MAINWINDOW_H
