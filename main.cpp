#include "youdaowindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    YoudaoWindow w;
    w.show();
    
    return a.exec();
}
