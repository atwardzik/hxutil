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
                QString value = text_box->text();

                settings.setValue("labelownia", value);
        }

private:
        Ui::Settings *ui;
        QSettings settings;
        QLineEdit *text_box;
};

#endif // SETTINGS_H
