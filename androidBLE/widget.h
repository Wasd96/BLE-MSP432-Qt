#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "device.h"

#include <QPainter>
#include <QPaintEvent>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void device_update_changed();
    void device_characteristics_updated(const QLowEnergyCharacteristic &c, const QByteArray &value);
    void device_characteristics_read(const QLowEnergyCharacteristic &c, const QByteArray &value);
    void device_services_ready();
    void device_scan_services();
    void continue_connect();

    void paintEvent(QPaintEvent *ev);

    void on_getMinutes_clicked();

    void on_setMinute_clicked();

    void on_reset_clicked();

    void on_drawCheckBox_toggled(bool checked);

private:
    Ui::Widget *ui;

    Device *dev;

    double minute[60];
    double tensec[10];
    int second;
    bool painting;
    bool gathering;
    bool firstScan;
    bool drawSource;
    int discovered;

    QLowEnergyService *service_data;
    QLowEnergyService *service_manager;
    QLowEnergyService *service_stream;
    QLowEnergyCharacteristic char_write_minute;
    QLowEnergyCharacteristic char_manager;
    QLowEnergyCharacteristic char_stream;
    QLowEnergyCharacteristic char_stream_minute;
};

#endif // WIDGET_H
