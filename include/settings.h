#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QStackedWidget>
#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>
#include <QSettings>
#include <QPushButton>
#include <QListWidget>
#include <QSpinBox>

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
        void on_buttonBox_accepted();

private:
        void setupPathsPage();

        void setupStylePage();

        void restoreShortcutsToDefault();

        void addPathLayout(QLayout *layout, QLineEdit *&path,
                                         const QString &labelText = "", const QString &setting = ""
        );

private:
        Ui::Settings *ui;
        QSettings settings;
        QStackedWidget *stackedWidget;
        QListWidget *listView;

        QLineEdit *cCompilerPath;
        QLineEdit *asmCompilerPath;
        QLineEdit *includePath;
        QLineEdit *ctagsPath;
        QLineEdit *clangPath;
        QLineEdit *clangFormatPath;
        QLineEdit *clangFormatConfigurationFile;

        QLineEdit *editorFont;
        QSpinBox *fontSize;
        QSpinBox *indent;
};

#endif // SETTINGS_H
