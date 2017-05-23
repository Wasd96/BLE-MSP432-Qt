#include "widget.h"
#include "ui_widget.h"

#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    setLayout(ui->gridLayout);
    ui->progressBar->setValue(10);

    dev = new Device();
    connect(dev, SIGNAL(updateChanged()), this, SLOT(device_update_changed()));
    connect(dev, SIGNAL(servicesReady()), this, SLOT(device_services_ready()));
    connect(dev, SIGNAL(timeToConnect()), this, SLOT(device_scan_services()));
    connect(dev, SIGNAL(detailsDiscovered()), this, SLOT(continue_connect()));

    for (int i = 0; i < 60; i++)
        minute[i] = 0;
    second = 0;
    painting = 0;
    gathering = false;
    firstScan = false;
    drawSource = true;
    ui->setMinute->setEnabled(0);
    ui->getMinutes->setEnabled(0);
    ui->reset->setEnabled(0);
    ui->drawCheckBox->setEnabled(0);
    ui->reset->setFixedWidth(150);

    dev->startDeviceDiscovery();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::device_scan_services()
{
    ui->progressBar->setValue(30);
    for (int i = 0; i < dev->devices.size(); i++)
    {
        DeviceInfo *devinfo = (DeviceInfo *)dev->devices.first();
        if (devinfo->getName().contains("MSP SD"))
            dev->scanServices(devinfo->getAddress());
    }
    ui->textEdit->append("* scanning services *");

}

void Widget::continue_connect()
{
    if (discovered == 0) // serially connect all services
    {
        ui->textEdit->append("* Data service discovered *");
        dev->connectToService(Device_UUID_Manager_Service);
        discovered++;
        ui->progressBar->setValue(40);
        return;
    }
    if (discovered == 1)
    {
        ui->textEdit->append("* Manager service discovered *");
        dev->connectToService(Device_UUID_Stream_Service);
        discovered++;
        ui->progressBar->setValue(45);
        return;
    }
    if (discovered == 2) // all services discovered, now can save them
    {
        // saving services in comfortable for using form
        for (int i = 0; i < dev->m_services.size(); i++) {
            ServiceInfo *serviceInfo = (ServiceInfo*)dev->m_services.at(i);
            QString str = serviceInfo->getUuid();
            if (str == Device_UUID_Data_Service)
                service_data = serviceInfo->service();
            if (str == Device_UUID_Manager_Service)
                service_manager = serviceInfo->service();
            if (str == Device_UUID_Stream_Service)
                service_stream = serviceInfo->service();
        }
        ui->progressBar->setValue(50);
        char_write_minute = service_data->characteristic(QBluetoothUuid(Device_UUID_Data_Char_Minute));
        char_stream_minute = service_data->characteristic(QBluetoothUuid(Device_UUID_Data_Char_Stream));
        char_manager = service_manager->characteristic(QBluetoothUuid(Device_UUID_Manager_Char));
        char_stream = service_stream->characteristic(QBluetoothUuid(Device_UUID_Stream_Char));



        // enabling notifications
        const QLowEnergyDescriptor m_notificationDesc = service_data->characteristic(
                    QBluetoothUuid(Device_UUID_Data_Char_Stream)).descriptor(
                    QBluetoothUuid::ClientCharacteristicConfiguration);
        if (m_notificationDesc.isValid())
            service_data->writeDescriptor(m_notificationDesc, QByteArray::fromHex("0100"));

        const QLowEnergyDescriptor m_notificationDesc2 = service_stream->characteristic(
                    QBluetoothUuid(Device_UUID_Stream_Char)).descriptor(
                    QBluetoothUuid::ClientCharacteristicConfiguration);
        if (m_notificationDesc2.isValid())
            service_stream->writeDescriptor(m_notificationDesc2, QByteArray::fromHex("0100"));
        ui->progressBar->setValue(55);

        connect(dev, SIGNAL(characteristicChanged(QLowEnergyCharacteristic,QByteArray)),
                this, SLOT(device_characteristics_updated(QLowEnergyCharacteristic,QByteArray)));

        connect(dev, SIGNAL(characteristicRead(QLowEnergyCharacteristic,QByteArray)),
                this, SLOT(device_characteristics_read(QLowEnergyCharacteristic,QByteArray)));
        ui->progressBar->setValue(0);

        ui->getMinutes->setEnabled(1);
        ui->reset->setEnabled(1);
        ui->drawCheckBox->setEnabled(1);

        ui->textEdit->append("* DEVICE READY *");
        painting = true;
        repaint();
    }
}

void Widget::device_update_changed()
{
    ui->textEdit->append("dev: "+dev->getUpdate());
}

