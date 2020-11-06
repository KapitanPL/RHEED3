#ifndef _QCOMMANDWIDGET_H_
#define _QCOMMANDWIDGET_H_

#include <qwidget.h>
#include "./Globals/globals.h"
#include "./../interfaces/camera_interface.h"
#include <QVariant>
#include <QVector>
#include <QStackedWidget>

class QDeviceWidget : public QStackedWidget
{
public:
	QDeviceWidget(QWidget* parent = nullptr);

	virtual void UpdateGui(S_command* pwhat = nullptr);

private:
	std::map<S_variant, QWidget*> m_mapWidgets;
};


class QCommandWidget : public QWidget
{
	Q_OBJECT;
public:
	QCommandWidget(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
	virtual void Init(S_command& command, I_BaseDevice * pDevice = nullptr, Qt::Orientation orientation = Qt::Horizontal);
private:
	QVector<QVariant> m_range;
	QVector<QVariant> m_vals;
	QVariant m_defaultValue;
	S_variant m_ID;
};

#endif //_QCOMMANDWIDGET_H_