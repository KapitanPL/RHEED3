#ifndef _COMMON_INTERFACE_H_
#define _COMMON_INTERFACE_H_

#include "./../common_includes.h"

#ifndef _I_COMMONBASE_
#define _I_COMMONBASE_
class I_CommonBase
{
public:
	virtual const char* GetIdentification() = 0;
};
#endif _I_COMMONBASE_

#ifndef _I_BASEDEVICE_
#define _I_BASEDEVICE_
class I_BaseDevice : public I_CommonBase
{
public:
	//the following functions can be probed. At least one of them MUST return something else then eERR_NOT_IMPLEMENTED

	//in the following methods if buffer is nullptr, required size for the buffer is returned in size
	virtual eRes GetData(void* buffer, uint32_t& size) = 0; 
	//uiCount is both, in and out. It will attempt to fill data up to uiCount, but if less, uiCOunt is changed
	virtual eRes GetNData(void* buffer, uint32_t& size, uint32_t& uiCount) = 0;
	//Starts data stream. The device calls setDataCallback to get memory where to write
	virtual eRes GetDataStream(void* (*setDataCallback(void*,uint32_t)), uint32_t& size) = 0;
	/*
	void * pTarget = nullptr;
	bool bRun = true;
	while (bRun)
	{
		pTarget = setDataCallback(pTarget,size); //on first run calls with nullptr but with size, that should be hanled as just request for buffer
		if (pTarget)
		{
			fillTargetBuffer(pTarget, size);
		}
		else
			bRun = false;
	};
	setDataCallBack(nullptr, 0); //in case the loop was interupted from devices, such call should notify the handler that the device stopped the data stream.
	*/
	//commands serve as input only to set the working mode
	//command can have stored in its Variant another Command. If so, the variant name should be S_command and variant type eUnknown
	virtual eRes GetAvailableCommands(S_command** commands, uint32_t& uiCommandCount) = 0;
	virtual eRes Command(S_command* comand) = 0;
	//For the case if device should request update of some value in GUI -> if null it just requires whole gui redraw. (mode completely chaned...)
	virtual void RegisterRedrawGUICallback(void(*RedrawGUI)(S_command*,void*, I_CommonBase * pParent),void* pParam) = 0;

	virtual const char* GetUserName() const = 0; //if empty, plugin name will be used
	virtual const char* GetIconPath() const = 0; //if empty, generic Icon will be used

	virtual eRes Connect() = 0;
	virtual eRes Disconnect() = 0;
};
#endif // !_I_BASEDEVICE_

#ifndef _I_BASEPROCESSOR_
#define _I_BASEPROCESOR_

class I_BaseProcessor : public I_BaseDevice
{
public:
	//if subclassed following setters and getters can return eERR_NOT_IMPLEMENTED. This defines what slots are available

	//in the following methods if buffer is nullptr, required size for the buffer is returned in size
	virtual eRes SetData(void* buffer, uint32_t& size) = 0;
	//uiCount is both, in and out. It will attempt to fill data up to uiCount, but if less, uiCOunt is changed
	virtual eRes SetNData(void* buffer, uint32_t& size, uint32_t& uiCount) = 0;
	//Starts data stream. The device calls setDataCallback to get memory where to write
	virtual eRes SetDataStream(void* bufferIn, uint32_t& size, void *bufferOut) = 0; //designed to be used with calling interface together with GetDataStream. This is for the setter. 
	//The output buffer is in arguments as return value can be probed for existence of implementation
};
#endif //  !_I_BASEPROCESSOR_

#endif // !_COMMON_INTERFACE_H_
