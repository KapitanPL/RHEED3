#ifndef CAMERA_INTERFACE_H
#define CAMERA_INTERFACE_H

#include "../common_includes.h" 

namespace RCI
{

	struct S_PictureFormat
	{
		//size in pixels
		uint32_t uiWidth;
		uint32_t uiHeight;
		//bit depth
		uint32_t uiBitDepth;
		//number of channels. Usually something like 3 - RGB, or 1 - mono
		uint32_t uiChannels;
	};

	struct S_commandParam
	{
		char* sCommand;
		void* pData;
		uint64_t uiSize;
		uint32_t uiRefCount;
	};

	struct S_command
	{
		char* sCommand;
		S_commandParam* pParams;
		uint64_t uiParamsCount;
		uint32_t uiRefCount;
	};


	class I_Camera :public I_CommonBase
	{
	public:
		virtual eRes GetDescription(S_PictureFormat& description) = 0;
		virtual eRes GetPicture(void* buffer, uint32_t& size) = 0;

		virtual eRes GetAvailableCommands(S_command** commands, uint32_t& uiCommandCount) = 0;
		virtual eRes Command(S_command* comand) = 0;

		virtual eRes RegisterImageReadyCallback(void(*imageReady(void))) = 0;
		
		virtual eRes Connect() = 0;
		virtual eRes Disconnect() = 0;
	};
}
#endif // !CAMERA_INTERFACE_H
