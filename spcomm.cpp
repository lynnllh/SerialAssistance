#include "spcomm.h"



/*/////////////////////
 * 函数名：构造函数
 * 功能：初始化波特率为115200，串口名为空
 *      无奇偶校验位，8位数据位，1位停止位
 *      无控制流
 */
SPComm::SPComm()
{

    m_serialPort = new QSerialPort();
    m_serialPortInfo=new QSerialPortInfo();

    m_baudRate = 115200;

    m_portName = "";

    m_Parity=0;

    m_DataBits=8;

    m_StopBits=1;

    m_FlowControl=0;

}


/*//////////////////////////
 * 函数名： 析构函数
 * 功能： delete new
 */
SPComm::~SPComm()
{

    delete m_serialPort;
    delete m_serialPortInfo;
}
//*************************************************************************
/*/////////////////////////////////
 *  函数名：void SPComm::setPortName(const QString &name)
 *  功能：设置串口的名字
 */ //window 下用"com1"   linux下用"/dev/ttyS0"
void SPComm::setPortName(const QString &name)   //这里const的意思是name这个引用变量的值在下面的具体实现中不会被改变
{

    m_portName = name;

}


/*//////////////////////////////
 * 函数名：QString SPComm::portName() const
 * 功能：用来获取串口的名字
 */
QString SPComm::portName() const    //const成员函数
{

    return m_portName;

}

//*************************************************************************
/*////////////////////////////////
 * 函数名：static QSerialPort::BaudRate getBaudRate(int baudRate)
 * 功能：这个函数用来设置波特率,打开串口的时候用到
 *      输入一个int型的波特率数值，返回一个相应的
 *      波特率枚举数
 */
QSerialPort::BaudRate SPComm::getBaudRate(int baudRate)  //BaudRate是一个枚举量，在作用域QSerialPort中
{
    switch (baudRate)
    {
    case 1200:
        return QSerialPort::Baud1200;
    case 2400:
        return QSerialPort::Baud2400;
    case 4800:
        return QSerialPort::Baud4800;
    case 9600:
        return QSerialPort::Baud9600;
    case 19200:
        return QSerialPort::Baud19200;
    case 38400:
        return QSerialPort::Baud38400;
    case 57600:
        return QSerialPort::Baud57600;
    case 115200:
        return QSerialPort::Baud115200;
    default:
        return QSerialPort::UnknownBaud;

    }

}

/*////////////////////////////////
 * 函数名：void SPComm::setBaudRate(int baudRate)
 * 功能：设置波特率 9600  19200  38400
 */
void SPComm::setBaudRate(int baudRate)
{

    m_baudRate = baudRate;

}

/*///////////////////////////////
 * 函数名：int SPComm::baudRate() const
 * 功能： 获取波特率值
 */

int SPComm::baudRate() const
{
    return m_baudRate;
}

//*************************************************************************
/*/////////////////////////////////
 * 函数名：QSerialPort::Parity SPComm::getParity(int Parity)
 * 功能：将输入的奇偶校验位数转化为Qt内的表示
 *      奇偶校验位的枚举类值
 */
QSerialPort::Parity SPComm::getParity(int Parity)
{
    switch(Parity)
    {
    case 0:
        return QSerialPort::NoParity;
    case 2:
        return QSerialPort::EvenParity;
    case 3:
        return QSerialPort::OddParity;
    case 4:
        return QSerialPort::SpaceParity;
    case 5:
        return QSerialPort::MarkParity;
    default:
        return QSerialPort::UnknownParity;
    }
}

/*////////////////////////////
 * 函数名： void SPComm::setParity(int Parity)
 * 功能： 设置奇偶校验位
 */
void SPComm::setParity(int Parity)
{
    m_Parity=Parity;
}


/*///////////////////////////
 * 函数名： int SPComm::Parity() const
 * 功能： 返回奇偶校验位
 */
int SPComm::Parity() const
{
    return m_Parity;
}


//************************************************************************
/*//////////////////////////////
 *函数名： QSerialPort::DataBits SPComm::getDataBits(int DataBits)
 *功能： 将输入的数据位值转化为Qt内的表示
 *      数据位的枚举类值
 */
QSerialPort::DataBits SPComm::getDataBits(int DataBits)
{
    switch(DataBits)
    {
    case 5:
        return QSerialPort::Data5;
    case 6:
        return QSerialPort::Data6;
    case 7:
        return QSerialPort::Data7;
    case 8:
        return QSerialPort::Data8;
    default:
        return QSerialPort::UnknownDataBits;
    }
}

/*////////////////////////
 * 函数名： void SPComm::setDataBits(int DataBits)
 * 功能： 设置数据位
 */
void SPComm::setDataBits(int DataBits)
{
    m_DataBits=DataBits;
}

/*//////////////////////////
 *函数名： int SPComm::DataBits() const
 *功能： 返回数据位值
 */
int SPComm::DataBits() const
{
    return m_DataBits;
}

//************************************************************************
/*////////////////////////////////
 *函数名： QSerialPort::StopBits SPComm::getStopBits(int StopBits)
 *功能： 将输入的停止位值转化为Qt内的表示
 *      停止位值的枚举类值
 */
QSerialPort::StopBits SPComm::getStopBits(int StopBits)
{
    switch(StopBits)
    {
    case 1:
        return QSerialPort::OneStop;
    case 3:
        return QSerialPort::OneAndHalfStop;
    case 2:
        return QSerialPort::TwoStop;
    default:
        return QSerialPort::UnknownStopBits;
    }
}
/*/////////////////////////////
 * 函数名：void SPComm::setStopBits(int StopBits)
 * 功能： 设置停止位
 */
