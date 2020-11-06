#include "virtual_camera.h"

static const char* ID_MODE = "id_mode";
static const char* ID_PATH = "id_path";
static const char* ID_PATH_BUTTON = "id_path_button";

static const char* ID_ROI_L = "id_roi_l";
static const char* ID_ROI_T = "id_roi_t";
static const char* ID_ROI_W = "id_roi_w";
static const char* ID_ROI_H = "id_roi_h";

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
		m_sName = u8"Virtual_camera";
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
		S_variant commandVariant;
		InitVariant(commandVariant, DataEnumType::eVariant, 2, "LAYOUT", false);
		S_variant* pSub = (S_variant*)commandVariant.pData;
		InitVariant(*pSub, DataEnumType::eVariant, 4, COM_COMBO, false);
		pSub = (S_variant*)pSub->pData;
		InitStringVariant(*pSub, COM_GUI_NAME, "Mode:");
		pSub++;
		InitStringVariant(*pSub, COM_ENUM_VALS, "File");
		pSub++;
		InitStringVariant(*pSub, COM_ENUM_VALS, "Scene");
		pSub++;
		InitStringVariant(*pSub, COM_ID_NAME, "id_mode");
		
		pSub = (S_variant*)commandVariant.pData;
		pSub++;
		InitVariant(*pSub, DataEnumType::eVariant, 2, COM_BUTTON, false);
		pSub = (S_variant*)pSub->pData;
		InitStringVariant(*pSub, COM_ID_NAME, "id_push_simulator_source");
		pSub++;
		InitStringVariant(*pSub, COM_GUI_NAME, "PATH");

		SaveVariantToTextFile("C:/Users/Štìpán Svoboda/Documents/var.txt", commandVariant);
	}

	eRes C_camera::modeChanged(uint8_t uiSelection)
	{
		if (uiSelection >= 0 && uiSelection <= 1)
		{
			m_uiMode = uiSelection;
			switch (m_uiMode)
			{
			case 0:
				m_pCurrentCommands = &m_vFileCommands;
				break;
			case 1:
				m_pCurrentCommands = &m_vSceneCommands;
				break;
			default:
				m_pCurrentCommands = &m_vFileCommands;
				break;
			}
			if (m_fRedrawGuiRequest)
				m_fRedrawGuiRequest(nullptr);
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
			(*commands) = new S_command[m_vCommands.size() + (m_pCurrentCommands ? m_pCurrentCommands->size() : 0)];
			uiCommandCount = (uint32_t)(m_vCommands.size() + (m_pCurrentCommands ? m_pCurrentCommands->size() : 0));
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
		
		return eOK;
	}

	void C_camera::RegisterRedrawGUICallback(void(*RedrawGUI)(S_command*, void*, I_CommonBase* pParent), void* pParam)
	{
		m_pRedrawGuiParam = pParam;
		m_fRedrawGuiRequest = [this, RedrawGUI](S_command* command) {RedrawGUI(command, m_pRedrawGuiParam, this); };
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
		const char* sMsg = "Virtual camera plugin initialized.";
		pThis->m_pGuestInterface->Call("CORE", "LOG", &sMsg, 0);
		//pThis->m_pGuestInterface->Call("CORE", "GLOBALS")
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

	const char * C_camera::GetModuleName()
	{
		C_camera* pThis = &C_camera::getInstance();
		return pThis->m_sName;
	}

	void C_camera::Call(const char* sModule, const char* sCommand, void* data, int iParam)
	{
		C_camera* pThis = &C_camera::getInstance();
		if (strcmp(sModule, pThis->m_sName) == 0)
		{
			if (strcmp(sCommand, u8"I_Camera") == 0 && data)
			{
				RCI::I_Camera** ppCam = static_cast<RCI::I_Camera**>(data);
				*ppCam = &C_camera::getInstance();
			}
		}
	}

	void C_camera::GetCallList(const char* sModule, const char* sCommand,const char** sCommands, uint32_t &uiCount)
	{
		C_camera* pThis = &C_camera::getInstance();
		if (!sCommands)
			return;
		if (strcmp(sModule, pThis->m_sName) == 0)
		{
			uiCount = 0;
			sCommands[0] = u8"I_camera";
			uiCount++;
		}
	}

}
