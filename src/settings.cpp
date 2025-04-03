#include "settings.h"

#include "../ui/ui_settings.h"

Settings::Settings(QWidget *parent) : QDialog(parent), ui(new Ui::Settings) {
        ui->setupUi(this);

        setWindowTitle("Settings");

        ui->stackedWidget->removeWidget(ui->page);
        ui->stackedWidget->removeWidget(ui->page_2);

        QString value = settings.value("labelownia").toString();

        QWidget *firstPageWidget = new QWidget();
        QVBoxLayout *firstPageLayout = new QVBoxLayout();
        firstPageLayout->addWidget(new QLabel(value));
        text_box = new QLineEdit();
        firstPageLayout->addWidget(text_box);
        QPushButton *ok_button = new QPushButton("Save this one!");
        firstPageLayout->addWidget(ok_button);
        firstPageLayout->addWidget(new QRadioButton("Are you sure?"));
        firstPageWidget->setLayout(firstPageLayout);

        QWidget *secondPageWidget = new QWidget();

        QStackedWidget *stackedWidget = ui->stackedWidget;
        stackedWidget->addWidget(firstPageWidget);
        stackedWidget->addWidget(secondPageWidget);


        QListWidget *listView = ui->listWidget;
        new QListWidgetItem(tr("Page 1"), listView);
        new QListWidgetItem(tr("Page 2"), listView);

        connect(listView, &QListWidget::currentRowChanged, ui->stackedWidget, &QStackedWidget::setCurrentIndex);
        connect(ok_button, &QPushButton::clicked, this, &Settings::ok_button_clicked);
}

Settings::~Settings() {
        delete ui;
}
