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
        Compile,
        Format,

        // Tabs
        MoveTabLeft,
        MoveTabRight,
        CloseTab,

        // Editor
        MoveLineUp,
        MoveLineDown,
        DeleteLine,
        CommentLine,
        DuplicateLine,
};

inline QString shortcutAsString(const Shortcut shortcut) {
        switch (shortcut) {
                //Actions
                case Compile:
                        return "CompileShortcut";
                case Format:
                        return "FormatShortcut";

                // Tabs
                case MoveTabLeft:
                        return "MoveTabLeftShortcut";
                case MoveTabRight:
                        return "MoveTabRightShortcut";
                case CloseTab:
                        return "CloseTabShortcut";

                // Editor
                case MoveLineUp:
                        return "MoveLineUpShortcut";
                case MoveLineDown:
                        return "MoveLineDownShortcut";
                case DeleteLine:
                        return "DeleteLineShortcut";
                case CommentLine:
                        return "CommentLineShortcut";
                case DuplicateLine:
                        return "DuplicateLineShortcut";

                default:
                        return "";
        }
}

inline QString getSetting(const QSettings &settings, const QString &setting) {
        return settings.value(setting).toString();
}

inline QKeySequence getShortcutSetting(const QSettings &settings, const Shortcut &shortcut) {
        return QKeySequence(settings.value(shortcutAsString(shortcut)).toString());
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
                QString value = compilerPath->text();

                settings.setValue("compiler_path", value);
        }

        void on_buttonBox_accepted();

private:
        void restoreShortcutsToDefault();

private:
        Ui::Settings *ui;
        QSettings settings;
        QLineEdit *compilerPath;
        QLineEdit *clangPath;
        QLineEdit *includePath;
        QLineEdit *clangFormatPath;
};

#endif // SETTINGS_H
