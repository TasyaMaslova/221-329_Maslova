#ifndef CREDENTIALS_H
#define CREDENTIALS_H

#include <QWidget>

namespace Ui {
class credentials;
}

class credentials : public QWidget
{
    Q_OBJECT

public:
    explicit credentials(const QString&site, const QString &loginpass, const int id,  QWidget *parent = nullptr);

    ~credentials();
    enum FIELD {
        LOGIN, PASSWORD
    };

private slots:
    void on_btnLoginCopy_clicked();
    void on_btnPassCopy_clicked();

signals:
    void decryptLoginPassword(int id, FIELD field);

private:
    Ui::credentials *ui;
    int m_id = -1;
};

#endif // CREDENTIALS_H
