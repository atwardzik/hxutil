//
// Created by Artur Twardzik on 18/04/2025.
//

#include "tab_widget.h"
#include "settings.h"


TabWidget::TabWidget(QWidget *parent) : QTabWidget(parent) {
        connect(this, &QTabWidget::tabCloseRequested, this, &TabWidget::onTabCloseRequested);

        QAction *saveAction = new QAction(this);
        saveAction->setShortcut(QKeySequence::Save);
        connect(saveAction, &QAction::triggered, this, &TabWidget::onTabSaveRequested);
        this->addAction(saveAction);

        QAction *moveTabLeftAction = new QAction(this);
        moveTabLeftAction->setShortcut(getShortcutSetting(settings, Shortcut::MoveTabLeft));
        connect(moveTabLeftAction, &QAction::triggered, this, [this]() {
                if (int currentIndex = this->currentIndex(); currentIndex > 0) {
                        this->setCurrentIndex(currentIndex - 1);
                }
        });
        this->addAction(moveTabLeftAction);

        QAction *moveTabRightAction = new QAction(this);
        moveTabRightAction->setShortcut(getShortcutSetting(settings, Shortcut::MoveTabRight));
        connect(moveTabRightAction, &QAction::triggered, this, [this]() {
                if (int currentIndex = this->currentIndex(); currentIndex < this->count() - 1) {
                        this->setCurrentIndex(currentIndex + 1);
                }
        });
        this->addAction(moveTabRightAction);


        QAction *closeTabAction = new QAction(this);
        closeTabAction->setShortcut(getShortcutSetting(settings, Shortcut::CloseTab));
        connect(closeTabAction, &QAction::triggered, this, &TabWidget::onCurrentTabCloseRequested);
        this->addAction(closeTabAction);
}

int TabWidget::addEditor(CodeEditor *editor, const QIcon &icon, const QString &label) {
        QWidget *tab = new QWidget();
        QVBoxLayout *tabLayout = new QVBoxLayout(tab);

        tabLayout->setContentsMargins(0, 0, 0, 0);
        tabLayout->addWidget(editor);

        const int ret = this->addTab(tab, icon, label);
        editors[ret] = editor;

        this->setCurrentIndex(ret);

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

void TabWidget::onCurrentTabCloseRequested() {
        if (CodeEditor *currentEditor = getCurrentEditor()) {
                const QString fileName = currentEditor->saveFile();
        }

        int index = this->currentIndex();
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
