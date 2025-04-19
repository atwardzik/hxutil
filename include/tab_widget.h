//
// Created by Artur Twardzik on 18/04/2025.
//

#ifndef TAB_H
#define TAB_H

#include "code_editor.h"

#include <QTabWidget>
#include <QIcon>
#include <QWidget>
#include <QVBoxLayout>

#include <map>
#include <iostream>

class TabWidget : public QTabWidget {
        Q_OBJECT

private:
        std::map<int, CodeEditor *> editors;

        // int addTab(QWidget *widget, const QIcon &icon, const QString &label);

        // int addTab(QWidget *widget, const QString &label);

private slots:
        void onTabCloseRequested(int index);

        void onTabSaveRequested();

public:
        explicit TabWidget(QWidget *parent);

        int addEditor(CodeEditor *editor, const QIcon &icon, const QString &label);

        CodeEditor *getCurrentEditor();
};

#endif //TAB_H
