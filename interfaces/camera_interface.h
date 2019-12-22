#ifndef CAMERA_INTERFACE_H
#define CAMERA_INTERFACE_H

#include "../common_includes.h" 
#include "common_interface.h"

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

	class I_Camera :public I_BaseDevice
	{
	public:
		virtual eRes GetPictureDescription(S_PictureFormat& description) = 0;
		
	};


}
#endif // !CAMERA_INTERFACE_H
