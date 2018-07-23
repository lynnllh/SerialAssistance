#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include "spcomm.h"
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include "disnumthread.h"
#include <QMetaType>
#include "qcustomplot.h"
#include <QTime>
#include <QDateTime>
#include <QFont>
#include <QBrush>
#include <QColor>
#include <QAbstractSlider>
#include <QtMath>
#include <QFileDialog>
#include <QFile>
#include <QIODevice>
#include <QTextCursor>

//#define Count 500

namespace Ui {
class MyWidget;
}

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MyWidget(QWidget *parent = 0);
    ~MyWidget();
    void portplot(QCustomPlot *customplot);    //图像的初始化函数

private slots:
    void on_SearchPortpushButton_clicked();

    void on_OpenPortpushButton_clicked();

    void on_sendcurvedatapushButton_clicked();

    void on_xAxisrangehorizontalSlider_sliderMoved(int position);

    void on_savepicturepushButton_clicked();

    void on_SaveDatapushButton_clicked();

    void on_ClearpushButton_clicked();

    void on_ClearSendpushButton_clicked();

    void on_SendMessagepushButton_clicked();

private:
    Ui::MyWidget *ui;
    SPComm MyComm;
    int ChangeNum;              //用来控制开启和关断的信号
    //char Buffer[Count];
    QString str;

    QString portname;           //用户设置的串口参数
    int databits;
    int baudrate;
    int stopbits;
    int parity;                 //用户设置的串口参数

    bool Justic;                //用来存储次线程传递过来的串口开启状态判断信号
    DisNumThread thread;        //次线程
    QString availportname;      //用来存储次线程传递过来的串口名
    int count;                  //用来挑选数据的计数信号
    int portNum;                //用来设置显示的曲线数目
    int xAxismax;       //x轴的最大值
    int yAxismin;       //y轴的最小值
    int yAxismax;       //y轴的最大值
    double percent;        //滑块的实时位置信号
    int issendcurvedata;    //是否按下发送图像设置按钮信号
    //QTextCursor cursor;     //实现自动滚屏

signals:
    void acceptdata(QString portname,int baudrate,int databits,int stopbits,int parity);
    void availableportdetect();
    void SendNumtoport(char* data,int i);

public slots:
    void checkopenslot(bool justic);
    void SendStringslot(QString PortMessageString);
    void SendIntslot(int PortMessageInt);
    void wrongdetecttwoslot();
    void wrongdetectoneslot();
    void availableportnameslot(QString str);

};

#endif // MYWIDGET_H
