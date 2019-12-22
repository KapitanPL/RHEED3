#include "virtual_camera.h"

namespace VirtualCamera
{

	I_PMS_V01* GetInterface()
	{
		C_camera * pCamera = &C_camera::getInstance();
		return &pCamera->m_interface;
	}

	C_camera::~C_camera()
	{
		if (m_fDetachCallback)
			m_fDetachCallback(&m_interface);
	}

	I_PMS_V01 C_camera::m_interface;

	C_camera::C_camera()
		: m_sUserName("virtual camera")
	{		
		m_sName = L"Virtual_camera";
		m_sID = "plgn_camera_virtual_V01";
		C_camera::m_interface.sVersion.iMajor = 1;
		C_camera::m_interface.sVersion.iMinor = 0;

		C_camera::m_interface.Attach = &C_camera::AttacheGuest;
		C_camera::m_interface.Detach = &C_camera::DetacheGuest;

		C_camera::m_interface.Call = &C_camera::Call;
		C_camera::m_interface.GetCallList = &C_camera::GetCallList;
		C_camera::m_interface.GetModuleName = &C_camera::GetModuleName;
		C_camera::m_interface.RegisterHostDetach = &C_camera::RegisterDetachCallback;

		generateCommands();
	}

	void C_camera::generateCommands()
	{
		S_command mode;
		mode.sCommand = "MODE";
		InitVariant(mode.sData, DataEnumType::eVariant, 4, COM_COMBO, true); 
		S_variant* pSub = (S_variant*)mode.sData.pData;
		const char* modeName = "Mode:";
		InitVariant(pSub[0], DataEnumType::eUnknown, 0, COM_COMBO, false);
		InitVariant(pSub[1], DataEnumType::eChar, 255, COM_GUI_NAME, false);
		strncpy_s((char*)pSub[1].pData, sizeof(modeName), modeName, 255);
		InitVariant(pSub[2], DataEnumType::eVariant, 2, COM_ENUM_VALS, false);
		S_variant* pSub2 = (S_variant*)pSub[2].pData;
		InitVariant(pSub2[0], DataEnumType::eChar, 255, COM_VALUE, true);
		strncpy_s((char*)pSub2[0].pData, sizeof("File simulator"), "File simulator",255);
		InitVariant(pSub2[1], DataEnumType::eChar, 255, COM_VALUE, true);
		strncpy_s((char*)pSub2[1].pData, sizeof("Scene simulator"), "Scene simulator", 255);
		InitVariant(pSub[3], DataEnumType::eChar, 255, COM_ID_NAME, true);
		strncpy_s((char*)pSub[3].pData, sizeof(ID_MODE), ID_MODE, 255);
		


		m_vCommands.push_back(mode); //pointers are not deep copied here, so it is ok to not release variants


	}

	eRes C_camera::modeChanged(uint8_t uiSelection)
	{
		if (uiSelection >= 0 && uiSelection <= 1)
		{
			m_uiMode = uiSelection;
			return eOK;
		}
		else
			return eERR_FAIL;
	}

	C_camera& C_camera::getInstance()
	{
		static C_camera aCamera;
		return aCamera;
	}
	const char* C_camera::GetIdentification()
	{
		return m_sID;
	}

	eRes C_camera::GetPictureDescription(RCI::S_PictureFormat& description)
	{
		return eOK;
	}

	eRes C_camera::GetData(void* buffer, uint32_t& size)
	{
		return eOK;
	}

	eRes C_camera::GetNData(void* buffer, uint32_t& size, uint32_t& uiCount)
	{
		return eRes();
	}

	eRes C_camera::GetDataStream(void* (*setDataCallback(void*, uint32_t)), uint32_t& size)
	{
		return eRes();
	}

	eRes C_camera::GetAvailableCommands(S_command** commands, uint32_t& uiCommandCount)
	{
		if (commands)
		{
			(*commands) = new S_command[m_vCommands.size() + m_pCurrentCommands ? m_pCurrentCommands->size() : 0];
			uiCommandCount = (uint32_t)m_vCommands.size();
			for (auto itCommand = m_vCommands.begin(); itCommand != m_vCommands.end(); ++itCommand)
			{
				(*commands)[itCommand - m_vCommands.begin()] = *itCommand;
			}
			if (m_pCurrentCommands)
			{
				for (auto itCommand = m_pCurrentCommands->begin(); itCommand != m_pCurrentCommands->end(); ++itCommand)
				{
					(*commands)[itCommand - m_pCurrentCommands->begin() + m_vCommands.size()] = *itCommand;
				}
			}
		}
		return eOK;
	}

