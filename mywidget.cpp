#include "mywidget.h"
#include "ui_mywidget.h"

//注册QString和bool
Q_DECLARE_METATYPE(QString)
Q_DECLARE_METATYPE(bool)
/*////////////////////////
 * 函数名： MyWidget::MyWidget(QWidget *parent)
 * 功能： 构造函数，完成初始化并且连接信号和槽
 */
MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyWidget)
{
    ui->setupUi(this);
    //初始化参数
    ChangeNum=0;
    availportname="";
    portNum=0;
    percent=1;

    //注册QString和bool
    int id=qRegisterMetaType<QString>("QString");
    int od=qRegisterMetaType<bool>("bool");
    qDebug()<<id<<od;
    //连接信号和槽
    connect(this,SIGNAL(acceptdata(QString,int,int,int,int)),&thread,SLOT(acceptdataslot(QString,int,int,int,int)));
    connect(&thread,SIGNAL(checkopen(bool)),this,SLOT(checkopenslot(bool)));
    connect(&thread,SIGNAL(SendString(QString)),this,SLOT(SendStringslot(QString)));
    connect(&thread,SIGNAL(SendInt(int)),this,SLOT(SendIntslot(int)));
    connect(&thread,SIGNAL(wrongdetectone()),this,SLOT(wrongdetectoneslot()));
    connect(&thread,SIGNAL(wrongdetecttwo()),this,SLOT(wrongdetecttwoslot()));
    connect(this,SIGNAL(availableportdetect()),&thread,SLOT(availableportdetectslot()));
    connect(&thread,SIGNAL(availableportname(QString)),this,SLOT(availableportnameslot(QString)));
    connect(this,SIGNAL(SendNumtoport(char*,int)),&thread,SLOT(SendNumtoportslot(char*,int)));
    issendcurvedata=0;

   // ui->NumDisplayplainTextEdit->setCenterOnScroll(1);
}

/*////////////////////////
 * 函数名：　MyWidget::~MyWidget()
 * 功能：　析构函数停止线程并且等待线程释放
 */
MyWidget::~MyWidget()
{
    thread.stop();
    thread.wait();
    delete ui;
}

/*/////////////////////
 * 函数名： void MyWidget::firstplot(QCustomPlot *customplot)
 * 功能： 图像的初始化函数，并且设置x个曲线
 */
void MyWidget::portplot(QCustomPlot *customplot)
{
    customplot->clearGraphs();
    ////////////////////////////设置xy轴文字，图例文字以及曲线样式
    customplot->setNotAntialiasedElements(QCP::aeAll);
    QFont font;
    font.setStyleStrategy(QFont::NoAntialias);
    font.setPointSize(9);
    font.setBold(true);
    font.setWeight(100);
    customplot->xAxis->setTickLabelFont(font);

    customplot->yAxis->setTickLabelFont(font);

    customplot->legend->setFont(font);

    ////////////////////////////增加x个曲线

    int i;
    for (i=0;i<portNum;i++)
    {
        customplot->addGraph();     //增加一个图层，如果要显示多条曲线可以显示多条曲线
        switch (i)                  //使用switch和for可以重复定义最多8条曲线,设置曲线的颜色和图例的名字
        {
        case 1:
            customplot->graph(1)->setPen(QPen(Qt::red));break;
        case 2:
            customplot->graph(2)->setPen(QPen(Qt::green));break;
        case 3:
            customplot->graph(3)->setPen(QPen(Qt::yellow));break;
        case 4:
            customplot->graph(4)->setPen(QPen(Qt::black));break;
        case 5:
            customplot->graph(5)->setPen(QPen(Qt::lightGray));break;
        case 6:
            customplot->graph(6)->setPen(QPen(Qt::darkMagenta));break;
        case 7:
            customplot->graph(7)->setPen(QPen(Qt::darkGreen));break;
        default:
            customplot->graph(0)->setPen(QPen(Qt::blue));break;
        }
        switch (i)
        {
        case 1:
            customplot->graph(1)->setName("Channel two");break;
        case 2:
            customplot->graph(2)->setName("Channel three");break;
        case 3:
            customplot->graph(3)->setName("Channel four");break;
        case 4:
            customplot->graph(4)->setName("Channel five");break;
        case 5:
            customplot->graph(5)->setName("Channel six");break;
        case 6:
            customplot->graph(6)->setName("Channel seven");break;
        case 7:
            customplot->graph(7)->setName("Channel eight");break;
        default:
            customplot->graph(0)->setName("Channel one");break;
        }
    }
    customplot->xAxis->setTickLabelType(QCPAxis::ltDateTime);//设置x轴的显示样式
    customplot->xAxis->setDateTimeFormat("mm:ss");

    customplot->xAxis->setAutoTickStep(true);
    //customplot->xAxis->setTickStep(1);      //设置x轴的单位间隔

    customplot->axisRect()->setupFullAxesBox();
    customplot->legend->setVisible(true);   //设置图例可见

    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    xAxismax=ui->xAxismaxlineEdit->text().toInt();
    yAxismin=ui->yAxisminlineEdit->text().toInt();
    yAxismax=ui->yAxismaxlineEdit->text().toInt();
    ui->plotwidget->yAxis->setRange(yAxismin,yAxismax);               //设置y轴的范围
    ui->plotwidget->xAxis->setRange(key+0.25,xAxismax,Qt::AlignRight); //设置x轴的范围

}


