#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "credentials.h"
#include "qlineedit.h"
#include <QFile>
#include <openssl/evp.h>
#include <QBuffer>
#include <QCryptographicHash>
#include <QGuiApplication>
#include <QClipboard>

//пин - 1995

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QObject::connect(ui->lineEdit, &QLineEdit::textEdited,
        this, &MainWindow::filterListWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Функция считывает учетные записи из файла JSON
bool MainWindow::readJSON(const QByteArray &aes256_key)
{
    QFile jsonFile("creden_enc.json");
    jsonFile.open(QFile::ReadOnly);
    if(!jsonFile.isOpen())
        return false;

    QByteArray hexEncryptedBytes = jsonFile.readAll();
    QByteArray encryptedBytes = QByteArray::fromHex(hexEncryptedBytes);
    QByteArray decryptedBytes;

    int ret_code = decryptByteArray(aes256_key, encryptedBytes, decryptedBytes);

    QJsonParseError jsonErr;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(decryptedBytes, &jsonErr);
    if(jsonErr.error != QJsonParseError::NoError)
        return false;

    QJsonObject rootObject = jsonDoc.object();
    m_jsonarray = rootObject["cridentials"].toArray();
    jsonFile.close();
    return true;
}

//Функция отображения виджетов и фильтрации
void MainWindow::filterListWidget(const QString &searchString)
{
    ui->listWidget->clear();
    for(int i = 0; i < m_jsonarray.size(); i++) {

        if(m_jsonarray[i].toObject()["site"]
                .toString().toLower()
                .contains(searchString.toLower())
                || searchString.isEmpty())
        {
            QListWidgetItem * newItem = new QListWidgetItem();
            credentials * itemWidget = new credentials(m_jsonarray[i].toObject()["site"].toString(),
                    m_jsonarray[i].toObject()["logpass"].toString(), i);
            QObject::connect(itemWidget, &credentials::decryptLoginPassword, this, &MainWindow::decryptLoginPassword);

            newItem->setSizeHint(itemWidget->sizeHint());
            ui->listWidget->addItem(newItem);
            ui->listWidget->setItemWidget(newItem, itemWidget);
        }

    }
}

//Функция расшифрования, возвращает int, для возврата кода ошибки
int MainWindow::decryptByteArray(
    const QByteArray &aes256_key,
    const QByteArray &encryptedBytes,
    QByteArray &decryptedBytes
   )
{
    //https://cryptii.com/pipes/aes-encryption
    unsigned char key[32] = {0};
    memcpy(key, aes256_key.data(), 32);

    QByteArray iv_hex("3f82004ab9a64cfac1fdeeee43ea43ce");
    QByteArray iv_ba = QByteArray::fromHex(iv_hex);

    unsigned char iv[16] = {0};
    memcpy(iv, iv_ba.data(), 16);

    EVP_CIPHER_CTX *ctx; //контекст
    ctx = EVP_CIPHER_CTX_new();
    if (!EVP_DecryptInit_ex2(ctx, EVP_aes_256_cbc(), key, iv, NULL)) {
        qDebug() << "*** EVP_DecryptInit_ex2() ERROR ";
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }
    qDebug() << "*** EVP_DecryptInit_ex2() OK ";
    #define BUF_LEN 256
    unsigned char encrypted_buf[BUF_LEN] = {0}, decrypted_buf[BUF_LEN] = {0};
    int encr_len, decr_len;

    QDataStream encrypted_stream(encryptedBytes);
    decryptedBytes.clear();

    QBuffer decrypted_buffer(&decryptedBytes);
    decrypted_buffer.open(QIODevice::WriteOnly);
    encr_len = encrypted_stream.readRawData(reinterpret_cast<char*>(encrypted_buf), BUF_LEN);
    while(encr_len > 0)
    {
        if (!EVP_DecryptUpdate(ctx,
                               decrypted_buf, &decr_len,
                               encrypted_buf, encr_len)) {
            /* Error */
            EVP_CIPHER_CTX_free(ctx);
            return -1;
        }
        decrypted_buffer.write(reinterpret_cast<char*>(decrypted_buf), decr_len);
        encr_len = encrypted_stream.readRawData(reinterpret_cast<char*>(encrypted_buf), BUF_LEN);
    };
    int tmplen = 0;
    if (!EVP_DecryptFinal_ex(ctx, decrypted_buf, &tmplen)) {
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }
    decrypted_buffer.write(reinterpret_cast<char*>(decrypted_buf), tmplen);

    decrypted_buffer.close();

    EVP_CIPHER_CTX_free(ctx);
    return 0;
}

//Функция обработки ввода пин-кода
void MainWindow::on_editPin_returnPressed()
{
    QByteArray hash = QCryptographicHash::hash(ui->editPin->text().toUtf8(),
                             QCryptographicHash::Sha256);
    if(m_isStartup) {
        if(readJSON(hash)) {
            ui->stackedWidget->setCurrentIndex(1);
            filterListWidget("");
            m_isStartup = false;
        }else {
            ui->labelLogin->setText("Неверный пин");
            ui->labelLogin->setStyleSheet("color:red;");
        }
    }
    else{
        QByteArray encrypted_creds = QByteArray::fromHex(
                    m_jsonarray[m_current_id].toObject()["logpass"].toString().toUtf8());
        QByteArray decrypted_creds;

        decryptByteArray(hash, encrypted_creds, decrypted_creds);

        QJsonObject jsonCopy = QJsonDocument::fromJson(decrypted_creds).object();
        if (m_field) {
            QGuiApplication::clipboard()->setText(jsonCopy["password"].toString());
        } else {
            QGuiApplication::clipboard()->setText(jsonCopy["login"].toString());
        }
        ui->stackedWidget->setCurrentIndex(1);
    }
    ui->editPin->setText(QString().fill('*', ui->editPin->text().size()));
    ui->editPin->clear();

    hash.setRawData(const_cast<char*>(QByteArray().fill('*', 32).data()), 32);
    hash.clear();
}

//Функция обработки сигнала при нажатии на кнопку копирования
void MainWindow::decryptLoginPassword(int id, credentials::FIELD field)
{
    m_field = field;
    m_current_id = id;
    ui->stackedWidget->setCurrentIndex(0);
}
