#include "MainWindow.h"
#include "ui_MainWindow.h"




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //device = new QSerialPort;
}

MainWindow::~MainWindow()
{
    delete ui;
    //delete device;
    //delete connection;
    //delete reply;
}

void MainWindow::addToLogs(QString message)// Metoda dodaje (QString) do okna Logs wiadomości z dodanym czasem
{
    QString curreentDateTime = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");
    ui->textEditLogs->append(curreentDateTime + "\t" + message);
}

void MainWindow::readRegister()
{
    *adresNumber_ptr = 7500;
    ui->lineEdit_SetRegisterAdress->setText(QString::number(*adresNumber_ptr));
    QModbusDataUnit readUnit(QModbusDataUnit::HoldingRegisters, *adresNumber_ptr, 33);

    if (auto *reply = connection->sendReadRequest(readUnit, 1))
    {
        if (!reply->isFinished())
        {
        // connect the finished signal of the request to your read slot
        connect(reply, &QModbusReply::finished, this, &MainWindow::readReady);

        }
        else
        {
        delete reply; // broadcast replies return immediately
        }
    }
    else
    {
    // request error
    }

qDebug() << reply;
qDebug() << connection->state();
qDebug() << *adresNumber_ptr;
}




void MainWindow::on_pushButtonDisconnectDevice_clicked()// Metoda rozłącza się z urządzeniem MODBUS
{
    connection->disconnectDevice();
    this->addToLogs("Rozłączono urządznie od portu " + portName);

    qDebug() << connection->state();
}



void MainWindow::on_pushButton_Read_Register_clicked()
{


       readRegister();

}

void MainWindow::readReady()
{
    QModbusReply *reply = qobject_cast<QModbusReply *>(sender());
        if (!reply)
            return;

        if (reply->error() == QModbusDevice::NoError)
        {
            QModbusDataUnit unit = reply->result();
            //int startAddress = &unit->startAddress(); // odczyt rejestru startowego


            //int value = unit.value(0); // wartość w rejestrze 16 bitów start + 0


            //ui->lcdNumber_L1->display(read_32_register(&unit, 0));


            ui->lcdNumber_L2->display(read_32_register(&unit, 14));


            //ui->lcdnumber_L3->display(read_32_register(&unit, 28));


            ui->lcdNumber_Freq->display(read_32_register(&unit, 56));

            qDebug() << read_32_register(&unit, 56);
            qDebug() << read_32_register(&unit, 14);



            //void *v_ptr_L1 = &value32_L1; //konwersja uint32 na float
            //float float_value_L1 = *(float*)v_ptr_L1;


            //*stringValue_ptr = QString::number(float_value_L1);
            //*stringStartAdress_prt = QString::number(startAddress);

            this->addToLogs("Odczytano adres: " + *stringStartAdress_prt +" " + "o wartości: " + *stringValue_ptr);



        }
        else
        {
            this->addToLogs("Urzadznie zajete komunikacją. Poczekaj na zakonćzenie komunikacji");
        }

        reply->deleteLater();
}

float MainWindow::conversion_to_float(quint32 value32)
{
    union u32_to_float //konwersja uint32 do float za pomocą Unii
    {
        uint32_t value32;
        float f_value;

    };

    union u32_to_float tmp;
    tmp.value32 = value32;
    float converted = tmp.f_value;

    return converted;

}

float MainWindow::read_32_register(QModbusDataUnit *unit, int index)
{
    quint32 value32 = unit->value(index);//odczyt pierwszych 16 bitów rejestru (adresy rejestrów 32 mnożymy x2 i to jest jego pierwsze 16 bitów)
    value32 = (value32 << 16) | unit->value(index + 1);//odczyt drugich 16 bitów rejestru
    float float_value = conversion_to_float(value32);
    return float_value;

}

void MainWindow::on_pushButton_Clouse_clicked()
{
    connection->disconnectDevice();
    MainWindow::close();
}


void MainWindow::on_pushButton_ConnectTCP_clicked()
{
    QString Adress_IP = "192.168.1.129";
    QString Port_TCP = "502";
    connection->setConnectionParameter(QModbusDevice::NetworkAddressParameter, Adress_IP);
    connection->setConnectionParameter(QModbusDevice::NetworkPortParameter, Port_TCP);
    connection->setTimeout(100);
    connection->connectDevice();
    this->addToLogs("Połączono z urządzeniem MODBUS o adresie " + Adress_IP + " port " + Port_TCP);

    qDebug() << connection->state();
    qDebug() << connection->errorString();
}













void MainWindow::on_pushButton_Start_clicked()
{

}

