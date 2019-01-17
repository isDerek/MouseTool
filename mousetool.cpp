#include "mousetool.h"
#include "ui_mousetool.h"
// 添加 Qss 文件
#include <QFile>
MouseTool::MouseTool(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MouseTool)
{
    ui->setupUi(this);
    initGUI();
}

MouseTool::~MouseTool()
{
    delete ui;
}
void MouseTool::initGUI()
{
    qssFile = new QFile(":/qss/qss/mainWindows.qss",this);
    // 只读方式打开该文件
    qssFile->open(QFile::ReadOnly);
    // 读取文件全部内容
    QString styleSheet = QString(qssFile->readAll());
    // 为 QApplication 设置样式表
    qApp->setStyleSheet(styleSheet);
    qssFile->close();
}


void MouseTool::on_usbDebugBtn_clicked()
{
    mousedebug->show();
}
