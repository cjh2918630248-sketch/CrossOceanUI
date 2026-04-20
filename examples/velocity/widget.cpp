#include "widget.h"
#include "ui_widget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    Init();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::Init()
{
    ui->SpeedGuageWidget->SetVelocity(0);
}

void Widget::on_speed_lineedit_returnPressed()
{
    ui->SpeedGuageWidget->SetVelocity(ui->speed_lineedit->text().toDouble());
}

