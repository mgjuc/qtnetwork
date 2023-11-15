#include <QCoreApplication>
#include "basehttpapi.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    BaseHttpAPI httpClient = new BaseHttpAPI(&a);
    qDebug()<<"start get request...";
    httpClient.lambget("http://cn.bing.com");

    return a.exec();
}
