#ifndef PMS_VIRTUAL_CAMERA
#define PMS_VITUAL_CAMERA

#include "./../common_includes.h"
#include "./../interfaces/camera_interface.h"
#include <functional>

namespace VirtualCamera
{
#ifdef __cplusplus
	extern "C" {
#endif // _cplusplus

		DLL_CALL I_PMS_V01* GetInterface(); //(I_PMS_V01*)

#ifdef __cplusplus 
	} // extern C
#endif // _cplusplus

	class NOT_EXPORTED C_camera : public RCI::I_Camera, public I_CommonBase
	{
	public:
		//I_CommonBase
		virtual int CommonBaseFunction() override;

		//I_Camera
		virtual eRes GetDescription(RCI::S_PictureFormat& description) override;
		virtual eRes GetPicture(void* buffer, uint32_t& size) override;

		virtual eRes GetAvailableCommands(RCI::S_command** commands, uint32_t& uiCommandCount) override;
		virtual eRes Command(RCI::S_command* comand) override;

		virtual eRes RegisterImageReadyCallback(void(*imageReady(void))) override;

		virtual eRes Connect() override;
		virtual eRes Disconnect() override;

		//I_PMS_V01
		static eRes AttacheGuest(void* iGuestInterface, S_Version guestVersion, S_Version minRequiredHostVersion);
		static void DetacheGuest(void* iGuestInterface);
		static void RegisterDetachCallback(void (*HostIsDetaching)(void* iHost));

		static void GetModuleName(const wchar_t * sName);
		static void Call(const wchar_t* sModule, const wchar_t* sCommand, void* data, int iParam);
		static void GetCallList(const wchar_t* sModule, const wchar_t* sCommand,const wchar_t** sCommands, uint32_t &uiCount);

		//lifetime
		static C_camera& getInstance();
		C_camera(C_camera const& other) = delete;
		C_camera(C_camera&& other) = delete;
		void operator=(C_camera const& other) = delete;


		virtual ~C_camera();
		static I_PMS_V01 m_interface;
		static const wchar_t* m_sName;

	private:
		C_camera();
		I_PMS_V01* m_pGuestInterface = nullptr;
		std::function<void(void* iHost)> m_fDetachCallback = nullptr;
	};
}

#endif //PMS_VITUAL_CAMERA