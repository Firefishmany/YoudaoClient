#ifndef YOUDAOWINDOW_H
#define YOUDAOWINDOW_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDateTime>
#include <QFile>

#include <QDebug>
#include <qjson/parser.h>

namespace Ui {
class YoudaoWindow;
}

class YoudaoWindow : public QWidget
{
    Q_OBJECT
    
public:
    explicit YoudaoWindow(QWidget *parent = 0);
    ~YoudaoWindow();

    QString getYouDaoStyleSheet ();
    
private slots:
    void on_pushButton_clicked();
    void finished (QNetworkReply *reply);

    void on_lineEdit_returnPressed();

private:
    Ui::YoudaoWindow *ui;
    QNetworkAccessManager *nam;

    QHash<QString, QString> cache;
    QList<QString> lru;

    void setHash (const QString & word, const QString & explain);
};

#endif // YOUDAOWINDOW_H
