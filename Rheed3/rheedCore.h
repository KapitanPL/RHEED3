#ifndef RHEED_CORE_H
#define RHEED_CORE_H

#include <string>
#include <map>

#include "./../interfaces/camera_interface.h"
#include "./../PMS_document_SDK/documentAPI.h"
#include "./Globals/globals.h"

class rheedCore
{
public:
	static rheedCore& GetInstance();
	rheedCore(rheedCore const& other) = delete;
	rheedCore(rheedCore&& other) = delete;
	void operator=(rheedCore const& other) = delete;

	virtual ~rheedCore();

	eRes loadDocument(std::string sPath);

	//pluginManager
	//I_PMS_V01
	static eRes AttacheGuest(void* iGuestInterface, S_Version guestVersion, S_Version minRequiredHostVersion);
	static void DetacheGuest(void* iGuestInterface);
	static void RegisterDetachCallback(void (*HostIsDetaching)(void* iHost));

	static const char* GetModuleName();
	static void Call(const char* sModule, const char* sCommand, void* data, int iParam);
	static void GetCallList(const char* sModule, const char* sCommand, const char** sCommands, uint32_t& uiCount);

	//intragrace
	const std::map<std::string, RCI::I_Camera*>* GetCameras() const;


private:
	rheedCore();
	void loadCameraPlugins();
	void updateCameras();

	std::map<std::string, C_LibLoader*> m_mapPlugins;
	std::map<std::string, RCI::I_Camera*> m_mapCameras;
	std::map<std::string, DocumentAPI::C_document*> m_mapDocuments;

	I_PMS_V01 m_iMainInterface;
	S_Version m_sMinRequired;
	const std::string m_sName = u8"CORE";

	static rheedCore* pThis;
};

#endif //RHEED_CORE_H