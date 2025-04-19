//
// Created by Artur Twardzik on 18/04/2025.
//

#include "tab_widget.h"


TabWidget::TabWidget(QWidget *parent) : QTabWidget(parent) {
        connect(this, &QTabWidget::tabCloseRequested, this, &TabWidget::onTabCloseRequested);

        QAction *action = new QAction(this);
        action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));
        connect(action, &QAction::triggered, this, &TabWidget::onTabSaveRequested);
        this->addAction(action);
}

int TabWidget::addEditor(CodeEditor *editor, const QIcon &icon, const QString &label) {
        QWidget *tab = new QWidget();
        QVBoxLayout *tabLayout = new QVBoxLayout(tab);

        tabLayout->setContentsMargins(0, 0, 0, 0);
        tabLayout->addWidget(editor);

        const int ret = this->addTab(tab, icon, label);
        editors[ret] = editor;

        return ret;
}

CodeEditor *TabWidget::getCurrentEditor() {
        const int currentIndex = this->currentIndex();

        if (!editors.contains(currentIndex)) {
                return nullptr;
        }

        return editors[currentIndex];
}

void TabWidget::onTabCloseRequested(int index) {
        if (editors.contains(index)) {
                editors[index]->saveFile();
        }

        QWidget *widget = this->widget(index);
        this->removeTab(index);
        delete widget;
}

void TabWidget::onTabSaveRequested() {
        if (CodeEditor *currentEditor = getCurrentEditor()) {
                const QString fileName = currentEditor->saveFile();

                this->setTabText(this->currentIndex(), fileName);
        }
}
