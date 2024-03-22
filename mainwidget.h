#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QTimer>
#include <QIODevice>
#include <QPainter>

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
    QWidget *painterCanvasWidget;
    QRect painterRect;
    QPen painterPen;

    bool running = false;
    char *buffer = nullptr;
public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

private slots:
    void startReading(bool);
    void paintEvent(QPaintEvent* /*event*/) override;
    void refreshPortList(bool);
    void baudRateChanged(int index);
};
#endif // MAINWIDGET_H
