#include "settings.h"

#include "../ui/ui_settings.h"

Settings::Settings(QWidget *parent) : QDialog(parent), ui(new Ui::Settings) {
        ui->setupUi(this);

        setWindowTitle("Settings");

        ui->stackedWidget->removeWidget(ui->page);
        ui->stackedWidget->removeWidget(ui->page_2);

        QWidget *firstPageWidget = new QWidget();
        QVBoxLayout *firstPageLayout = new QVBoxLayout();

        compilerPath = new QLineEdit(settings.value("CompilerPath").toString());
        firstPageLayout->addWidget(compilerPath);

        clangFormatPath = new QLineEdit(settings.value("clang-formatPath").toString());
        firstPageLayout->addWidget(clangFormatPath);
        firstPageWidget->setLayout(firstPageLayout);


        QWidget *secondPageWidget = new QWidget();
        QVBoxLayout *secondPageLayout = new QVBoxLayout;
        QPushButton *restoreToDefaultBtn = new QPushButton("Restore default keymap");
        secondPageLayout->addWidget(restoreToDefaultBtn);
        secondPageWidget->setLayout(secondPageLayout);

        QStackedWidget *stackedWidget = ui->stackedWidget;
        stackedWidget->addWidget(firstPageWidget);
        stackedWidget->addWidget(secondPageWidget);


        QListWidget *listView = ui->listWidget;
        auto *first = new QListWidgetItem(tr("Compilers"), listView);
        new QListWidgetItem(tr("Shortcuts"), listView);
        listView->setCurrentItem(first);

        connect(listView, &QListWidget::currentRowChanged, ui->stackedWidget, &QStackedWidget::setCurrentIndex);
        connect(restoreToDefaultBtn, &QPushButton::clicked, this, &Settings::restoreShortcutsToDefault);
}

Settings::~Settings() {
        delete ui;
}

void Settings::restoreShortcutsToDefault() {
        // Actions
        settings.setValue(shortcutAsString(Shortcut::Compile),
                          QKeySequence(Qt::CTRL | Qt::Key_B));
        settings.setValue(shortcutAsString(Shortcut::Format),
                          QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_L));

        // Tabs
        settings.setValue(shortcutAsString(Shortcut::MoveTabLeft),
                          QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_BracketLeft));
        settings.setValue(shortcutAsString(Shortcut::MoveTabRight),
                          QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_BracketRight));
        settings.setValue(shortcutAsString(Shortcut::CloseTab),
                          QKeySequence(Qt::CTRL | Qt::Key_W));

        // Editor
        settings.setValue(shortcutAsString(Shortcut::MoveLineUp),
                          QKeySequence(Qt::ALT | Qt::SHIFT | Qt::Key_Up));
        settings.setValue(shortcutAsString(Shortcut::MoveLineDown),
                          QKeySequence(Qt::ALT | Qt::SHIFT | Qt::Key_Down));
        settings.setValue(shortcutAsString(Shortcut::DeleteLine),
                          QKeySequence(Qt::CTRL | Qt::Key_Backspace));
        settings.setValue(shortcutAsString(Shortcut::CommentLine),
                          QKeySequence(Qt::CTRL | Qt::Key_Slash));
        settings.setValue(shortcutAsString(Shortcut::DuplicateLine),
                          QKeySequence(Qt::CTRL | Qt::Key_D));
}

void Settings::on_buttonBox_accepted() {
        settings.setValue("CompilerPath", compilerPath->text());
        settings.setValue("clang-formatPath", clangFormatPath->text());
}
