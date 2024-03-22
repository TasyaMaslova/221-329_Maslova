#include "credentials.h"
#include "ui_credentials.h"

credentials::credentials(const QString &site, const QString &login, const int id, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::credentials),
    m_id(id)
{
    ui->setupUi(this);
    QPixmap pix("../lab1/images/parole2.png");
    QPixmap pixCopy("../lab1/images/copy.png");
    ui->btnLoginCopy->setIcon(pixCopy);
    ui->btnPassCopy->setIcon(pixCopy);
    ui->icon->setPixmap(pix);
    ui->label->setText(site);
    ui->login->setText(login);
    ui->password->setText(login);
}

credentials::~credentials()
{
    delete ui;
}

void credentials::on_btnLoginCopy_clicked()
{
    emit decryptLoginPassword(m_id, FIELD::LOGIN);
}


void credentials::on_btnPassCopy_clicked()
{
    emit decryptLoginPassword(m_id, FIELD::PASSWORD);
}