/*////////////////////////
 * 函数名： void MyWidget::on_SearchPortpushButton_clicked()
 * 功能： 搜索串口，如果没搜到那么弹出错误，如果有那么在
 *       串口选择的下拉框中将出现串口名
 */
void MyWidget::on_SearchPortpushButton_clicked()
{
    if(1==ui->SelectPortcomboBox->count())//检测串口选择的下拉框中有几个item，如果多于一个则不再往里面加item
    {
        emit availableportdetect();
        //qDebug()<<availportname;
        MyComm.sleep(5);
        if(availportname=="")
        {
            QMessageBox::warning(this,"Error information","the port can't be found");
        }
        else
        {
        ui->SelectPortcomboBox->addItem(availportname);
        }
    }

}

/*////////////////////////
 * 函数名： void MyWidget::on_OpenPortpushButton_clicked()
 * 功能： 当鼠标点击打开串口按钮时，将combobox中的参数赋
 *       给串口类进行初始化并且打开串口，文字变为关闭串口
 *       点击关闭串口则关闭串口
 */
void MyWidget::on_OpenPortpushButton_clicked()
{
    if (0==ChangeNum)       //用来判断有没有关闭串口
    {
        ChangeNum=1;
        count=1;            //在打开串口的时候初始化挑选数据的计数信号
        //////////////////////将用户输入的串口参数发送给次线程
        portname=ui->SelectPortcomboBox->currentText();
        baudrate=ui->BaudRatecomboBox->currentText().toInt();
        databits=ui->DataBitscomboBox->currentText().toInt();
        stopbits=ui->StopBitscomboBox->currentIndex();
        parity=ui->ParitycomboBox->currentIndex();
        emit acceptdata(portname,baudrate,databits,stopbits,parity);
        //////////////////////
        MyComm.sleep(200);      //留给传递信号一点时间
        thread.start();         //开启线程打开串口
//是否需要延时一段时间
        MyComm.sleep(200);      //等待信号传递过来

        if (Justic==true)       //判断串口是否打开，如果打开就把“打开串口”变成“关闭串口”。如果没打开就弹出警告
        {
            ui->OpenPortpushButton->setText("关闭串口");
        }
       else
        {
            QMessageBox::warning(this,"Error information","the port can't open!");
        }

        //int n;
       // n=MyComm.readData(Buffer,Count);

        //for(int i=0;i<n;i++)
        //{
        //    ui->NumDisplayplainTextEdit->insertPlainText(QString(Buffer[i]));
       // }
    }
    else            //如果点击关闭串口则停止线程并将“关闭串口”变成“打开串口”
    {
        ChangeNum=0;
        thread.stop();
        ui->OpenPortpushButton->setText("打开串口");
    }
}

