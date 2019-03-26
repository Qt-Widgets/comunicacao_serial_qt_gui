#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "comserial.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:

    void WriteData(const QByteArray data);
    void ReadData();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::Widget *ui;

    QSerialPort *devserial;
    comserial *procSerial;

    void CarregarInfoDispSerial(void);
};

#endif // WIDGET_H
