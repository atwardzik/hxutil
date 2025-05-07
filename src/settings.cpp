#include "settings.h"

#include <filesystem>
#include <format>
#include "../ui/ui_settings.h"

Settings::Settings(QWidget *parent) : QDialog(parent), ui(new Ui::Settings) {
        ui->setupUi(this);

        setWindowTitle("Settings");

        ui->stackedWidget->removeWidget(ui->page);
        ui->stackedWidget->removeWidget(ui->page_2);


        QWidget *secondPageWidget = new QWidget();
        QVBoxLayout *secondPageLayout = new QVBoxLayout;
        QPushButton *restoreToDefaultBtn = new QPushButton("Restore default keymap");
        secondPageLayout->addWidget(restoreToDefaultBtn);
        secondPageWidget->setLayout(secondPageLayout);

        stackedWidget = ui->stackedWidget;
        listView = ui->listWidget;
        setupPathsPage();
        setupStylePage();
        stackedWidget->addWidget(secondPageWidget);


        new QListWidgetItem(tr("Shortcuts"), listView);

        connect(listView, &QListWidget::currentRowChanged, ui->stackedWidget, &QStackedWidget::setCurrentIndex);
        connect(restoreToDefaultBtn, &QPushButton::clicked, this, &Settings::restoreShortcutsToDefault);
}

Settings::~Settings() {
        delete ui;
}

void Settings::addPathLayout(QLayout *layout, QLineEdit *&path,
                             const QString &labelText,
                             const QString &setting
) {
        QString currentPath = settings.value(setting).toString();
        path = new QLineEdit(currentPath);


        QLabel *label = new QLabel(labelText);
        if (!std::filesystem::exists(currentPath.toStdString())
            && system(std::format("which {} > /dev/null 2>&1", currentPath.toStdString()).c_str())) {
                label->setText(labelText + "<span style='color: red';>(Invalid)</span>");
        }
        layout->addWidget(label);
        layout->addWidget(path);
}

void Settings::setupPathsPage() {
        QWidget *widget = new QWidget();
        QVBoxLayout *layout = new QVBoxLayout();

        addPathLayout(layout, cCompilerPath,
                      "Default C compiler path: ", "cCompilerPath");

        addPathLayout(layout, asmCompilerPath,
                      "Default ASM compiler path: ", "asmCompilerPath");

        addPathLayout(layout, includePath,
                      "Include path: ", "IncludePath");

        addPathLayout(layout, ctagsPath,
                      "ctags path: ", "ctagsPath");

        addPathLayout(layout, clangPath,
                      "Clang path: ", "clangPath");

        addPathLayout(layout, clangFormatPath,
                      "clang-format path: ", "clang-formatPath");

        addPathLayout(layout, clangFormatConfigurationFile,
                      "clang-format configuration file path: ", "clang-formatConfig");


        widget->setLayout(layout);
        stackedWidget->addWidget(widget);

        auto *first = new QListWidgetItem(tr("Paths"), listView);
        listView->setCurrentItem(first);
}

void Settings::setupStylePage() {
        QWidget *widget = new QWidget();
        QVBoxLayout *layout = new QVBoxLayout();

        addPathLayout(layout, editorFont,
                      "Default editor font: ", "EditorFont");

        QHBoxLayout *indentLayout = new QHBoxLayout;
        QLabel *label = new QLabel("Indentation size in spaces: ");
        indent = new QSpinBox();
        indent->setValue(settings.value("IndentSize").toInt());
        indentLayout->addWidget(label);
        indentLayout->addWidget(indent);

        layout->addLayout(indentLayout);

        widget->setLayout(layout);
        stackedWidget->addWidget(widget);

        new QListWidgetItem(tr("Editor"), listView);
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
        settings.setValue("cCompilerPath", cCompilerPath->text());
        settings.setValue("asmCompilerPath", asmCompilerPath->text());
        settings.setValue("IncludePath", includePath->text());
        settings.setValue("ctagsPath", ctagsPath->text());
        settings.setValue("clangPath", clangPath->text());
        settings.setValue("clang-formatPath", clangFormatPath->text());
        settings.setValue("clang-formatConfig", clangFormatConfigurationFile->text());
        settings.setValue("EditorFont", editorFont->text());
        settings.setValue("IndentSize", indent->value());
}