/*//////////////////////////
 * 函数名： void MyWidget::checkopenslot(bool justic)
 * 功能： 接受来自次线程的关于串口是否打开的信号
 */
void MyWidget::checkopenslot(bool justic)
{
    Justic=justic;
}

/*/////////////////////////
 * 函数名： void MyWidget::SendStringslot(QString PortMessageString)
 * 功能： 接受来自次线程的串口输出的单个字符，并把它打印到textedit中
 *       并实现自动滚屏
 */
void MyWidget::SendStringslot(QString PortMessageString)
{

    ui->NumDisplayplainTextEdit->insertPlainText(PortMessageString);
    ui->NumDisplayplainTextEdit->moveCursor(QTextCursor::EndOfLine);
    //QTextCursor cursor=ui->NumDisplayplainTextEdit->textCursor();
    //cursor.movePosition(QTextCursor::End);
    //ui->NumDisplayplainTextEdit->appendPlainText("");
}

/*/////////////////////////////
 * 函数名：void MyWidget::SendIntslot(int PortMessageInt)
 * 功能： 接受来自次线程的串口输出的数据，并且把该数据按用户设置输出成曲线
 */
void MyWidget::SendIntslot(int PortMessageInt)
{
    if (issendcurvedata)    //判断有没有按下发送图像设置按钮，如果没有按下则不显示图像
    {

    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    ui->plotwidget->xAxis->setRange(key+0.25,qCeil(xAxismax*percent),Qt::AlignRight); //设置x轴的范围
    //qDebug()<<key;

    switch (count)
    {

    case 2:
        if (ui->ChanneltwocheckBox->isChecked())    //判断channel one的复选框有没有选中，如果选中则显示曲线
        {
            if (count!=portNum) //判断是否到达最大可显示曲线数目
            {
                count++;
            }
            else
            {
                count=1;
            }
            ui->plotwidget->graph(1)->addData(key,PortMessageInt); //把数据增加到曲线上
            ui->plotwidget->graph(1)->removeDataBefore(key-qCeil(xAxismax*percent));     //清楚前面的数据

        }
        else
        {
            if (count!=portNum) //判断是否到达最大可显示曲线数目
            {
                count++;
            }
            else
            {
                count=1;
            }
        }
        break;
    case 3:
        if (ui->ChannelthreecheckBox->isChecked())
        {
            if (count!=portNum) //判断是否到达最大可显示曲线数目
            {
                count++;
            }
            else
            {
                count=1;
            }
            ui->plotwidget->graph(2)->addData(key,PortMessageInt); //把数据增加到曲线上
            ui->plotwidget->graph(2)->removeDataBefore(key-qCeil(xAxismax*percent));     //清楚前面的数据

        }
        else
        {
            if (count!=portNum) //判断是否到达最大可显示曲线数目
            {
                count++;
            }
            else
            {
                count=1;
            }
        }
        break;
    case 4:
        if (ui->ChannelfourcheckBox->isChecked())
        {
            if (count!=portNum) //判断是否到达最大可显示曲线数目
            {
                count++;
            }
            else
            {
                count=1;
            }
            ui->plotwidget->graph(3)->addData(key,PortMessageInt); //把数据增加到曲线上
            ui->plotwidget->graph(3)->removeDataBefore(key-qCeil(xAxismax*percent));     //清楚前面的数据
        }
        else
        {
            if (count!=portNum) //判断是否到达最大可显示曲线数目
            {
                count++;
            }
            else
            {
                count=1;
            }
        }
        break;

    case 5:
        if (ui->ChannelfivecheckBox->isChecked())
        {
            if (count!=portNum) //判断是否到达最大可显示曲线数目
            {
                count++;
            }
            else
            {
                count=1;
            }
            ui->plotwidget->graph(4)->addData(key,PortMessageInt); //把数据增加到曲线上
            ui->plotwidget->graph(4)->removeDataBefore(key-qCeil(xAxismax*percent));     //清楚前面的数据
        }
        else
        {
            if (count!=portNum) //判断是否到达最大可显示曲线数目
            {
                count++;
            }
            else
            {
                count=1;
            }
        }
        break;
    case 6:
        if (ui->ChannelsixcheckBox->isChecked())
        {
            if (count!=portNum) //判断是否到达最大可显示曲线数目
            {
                count++;
            }
            else
            {
                count=1;
            }
            ui->plotwidget->graph(5)->addData(key,PortMessageInt); //把数据增加到曲线上
            ui->plotwidget->graph(5)->removeDataBefore(key-qCeil(xAxismax*percent));     //清楚前面的数据
        }
        else
        {
            if (count!=portNum) //判断是否到达最大可显示曲线数目
            {
                count++;
            }
            else
            {
                count=1;
            }
        }
        break;
    case 7:
        if (ui->ChannelsevencheckBox->isChecked())
        {
            if (count!=portNum) //判断是否到达最大可显示曲线数目
            {
                count++;
            }
            else
            {
                count=1;
            }
            ui->plotwidget->graph(6)->addData(key,PortMessageInt); //把数据增加到曲线上
            ui->plotwidget->graph(6)->removeDataBefore(key-qCeil(xAxismax*percent));     //清楚前面的数据
        }
        else
        {
            if (count!=portNum) //判断是否到达最大可显示曲线数目
            {
                count++;
            }
            else
            {
                count=1;
            }
        }
        break;
    case 8:
        if (ui->ChanneleightcheckBox->isChecked())
        {
            if (count!=portNum) //判断是否到达最大可显示曲线数目
            {
                count++;
            }
            else
            {
                count=1;
            }
            ui->plotwidget->graph(7)->addData(key,PortMessageInt); //把数据增加到曲线上
            ui->plotwidget->graph(7)->removeDataBefore(key-qCeil(xAxismax*percent));     //清楚前面的数据
        }
        else
        {
            if (count!=portNum) //判断是否到达最大可显示曲线数目
            {
                count++;
            }
            else
            {
                count=1;
            }
        }
        break;
    default:
        if (ui->ChannelonecheckBox->isChecked())
        {
        if (count!=portNum) //判断是否到达最大可显示曲线数目
        {
            count++;
        }
        else
        {
            count=1;
        }
        ui->plotwidget->graph(0)->addData(key,PortMessageInt); //把数据增加到曲线上
        ui->plotwidget->graph(0)->removeDataBefore(key-qCeil(xAxismax*percent));     //清楚前面的数据
        }
        else
        {
            if (count!=portNum) //判断是否到达最大可显示曲线数目
            {
                count++;
            }
            else
            {
                count=1;
            }
        }
        break;

    }

    ui->plotwidget->replot();           //重绘图案

    }

}


