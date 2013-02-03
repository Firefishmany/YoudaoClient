#include "youdaowindow.h"
#include "ui_youdaowindow.h"

YoudaoWindow::YoudaoWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::YoudaoWindow),
    nam(new QNetworkAccessManager(this))
{
    ui->setupUi(this);
    connect (nam, SIGNAL(finished(QNetworkReply*)), SLOT(finished(QNetworkReply*)));

    ui->textBrowser->document()->setDefaultStyleSheet(getYouDaoStyleSheet());

    on_pushButton_clicked();
}

YoudaoWindow::~YoudaoWindow()
{
    delete ui;
}

void YoudaoWindow::on_pushButton_clicked()
{
    const QString & word = ui->lineEdit->text();
    if (cache.contains(word))
    {
        ui->textBrowser->setHtml(cache.value(word));
        return;
    }

    QUrl url = QUrl::fromEncoded("http://fanyi.youdao.com/openapi.do?"
                                 "type=data&doctype=jsonp&version=1.1"
                                 "&relatedUrl=http%3A%2F%2Ffanyi.youdao.com"
                                 "%2Fopenapi%3Fpath%3Dweb-mode%26mode%3Ddicter&keyfrom=test"
                                 "&key=null&callback=a&translate=on");

    url.addQueryItem("ts", QString::number(QDateTime::currentMSecsSinceEpoch()));
    url.addQueryItem("q", word);

    nam->get (QNetworkRequest (url));
}

QString YoudaoWindow::getYouDaoStyleSheet()
{
    QFile fp (":/style.css");
    if (fp.open(QIODevice::ReadOnly))
    {
        return QString::fromUtf8(fp.readAll());
    }

    return QString ();
}

void YoudaoWindow::setHash (const QString &word, const QString &explain)
{
    if (lru.size() > 50)
    {
        cache.remove(lru.first());
        lru.pop_front();
    }

    cache.insert(word, explain);
}

void YoudaoWindow::finished(QNetworkReply *reply)
{
    const int & httpStatus = reply->attribute
            (QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QByteArray data = reply->readAll();
    const QUrl & url = reply->url();
    const QString & urlStr = url.toString();

    if (httpStatus < 200 || httpStatus > 400)
    {
        qDebug() << "Error: " << urlStr << httpStatus;
        return;
    }

    if (urlStr.startsWith("http://fanyi.youdao.com/openapi.do"))
    {
        QJson::Parser parser;
        bool ok = false;
        QByteArray json = data; json.chop(2); json.remove(0, 2);
        QString html;

        QVariant result = parser.parse(json, &ok);
        if (ok)
        {
            QVariantMap basic = result.toMap().value("basic").toMap();
            QVariantList explains = basic.value("explains").toList();
            const QString & word = result.toMap().value("query").toString();

            html += QString ("<h2 id=\"title\">%1<span id=\"phonetic\">"
                             "&nbsp;&nbsp;/%2/</span></h2>")
                    .arg(word)
                    .arg(basic.value("phonetic").toString());

            html += "<div id=\"basic\">";

            foreach (const QVariant & explain, explains)
            {
                html += QString("<p>%1</p>").arg(explain.toString());
            }

            html += "</div>";

            html = "<div id=\"youdaoDictCenter\"><div id=\"result\">"
                    + html + "</div></div>";

            setHash(word, html);

            ui->textBrowser->setHtml(html);
        }
        else
        {
            qDebug() << "Parse error";
            qDebug() << data;
        }
    }

}

void YoudaoWindow::on_lineEdit_returnPressed()
{
    on_pushButton_clicked();
}
