#ifndef PMS_VIRTUAL_CAMERA
#define PMS_VITUAL_CAMERA

#include "./../common_includes.h"
#include "./../interfaces/camera_interface.h"
#include <functional>

static const char* ID_MODE = "id_mode";

namespace VirtualCamera
{
#ifdef __cplusplus
	extern "C" {
#endif // _cplusplus

		DLL_CALL I_PMS_V01* GetInterface(); //(I_PMS_V01*)

#ifdef __cplusplus 
	} // extern C
#endif // _cplusplus

	class NOT_EXPORTED C_camera : public RCI::I_Camera
	{
	public:
		//I_CommonBase
		virtual const char* GetIdentification() override;

		//I_Camera
		virtual eRes GetPictureDescription(RCI::S_PictureFormat& description) override;
		virtual eRes GetData(void* buffer, uint32_t& size) override;
		//uiCount is both, in and out. It will attempt to fill data up to uiCount, but if less, uiCOunt is changed
		virtual eRes GetNData(void* buffer, uint32_t& size, uint32_t& uiCount) override;
		//Starts data stream. The device calls setDataCallback to get memory where to write
		virtual eRes GetDataStream(void* (*setDataCallback(void*, uint32_t)), uint32_t& size) override;

		virtual eRes GetAvailableCommands(S_command** commands, uint32_t& uiCommandCount) override;
		virtual eRes Command(S_command* comand) override;
		virtual void RegisterRedrawGUICallback(void(*RedrawGUI)(S_command*, void*, I_CommonBase* pParent), void* pParam) override;

		virtual const char* GetUserName() const override; //if empty, plugin name will be used
		virtual const char* GetIconPath() const override; //if empty, generic Icon


		virtual eRes Connect() override;
		virtual eRes Disconnect() override;

		//I_PMS_V01
		static eRes AttacheGuest(void* iGuestInterface, S_Version guestVersion, S_Version minRequiredHostVersion);
		static void DetacheGuest(void* iGuestInterface);
		static void RegisterDetachCallback(void (*HostIsDetaching)(void* iHost));

		static const char * GetModuleName();
		static void Call(const char* sModule, const char* sCommand, void* data, int iParam);
		static void GetCallList(const char* sModule, const char* sCommand,const char** sCommands, uint32_t &uiCount);

		//lifetime
		static C_camera& getInstance();
		C_camera(C_camera const& other) = delete;
		C_camera(C_camera&& other) = delete;
		void operator=(C_camera const& other) = delete;


		virtual ~C_camera();
		static I_PMS_V01 m_interface;
		const char* m_sName;

	private:
		C_camera();
		void generateCommands();
		eRes modeChanged(uint8_t uiSelection);

		uint32_t m_uiMode = 0; //0 == file sim, 1 = scene simiulation

		I_PMS_V01* m_pGuestInterface = nullptr;
		std::function<void(void* iHost)> m_fDetachCallback = nullptr;

		const char* m_sID = nullptr;
		const char* m_sUserName = nullptr;
		std::vector<S_command> m_vCommands;
		std::vector<S_command> m_vFileCommands;
		std::vector<S_command> m_vSceneCommands;
		std::vector<S_command> * m_pCurrentCommands = &m_vFileCommands;

		void* m_pRedrawGuiParam = nullptr;
		std::function<void(S_command*)> m_fRedrawGuiRequest = nullptr;
	};
}

#endif //PMS_VITUAL_CAMERA