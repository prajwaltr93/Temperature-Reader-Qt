#include "mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent), mainScreenWidget{this}
{
    QVector<QString> samplesPerSecListViewItems = {"8", "16", "32"};
    QVector<QString> baudRateListViewItems = {"8", "16", "32"};
    // TODO: move these resolution constants
    this->resize(400, 600);
    mainScreen.setupUi(&mainScreenWidget);
    mainScreenWidget.move(0, 0);
    mainScreenWidget.setParent(this);

    baudRateListView = mainScreen.baudRateComboBox;
    samplesPerSecListView = mainScreen.sampleRateComboBox;
    portListView = mainScreen.portComboBox;

    // TODO: size Temperate Reading Widget properly

    // Populate List views here
    // Samples/sec list view
    for(auto item = samplesPerSecListViewItems.begin(); item != samplesPerSecListViewItems.end(); item++) {
        samplesPerSecListView->addItem(*item);
    }

    // BaudRate List view
    for(auto item = baudRateListViewItems.begin(); item != baudRateListViewItems.end(); item++) {
        baudRateListView->addItem(*item);
    }

    // Port List view
    QSerialPortInfo portInfo;
    QList<QSerialPortInfo> availablePorts = portInfo.availablePorts();
    for(auto item = availablePorts.begin(); item != availablePorts.end(); item++) {
        portListView->addItem(item->portName());
    }

}

MainWidget::~MainWidget() {}
