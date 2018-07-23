#ifndef SPCOMM_H
#define SPCOMM_H
/*/////////////////////////
 * 类名：SPComm
 * 功能：提供一个支持串口操作的类，可以基于这个类进行后续的开发。
 *      调用前,需要先调用相应的函数来设置串口名字和波特率,数据位，停止位，奇偶校验率。默认
 *      无控制流默认读缓存大小1024字节，默认双向打开即可读可写。必须先设置串口名才能打开串
 *      口。
 *      调用open函数时返回true则说明串口打开成功，返回false则说明打开失败
 *      调用clear函数时返回true则说明重启清除数据成功，返回false说明串口未打开或打开失败
 *      调用readData函数时返回-1说明有错误发生，比如读只写的文件，返回-2说明没有数据可读
 *                      或者说数据已经读完，如果数组中有数据那么说明数据已经读完，如果是
 *                      空数组说明串口中没有数据。返回一个非零的正数说明读成功，返回的是
 *                      最大可读个数，说明之后可能还有数据未读。
 *      调用writeData函数时返回-1说明写错误。如果返回其他数说明写未完成，如果返回数组的最
 *                       大数则说明写完成。
 *      调用getavailablePort函数如果检测到串口那么返回串口名，如果没有则返回空字符串
 *
 *              函数名-------------形参以及返回值-------------作用
 *参数设置函数    setPortName       QString &name;无         设置需要打开的串口
 *              setBaudRate       int baudRate;无          设置波特率
 *              setParity         int Parity;无            设置奇偶校验性
 *              setDataBits       int DataBits;无          设置数据位
 *              setStopBits       int StopBits;无          设置停止位
 *              -----------------------------------------------------------
 *              波特率，奇偶校验位，数据位，停止位的值请查看QSerialPort中相应的枚举值
 *              -----------------------------------------------------------
 *功能函数       isOpen             无;bool                  检测串口是否打开
 *              getavailablePost   无;QList                 获取可使用的串口信息
 *              portName           无;QString               获取使用的串口名
 *              baudRate           无;int                   获取使用的波特率
 *              Parity             无;int                   获取使用的奇偶校验位
 *              DataBits           无;int                   获取使用的数据位
 *              StopBits           无;int                   获取使用的停止位
 *              open               无;bool                  打开使用的串口
 *              close              无;无                    关闭使用中的串口
 *              clear              无;bool                  重启串口，清除数据
 *              readData           char *buffer, int count, int timeout;int
 *                                                          把数据读到buffer这
 *                                 个数组中，count指最大的读取个数，timeout指扫描
 *                                 检测串口中有没有数据传过来的时间，相当于是读延迟
 *
 *              writeData          char *data, int size     把最大size大小的数
 *                                 据从data这个数组中写到串口里
 *              ------------------------------------------------------------
 *              sleep               unsigned int msec;无     延时msec毫秒
 */

#include <QtSerialPort/QtSerialPort>
#include <QSerialPortInfo>
#include <QString>
#include <QList>
#include <QTime>
#include <QIODevice>
#include <QDebug>

class SPComm
{


public:
    SPComm();   //构造函数
    ~SPComm();

    bool isOpen() const;    //检测串口是否打开
    QString getavailablePort() const;     //获取串口信息
    void setPortName(const QString &name);  //设置串口名
    QString portName() const;   //获得串口名

    void setBaudRate(int baudRate); //设置波特率
    int baudRate() const;   //返回波特率

    void setParity(int Parity);     //设置奇偶检验
    int Parity() const;     //返回奇偶校验

    void setDataBits(int DataBits);     //设置数据位
    int DataBits() const;       //返回数据位

    void setStopBits(int StopBits);     //设置停止位
    int StopBits() const;       //返回停止位

    void SetFlowControl(int FlowControl);       //设置控制流
    int FlowControl() const;        //返回控制流

    virtual bool open();    //打开串口
    virtual void close();   //关闭串口
    virtual bool clear();   //重启串口，清除数据

    int readData(char *buffer, int count, int timeout = 1000);  //读数据
    int writeData(char *data, int size);    //写数据
    //int write(char ch);


    //静态成员函数
    static QSerialPort::BaudRate getBaudRate(int baudRate);
    static QSerialPort::Parity getParity(int Parity);
    static QSerialPort::DataBits getDataBits(int DataBits);
    static QSerialPort::StopBits getStopBits(int StopBits);
    static QSerialPort::FlowControl getFlowControl(int FlowControl);

    //*************************************************************************
    void sleep(unsigned int msec);
    //*************************************************************************

protected:
    QString m_portName;
    int m_baudRate;
    int m_Parity;
    int m_DataBits;
    int m_StopBits;
    int m_FlowControl;

    QSerialPort *m_serialPort;
    QSerialPortInfo *m_serialPortInfo;
};

#endif // SPCOMM_H
