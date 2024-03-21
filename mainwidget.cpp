#include "mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent), mainScreenWidget{this}, serialPort{new QSerialPort(this)}
{
    QVector<QString> samplesPerSecListViewItems = {"8", "16", "32"};
    QSerialPortInfo portInfo;
    // TODO: move these resolution constants
    this->resize(400, 600);
    mainScreen.setupUi(&mainScreenWidget);
    mainScreenWidget.move(0, 0);
    mainScreenWidget.setParent(this);

    baudRateListView = mainScreen.baudRateComboBox;
    samplesPerSecListView = mainScreen.sampleRateComboBox;
    portListView = mainScreen.portComboBox;

    temperatureLabel = mainScreen.temperatureLabel;

    // TODO: size Temperate Reading Widget properly

    // Populate List views here
    // Samples/sec list view
    for(auto item = samplesPerSecListViewItems.begin(); item != samplesPerSecListViewItems.end(); item++) {
        samplesPerSecListView->addItem(*item);
    }

    // BaudRate List view
    QList<qint32> standardBaudRates = portInfo.standardBaudRates();
    for(auto item = standardBaudRates.begin(); item != standardBaudRates.end(); item++) {
        baudRateListView->addItem(QString::asprintf("%d", *item));
    }

    // Port List view
    QList<QSerialPortInfo> availablePorts = portInfo.availablePorts();
    for(auto item = availablePorts.begin(); item != availablePorts.end(); item++) {
        portListView->addItem(item->portName());
    }

    // configure defaults
    serialPort->setBaudRate(baudRateListView->currentData().toInt(), QSerialPort::Input); // TODO: read only for now
    serialPort->setPortName(portListView->currentText());
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);

    refreshRate = new QTimer(this);
    connect(refreshRate, &QTimer::timeout, this, &MainWidget::readUARTData);

    // setup button trigger
    connect(mainScreen.startButton, &QPushButton::clicked, this, &MainWidget::startReading);

    // allocate buffer to read data
    buffer = (char *)malloc(2 * sizeof(char));
}

MainWidget::~MainWidget() {
    // not required, but meh :)
    free(buffer);
}

void MainWidget::readUARTData(void) {
    int bytesRead = 0;
    bytesRead = serialPort->read(buffer, 2);

    // TODO: check bytesRead here
    temperatureLabel->setText(buffer);
}

void MainWidget::startReading(bool clicked)
{
    if(!running) {
    bool portOpen = false;
        portOpen = serialPort->open(QIODevice::ReadOnly);
        if(!portOpen) {
            temperatureLabel->setText("Port Open Failed");
            return;
        }
        refreshRate->start(100);
        mainScreen.startButton->setText("Stop");
        running = true;
    }
    else {
        refreshRate->stop();
        running = false;
        mainScreen.startButton->setText("Start");
        serialPort->close();
        temperatureLabel->setText("N/A");
    }
}

