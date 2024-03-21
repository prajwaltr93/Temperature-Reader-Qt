#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QTimer>
#include <QIODevice>

#include "ui_MainWidget.h"

class MainWidget : public QWidget
{
    Q_OBJECT

    Ui::MainWidgetScreen mainScreen;
    QWidget mainScreenWidget;

    QComboBox *samplesPerSecListView;
    QComboBox *baudRateListView;
    QComboBox *portListView;
    QTimer *refreshRate;
    QSerialPort *serialPort;
    QLabel *temperatureLabel;

    bool running = false;
    char *buffer = nullptr;
public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

private slots:
    void readUARTData(void);
    void startReading(bool);
};
#endif // MAINWIDGET_H