/*///////////////////////
 * 函数名： void MyWidget::wrongdetectoneslot()
 * 功能： 用来接收来自次线程的错误信息，并弹出警告,并且关闭串口
 */
void MyWidget::wrongdetectoneslot()
{
    QMessageBox::warning(this,"Error information","readdata have some mysterious problems!");
    ChangeNum=0;
    thread.stop();
    ui->OpenPortpushButton->setText("打开串口");
}


/*///////////////////////
 * 函数名： void MyWidget::wrongdetecttwoslot()
 * 功能： 用来接收来自次线程的错误信息，并弹出警告，并且关闭串口
 */
void MyWidget::wrongdetecttwoslot()
{
    QMessageBox::information(this,"prompt information","no data can read!");
    ChangeNum=0;
    thread.stop();
    ui->OpenPortpushButton->setText("打开串口");
}


/*///////////////////////
 * 函数名： void MyWidget::availableportnameslot(QString str)
 * 功能： 用来接收来自次线程发送的串口名
 */
void MyWidget::availableportnameslot(QString str)
{
    availportname=str;
}

/*///////////////////////
 * 函数名： void MyWidget::on_sendcurvedatapushButton_clicked()
 * 功能： 检测输入并且发送图像参数并且初始化
 */
void MyWidget::on_sendcurvedatapushButton_clicked()
{
    //初始化图像
    //qDebug()<<ui->xAxismaxlineEdit->text().isEmpty();
    //判断输入框中有没有输入，如果没有输入则报错，然后再判断有没有修改，然后判断有没有重复按发送参数按钮，
    //如果重复按下则没有任何效果
    if (!ui->xAxismaxlineEdit->text().isEmpty() && !ui->yAxismaxlineEdit->text().isEmpty() && !ui->yAxisminlineEdit->text().isEmpty())
    {

        if (ui->xAxismaxlineEdit->isModified() || ui->yAxismaxlineEdit->isModified() || ui->yAxisminlineEdit->isModified())
        {
            ui->xAxismaxlineEdit->setModified(false);
            ui->yAxismaxlineEdit->setModified(false);
            ui->yAxisminlineEdit->setModified(false);

            if(0==ChangeNum)//按下之后还要判断是否处于关闭串口的状态，如果串口关闭
            {               //那么初始化图像，如果串口打开那么关闭串口然后再初始化图像
                portNum=ui->ChannelnumcomboBox->currentText().toInt();
                portplot(ui->plotwidget);
                //qDebug()<<1;
                //qDebug()<<ui->xAxismaxlineEdit->isModified()<<ui->yAxismaxlineEdit->isModified()<<ui->yAxisminlineEdit->isModified();

                issendcurvedata=1;
                ui->plotwidget->replot();return;
            }
            else if (1==ChangeNum)
            {
                ChangeNum=0;
                thread.stop();
                ui->OpenPortpushButton->setText("打开串口");

                portNum=ui->ChannelnumcomboBox->currentText().toInt();
                portplot(ui->plotwidget);
                //qDebug()<<2;
                issendcurvedata=1;
                ui->plotwidget->replot();return;
            }
        }
        if(portNum!=ui->ChannelnumcomboBox->currentText().toInt())
        {

                if(0==ChangeNum)//按下之后还要判断是否处于关闭串口的状态，如果串口关闭
                {               //那么初始化图像，如果串口打开那么关闭串口然后再初始化图像
                    portNum=ui->ChannelnumcomboBox->currentText().toInt();
                    portplot(ui->plotwidget);
                    //qDebug()<<3;
                    issendcurvedata=1;
                    ui->plotwidget->replot();return;
                }
                else if (1==ChangeNum)
                {
                    ChangeNum=0;
                    thread.stop();
                    ui->OpenPortpushButton->setText("打开串口");
                    //qDebug()<<4;
                    portNum=ui->ChannelnumcomboBox->currentText().toInt();
                    portplot(ui->plotwidget);
                    issendcurvedata=1;
                    ui->plotwidget->replot();return;
                }


        }
        else
        {
            QMessageBox::warning(this,"Error information","no modified!");

        }

    }
    else
    {
        QMessageBox::warning(this,"Error information","no input!");

    }

}

