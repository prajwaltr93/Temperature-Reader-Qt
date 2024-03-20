#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QSerialPortInfo>

#include "ui_mainwidget.h"

class MainWidget : public QWidget
{
    Q_OBJECT

    Ui::MainWidgetScreen mainScreen;
    QWidget mainScreenWidget;

    QComboBox *samplesPerSecListView;
    QComboBox *baudRateListView;
    QComboBox *portListView;


public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();
};
#endif // MAINWIDGET_H
