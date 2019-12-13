#include "rheedCore.h"

#include <filesystem> 
#include <wchar.h>


static C_globals* pglobals = &C_globals::getInstance();

const std::map<std::string, RCI::I_Camera*>* rheedCore::GetCameras() const
{
	return &m_mapCameras;
}

rheedCore::rheedCore()
{
	wchar_t sMsg[256];
	_snwprintf_s(sMsg, sizeof(sMsg), L"DLL test call - %d", DocumentAPI::testCall(2, 8));
	pglobals->m_pLogger->logMsg(sMsg);

	m_sMinRequired.iMajor = 1;
	m_sMinRequired.iMinor = 0;

	m_iMainInterface.sVersion.iMajor = 1;
	m_iMainInterface.sVersion.iMinor = 0;

	m_iMainInterface.Attach = &rheedCore::AttacheGuest;
	m_iMainInterface.Detach = &rheedCore::DetacheGuest;
	m_iMainInterface.RegisterHostDetach = &rheedCore::RegisterDetachCallback;
	m_iMainInterface.Call = &rheedCore::Call;
	m_iMainInterface.GetCallList = &rheedCore::GetCallList;
	m_iMainInterface.GetModuleName = &rheedCore::GetModuleName;

	rheedCore::pThis = this;

	loadCameraPlugins();
}

rheedCore* rheedCore::pThis = nullptr;

rheedCore& rheedCore::GetInstance()
{
	static rheedCore aCore;
	return aCore;
}

rheedCore::~rheedCore()
{
	//free cameras
	for (auto& itCamera : m_mapCameras)
	{
		itCamera.second->Disconnect();
	}
	//unload libs
	while (m_mapPlugins.size())
	{
		auto itPlugin = m_mapPlugins.begin();
		delete itPlugin->second;
		m_mapPlugins.erase(itPlugin);
	}
}

eRes rheedCore::loadDocument(std::string sPath)
{
	return eOK;
}

eRes rheedCore::AttacheGuest(void* /*iGuestInterface*/, S_Version /*guestVersion*/, S_Version /*minRequiredHostVersion*/)
{
	//nothing shoudl attach to main
	return eERR_FAIL;
}

void rheedCore::DetacheGuest(void* /*iGuestInterface*/)
{
	//nothing to attach, nothing to detach
	return;
}

void rheedCore::RegisterDetachCallback(void(*HostIsDetaching)(void* iHost))
{
	//this is the core. If this finishes, This will end all others. No need to notify
	return;
}

const wchar_t* rheedCore::GetModuleName()
{
	return pThis->m_sName.c_str();
}

void rheedCore::Call(const wchar_t* sModule, const wchar_t* sCommand, void* data, int iParam)
{
	if (pThis)
	{
		if (wcscmp(sModule, pThis->m_sName.c_str()) == 0)
		{
			if (wcscmp(sCommand, L"LOG") == 0 && pglobals && pglobals->m_pLogger)
			{
				const wchar_t** sMsg = static_cast<const wchar_t**>(data);
				pglobals->m_pLogger->logMsg(*sMsg);
			}
		}
		else
		{
			//TODO translate msg, or generic?
		}
	}
}

void rheedCore::GetCallList(const wchar_t* sModule, const wchar_t* sCommand, const wchar_t** sCommands, uint32_t& uiCount)
{
}

void rheedCore::loadCameraPlugins()
{
	std::wstring sPath;
	pglobals->GetCameraPluginPath(sPath);
	if (std::filesystem::exists(sPath))
	{
		for (const auto& entry : std::filesystem::directory_iterator(sPath))
		{
			if (entry.is_regular_file())
			{
				std::wstring sName = entry.path().filename().wstring();
				std::transform(sName.begin(), sName.end(), sName.begin(), [](const wchar_t c)->wchar_t {return std::tolower(c); });
				if (sName.compare(sName.length() - 3, 3, L"dll") == 0 && sName.compare(0, 4, L"plgn") == 0)
				{
					//sName is the name of plugin to load.
					C_LibLoader* plugin = new C_LibLoader(entry.path().wstring(), &m_iMainInterface, m_sMinRequired);
					if (plugin->IsValid())
					{
						m_mapPlugins[entry.path().wstring()] = plugin;
						RCI::I_Camera* pCamera = nullptr;
						plugin->GetInterface()->Call(plugin->GetInterface()->GetModuleName(), L"I_Camera", &pCamera, 0);
						if (pCamera)
						{
							m_mapCameras[pCamera->GetIdentification()] = pCamera;
						}
						else
						{
							delete plugin;
							m_mapPlugins.erase(entry.path().wstring());
						}
					}
				}
			}
		}
	}
}

void rheedCore::updateCameras()
{
}
