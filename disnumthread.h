#ifndef DISNUMTHREAD_H
#define DISNUMTHREAD_H

#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include "spcomm.h"
#include <QString>
#include <QDebug>

class DisNumThread : public QThread
{
    Q_OBJECT

public:
    DisNumThread();
    ~DisNumThread();
    void stop();    //用来控制线程中断

protected:
    void run();     //控制线程开启

private:
    bool stopped;      //作为开启或者关断的控制信号，后称为线程状态信号
    QMutex mutex;       //用来保护易失变量
    QString PortName;       //用来保存主线程中传递过来的串口名


    int BaudRate;       //用来保存主线程中传递过来的波特率
    int DataBits;       //用来保存主线程中传递过来的数据位
    int StopBits;       //用来保存主线程中传递过来的停止位
    int Parity;         //用来保存主线程中传递过来的奇偶校验位


    SPComm Comm;        //串口类实例化以使用成员函数
    char room;          //用来保存从串口中传递出来的单个字符
    signed int readdatareturn;      //串口读数据的返回值，用来显示串口读数据的状态
    QString dataNum;        //用来保存串口中读出来的数据（不是单个字符，由单个字符组成）
    //QString dataStr;
    int MiddledataNum;      //用来转化的中间变量

public slots:
    void acceptdataslot(QString portname,int baudrate,int databits,int stopbits,int parity);
    void availableportdetectslot();
    void SendNumtoportslot(char* data,int i);

signals:
    void checkopen(bool justic);
    void SendString(QString PortMessageString);
    void SendInt(int PortMessageInt);
    void wrongdetectone();
    void wrongdetecttwo();
    void availableportname(QString str);


};

#endif // DISNUMTHREAD_H
