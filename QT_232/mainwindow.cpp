#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QSerialPort"
#include "QSerialPortInfo"
#include <QMessageBox>
#include <cstringt.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
        //auto get port name
    const auto infos = QSerialPortInfo::availablePorts();
    for(const QSerialPortInfo &info : infos)
    {
        ui->comboBox_name->addItem(info.portName());
    }
    //  QStringList list_name=(QStringList()<<"COM1"<<"COM2"<<"COM3"<<"COM4"<<"COM5");
     // ui->comboBox_name->addItems(list_name);
   //
    QStringList list_badrate=(QStringList()<<"4800"<<"9600"<<"19200"<<"115200");
    ui->comboBox_baudrate->addItems(list_badrate);
    QStringList list_size=(QStringList()<<"8");
    ui->comboBox_datasize->addItems(list_size);
    QStringList list_odd=(QStringList()<<"NoParity"<<"EvenParity"<<"OddParity");
    ui->comboBox_parity->addItems(list_odd);
    ui->radioButton->setChecked(1);
   // aa();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//QString ->
void MainWindow::on_pushButton_connect_clicked()
{

     if(!(serialp->isOpen()))
     {
     serialp->setPortName(ui->comboBox_name->currentText());
     serialp->setBaudRate(ui->comboBox_baudrate->currentText().toInt());
     QString case_size =ui->comboBox_datasize->currentText();
     if(case_size =="8")
         serialp->setDataBits(QSerialPort::Data8);
     QString case_odd =ui->comboBox_parity->currentText();
     if(case_odd =="NoParity")
         serialp->setParity(QSerialPort::NoParity);
     else if(case_odd =="EvenParity")
         serialp->setParity(QSerialPort::EvenParity);
     else if(case_odd =="OddParity")
         serialp->setParity(QSerialPort::OddParity);

     serialp->open(QIODevice::ReadWrite);
     QMessageBox::information(this,"Thong bao","Da ket noi uart");
     ui->pushButton_connect->setText(QString::fromUtf8("Disconnect"));
     ui->statusbar->showMessage("Connected "+(serialp->portName()));
     connect(serialp,SIGNAL(readyRead()),this,SLOT(serialport_read()));
     }
     else if(serialp->isOpen())
     {
         serialp->close();
         QMessageBox::information(this,"Thong bao","Da ngat ket noi uart");
         ui->pushButton_connect->setText(QString::fromUtf8("Connect"));
        ui->statusbar->clearMessage();

     }


}


void MainWindow::on_pushButton_pos_clicked()  //write
{
    if(!(serialp->isOpen()))
    {
        QMessageBox::information(this,"Thong bao","Chua ket noi Serial");

    }
           else
           {
        if(ui->radioButton->isChecked())
        r_cmd[0]=0x43; //mode 1 bytes //'A'
        else if(ui->radioButton_2->isChecked())
        r_cmd[0]=0x44; //mode 5bytes 'E'
        QString ADDR= ui->lineEdit_addr->text();
        QString DATA_0=ui->lineEdit_data_0->text();
        int8_t DATA_0i=DATA_0.toInt();
        QString DATA_1=ui->lineEdit_data_1->text();
        int8_t DATA_1i=DATA_1.toInt();
        QString DATA_2=ui->lineEdit_data_2->text();
        int8_t DATA_2i=DATA_2.toInt();
        QString DATA_3=ui->lineEdit_data_3->text();
        int8_t DATA_3i=DATA_3.toInt();
        QString DATA_4=ui->lineEdit_data_4->text();
        int8_t DATA_4i=DATA_4.toInt();

        //
        int16_t ADDRi =ADDR.toInt();
        r_address[0]=ADDRi&0xFF;
        r_address[1]=(ADDRi>>8)&0xFF;
        r_data[0]=DATA_0i&0xFF;
        r_data[1]=DATA_1i&0xFF;
        r_data[2]=DATA_2i&0xFF;
        r_data[3]=DATA_3i&0xFF;
        r_data[4]=DATA_4i&0xFF;
            //
            quint8 index=0;
            memcpy(protocol+index,&STX,1);
            index=index+sizeof(STX);
            memcpy(protocol+index,r_cmd,sizeof(r_cmd));
            index=index+sizeof(r_cmd);
            memcpy(protocol+index,r_address,sizeof(r_address));
            index=index+sizeof(r_address);
            memcpy(protocol+index,r_data,sizeof(r_data));
            index=index+sizeof(r_data);
            memcpy(protocol+index,&ETX,sizeof(ETX));
            index=index+sizeof(ETX);
            //
             QString cmd;
            cmd.append(protocol);
            //
            ui->textEdit_rdata->setText("WRITE EEPROM");
            ui->textEdit_rdata->moveCursor(QTextCursor::End);
           // ui->textEdit_rdata->;
            serialp->write(protocol,index);
           }


}
//

//
void MainWindow::serialport_read()
{

           size =serialp->bytesAvailable();
    if(size==514)
    {
      data_rv=serialp->readAll();
     x+=1;
    if(data_rv[0]=='2' && data_rv[513]=='3')
            ui->statusbar->showMessage("Nhan data thanh cong");
             int h=1;
    for(int i=0;i<(ui->tableWidget->rowCount());i++)
               {
                   QTableWidgetItem *item;
                    for(int j=0;j<(ui->tableWidget->columnCount());j++)
                    {
                       item =new QTableWidgetItem;
                        uint8_t c=data_rv[h];
                       char string[5];
                       utostring(c,string);
                       QString str_data(string);
                       item->setText(str_data);
                       h++;
                       ui->tableWidget->setItem(i,j,item);
                     }
                  }

    }

}

