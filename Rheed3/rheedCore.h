#ifndef RHEED_CORE_H
#define RHEED_CORE_H

#include <string>
#include <map>

#include "./../interfaces/camera_interface.h"
#include "./../PMS_document_SDK/documentAPI.h"

class rheedCore
{
public:
	rheedCore();
	virtual ~rheedCore();

	eRes loadDocument(std::string sPath);

private:
	void updateCameras();

	std::map<std::string, RCI::I_Camera*> m_mapCameras;
	std::map<std::string, DocumentAPI::C_document*> m_mapDocuments;
};

#endif //RHEED_CORE_H