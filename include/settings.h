#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QStackedWidget>
#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>
#include <QSettings>
#include <QPushButton>

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
        Ui::Settings *ui;
        QSettings settings;
        QLineEdit *compiler_path;
};

#endif // SETTINGS_H