void Widget::device_characteristics_updated(const QLowEnergyCharacteristic &c,
                                            const QByteArray &value)
{

    QString str = QString::number(value.toDouble());
    if (c.uuid().toString().contains("f0001221")) // stream (current distance)
    {
        bool ok;
        double dst = str.toDouble(&ok);
        for (int i  = 0; i < 9; i++)
            tensec[i] = tensec[i+1];
        if (ok)
        {
            ui->stream->setText(str);
            tensec[9] = dst;
        }
        else
        {
            tensec[9] = tensec[8];
        }
        if (drawSource)
        {
            painting = true;
            repaint();
        }

    }
    if (c.uuid().toString().contains("f0001212")) // minute streaming
    {
        if (gathering)
        {
            minute[second] = str.toDouble();
            second++;
            QByteArray byteArray;
            byteArray.setNum(second);
            service_data->writeCharacteristic(char_write_minute, byteArray, QLowEnergyService::WriteWithoutResponse);
            if (second >= 59) {
                second = 0;
                painting = true;
                gathering = false;
                ui->drawCheckBox->setChecked(0);
                repaint();
            }
            ui->progressBar->setValue(second);
        }
    }

    ui->textEdit->update();
}

void Widget::device_characteristics_read(const QLowEnergyCharacteristic &c, const QByteArray &value)
{
    if (c.uuid().toString().contains("f0001201")) // manager service, reading max minute
    {
        QString str = QString::number(value.toInt());
        ui->minutes->setText(str);
        ui->textEdit->append("Max minute: "+str);
        ui->setMinute->setEnabled(1);
    }
}

void Widget::device_services_ready()
{
    if (firstScan == false)
    {
        ui->progressBar->setValue(35);
        ui->textEdit->append("* device connecting *");
        dev->connectToService(Device_UUID_Data_Service);
        firstScan = true;
    }
}



void Widget::paintEvent(QPaintEvent *ev)
{
    if (painting)
    {
        QPainter p(this);
        QPen pen;
        pen.setColor(Qt::red);
        pen.setWidth(3);
        p.setPen(pen);
        double x1, y1, x2, y2;
        int maxh = ui->textEdit->y()-(ui->reset->y()+ui->reset->height());
        int base = ui->textEdit->y();

        if (drawSource) // current
        {
            for (int i = 0; i < 9; i++)
            {
                x1 = width()/10*i;
                y1 = base-tensec[i]*2;
                if (y1 < base-maxh)
                    y1 = base-maxh;
                x2 = width()/10*(i+1);
                y2 = base-tensec[i+1]*2;
                if (y2 < base-maxh)
                    y2 = base-maxh;
                p.drawLine(x1, y1, x2, y2);
            }
        }
        else // minute
        {
            for (int i = 0; i < 59; i++)
            {
                x1 = width()/60*i;
                y1 = base-minute[i]*2;
                if (y1 < base-maxh)
                    y1 = base-maxh;
                x2 = width()/60*(i+1);
                y2 = ui->textEdit->y()-minute[i+1]*2;
                if (y2 < base-maxh)
                    y2 = base-maxh;
                p.drawLine(x1, y1, x2, y2);
            }
        }
        pen.setColor(Qt::gray); // draw grid
        pen.setWidth(1);
        p.setPen(pen);
        int verticals = 10;
        for (int i = 0; i <= 10; i++)
            p.drawLine(0, base-maxh/10*i, width(), base-maxh/10*i);
        for (int i = 0; i < verticals; i++)
            p.drawLine(width()/verticals*i, base-maxh, width()/verticals*i, base);

    }

}

void Widget::on_getMinutes_clicked()
{
    if (service_manager != NULL)
        service_manager->readCharacteristic(char_manager);
    ui->textEdit->append("* getting minutes counter *");
}

void Widget::on_setMinute_clicked()
{
    QByteArray byteArray;
    bool ok;
    int a = ui->minuteEdit->text().toInt(&ok, 10);
    if (!ok || a > ui->minutes->text().toInt())
    {
        ui->textEdit->append("* wrong minute! *");
        return;
    }
    byteArray.append(ui->minuteEdit->text());
    service_data->writeCharacteristic(char_write_minute, byteArray, QLowEnergyService::WriteWithoutResponse);
    gathering = true;
    ui->textEdit->append("* getting "+ui->minuteEdit->text()+" minute *");
}

void Widget::on_reset_clicked()
{
    QByteArray byteArray;
    byteArray.append("r"); // "r" for reset
    service_manager->writeCharacteristic(char_manager, byteArray, QLowEnergyService::WriteWithoutResponse);
    ui->textEdit->append("* device storage cleared *");
    on_getMinutes_clicked();
}

void Widget::on_drawCheckBox_toggled(bool checked)
{
    drawSource = checked;
    painting = true;
    repaint();
}
