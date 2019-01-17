#ifndef MOUSEDEBUG_H
#define MOUSEDEBUG_H

#include <QMainWindow>
#include <QThread>
#include <QObject>
#include <QTimer>

#include "usbreadthread.h"
namespace Ui {
class MouseDebug;
}

class MouseDebug : public QMainWindow
{
    Q_OBJECT

public:
    explicit MouseDebug(QWidget *parent = nullptr);
    ~MouseDebug();
    void getHIDDevceInfo(); // 将 HID 设备信息添加进 List
    void setComboHIDBox(); // 将 HID 信息写入设备下拉框
    void clearHIDDeviceInfoList(); // 清空 HID 设备信息 List
    void setDisplayParams(); // 设置显示参数
    unsigned short HexStrToUShort(QString str, int length); // 16进制字符串转 Ushort
private:
    Ui::MouseDebug *ui;
    QStringList HIDDeviceList; // HID 设备显示
    QStringList productList; // 产品信息显示
    QStringList manufacturerList; // 厂商信息显示
    QStringList releaseNumList;// 发布号显示
    QStringList interfaceNumList; // 接口编号显示
    QStringList VIDList; // VID 显示
    QStringList PIDList; // PID 显示
    QStringList usagePageList; // 使用页数显示
    QStringList usageList;  // 使用页显示
    QTimer *rfStatusTmr; // 刷新状态栏，自动刷新端口定时器
    USBReadThread usbReadThread; // 读取 HID 设备消息线程
    bool HIDDeviceIsOpen; // hid 设备是否开启
private slots:
    void slot_rfStatusTmr();
    void slot_getHIDDeviceIsOpen(bool);
    void on_on_offBtn_clicked();
    void slot_getRevData(QByteArray data);
    void on_clearRevDataBtn_clicked();
    void on_clearSendDataBtn_clicked();
    void on_sendDataBtn_clicked();
};

#endif // MOUSEDEBUG_H