	eRes C_camera::Command(S_command* command)
	{
		if (command->sCommand = COM_DEVICE_CALLBACK)
		{
			if (command->sData.eType == DataEnumType::eVariant)
			{
				S_variant* pSub = (S_variant*)command->sData.pData;
				const char* id = 0;
				std::vector<S_variant*> vpDatas;
				for (uint32_t ui = 0; ui < command->sData.uiCount; ++ui)
				{
					if (strncmp(pSub[ui].sName, COM_ID_NAME, sizeof(COM_ID_NAME)) == 0 && pSub[ui].eType == DataEnumType::eChar)
					{
						id = (const char*)pSub[ui].pData;
					}
					else if (strncmp(pSub[ui].sName, COM_VALUE, sizeof(COM_VALUE)) == 0 )
					{
						vpDatas.push_back(&pSub[ui]);
					}
					else if (strncmp(pSub[ui].sName, COM_ENUM_VALS, sizeof(COM_ENUM_VALS)) == 0)
					{
						vpDatas.push_back(&pSub[ui]);
					}
				}
				if (id && strncmp(id, ID_MODE, sizeof(ID_MODE))==0 && vpDatas.size())
				{
					auto itData = std::find_if(vpDatas.begin(), vpDatas.end(), [](auto it) {return it->eType == DataEnumType::eUInt8 || it->eType == DataEnumType::eChar; });
					if (itData != vpDatas.end())
					{
						if ((*itData)->eType == DataEnumType::eUInt8)
						{
							return modeChanged(*(uint8_t*)(*itData)->pData);
						}
					}
				}

			}

		}
		return eOK;
	}

	void C_camera::RegisterRedrawGUICallback(void(*RedrawGUI(S_command*, void*)), void* pParam)
	{
		m_pRedrawGuiParam = pParam;
		m_fRedrawGuiRequest = [this, RedrawGUI](S_command* command) {RedrawGUI(command, m_pRedrawGuiParam); };
	}

	const char* C_camera::GetUserName() const
	{
		return m_sUserName;
	}

	const char* C_camera::GetIconPath() const
	{
		return nullptr;
	}

	eRes C_camera::Connect()
	{
		return eOK;
	}

	eRes C_camera::Disconnect()
	{
		return eOK;
	}

	eRes C_camera::AttacheGuest(void* iGuestInterface, S_Version guestVersion, S_Version minRequiredHostVersion)
	{
		C_camera* pThis = &C_camera::getInstance();
		if (pThis->m_pGuestInterface)
			return eERR_ALREADYINUSE;
		if (minRequiredHostVersion.iMajor > pThis->m_interface.sVersion.iMajor)
			return eERR_VERSION;
		if (minRequiredHostVersion.iMajor == pThis->m_interface.sVersion.iMajor && minRequiredHostVersion.iMinor > pThis->m_interface.sVersion.iMinor)
			return eERR_VERSION;

		pThis->m_pGuestInterface = (I_PMS_V01*)iGuestInterface;
		const wchar_t* sMsg = L"Virtual camera plugin initialized.";
		pThis->m_pGuestInterface->Call(L"CORE", L"LOG", &sMsg, 0);
		return eOK;
	}

	void C_camera::DetacheGuest(void* iGuestInterface)
	{
		C_camera* pThis = &C_camera::getInstance();
		if (pThis->m_pGuestInterface && iGuestInterface)
		{
			I_PMS_V01* pGuest = (I_PMS_V01*)iGuestInterface;
			if (pGuest == pThis->m_pGuestInterface)
			{
				pThis->m_pGuestInterface = nullptr;
			}
		}
	}

	void C_camera::RegisterDetachCallback(void(*HostIsDetaching)(void* iHost))
	{
		C_camera* pThis = &C_camera::getInstance();
		if (!pThis->m_fDetachCallback)
			pThis->m_fDetachCallback = HostIsDetaching;
	}

	const wchar_t * C_camera::GetModuleName()
	{
		C_camera* pThis = &C_camera::getInstance();
		return pThis->m_sName;
	}

	void C_camera::Call(const wchar_t* sModule, const wchar_t* sCommand, void* data, int iParam)
	{
		C_camera* pThis = &C_camera::getInstance();
		if (wcscmp(sModule, pThis->m_sName) == 0)
		{
			if (wcscmp(sCommand, L"I_Camera") == 0 && data)
			{
				RCI::I_Camera** ppCam = static_cast<RCI::I_Camera**>(data);
				*ppCam = &C_camera::getInstance();
			}
		}
	}

	void C_camera::GetCallList(const wchar_t* sModule, const wchar_t* sCommand,const wchar_t** sCommands, uint32_t &uiCount)
	{
		C_camera* pThis = &C_camera::getInstance();
		if (!sCommands)
			return;
		if (wcscmp(sModule, pThis->m_sName) == 0)
		{
			uiCount = 0;
			sCommands[0] = L"I_camera";
			uiCount++;
		}
	}

}
