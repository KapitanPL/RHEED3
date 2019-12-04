#include "mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QResizeEvent>
#include <QMoveEvent>
#include <QCloseEvent>

#include <./Globals/globals.h>

#ifdef _DEBUG
    #include <QDebug>
#endif

static C_globals * pglobals = &C_globals::getInstance();

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    if (pglobals)
        this->setGeometry(pglobals->m_mainWindowPos.x(), pglobals->m_mainWindowPos.y(), pglobals->m_mainWindowSize.width(), pglobals->m_mainWindowSize.height());
    else
        this->setGeometry(20,20, 800,600);

    QVBoxLayout * systemLayout = new QVBoxLayout();
    setLayout(systemLayout);
    systemLayout->setSpacing(0);

    QWidget * menuBar = new QWidget;
    menuBar->setObjectName("systemMenu");
    systemLayout->addWidget(menuBar,0);

    QWidget * main = new QWidget;
    main->setObjectName("mainArea");
    systemLayout->addWidget(main,1);

    QWidget * systemTray = new QWidget();
    systemTray->setObjectName("systemTray");
    systemLayout->addWidget(systemTray,0);

	setupMainMenu();
	setupCameraGui();
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

void MainWindow::setupCameraGui()
{
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

