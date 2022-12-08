#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
//data
    QSerialPort *serialp = new QSerialPort;
    //protocol
    uint8_t x=0;
    int size=0;
    char protocol[10];
    unsigned char STX={0x02};
    unsigned char ETX={0x03};
    quint8 r_cmd[1]={0x00};
    quint8 r_address[2]={0x00,0x00};
    quint8 r_data[5]={0x00,0x01,0x00,0x00,0x00};
    quint8 i=1;
    char read_data[6];
    //quint8 t_data[10]={0x4D,0x4F,0x56,0x4C,0x4C,0x4D,0x4D,0x4D,0x4D,0x4D,0x00};
    //
    QByteArray data_rv;
    QByteArray data_rvbuff;
private slots:
    void on_pushButton_connect_clicked();

    void on_pushButton_pos_clicked();
    void aa();
    void utostring(uint8_t c,char *str);
    void on_pushButton_read_clicked();
    void serialport_read();
    void on_lineEdit_addr_textChanged(const QString &arg1);

    void on_lineEdit_data_0_textChanged(const QString &arg1);

    void on_lineEdit_data_1_textChanged(const QString &arg1);

    void on_lineEdit_data_2_textChanged(const QString &arg1);

    void on_lineEdit_data_3_textChanged(const QString &arg1);

    void on_lineEdit_data_4_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    QString buffer;
    QString code;
    int codeSize;


};
#endif // MAINWINDOW_H
