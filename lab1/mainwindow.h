#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QJsonArray>
#include <QMainWindow>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>
#include "credentials.h"

QT_BEGIN_NAMESPACE //это макросы, используемые в Qt для определения пространства имен Qt.
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool readJSON(const QByteArray &aes256_key);

public slots:
    int decryptByteArray(const QByteArray &aes256_key,
                    const QByteArray &encryptedBytes, QByteArray &decryptedBytes);
    void filterListWidget(const QString &searchString);

private slots:

    void on_editPin_returnPressed();
    void decryptLoginPassword(int id, credentials::FIELD field);

private:
    Ui::MainWindow *ui;
    QJsonArray m_jsonarray;
    bool m_isStartup = true;
    bool m_field = 0;
    int m_current_id = -1;
};
#endif // MAINWINDOW_H
