#include "QCommandWidget.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QToolButton>
#include <QComboBox>
#include <QBitmap>
#include <QCheckbox>

#include <QDebug>

QCommandWidget::QCommandWidget(QWidget* parent, Qt::WindowFlags f)
	: QWidget(parent, f)
{
    InitVariant(m_ID, DataEnumType::eUnknown, 0, 0, true);
}



void QCommandWidget::Init(S_command& command, I_BaseDevice* pDevice, Qt::Orientation orientation)
{
    if (!command.pData)
        return;
    QLayout* ret = nullptr;
    if (orientation == Qt::Horizontal)
        ret = new QHBoxLayout();
    else
        ret = new QVBoxLayout();
    ret->setSpacing(1);
    ret->setContentsMargins(1, 1, 1, 1);
    setLayout(ret);

    bool bCommandGroup = false;
    if (strncmp(command.sCommand, GUI_COMMAND_GROUP, sizeof(GUI_COMMAND_GROUP)) == 0)
        bCommandGroup = true;

    if (command.pData->eType == DataEnumType::eVariant && command.pData->uiCount > 0)
    {
        QString sName;
        QString sTooltip;
        QString sIconPath;
        bool bButton = false;
        bool bEdit = false;
        bool bCheckBoxe = false;
        bool bCombo = false;
        
        S_variant* pIter = nullptr;
        if (strncmp(command.pData->sName, COM_BUTTON, sizeof(COM_BUTTON)) == 0)
        {
            bButton = true;
        }
        else if (strncmp(command.pData->sName, COM_CHECKBOX, sizeof(COM_CHECKBOX)) == 0)
        {
            bCheckBoxe = true;
        }
        else if (strncmp(command.pData->sName, COM_COMBO, sizeof(COM_COMBO)) == 0)
        {
            bCombo = true;
        }

        else if (strncmp(command.pData->sName, COM_STRING_INPUT, sizeof(COM_STRING_INPUT)) == 0)
        {
            bEdit = true;
        }
        S_variant* pVar = (S_variant*)command.pData->pData;
        for (uint32_t uiV = 0; uiV < command.pData->uiCount; ++uiV)
        {
            if (strncmp(pVar->sName, COM_GUI_NAME, sizeof(COM_GUI_NAME)) == 0 && pVar->eType == DataEnumType::eChar)
            {
                sName = (QString((const char*)pVar->pData));
            }
            else if ((strncmp(pVar->sName, COM_RANGE, sizeof(COM_RANGE)) == 0) && pVar->eType == DataEnumType::eVariant && pVar->uiCount == 2)
            {
                m_range.resize(2);
                pIter = (S_variant*)pVar->pData;
                for (uint32_t uiIt = 0; uiIt < 2; uiIt++)
                {
                    QVariant* pTarg = nullptr;
                    if (strncmp(pIter->sName, COM_MAX, sizeof(COM_MAX)) == 0)
                    {
                        pTarg = &m_range[1];
                    }
                    else if (strncmp(pIter->sName, COM_MIN, sizeof(COM_MIN)) == 0)
                    {
                        pTarg = &m_range[0];
                    }

                    UseVariantValue(*pIter, [pTarg](auto val, uint32_t uiCount) {pTarg->setValue(*val); return eOK; });
                    pIter++;
                }
            }
            else if (strncmp(pVar->sName, COM_ENUM_VALS, sizeof(COM_ENUM_VALS)) == 0 && pVar->eType == DataEnumType::eVariant && pVar->uiCount > 0)
            {
                pIter = (S_variant*)pVar->pData;
                QVariant* pTarg = nullptr;
                m_vals.resize(pVar->uiCount);
                for (uint32_t uiIt = 0; uiIt < pVar->uiCount; uiIt++)
                {
                    pTarg = &m_vals[uiIt];
                    if (pIter->eType == DataEnumType::eChar)
                    {
                        QString sVal((char*)pIter->pData);
                        pTarg->setValue(sVal);
                    }
                    else
                        UseVariantValue(*pIter, [pTarg](auto val, uint32_t uiCount) {pTarg->setValue(*val); return eOK; });
                    pIter++;
                }
            }
            else if (strncmp(pVar->sName, COM_ENUM_VALS, sizeof(COM_ENUM_VALS)) == 0  && pVar->uiCount > 0
                && (pVar->eType > DataEnumType::eUnknown && pVar->eType < DataEnumType::eChar)) //numeric Values
             {
                QVariant* pTarg = nullptr;
                m_vals.resize(pVar->uiCount);
                for (uint32_t uiIt = 0; uiIt < pVar->uiCount; uiIt++)
                {
                    pTarg = &m_vals[uiIt];
                    UseVariantValue(*pIter, [pTarg,uiIt](auto val, uint32_t uiCount) {pTarg->setValue(val[uiIt]); return eOK; });
                }
            }
            else if (strncmp(pVar->sName, COM_VALUE, sizeof(COM_VALUE)) == 0)
            {
                if (pVar->uiCount > 0)
                {
                    QVariant* pTarg = &m_defaultValue;
                    UseVariantValue(*pVar, [pTarg](auto val, uint32_t uiCount)
                        {
                            pTarg->setValue(*val);
                            return eOK;
                        });
                }
            }
            else if (strncmp(pVar->sName, COM_ID_NAME, sizeof(COM_ID_NAME)) == 0)
            {
                CopyVariant(m_ID, *pVar);
            }
            else if (strncmp(pVar->sName, COM_TOOLTIP, sizeof(COM_TOOLTIP)) == 0)
            {
                if (pVar->eType == DataEnumType::eChar)
                {
                    sTooltip = QString((char*)pVar->pData);
                }
            }
            else if (strncmp(pVar->sName, COM_ICON, sizeof(COM_ICON)) == 0)
            {
                if (pVar->eType == DataEnumType::eChar)
                {
                    sIconPath = QString((char*)pVar->pData);
                }
            }
            pVar++;
        }

        QWidget* pAddedWidget = nullptr;

        if (bButton)
        {
            if (m_defaultValue.isValid())
                pAddedWidget = new QToolButton();
            else
                pAddedWidget = new QPushButton();
            QAbstractButton* pPush = dynamic_cast<QAbstractButton*>(pAddedWidget);
            if (pDevice && pAddedWidget)
            {
                pPush->connect(pPush, &QPushButton::clicked, this,
                    [pDevice, this, pPush]()
                    {
                        S_command comm;
                        comm.pData = new S_variant;
                        comm.sCommand = COM_DEVICE_CALLBACK;
                        InitVariant(*comm.pData, DataEnumType::eVariant, 2, COM_BUTTON, true);
                        S_variant* pSubVar = (S_variant*)comm.pData->pData;
                        CopyVariant(pSubVar[0], m_ID);
                        InitVariant(pSubVar[1], DataEnumType::eInt8, 1, COM_VALUE, true);
                        *(int8_t*)pSubVar[1].pData = pPush->isChecked() ? 1 : 0;
                        pDevice->Command(&comm);
                    });
            }

            if (!sIconPath.isEmpty() && pPush)
            {
                QIcon icon(sIconPath);
                pPush->setIcon(icon);
            }
            if (!sName.isEmpty())
            {
                pPush->setText(sName);
            }
            ret->addWidget(pPush);
        }
        else //only buttons can have name and icon on them.
        {
            if (!sIconPath.isEmpty())
            {
                QIcon icon(sIconPath);
                QPixmap pixmap = icon.pixmap(QSize());
                QLabel* lab = new QLabel();
                lab->setPixmap(pixmap);
                lab->setMask(pixmap.mask());
                ret->addWidget(lab);
            }
            if (!sName.isEmpty())
                ret->addWidget(new QLabel(sName));
        }

        if (bCombo)
        {
            QComboBox* pCombo = new QComboBox();
            pAddedWidget = pCombo;
            int32_t iDefault = 0;
            for (auto& itVal : m_vals)
            {
                pCombo->addItem(itVal.toString(), itVal);
            }
            if (m_defaultValue.isValid())
            {
                for (uint i = 0; i < pCombo->count(); ++i)
                {
                    if (m_defaultValue == pCombo->itemData(i))
                    {
                        pCombo->setCurrentIndex(i);
                        break;
                    }
                }
            }
            pCombo->connect(pCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [pCombo, this, pDevice](int index) {
                S_command comm;
                comm.sCommand = COM_DEVICE_CALLBACK;
                comm.pData = new S_variant;
                InitVariant(*comm.pData, DataEnumType::eVariant, 3, COM_COMBO, true);
                S_variant* pSubVar = (S_variant*)comm.pData->pData;
                CopyVariant(pSubVar[0], m_ID);
                InitVariant(pSubVar[1], DataEnumType::eUInt8, 1, COM_VALUE, true);
                *(int8_t*)pSubVar[1].pData = pCombo->currentIndex();
                InitVariant(pSubVar[2], DataEnumType::eChar, pCombo->currentText().size(), COM_VALUE, true);
                strncpy((char*)pSubVar[2].pData, pCombo->currentText().toStdString().c_str(), pCombo->currentText().size());
                pDevice->Command(&comm);
                });
        }

        if (bCheckBoxe)
        {
            QCheckBox* pCheck = new QCheckBox(sName);
            if (m_defaultValue.isValid())
                pCheck->setChecked(m_defaultValue.toBool());
            pAddedWidget = pCheck;
            pCheck->connect(pCheck, &QCheckBox::stateChanged, this, [this, pCheck, pDevice](int)
                {
                    S_command comm;
                    comm.pData = new S_variant;
                    comm.sCommand = COM_DEVICE_CALLBACK;
                    InitVariant(*comm.pData, DataEnumType::eVariant, 2, COM_CHECKBOX, true);
                    S_variant* pSubVar = (S_variant*)comm.pData->pData;
                    CopyVariant(pSubVar[0], m_ID);
                    InitVariant(pSubVar[1], DataEnumType::eInt8, 1, COM_VALUE, true);
                    *(int8_t*)pSubVar[1].pData = pCheck->isChecked() ? 1 : 0;
                    pDevice->Command(&comm);
                });
        }

        if (bEdit)
        {
            QLineEdit* pLine = new QLineEdit();
            pAddedWidget = pLine;
            if (m_defaultValue.isValid())
                pLine->setText(m_defaultValue.toString());
        }

        if (pAddedWidget && !sTooltip.isEmpty())
        {
            pAddedWidget->setToolTip(sTooltip);
        }
        if (pAddedWidget)
            ret->addWidget(pAddedWidget);
        QHBoxLayout* pH = dynamic_cast<QHBoxLayout*>(ret);
        QVBoxLayout* pV = dynamic_cast<QVBoxLayout*>(ret);
        if (pH)
            pH->addStretch(1);
        if (pV)
            pV->addStretch(1);
    }
    else if (command.pData && command.pData->eType == DataEnumType::eUnknown && strncmp(command.pData->sName, GUI_COMMAND, sizeof("GUI_COMMAND")) == 0)
    {
        
        S_variant* pVar = (S_variant*)command.pData;
        S_command* pSubCommand = static_cast<S_command*>(pVar->pData);
        for (uint32_t ui = 0; ui < command.pData->uiCount; ui++, pSubCommand++)
        {
            QCommandWidget* pSub = new QCommandWidget();
            if (pSubCommand)
                pSub->Init(*pSubCommand, pDevice, bCommandGroup ? orientation : (orientation == Qt::Horizontal ? Qt::Vertical : Qt::Horizontal));
            ret->addWidget(pSub);
        }
        
    }
    
}

QDeviceWidget::QDeviceWidget(QWidget* parent)
    : QStackedWidget(parent)
{
}

void QDeviceWidget::UpdateGui(S_command* pWhat)
{
    if (pWhat == nullptr)
    {
    }
    else 
    {
        //TODO command specific request
    }
}
