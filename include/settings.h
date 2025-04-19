#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QStackedWidget>
#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>
#include <QSettings>
#include <QPushButton>

enum Shortcut {
        // Actions
        CompileShortcut,
        FormatShortcut,

        // Tabs
        MoveTabLeftShortcut,
        MoveTabRightShortcut,
        CloseTabShortcut,

        // Editor
        MoveLineUpShortcut,
        MoveLineDownShortcut,
        DeleteLineShortcut,
        CommentLineShortcut,
        DuplicateLineShortcut,
};

inline QString shortcutAsString(const Shortcut shortcut) {
        switch (shortcut) {
                //Actions
                case CompileShortcut:
                        return "CompileShortcut";
                case FormatShortcut:
                        return "FormatShortcut";

                // Tabs
                case MoveTabLeftShortcut:
                        return "MoveTabLeftShortcut";
                case MoveTabRightShortcut:
                        return "MoveTabRightShortcut";
                case CloseTabShortcut:
                        return "CloseTabShortcut";

                // Editor
                case MoveLineUpShortcut:
                        return "MoveLineUpShortcut";
                case MoveLineDownShortcut:
                        return "MoveLineDownShortcut";
                case DeleteLineShortcut:
                        return "DeleteLineShortcut";
                case CommentLineShortcut:
                        return "CommentLineShortcut";
                case DuplicateLineShortcut:
                        return "DuplicateLineShortcut";

                default:
                        return "";
        }
}


namespace Ui {
        class Settings;
}

class Settings : public QDialog {
        Q_OBJECT

public:
        explicit Settings(QWidget *parent = nullptr);

        ~Settings();

private slots:
        void ok_button_clicked() {
                QString value = compiler_path->text();

                settings.setValue("compiler_path", value);
        }

        void on_buttonBox_accepted();

private:
        void restoreShortcutsToDefault();

private:
        Ui::Settings *ui;
        QSettings settings;
        QLineEdit *compiler_path;
};

#endif // SETTINGS_H
