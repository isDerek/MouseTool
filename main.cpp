#include "mousetool.h"
#include <QApplication>
#include <QStyleFactory> // 添加样式
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle(QStyleFactory::create("fusion"));
    MouseTool w;
    w.show();

    return a.exec();
}
