#include "mousedebug.h"
#include "ui_mousedebug.h"

#include "hidapi.h"
#include <QDebug>
#include "usbreadthread.h"
MouseDebug::MouseDebug(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MouseDebug)
{
    HIDDeviceIsOpen = false;
    ui->setupUi(this);
    rfStatusTmr = new QTimer;
    connect(rfStatusTmr, SIGNAL(timeout()), this, SLOT(slot_rfStatusTmr()));
    rfStatusTmr->start(1000);
    ui->recvDataTextEdit->setReadOnly(true);
    connect(&usbReadThread, SIGNAL(postHIDDeviceOpen(bool)), this, SLOT(slot_getHIDDeviceIsOpen(bool)));// 接收线程传来设备是否开启的数据
    connect(&usbReadThread,SIGNAL(postRevData(QByteArray)),this,SLOT(slot_getRevData(QByteArray))); // 接收线程传来的数据
//    usbReadThread.start();
}

MouseDebug::~MouseDebug()
{
    delete ui;
}
char MouseDebug::ConvertHexChar(char ch)
{
    if((ch >= '0') && (ch <= '9'))
    {
        return ch-0x30;
    }
    else if(ch >= 'A' && ch <= 'F')
    {
        return ch-'A'+10;
    }
    else if(ch >= 'a' && ch <= 'f')
    {
        return ch-'a'+10;
    }
    else return ch-ch;
}

void MouseDebug::StringToHex(QString str, QByteArray &sendData)
{
    char hexdata,lowhexdata;
    int hexdatalen = 0;
    int len = str.length();
    sendData.resize(len/2);
    char lstr,hstr;
    for (int i=0; i<len;) {
        hstr=str[i].toLatin1();
        if(hstr == ' ')
        {
//            qDebug()<< tr("null");
            i++;
            continue;
        }
        i++;
        if(i>=len)
        {
//            qDebug()<< tr("over");
            break;
        }
        lstr = str[i].toLatin1();
        hexdata = ConvertHexChar(hstr);
        lowhexdata = ConvertHexChar(lstr);
        if((hexdata == 16) || (lowhexdata == 16))
        {
//            qDebug()<< tr("error");
            break;
        }
        else
        {
            hexdata =  hexdata *16 + lowhexdata;
        }
        i++;
//        qDebug("转化为16进制数：%d",hexdata);
        sendData[hexdatalen] = hexdata;
        hexdatalen++;
    }
    sendData.resize(hexdatalen);
//    qDebug()<<sendData.length();
}

void MouseDebug::slot_rfStatusTmr()
{
    getHIDDevceInfo();
    setComboHIDBox();
    setDisplayParams();
    clearHIDDeviceInfoList();
}

void MouseDebug::getHIDDevceInfo()
{
    struct hid_device_info *devs, *cur_dev;
     devs = hid_enumerate(0x0, 0x0);
     cur_dev = devs;

     char HID[20],product[20],manufacturer[20],releaseNum[20],interfaceNum[20],
             VID[20],PID[20],usagePage[20],usage[20];
     while (cur_dev) {
 //        qDebug("Device Found\n  type: %04hx %04hx\n  path: %s\n  serial_number: %ls", cur_dev->vendor_id, cur_dev->product_id, cur_dev->path, cur_dev->serial_number);
 //        qDebug("\n");

    sprintf(HID,"VID:%x PID:%x",cur_dev->vendor_id,cur_dev->product_id);
    sprintf(product,"%ls",cur_dev->product_string);
    sprintf(manufacturer,"%ls",cur_dev->manufacturer_string);
    sprintf(releaseNum,"%hx",cur_dev->release_number);
    sprintf(interfaceNum,"%d",cur_dev->interface_number);
    sprintf(VID,"%x",cur_dev->vendor_id);
    sprintf(PID,"%x",cur_dev->product_id);
    sprintf(usagePage,"%d",cur_dev->usage_page);
    sprintf(usage,"%d",cur_dev->usage);
    HIDDeviceList.append(QString(HID));
    productList.append(QString(product));
    manufacturerList.append(QString(manufacturer));
    releaseNumList.append(QString(releaseNum));
    interfaceNumList.append(QString(interfaceNum));
    VIDList.append(QString(VID));
    PIDList.append(QString(PID));
    usagePageList.append(QString(usagePage));
    usageList.append(QString(usage));
//    qDebug()<<HIDDeviceList;
//         qDebug("  Manufacturer: %ls\n", cur_dev->manufacturer_string);
//         qDebug("  Product:      %ls\n", cur_dev->product_string);
//         qDebug("  Release:      %hx\n", cur_dev->release_number);
//         qDebug("  Interface:    %d\n", cur_dev->interface_number);
//         qDebug("  path:    %s\n", cur_dev->path);
//         qDebug("  vendor_id:    %x\n", cur_dev->vendor_id);
//         qDebug("  product_id:    %x\n", cur_dev->product_id);
//         qDebug("  usage_page:    %d\n", cur_dev->usage_page);
//         qDebug("  usage:    %d\n", cur_dev->usage);
//         qDebug("\n");
         cur_dev = cur_dev->next;
     }
     hid_free_enumeration(devs);
}