void SPComm::setStopBits(int StopBits)
{
    m_StopBits=StopBits;
}

/*///////////////////////////
 * 函数名： int SPComm::StopBits() const
 * 功能： 返回停止位
 */
int SPComm::StopBits() const
{
    return m_StopBits;
}

//*************************************************************************
/*函数名：QSerialPort::FlowControl SPComm::getFlowControl(int FlowControl)
 * 功能： 将输入的控制流值转化为Qt内的表示
 *       控制流的枚举类值
 */
QSerialPort::FlowControl SPComm::getFlowControl(int FlowControl)
{
    switch(FlowControl)
    {
    case 0:
        return QSerialPort::NoFlowControl;
    case 1:
        return QSerialPort::HardwareControl;
    case 2:
        return QSerialPort::SoftwareControl;
    default:
        return QSerialPort::UnknownFlowControl;
    }
}

/*///////////////////////////////////
 *函数名： void SetFlowControl(int FlowControl)
 *功能： 设置控制流
 */
void SPComm::SetFlowControl(int FlowControl)
{
    m_FlowControl=FlowControl;
}

/*//////////////////////////////////
 * 函数名： int SPComm::FlowControl() const
 * 功能： 返回控制流值
 */
int SPComm::FlowControl() const
{
    return m_FlowControl;
}


//*************************************************************************
/*///////////////////////////////
 * 函数名： bool SPComm::isOpen() const
 * 功能： 检测是否打开串口
 */
bool SPComm::isOpen() const
{
    return m_serialPort->isOpen();
}

/*///////////////////////////////
 *函数名： QList SPComm::getavailablePost() const
 *功能： 获取串口信息,如果检测到串口那么返回串口名，如果没有
 *      则返回空字符串
 */
QString SPComm::getavailablePort() const
{

   foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
   {
               qDebug() << "Name : " << info.portName();
               qDebug() << "Description : " << info.description();
               qDebug() << "Manufacturer: " << info.manufacturer();
               qDebug() << "Serial Number: " << info.serialNumber();
               qDebug() << "System Location: " << info.systemLocation();
               return info.portName();
   }
   return "";

}

/*//////////////////////////////
 * 函数名：bool SPComm::open()
 * 功能：用来打开串口，调用前，先设置串口名字和波特率
 *      数据位，停止位，奇偶校验率。默认无控制流，如果
 *      以后要改直接把QSerialPort::NoFlowControl
 *      改成getFlowControl(m_FlowControl)就行了
 *      默认读缓存大小1024字节，默认双向打开即可读可写
 */
bool SPComm::open()
{
    if (m_serialPort->isOpen()) //先判断是否已经打开了串口
    {
        return true;
    }
    m_serialPort->setPortName(m_portName);
    m_serialPort->setBaudRate(getBaudRate(m_baudRate));
    m_serialPort->setParity(getParity(m_Parity));
    m_serialPort->setDataBits(getDataBits(m_DataBits));
    m_serialPort->setStopBits(getStopBits(m_StopBits));
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);
    m_serialPort->setReadBufferSize(1024);
    return m_serialPort->open(QIODevice::ReadWrite);

}


/*/////////////////////////
 * 函数名： void SPComm::close()
 * 功能： 关闭串口
 */
void SPComm::close()

{
    if (m_serialPort->isOpen())
    {

        m_serialPort->close();

    }
}


/*//////////////////////////
 * 函数名： bool SPComm::clear()
 * 功能： 重启串口,清楚数据
 */
bool SPComm::clear()

{
    if (m_serialPort->isOpen())
    {
        m_serialPort->clear();

        this->close();

        return this->open();
    }
    return false;
}


/*///////////////////////////
 * 函数名：int SPComm::readData(char *buffer, int count, int timeout)
 * 功能： 读数据，将数据存在buffer数组中，最大
 *       读count个数据
 */
int SPComm::readData(char *buffer, int count, int timeout)
{
    int len = 0;

    forever //无限循环
    {
        /* 返回read到的数据个数，从buffer[0]
           开始存储，如果len不等于0，那就从后面空的空间
           开始存储，count是最大的存储长度，如果超过这个
        *  长度那就不读了，如果没有超过这个长度那就接在后面继续读
        */
        int n = m_serialPort->read(&buffer[len], count - len);
        if (n == -1)
        {
            return -1;
        }
        else if (n == 0 && !m_serialPort->waitForReadyRead(timeout))//返回0说明没有数据，waitforreadyread是一个读写延迟，如果在timeout的时间内没有数据可读，那么返回假，如果有数据可读那么返回真
        {
            return -2;
        }
        else
        {
            len += n;   //检测buffer中的空间有没有被塞满

            if (count == len)

                break;
        }
    }
    return len;
}


/*////////////////////////////
 * 函数名： int SPComm::writeData(char *data, int size)
 * 功能： 发送数据到串口  比如发送协议
 */
int SPComm::writeData(char *data, int size)
{

    int len = 0;

    forever
    {

        int n = m_serialPort->write(data, size - len);

        if (n == -1)
        {

            return -1;

        }
        else
        {

            len += n;
            if (size == len)
                break;
        }
    }

    return len;

}



//*******************************************************************************
/*//////////////////////
 *函数： sleep()
 *功能：延时一段时间
 *//////////////////////
void SPComm::sleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
