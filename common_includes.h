#ifndef COMMON_INCLUDES_H
#define COMMON_INCLUDES_H

#include "stdint.h"

#if defined _WIN32 || defined __CYGWIN__
  #ifdef DLL_EXPORT
    // Exporting...
    #ifdef __GNUC__
      #define DLL_CALL __attribute__ ((dllexport))
    #else
      #define DLL_CALL __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #else
    #ifdef __GNUC__
      #define DLL_CALL __attribute__ ((dllimport))
    #else
      #define DLL_CALL __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #endif
  #define NOT_EXPORTED
#else
  #if __GNUC__ >= 4
    #define DLL_CALL __attribute__ ((visibility ("default")))
    #define NOT_EXPORTED  __attribute__ ((visibility ("hidden")))
  #else
    #define DLL_CALL
    
    #define NOT_EXPORTED
  #endif
#endif

const wchar_t* G_utf8towcs(const char* str);

typedef int eRes;
#define	eERR_NOT_IMPLEMENTED -4
#define	eERR_VERSION -3
#define	eERR_ALREADYINUSE -2
#define	eERR_FAIL -1
#define	eOK 0

//variant names
static const char* COM_GUI_NAME = "GUI_NAME"; //displayed in userspace
static const char* COM_ID_NAME = "ID_NAME"; //id To be send back, to inform what triggered it
static const char* COM_VALUE = "VALUE"; //single value, usually used when calling back. When filled will be used as default
///if value supplied together with Button it can indicate toolbutton /button as checkbox
static const char* COM_RANGE = "RANGE"; //is supposed to contain two values, MIN and MAX
static const char* COM_MAX = "MAX";
static const char* COM_MIN = "MIN";
static const char* COM_ENUM_VALS = "ENUM_VALS"; //multiple values. Can be used to populate radio group, or combo box
static const char* COM_STRING_INPUT = "STRING_INPUT"; //asks for string back, usually edit
static const char* COM_BUTTON = "BUTTON"; //if data && data.type == eInt8, 0 - normal push button, 1 - 
static const char* COM_CHECKBOX = "CHECK_BOX";
static const char* COM_COMBO = "COMBO_BOX";
static const char* COM_TOOLTIP = "TOOLTIP";

static const char* COM_ICON = "ICON"; //expects path to icon used for the command

static const char* COM_DEVICE_CALLBACK = "DEVICE_CALLBACK"; //gui changed, report to device

#ifdef __cplusplus
#include <functional>
#include <fstream>
#include <iostream>
#include <list>
#include <utility>
extern "C" {
#endif

#ifndef _S_Variant_
#define _S_Variant_
	namespace DataEnumType
	{
		typedef enum eDataType : uint32_t
		{
			eUnknown = 0,
			eInt8, 
			eUInt8,
			eInt16,
			eUInt16,
			eInt32,
			eUInt32,
			eInt64,
			eUInt64,
			eFloat,
			eDouble,
			eChar,
			eByte,
			eVariant,
			eMax
		} eDataType;
	}

	struct S_variant
	{
		uint32_t uiCount;
		void* pData;
		const char* sName; //use only as sName = "xyz" or you are responsible for memory management
		DataEnumType::eDataType eType;
	};

	struct S_variantMeta
	{
		S_variant* pVar;
		int8_t iLevel;
	};

	eRes InitVariant(S_variant& var, DataEnumType::eDataType eType, uint32_t uiCount, const char* sName, bool bZeroMemory);

	eRes ReleaseVariant(S_variant& var);

	eRes CopyVariant(S_variant& dst, S_variant& src);

	eRes WalkVariant(S_variant& var, eRes(*callback)(S_variantMeta &varMeta, void* callbackParam), void* callbackparam);

	size_t VariantDataSize(S_variant& var);

	//bVolatileData can mean that the data needs to be saved at least to intermediate file, the original data will soon be released
	eRes SaveVariantToTextFile(const char* sFileName, S_variant& var, bool bForceRawData = false, bool bVolatileData = false);

	eRes SaveVariantToBinFile(const char* sFileName, S_variant& var, bool bVolatileData = false); 

	eRes WriteVariantToFile(S_variantMeta& varMeta, /*S_writeVariantContext&*/ void* context);

	//S_variant& FindSubVariant(S_variant& var, DataEnumType::eDataType eType, const char* sName);
#endif //_S_variant

	struct S_command
	{
		const char* sCommand;
		S_variant sData;
	};

	struct S_Version
	{
		int iMajor;
		int iMinor;
	};

	struct I_PMS_V01
	{
		S_Version sVersion;

		eRes (*Attach)(void* iGuest, S_Version guestVersion, S_Version minRequiredHostVersion);
		void (*Detach)(void* iGuest);
		void (*RegisterHostDetach)(void (*HostIsDetaching)(void* iHost));

		const char * (*GetModuleName)();
		void (*Call)(const char * sModule, const char * sCommand, void* data, int iParam);
		void (*GetCallList)(const char* sModule, const char* sCommand, const char** sCommands, uint32_t& uiCount);

		//version Info:
		//1.0 This initial version
	};

#ifdef __cplusplus
} // extern "C"

struct S_writeVariantContext
{
	std::fstream target;
	std::fstream tmpData;
	int8_t iLastLevel;
	std::list<std::pair<std::streampos, std::pair<void*,size_t>>> listOffsets;
};

bool F_FileExists(const char* sName);

template<typename T> eRes UseVariantValue(S_variant& var, T fun)
{
	switch (var.eType)
	{
	case DataEnumType::eInt8:
		return fun((int8_t*)var.pData, var.uiCount);
	case DataEnumType::eUInt8:
		return fun((uint8_t*)var.pData, var.uiCount);
	case DataEnumType::eInt16:
		return fun((int16_t*)var.pData, var.uiCount);
	case DataEnumType::eUInt16:
		return fun((uint16_t*)var.pData, var.uiCount);
	case DataEnumType::eInt32:
		return fun((int32_t*)var.pData, var.uiCount);
	case DataEnumType::eUInt32:
		return fun((uint32_t*)var.pData, var.uiCount);
	case DataEnumType::eInt64:
		return fun((int64_t*)var.pData, var.uiCount);
	case DataEnumType::eUInt64:
		return fun((uint64_t*)var.pData, var.uiCount);
	case DataEnumType::eFloat:
		return fun((float*)var.pData, var.uiCount);
	case DataEnumType::eDouble:
		return fun((double*)var.pData, var.uiCount);
	case DataEnumType::eChar:
		return fun((char*)var.pData, var.uiCount);
	case DataEnumType::eByte:
		return fun((unsigned char*)var.pData, var.uiCount);
	case DataEnumType::eVariant:
		return fun((S_variant*)var.pData, var.uiCount);
	default:
		return eERR_NOT_IMPLEMENTED;
	}
};

#endif // _cplusplus
/*
class C_variant
{
public:
	C_variant(S_variant* pVar);
private:
	S_variant* m_pRef = nullptr;
	std::multimap<const char*, S_variant*> m_mapSubVars;
	uint64_t uiTotalSubSize = 0;
};*/


#endif // !COMMON_INCLUDES_H