void MainWindow::aa()
{
    int h=1;
   // QString datax(data_rv);
    for(int i=0;i<(ui->tableWidget->rowCount());i++)
    {
        QTableWidgetItem *item;
         for(int j=0;j<(ui->tableWidget->columnCount());j++)
         {
            item =new QTableWidgetItem;
            uint8_t c=2;
            QString cmd;
            char str2=(c&0x0F)+0x30U;
            char str1=((c>>4)&0x0F)+0x30U;

            char str[5]="0x";
            str[2]=str1;
            str[3]=str2;
            str[4]='\0';
            QString str_data(str);
            item->setText(str_data);
            h++;
            ui->tableWidget->setItem(i,j,item);
         }
    }
}

void MainWindow::utostring(uint8_t c,char *str)
{
    char str1;
    char str2;
    uint8_t a=c&0xF;

    if(a<10)
    {
    str2=(c&0x0F)+48;
    }
    else if(a>=10)
    str2=(c&0x0F)+55;

    uint8_t b=(c>>4)&0x0F;
    if(b<10)
    {
    str1=((c>>4)&0x0F)+48;
    }
    else if(b>=10)
     str1=((c>>4)&0x0F)+55;
    str[0]='0';
    str[1]='x';
    str[2]=str1;
    str[3]=str2;
    str[4]='\0';
}

void MainWindow::on_pushButton_read_clicked()  //read
{
    if(!(serialp->isOpen()))
    {
        QMessageBox::information(this,"Thong bao","Chua ket noi Serial");

    }
           else
           {
            r_cmd[0]=0x41;
            //QString ADDR= ui->lineEdit_addr->text();
            //int8_t ADDRi =ADDR.toInt();
            //r_address[0]=ADDRi&0xFF;
           // r_address[1]=(ADDRi>>8)&0xFF;
            //
            quint8 index=0;
            memcpy(protocol+index,&STX,1);
            index=index+sizeof(STX);
            memcpy(protocol+index,r_cmd,sizeof(r_cmd));
            index=index+sizeof(r_cmd);
            memcpy(protocol+index,r_address,sizeof(r_address));
            index=index+sizeof(r_address);
            memcpy(protocol+index,r_data,sizeof(r_data));
            index=index+sizeof(r_data);
            memcpy(protocol+index,&ETX,sizeof(ETX));
            index=index+sizeof(ETX);
            //
           //  QString cmd;
         //   cmd.append(protocol);
            //
           ui->textEdit_rdata->setText("READ ALL EPPROM");
           ui->textEdit_rdata->moveCursor(QTextCursor::End);

            serialp->write(protocol,index);



           }

}


void MainWindow::on_lineEdit_addr_textChanged(const QString &arg1)
{
    QString data_str=ui->lineEdit_addr->text();
    int data_int=data_str.toInt();
    char data_c[6];
    data_c[0]='0';
    data_c[1]='x';
    data_c[2]=(data_int>>8)&0x0F;
    data_c[3]=(data_int>>4)&0x0F;
    data_c[4]=data_int&0x0F;
    if(data_c[2]<10)
     data_c[2]+=48;
    else if(data_c[2]>=10)
     data_c[2]+=55;
    if(data_c[3]<10)
     data_c[3]+=48;
    else if(data_c[0]>=10)
     data_c[3]+=55;
    if(data_c[4]<10)
     data_c[4]+=48;
    else if(data_c[0]>=10)
     data_c[4]+=55;
    data_c[5]='\0';
    QString adr_data(data_c);

ui->label_7->setText(adr_data);
}


void MainWindow::on_lineEdit_data_0_textChanged(const QString &arg1)
{
    QString data_0=ui->lineEdit_data_0->text();
    int data0_int=data_0.toInt();
    char sdata_0[5];
    utostring(data0_int,sdata_0);
    QString str_data0(sdata_0);
   ui->label_dt1->setText(str_data0);

}


void MainWindow::on_lineEdit_data_1_textChanged(const QString &arg1)
{
    QString data_0=ui->lineEdit_data_1->text();
    int data0_int=data_0.toInt();
    char sdata_0[5];
    utostring(data0_int,sdata_0);
    QString str_data0(sdata_0);
   ui->label_dt2->setText(str_data0);
}


void MainWindow::on_lineEdit_data_2_textChanged(const QString &arg1)
{
    QString data_0=ui->lineEdit_data_2->text();
    int data0_int=data_0.toInt();
    char sdata_0[5];
    utostring(data0_int,sdata_0);
    QString str_data0(sdata_0);
   ui->label_dt3->setText(str_data0);
}


void MainWindow::on_lineEdit_data_3_textChanged(const QString &arg1)
{
    QString data_0=ui->lineEdit_data_3->text();
    int data0_int=data_0.toInt();
    char sdata_0[5];
    utostring(data0_int,sdata_0);
    QString str_data0(sdata_0);
   ui->label_dt4->setText(str_data0);
}


void MainWindow::on_lineEdit_data_4_textChanged(const QString &arg1)
{
    QString data_0=ui->lineEdit_data_4->text();
    int data0_int=data_0.toInt();
    char sdata_0[5];
    utostring(data0_int,sdata_0);
    QString str_data0(sdata_0);
   ui->label_12->setText(str_data0);
}

