#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QSerialPort"
#include "QSerialPortInfo"
#include <QMessageBox>

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
    aa();
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
     }
     else if(serialp->isOpen())
     {
         serialp->close();
         QMessageBox::information(this,"Thong bao","Da ngat ket noi uart");
         ui->pushButton_connect->setText(QString::fromUtf8("Connect"));
        ui->statusbar->clearMessage();
     }

      connect(serialp,SIGNAL(readyRead()),this,SLOT(serialport_read()));
}


void MainWindow::on_pushButton_pos_clicked()  //write
{
    if(!(serialp->isOpen()))
    {
        QMessageBox::information(this,"Thong bao","Chua ket noi Serial");

    }
           else
           {
        if(ui->radioButton->isCheckable())
        r_cmd[0]=0x41; //mode 1 bytes
        else if(ui->radioButton_2->isCheckable())
        r_cmd[0]=0x45; //mode 5bytes
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
   // ui->textEdit_rdata->moveCursor(QTextCursor::End);
  //  ui->textEdit_rdata->insertPlainText(serialp->readAll());
    serialp->read(read_data,18);
//   data_rv=serialp->readAll();
//   data_rvbuff=data_rvbuff+data_rv;
 //  data_rv.clear();
   // uint16_t a=data_rv.size();
  //  if(data_rv.size()==514)
  //  {
  //      ui->statusbar->showMessage("Nhan data thanh cong1",5000);
 //   QString dataf(data_rv);
  //  ui->textEdit_rdata->setText(dataf);
  //  ui->textEdit_rdata->moveCursor(QTextCursor::End);
   // data_rv.clear();
  //  }
   // ui->statusbar->showMessage("Nhan data thanh cong1",1000);
  //  uint16_t x=data_rv.size();

    //ui->statusbar->showMessage(x);
  //  if(data_rv.size()==514)
    {
      //  ui->statusbar->showMessage("Nhan data thanh cong2");
       if((read_data[0]=='2') && (read_data[17]=='3'))
       {
           ui->statusbar->showMessage("Nhan data thanh cong3");
           int h=1;
          // QString datax(data_rv);
           for(int i=0;i<ui->tableWidget->rowCount();i++)
           {
               QTableWidgetItem *item;
                for(int j=0;j<ui->tableWidget->colorCount();j++)
                {
                   item =new QTableWidgetItem;
                    uint8_t c=data_rv[h];
                   QString cmd;
                   cmd.append(c);
                   item->setText(cmd);
                   h++;

                   ui->tableWidget->setItem(i,j,item);
                }
           }

       }
     //  if(data_rvbuff[17]!='3')
           // data_rvbuff.clear();
    }

}

void MainWindow::aa()
{
    int h=1;
   // QString datax(data_rv);
    for(int i=0;i<ui->tableWidget->rowCount();i++)
    {
        QTableWidgetItem *item;
         for(int j=0;j<ui->tableWidget->colorCount();j++)
         {
            item =new QTableWidgetItem;
             uint8_t c='2';
            QString cmd;
            cmd.append(c);
            item->setText(cmd);
            h++;

            ui->tableWidget->setItem(i,j,item);
         }
    }
}


void MainWindow::on_pushButton_read_clicked()  //read
{
    if(!(serialp->isOpen()))
    {
        QMessageBox::information(this,"Thong bao","Chua ket noi Serial");

    }
           else
           {
            r_cmd[0]=0x40;
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
           // ui->textEdit_rdata->setText("WRITE DATA");
         //   ui->textEdit_rdata->moveCursor(QTextCursor::End);
           // ui->textEdit_rdata->;
            serialp->write(protocol,index);
         //   code = ui->lineEdit_rdata->text();
         //   codeSize = code.size();


           }

}

