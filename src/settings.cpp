#include "settings.h"

#include "../ui/ui_settings.h"

Settings::Settings(QWidget *parent) : QDialog(parent), ui(new Ui::Settings) {
        ui->setupUi(this);

        setWindowTitle("Settings");

        ui->stackedWidget->removeWidget(ui->page);
        ui->stackedWidget->removeWidget(ui->page_2);

        QWidget *firstPageWidget = new QWidget();
        QVBoxLayout *firstPageLayout = new QVBoxLayout();

        compiler_path = new QLineEdit(settings.value("compiler_path").toString());
        firstPageLayout->addWidget(compiler_path);
        firstPageWidget->setLayout(firstPageLayout);

        QWidget *secondPageWidget = new QWidget();

        QStackedWidget *stackedWidget = ui->stackedWidget;
        stackedWidget->addWidget(firstPageWidget);
        stackedWidget->addWidget(secondPageWidget);


        QListWidget *listView = ui->listWidget;
        new QListWidgetItem(tr("Page 1"), listView);
        new QListWidgetItem(tr("Page 2"), listView);

        connect(listView, &QListWidget::currentRowChanged, ui->stackedWidget, &QStackedWidget::setCurrentIndex);
        // connect(ok_button, &QPushButton::clicked, this, &Settings::ok_button_clicked);
}

Settings::~Settings() {
        delete ui;
}

void Settings::on_buttonBox_accepted() {
        settings.setValue("compiler_path", compiler_path->text());
}
