#include "about.h"
#include <QStyle>
#include "ui_about.h"

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
    ui->pushButton->setIcon(style()->standardIcon(QStyle::SP_DialogCloseButton));
    ui->pushButton->setIconSize(QSize(fontMetrics().height(),fontMetrics().height()));
}

About::~About()
{
    delete ui;
}

void About::on_pushButton_clicked()
{
    this->close();
}
