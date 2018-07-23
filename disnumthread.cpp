#include "disnumthread.h"

/*////////////////////////
 * 函数名： DisNumThread::DisNumThread()
 * 功能： 构造函数，将线程状态信号置为可开启状态
 */
DisNumThread::DisNumThread()
{
    stopped=false;
}

/*////////////////////////////
 * 函数名： DisNumThread::~DisNumThread()
 * 功能： 析构函数
 */
DisNumThread::~DisNumThread()
{

}

/*////////////////////////////
 * 函数名： void DisNumThread::run()
 * 功能： 线程运行函数，将主线程中传递过来的参数
 *       设置串口参数，打开串口发送串口开启状态，
 *       将串口中传递过来的单个字符和数据发送给
 *       主线程显示
 */
void DisNumThread::run()
{
    stopped=false;  //防止串口掉了之后无法重新开启串口
    //////////////////////////////设置串口参数
    Comm.setPortName(PortName);
    Comm.setBaudRate(BaudRate);
    Comm.setDataBits(DataBits);
    switch(Parity)
    {
    case 1:
        Comm.setParity(3);break;
    case 2:
        Comm.setParity(2);break;
    default:
        Comm.setParity(0);break;
    }

    switch (StopBits)
    {
    case 1:
        Comm.setStopBits(3);break;
    case 2:
        Comm.setStopBits(2);break;
    default:
        Comm.setStopBits(1);break;
    }
    ////////////////////////////////
    bool justic;
    justic=Comm.open();     //将串口开启的状态发送给主线程，由此来将串口的状态显示给用户
    emit checkopen(justic);
    //qDebug()<<justic;
    dataNum="";         //将数据置空，防止再开启时数据变量中有值
    //dataStr="";
    /////////////////////////无限循环不断的从串口中读数据
    forever
    {

        //////////////////////互斥量防止同时有线程访问stopped变量，当线程状态信号
        //////////////////////变成关断时跳出循环
        {
            //qDebug()<<stopped;
            QMutexLocker locker(&mutex);
            if (stopped)
            {
                //qDebug()<<1;
                stopped=false;
                //qDebug()<<stopped;
                break;
            }
        }
        //////////////////////
        if (!stopped)
        {
            room=' ';       //将串口数据临时存储空间置空，每次只读一个字符
            readdatareturn=Comm.readData(&room,1);

            //qDebug()<<room;
            if (!stopped)       //实时监测串口有没有关闭
            {

                if (1==readdatareturn)      //如果读到字符则进循环
                {
                        emit SendString(QString(room));//发送单个字符到主线程，用来在数据显示区显示
                        if ('\t'!=room && ' '!=room && '\n'!=room && '\0'!=room) //判断字符是不是数字
                        {
                            //qDebug()<<room;
                            dataNum=dataNum+room;   //将连续的数字字符组合成一个数据
                            //qDebug()<<dataNum;
                        }
                        else        //检测到以上这些字符的时候就说明该数据就是由之前的连续数字字符构成，将这个数据发送给主线程用来实时显示曲线
                        {

                            if (""!=dataNum)        //用来防止空格之后又有空格，造成发送一个空字符
                            {
                                MiddledataNum=dataNum.toInt();
                               // qDebug()<<MiddledataNum;
                                emit  SendInt(MiddledataNum);
                                dataNum="";
                            }

                        }


                }
                else if (-1==readdatareturn)    //返回-1说明有错误发生，将这个信号传递到主线程用来用户提示
                {
                    emit wrongdetectone();
                    QMutexLocker locker(&mutex);    //避免重复出现警告窗口
                    stopped=true;

                }
                else if (-2==readdatareturn)    //返回-2说明已经读完了没有数据了，将这个信号传递到主线程用来用户提示
                {
                    emit wrongdetecttwo();
                    QMutexLocker locker(&mutex);
                    stopped=true;

                }
            }
        }
    }
    ///////////////////////////////////////
}


/*//////////////////////////////
 * 函数名： void DisNumThread::stop()
 * 功能： 将该线程状态变量置为关断，并且关闭串口
 */
void DisNumThread::stop()
{
    QMutexLocker locker(&mutex);
    stopped=true;
    Comm.close();
}

/*//////////////////////////
 * 函数名： void DisNumThread::acceptdataslot(QString portname, int baudrate, int databits, int stopbits, int parity)
 * 功能： 槽函数用来接收来自主线程的关于串口参数的数据
 */
void DisNumThread::acceptdataslot(QString portname, int baudrate, int databits, int stopbits, int parity)
{
    PortName=portname;
    BaudRate=baudrate;
    DataBits=databits;
    StopBits=stopbits;
    Parity=parity;
    //qDebug()<<PortName<<BaudRate<<DataBits<<StopBits<<Parity;
}

/*//////////////////////////////
 * 函数名： void DisNumThread::availableportdetectslot()
 * 功能： 槽函数，当主线程搜索串口时，该线程搜索可用的串口并且将其
 *       串口名发送到主线程
 */
void DisNumThread::availableportdetectslot()
{
    QString str;
    str=Comm.getavailablePort();
    emit availableportname(str);
}


void DisNumThread::SendNumtoportslot(char *data, int i)
{
    Comm.writeData(data,i);
    //qDebug()<<int(data[0])<<i;
}
