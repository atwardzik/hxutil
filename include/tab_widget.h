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

public:
        explicit TabWidget(QWidget *parent) : QTabWidget(parent) {}

        int addEditor(CodeEditor *editor, const QIcon &icon, const QString &label) {
                QWidget *tab = new QWidget();
                QVBoxLayout *tabLayout = new QVBoxLayout(tab);

                tabLayout->setContentsMargins(0, 0, 0, 0);
                tabLayout->addWidget(editor);

                const int ret = this->addTab(tab, icon, label);
                editors[ret] = editor;

                return ret;
        }


        CodeEditor *getCurrentEditor() {
                const int currentIndex = this->currentIndex();

                if (!editors.contains(currentIndex)) {
                        return nullptr;
                }

                return editors[currentIndex];
        }
};

#endif //TAB_H
