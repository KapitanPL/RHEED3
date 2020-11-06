#include "QCameraWidget.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>

QCameraWidget::QCameraWidget(RCI::I_Camera* pCamera, QWidget * parent)
{
    QWidget* pInitWidget = new QWidget();
    QWidget* pRunningWidget = new QWidget();
    addWidget(pInitWidget);
    addWidget(pRunningWidget);

    QVBoxLayout* pInitL = new QVBoxLayout();
    pInitWidget->setLayout(pInitL);
    QPushButton* pInitButton = new QPushButton(tr("Connect"));
    pInitButton->connect(pInitButton, &QPushButton::clicked, this,
        [pCamera, this]()
        {
            if (pCamera->Connect() == eOK)
                this->setCurrentIndex(1);
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
        [pCamera, this]()
        {
            QMessageBox msg;
            msg.setText(tr("Warning!"));
            msg.setInformativeText(tr("Do you really want to disconnect this device?"));
            msg.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
            msg.setDefaultButton(QMessageBox::Cancel);
            int ret = msg.exec();
            if (ret == QMessageBox::Yes && pCamera->Disconnect() == eOK)
                this->setCurrentIndex(0);
        }
    );
    pRunL->addWidget(pCloseButton);

    QHBoxLayout* hPlay = new QHBoxLayout();
    hPlay->setSpacing(1);
    hPlay->setContentsMargins(1, 1, 1, 1);
    pRunL->addLayout(hPlay);
    uint32_t uiSize = 0;
    uint32_t uiCount = 0;
    QSize iconSize = QSize(40, 40);
    if (pCamera->GetData(nullptr, uiSize) != eERR_NOT_IMPLEMENTED)
    {
        QPushButton* pFrameCapture = new QPushButton();
        pFrameCapture->setObjectName("singleFrameCapture");
        pFrameCapture->setIcon(QIcon(":/capture.svg"));
        pFrameCapture->setIconSize(iconSize);
        pFrameCapture->setToolTip(tr("Capture single frame"));
        hPlay->addWidget(pFrameCapture);
    }

    if (pCamera->GetNData(nullptr, uiSize, uiCount) != eERR_NOT_IMPLEMENTED)
    {
        QPushButton* pNFrameCapture = new QPushButton();
        pNFrameCapture->setObjectName("NFrameCapture");
        pNFrameCapture->setIcon(QIcon(":/captureN.svg"));
        pNFrameCapture->setIconSize(iconSize);
        pNFrameCapture->setToolTip(tr("Capture N frame"));
        hPlay->addWidget(pNFrameCapture);
    }

    if (pCamera->GetDataStream(nullptr, uiSize) != eERR_NOT_IMPLEMENTED)
    {
        QPushButton* pDataStream = new QPushButton();
        pDataStream->setObjectName("dataStream");
        pDataStream->setIcon(QIcon(":/play.svg"));
        pDataStream->setIconSize(iconSize);
        pDataStream->setToolTip(tr("Starts data stream"));
        hPlay->addWidget(pDataStream);
    }

    S_command* pCommands = nullptr;
    uint32_t uiCommandcount = 0;
    pCamera->GetAvailableCommands(&pCommands, uiCommandcount);
    for (uint32_t uiCom = 0; uiCom < uiCommandcount; uiCom++)
    {
        QCommandWidget* pWidget = new QCommandWidget();
        pWidget->Init(pCommands[uiCom], pCamera);
        pRunL->addWidget(pWidget);
    }

    pRunL->addStretch(1);
}