/*/////////////////////
 * 函数名： void MyWidget::on_xAxisrangehorizontalSlider_sliderMoved(int position)
 * 功能：实时监测滑块位置
 */
void MyWidget::on_xAxisrangehorizontalSlider_sliderMoved(int position)
{
    qDebug()<<position;
    percent=ui->xAxisrangehorizontalSlider->value()/100.0;
    //qDebug()<<percent;
}

/*//////////////////////
 * 函数名： void MyWidget::on_savepicturepushButton_clicked()
 * 功能： 保存图片
 */
void MyWidget::on_savepicturepushButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("save file"), "picture1.png",  tr("Allfile(*.*);;pngfile(*.png)"));
    if(fileName.isEmpty())
       {
           QMessageBox::information(this,"Error Message","Plesase Select a Text File");
           return;
       }
    ui->plotwidget->savePng(fileName,861,711);
    //qDebug()<<fileName;
}


/*///////////////////////////
 * 函数名： void MyWidget::on_SaveDatapushButton_clicked()
 * 功能： 保存窗口数据
 */
void MyWidget::on_SaveDatapushButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("save file"), "data1.txt",  tr("Allfile(*.*);;txtfile(*.txt)"));
    if(fileName.isEmpty())
       {
           QMessageBox::information(this,"Error Message","Plesase Select a Text File");
           return;
       }
       QFile file(fileName);
       bool ok=file.open(QIODevice::WriteOnly);
       if(ok)
       {
           QTextStream out(&file);
           QString text=ui->NumDisplayplainTextEdit->toPlainText();
           text.replace(QString("\n"),QString("\r\n"));
           out<<text;
           file.close();
       }
       else
       {
           QMessageBox::information(this,"Error Messaage","File Save Error");
           return;
       }

}

