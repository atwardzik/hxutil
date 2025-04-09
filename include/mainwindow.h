#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QLabel>
#include <QMenu>
#include <QAction>
#include <QToolButton>
#include <QShortcut>
#include <QComboBox>
#include <QSettings>
#include <QProcess>

#include "file_modifier.h"
#include "highlighter.h"
#include "code_editor.h"
#include "../ui/ui_mainwindow.h"
#include "lanugage_specifics.h"
#include "settings.h"

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
        void on_actionAssemblyNewFile_triggered(bool checked);

        void on_actionAssemblyOpenFile_triggered(bool checked);

        void on_actionHEXOpenFile_triggered(bool checked);

        void on_actionCompileButton_triggered(bool checked);

        void onTabCloseRequested(int index) {
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

        void setupTabWidget();

        void changePalette();

        void createTab(CodeEditor *editor, const QIcon &icon, const QString &name);

private:
        Ui::MainWindow *ui;
        QMenu *preferences;
        QMenuBar *menuBar;
        QAction *menuAction;
        QTabWidget *tabWidget;
        CodeEditor *codeEditor;
        QLabel *modeLabel;
        QTextEdit *executedCommands;

        QSettings settings;

        void showPreferencesDialog() {
                QSettings settings("Artur Twardzik", "hxutil", this);

                Settings *settingsWindow = new Settings(this);
                settingsWindow->exec();
        }
};
#endif // MAINWINDOW_H
