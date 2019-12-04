#include "rheedCore.h"
#include <QDebug>

rheedCore::rheedCore()
{
	qDebug() << DocumentAPI::testCall(2, 8);
}

rheedCore::~rheedCore()
{
}

eRes rheedCore::loadDocument(std::string sPath)
{
	return eOK;
}

void rheedCore::updateCameras()
{
}
