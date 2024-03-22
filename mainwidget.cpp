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
    serialPort->setBaudRate(baudRateListView->currentText().toInt(), QSerialPort::Input); // TODO: read only for now
    serialPort->setPortName(portListView->currentText());
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);

    refreshRate = new QTimer(this);
    connect(refreshRate, &QTimer::timeout, this, QOverload<>::of(&MainWidget::update));

    // setup button trigger
    connect(mainScreen.startButton, &QPushButton::clicked, this, &MainWidget::startReading);

    // allocate buffer to read data
    buffer = (char *)malloc(2 * sizeof(char));

    connect(mainScreen.portRefreshButton, &QPushButton::clicked, this, &MainWidget::refreshPortList);

    painterCanvasWidget = mainScreen.temperatureWidget;

    // TODO: set this to green, yello, orange, red
    QConicalGradient gradient;

    // TODO: derive these magic constants
    painterRect.setRect(20, 20, 360, 360);

    gradient.setCenter(painterRect.center());
    gradient.setAngle(240);
    gradient.setColorAt(0, Qt::red);
    gradient.setColorAt(0.5, Qt::yellow);
    gradient.setColorAt(1, Qt::green);

    QBrush paintBrush(gradient);

    painterPen.setCapStyle(Qt::RoundCap);
    painterPen.setWidth(20);
    painterPen.setBrush(paintBrush);

    connect(baudRateListView, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWidget::baudRateChanged);
}

MainWidget::~MainWidget() {
    // not required, but meh :)
    free(buffer);
}

void MainWidget::paintEvent(QPaintEvent* /*event*/) {
    QPainter temperatureWidgetPainter(this);
    temperatureWidgetPainter.setPen(painterPen);

    if(running) {
        QByteArray bytesRead = serialPort->read(2);

        // TODO: check bytesRead here
        temperatureLabel->setText(bytesRead);

        int percentageOfArc = QString(bytesRead).toInt();

        int finalArc = (160 + percentageOfArc) * -1;
        temperatureWidgetPainter.drawArc(painterRect, -160*16, finalArc * 16);
    }
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
        refreshRate->start(1000);
        mainScreen.startButton->setText("Stop");
        running = true;
    }
    else {
        serialPort->close();
        refreshRate->stop();
        running = false;
        mainScreen.startButton->setText("Start");
        temperatureLabel->setText("N/A");
    }
}

void MainWidget::refreshPortList(bool clicked) {
    // Port List view update
    QSerialPortInfo portInfo;
    QList<QSerialPortInfo> availablePorts = portInfo.availablePorts();

    // clear all items
    portListView->clear();

    for(auto item = availablePorts.begin(); item != availablePorts.end(); item++) {
        portListView->addItem(item->portName());
    }

}

void MainWidget::baudRateChanged(int index) {
    // stop serial port and set port property
    if(running) {
        refreshRate->stop();
        running = false;
        mainScreen.startButton->setText("Start");
        temperatureLabel->setText("N/A");
    }
    serialPort->close();
    serialPort->setBaudRate(baudRateListView->itemText(index).toInt(), QSerialPort::Input);
}