void MouseDebug::setComboHIDBox()
{
    QStringList portItems;
    int portItemsCounts;
//    // 如果串口正在使用，然后串口定时刷新，当前串口字段依旧保留
//    if(m_serial->isOpen())
//    {
//      QString serialPort = m_serial->portName();
//      commPortList.insert(0,serialPort);
//    }
//    qDebug() << commPortList;
    if(! HIDDeviceList.isEmpty())
    {
        portItemsCounts = ui->comboHIDBox->count();
        for(int i = 0; i<portItemsCounts;i++)
        {
            portItems.append(ui->comboHIDBox->itemText(i));
        }
        if(portItems == HIDDeviceList)
        {
            return;
        }
        else
        {
          ui->comboHIDBox->clear();
          ui->comboHIDBox->addItems(HIDDeviceList);
        }
    }
    // 如果沒扫到 HID 设备，清空 HID 设备下拉框
    else
    {
        ui->comboHIDBox->clear();
    }
}

void MouseDebug::setDisplayParams()
{
    int currentIndex;
    currentIndex = ui->comboHIDBox->currentIndex();
    ui->manufacturerText->setText(manufacturerList[currentIndex]);
    ui->productText->setText(productList[currentIndex]);
    ui->releaseNumText->setText(releaseNumList[currentIndex]);
    ui->interfaceNumText->setText(interfaceNumList[currentIndex]);
    ui->VIDText->setText(VIDList[currentIndex]);
    ui->PIDText->setText(PIDList[currentIndex]);
    ui->usagePageText->setText(usagePageList[currentIndex]);
    ui->usageText->setText(usageList[currentIndex]);
}
void MouseDebug::clearHIDDeviceInfoList()
{
    HIDDeviceList.clear();
    productList.clear();
    manufacturerList.clear();
    releaseNumList.clear();
    interfaceNumList.clear();
    VIDList.clear();
    PIDList.clear();
    usagePageList.clear();
    usageList.clear();
}

unsigned short MouseDebug::HexStrToUShort(QString str, int length)
{
    // 初始化变量为 0 ，否则会由于不明因素造成数据紊乱
    unsigned short tempData = 0;
    int data = 0;
    for(int i = 0; i< length; i++)
    {
        if((str[i]>='A') && (str[i]<= 'F'))
            data = str[i].toLatin1() - 'A' +10; // 字符串转字符
        else if((str[i]>='a')&&(str[i])<='f')
            data = str[i].toLatin1() - 'a' + 10;
        else data = str[i].toLatin1()-'0';
        tempData = tempData*16+data;
    }
    return tempData;
}


void MouseDebug::on_on_offBtn_clicked()
{
    unsigned short currentPID, currentVID;

    QString currentPIDText,currentVIDText;
    currentPIDText = ui->PIDLineEdit->text();
    currentVIDText = ui->VIDLineEdit->text();
//  下拉框选择端口打开，但是打开端口后，当前的串口并不能扫描出来，已被占用，显示有问题
//    currentPIDText = ui->comboHIDBox->currentText().right(4);
//    currentVIDText = ui->comboHIDBox->currentText().mid(4,4).simplified();
//    qDebug()<<currentPIDText;
//    qDebug()<<currentVIDText;
    currentPID = HexStrToUShort(currentPIDText,currentPIDText.length());
    currentVID = HexStrToUShort(currentVIDText,currentVIDText.length());
    usbReadThread.getOpenHIDDevice(currentPID,currentVID,!HIDDeviceIsOpen);// 调用线程函数去传递数据
    usbReadThread.start();
}
// 线程传递回来的设备是否开启参数，改变 UI 的开启关闭状态，同时将设备状态保存在当前类中
void MouseDebug::slot_getHIDDeviceIsOpen(bool isOpen)
{
    HIDDeviceIsOpen = isOpen;
    if(isOpen)
    {
        ui->on_offBtn->setText("关闭");
        ui->PIDLineEdit->setEnabled(false);
        ui->VIDLineEdit->setEnabled(false);
        HIDDeviceIsOpen = true;
    }
    else
    {
        HIDDeviceIsOpen = false;
        ui->on_offBtn->setText("打开");
        ui->PIDLineEdit->setEnabled(true);
        ui->VIDLineEdit->setEnabled(true);
    }
}

void MouseDebug::slot_getRevData(QByteArray data)
{
//    qDebug()<<data;
    if(!data.isEmpty())
    {
        data = data.toHex().toUpper().simplified();
        int len = data.count()/2;
        for(int i = 1; i<len+1; i++)
        {
            data.insert(3*i-1," ");
        }
        QString strRecv = QString::fromLocal8Bit(data);
        ui->recvDataTextEdit->insertPlainText(strRecv);
        ui->recvDataTextEdit->moveCursor(QTextCursor::End);//保证 Text 数据不会自动换行
    }
}

void MouseDebug::on_clearRevDataBtn_clicked()
{
    ui->recvDataTextEdit->clear();
}

void MouseDebug::on_clearSendDataBtn_clicked()
{
    ui->sendDataTextEdit->clear();
}

void MouseDebug::on_sendDataBtn_clicked()
{
    QByteArray sendData;
    QByteArray HexBytes = ui->sendDataTextEdit->toPlainText().toLocal8Bit();
    int reportID = ui->rpIDLineEdit->text().toInt();
    int dataLength = ui->sendBytesLineEdit->text().toInt();
    // 如果发送窗口为空，则不发送
    if(HexBytes == "")
    {
        return;
    }
    StringToHex(HexBytes,sendData);
//    qDebug()<<"reportID ="<<reportID<<" datalength = "<<dataLength<<" HIDOpen = "<<HIDDeviceIsOpen<<" SendData = "<<sendData;
    if(!sendData.isEmpty()&& reportID != 0 && dataLength != 0 && HIDDeviceIsOpen == true)
    {
        usbReadThread.getSendData(sendData,dataLength,reportID);
    }
    else
    {
        ui->recvDataTextEdit->setText("参数漏啦，或者设备没打开");
    }
}
