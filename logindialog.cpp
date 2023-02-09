#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_pushButton_login_clicked()
{
    qDebug() << "Login";

    QString name = ui->textEdit_username->toPlainText();
    emit login(name);//发送信号
    this->close();

}

void LoginDialog::on_pushButton_cancel_clicked()
{
    this->close();
}


