#ifndef CAMERA_INTERFACE_H
#define CAMERA_INTERFACE_H

#include "../common_includes.h" 

namespace RCI
{

#ifndef _S_PictureFormat_
#define _S_PictureFormat_
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
#endif //_S_PictureFormat_

	class I_Camera :public I_CommonBase
	{
	public:
		virtual eRes GetDescription(S_PictureFormat& description) = 0;
		virtual eRes GetPicture(void* buffer, uint32_t& size) = 0;

		virtual eRes GetAvailableCommands(S_command** commands, uint32_t& uiCommandCount) = 0;
		virtual eRes Command(S_command* comand) = 0;

		virtual const char* GetUserName() const = 0; //if empty, plugin name will be used
		virtual const char* GetIconPath() const = 0; //if empty, generic Icon will be used

		virtual eRes RegisterImageReadyCallback(void(*imageReady(void))) = 0;
		
		virtual eRes Connect() = 0;
		virtual eRes Disconnect() = 0;
	};


}
#endif // !CAMERA_INTERFACE_H
