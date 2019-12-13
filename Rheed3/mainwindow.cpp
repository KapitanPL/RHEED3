#include "mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QResizeEvent>
#include <QMoveEvent>
#include <QCloseEvent>
#include <QToolBox>
#include <QResource>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QMessageBox>

#include <./Globals/globals.h>

#ifdef _DEBUG
    #include <QDebug>
#endif



static C_globals * pglobals = &C_globals::getInstance();

MainWindow::MainWindow(rheedCore * core,QWidget *parent)
    : QWidget(parent)
    , m_pCore(core)
{
    if (pglobals)
    {
        this->setGeometry(pglobals->m_mainWindowPos.x(), pglobals->m_mainWindowPos.y(), pglobals->m_mainWindowSize.width(), pglobals->m_mainWindowSize.height());
        QResource::registerResource(pglobals->m_sIconResources);
    }
    else
        this->setGeometry(20,20, 800,600);

    QVBoxLayout * systemLayout = new QVBoxLayout();
    this->setLayout(systemLayout);
    systemLayout->setSpacing(0);
    systemLayout->setContentsMargins(0, 0, 0, 0);

    QWidget * menuBar = new QWidget;
    menuBar->setObjectName("systemMenu");
    systemLayout->addWidget(menuBar,0);

    QWidget * main = new QWidget;
    QHBoxLayout* mainH = new QHBoxLayout();
    mainH->setSpacing(1);
    mainH->setContentsMargins(1, 1, 1, 1);
    main->setLayout(mainH);
    main->setObjectName("mainArea");
    systemLayout->addWidget(main,1);

    QWidget * systemTray = new QWidget();
    systemTray->setObjectName("systemTray");
    systemLayout->addWidget(systemTray,0);

	setupMainMenu();
	setupDeviceGui();
	setupMainDocumentArea();
	setupMeasurementArea();
	setupSystemTray();
}

MainWindow::~MainWindow()
{
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    /*if(pglobals)
    {
        pglobals->m_mainWindowSize = event->size();
    }*/
    QWidget::resizeEvent(event);
}

void MainWindow::moveEvent(QMoveEvent *event)
{
    /*if(pglobals)
    {
        pglobals->m_mainWindowPos = event->pos();
    }*/
    QWidget::moveEvent(event);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
#ifdef _DEBUG
    qDebug() << "CloseEvent";
#endif
    if(pglobals)
    {
        pglobals->m_mainWindowPos = this->pos();
        pglobals->m_mainWindowSize = this->size();
        pglobals->Save();
    }
    QWidget::closeEvent(event);
}

void MainWindow::setupMainMenu()
{

}

void MainWindow::setupDeviceGui()
{
    QWidget* main = findChild<QWidget*>("mainArea");
    if (main && m_pCore)
    {
        QLayout* layout = main->layout();
        if (layout)
        {
            QToolBox* devices = new QToolBox();
            devices->setObjectName("devicesToolbox");
            layout->addWidget(devices);

            for (auto itDevice = m_pCore->GetCameras()->begin(); itDevice != m_pCore->GetCameras()->end(); ++itDevice)
            {
                QIcon icon;
                if (itDevice->second->GetIconPath())
                    icon.addFile(itDevice->second->GetIconPath());
                else
                    icon.addFile(":/camera.svg");
                devices->addItem(createCameraGUI(itDevice->second), icon, QString(itDevice->second->GetUserName()));
            }
        }
    }
}

void MainWindow::setupMainDocumentArea()
{
}

void MainWindow::setupMeasurementArea()
{
}

void MainWindow::setupSystemTray()
{
}

QWidget* MainWindow::createCameraGUI(RCI::I_Camera* pCamera)
{
    if (pCamera)
    {
        QStackedWidget* pCameraW = new QStackedWidget();

        QWidget* pInitWidget = new QWidget();
        QWidget* pRunningWidget = new QWidget();
        pCameraW->addWidget(pInitWidget);
        pCameraW->addWidget(pRunningWidget);
        
        QVBoxLayout* pInitL = new QVBoxLayout();
        pInitWidget->setLayout(pInitL);
        QPushButton* pInitButton = new QPushButton(tr("Connect"));
        pInitButton->connect(pInitButton, &QPushButton::clicked, this, 
            [pCamera, pCameraW]()
            {
                if (pCamera->Connect() == eOK)
                    pCameraW->setCurrentIndex(1);
            }
        );
        pInitL->addWidget(pInitButton);
        pInitL->addStretch(1);

        QVBoxLayout* pRunL = new QVBoxLayout();
        pRunL->setSpacing(1);
        pRunL->setContentsMargins(1, 1, 1, 1);
        pRunningWidget->setLayout(pRunL);
        QPushButton* pCloseButton = new QPushButton(tr("Disconnect"));
        QPalette palette = pCloseButton->palette();
        palette.setColor(QPalette::Button, Qt::red);
        pCloseButton->setPalette(palette);
        pCloseButton->setAutoFillBackground(true);
        pCloseButton->connect(pCloseButton, &QPushButton::clicked, this,
            [pCamera, pCameraW]()
            {
                QMessageBox msg;
                msg.setText(tr("Warning!"));
                msg.setInformativeText(tr("Do you really want to disconnect this device?"));
                msg.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
                msg.setDefaultButton(QMessageBox::Cancel);
                int ret = msg.exec();
                if (ret == QMessageBox::Yes && pCamera->Disconnect() == eOK)
                    pCameraW->setCurrentIndex(0);
            }
        );
        pRunL->addWidget(pCloseButton);

        S_command* pCommands = nullptr;
        uint32_t uiCount = 0;
        pCamera->GetAvailableCommands(&pCommands, uiCount);

        pRunL->addStretch(1);

        return pCameraW;
    }
    return nullptr;
}

