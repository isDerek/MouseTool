#include "usbreadthread.h"
#include <QThread>
#include <QDebug>
#include "hidapi.h"


static int res;
static hid_device *handle;

USBReadThread::USBReadThread(QObject *parent):QThread (parent)
{
    startHIDDeviceFlag = false;
}

void USBReadThread::run()
{

    unsigned char buf[64];
    QByteArray revData;
    while(1)
    {
//        unsigned char sendBuf[65];
//        sendBuf[0]= 0x03;
//        for(int a = 1; a< 64; a++)
//        {
//            sendBuf[a] = a;
//        }
//        hid_write(handle,sendBuf,65);
        //如果设备存在
        if(handle)
        {
            res = hid_read(handle,buf,64);
            revData.resize(64);
            for(int i=0; i<64; i++)
            {
                revData[i] = buf[i];
            }
            emit postRevData(revData);
        }
    }
}

void USBReadThread::getOpenHIDDevice(unsigned short PID, unsigned short VID, bool isOpen)
{
    currentPID = PID;
    currentVID = VID;
    startHIDDeviceFlag = isOpen;
    qDebug()<<"开启设备 Flag"<<isOpen;
//    qDebug()<<currentPID;
//    qDebug()<<currentVID;
    if(isOpen)
    {
        handle = hid_open(currentVID,currentPID,nullptr);
        qDebug()<<handle;
        if(!handle)
        {
            emit postHIDDeviceOpen(false);
            return;
        }
        emit postHIDDeviceOpen(true);
        qDebug()<<"设备已开启";
    //    res = hid_set_nonblocking(handle,0);
    }
    else
    {
        qDebug()<<"设备已关闭";
        if(handle)
        {
           hid_close(handle);
        }
        emit postHIDDeviceOpen(false);
    }
}
