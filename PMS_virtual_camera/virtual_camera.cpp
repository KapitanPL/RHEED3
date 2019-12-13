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

	eRes C_camera::GetDescription(RCI::S_PictureFormat& description)
	{
		return eOK;
	}

	eRes C_camera::GetPicture(void* buffer, uint32_t& size)
	{
		return eOK;
	}

	eRes C_camera::GetAvailableCommands(S_command** commands, uint32_t& uiCommandCount)
	{
		if (commands)
		{
			(*commands) = new S_command[m_vCommands.size()];
			uiCommandCount = m_vCommands.size();
			for (auto itCommand = m_vCommands.begin(); itCommand != m_vCommands.end(); ++itCommand)
			{
				(*commands)[itCommand - m_vCommands.begin()] = *itCommand;
			}
		}
		return eOK;
	}

	eRes C_camera::Command(S_command* comand)
	{
		return eOK;
	}

	const char* C_camera::GetUserName() const
	{
		return m_sUserName;
	}

	const char* C_camera::GetIconPath() const
	{
		return nullptr;
	}

	eRes C_camera::RegisterImageReadyCallback(void(*imageReady(void)))
	{
		return eOK;
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
