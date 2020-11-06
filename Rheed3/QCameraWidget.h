#ifndef _QCAMERAWIDGET_H_
#define _QCAMERAWIDGET_H_

#include "QCommandWidget.h"

class QCameraWidget :public QDeviceWidget
{
public:
	QCameraWidget(RCI::I_Camera* pCamera, QWidget * parent = nullptr);
};

#endif // !_QCAMERAWIDGET_H_