/*////////////////////////
 * 函数名： void MyWidget::on_ClearpushButton_clicked()
 * 功能： 清除数据显示区的数据,并且重启串口。如果当前处于关闭串口状态
 *       那么清除数据，不启动串口。如果处于开启串口状态，那么重启串口
 */
void MyWidget::on_ClearpushButton_clicked()
{
    ui->NumDisplayplainTextEdit->clear();

    if (1==ChangeNum)
    {

        ChangeNum=0;
        thread.stop();
        ui->OpenPortpushButton->setText("打开串口");

        if (0==ChangeNum)       //用来判断有没有关闭串口
        {
            ChangeNum=1;
            count=1;            //在打开串口的时候初始化挑选数据的计数信号
            //////////////////////将用户输入的串口参数发送给次线程
            portname=ui->SelectPortcomboBox->currentText();
            baudrate=ui->BaudRatecomboBox->currentText().toInt();
            databits=ui->DataBitscomboBox->currentText().toInt();
            stopbits=ui->StopBitscomboBox->currentIndex();
            parity=ui->ParitycomboBox->currentIndex();
            emit acceptdata(portname,baudrate,databits,stopbits,parity);
            //////////////////////
            MyComm.sleep(200);      //留给传递信号一点时间
            thread.start();         //开启线程打开串口
    //是否需要延时一段时间
            MyComm.sleep(200);      //等待信号传递过来

            if (Justic==true)       //判断串口是否打开，如果打开就把“打开串口”变成“关闭串口”。如果没打开就弹出警告
            {
                ui->OpenPortpushButton->setText("关闭串口");
            }
           else
            {
                QMessageBox::warning(this,"Error information","the port can't open!");
            }

            //int n;
           // n=MyComm.readData(Buffer,Count);

            //for(int i=0;i<n;i++)
            //{
            //    ui->NumDisplayplainTextEdit->insertPlainText(QString(Buffer[i]));
           // }
        }
    }


}

/*///////////////////////
 * 函数名： void MyWidget::on_ClearSendpushButton_clicked()
 * 功能： 清除输入框中数字
 */
void MyWidget::on_ClearSendpushButton_clicked()
{
    ui->SendMessagetextEdit->clear();
}
/*///////////////////////
 * 函数名： void MyWidget::on_SendMessagepushButton_clicked()
 * 功能： 向串口中写数据，最大5位数
 */
void MyWidget::on_SendMessagepushButton_clicked()
{
    char data[7];
    long num,num2;
    int i;
    num=ui->SendMessagetextEdit->toPlainText().toInt();
    num2=num;

    for(i=0;num2;i++)   //判断有几位数
    {
        num2=num2/10;
    }

    for(int a=0;a<i;a++)
    {
        data[i-a-1]=num%10;
        num=num/10;
    }
    //qDebug()<<int(data[0])<<int(data[1]);
    data[i]='\r';
    data[i+1]='\n';
//    for (int a=0;a<i+2;a++)
//    {
//        qDebug()<<int(data[a]);
//    }
    emit SendNumtoport(data,i+2);
}

