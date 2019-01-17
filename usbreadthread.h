#ifndef USBREADTHREAD_H
#define USBREADTHREAD_H

#include <QThread>
class USBReadThread :public QThread
{
    Q_OBJECT
public:
    explicit USBReadThread(QObject *parent = nullptr);
    void getOpenHIDDevice(unsigned short PID,unsigned short VID,bool IsOpen);
public slots:

protected:
    void run();

private:
    unsigned short currentPID;
    unsigned short currentVID;
    bool HIDDeviceIsOpen; // HID 设备是否开启
    bool startHIDDeviceFlag; // 点击打开操作

signals:
    void postHIDDeviceOpen(bool open);
    void postRevData(QByteArray data);
};

#endif // USBREADTHREAD_H
