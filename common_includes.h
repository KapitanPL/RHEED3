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

typedef enum eRes
{
	eERR_VERSION = -3,
	eERR_ALREADYINUSE = -2,
	eERR_FAIL = -1,
	eOK = 0,
	eUnknown
} eRes;

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _S_Variant_
#define _S_Variant_
	namespace DataEnumType
	{
		typedef enum _eDataType
		{
			eUnknown = 0x0000,
			eSigned = 1 << 1,
			eInt8 = 1 << 2, // byte
			eInt16 = 1 << 3,
			eInt32 = 1 << 4,
			eInt64 = 1 << 5,
			eFloat = 1 << 6,
			eDouble = 1 << 7,
			eVariant = 1 << 8,
			eMax = 1 << 9
		} eDataType;
	}

	struct S_variant
	{
		const char* sName; //use only as sName = "xyz" or you are responsible for memory management
		DataEnumType::eDataType eType;
		uint32_t uiCount;
		void* pData;
	};

	eRes InitVariant(S_variant& var, DataEnumType::eDataType eType, uint32_t uiCount, const char* sName, bool bZeroMemory);

	eRes ReleaseVariant(S_variant& var);
#endif //_S_variant

	struct S_commandParam
	{
		char* sCommand;
		S_variant sData;
		uint32_t uiRefCount;
	};

	struct S_command
	{
		char* sCommand;
		S_commandParam* pParams;
		uint64_t uiParamsCount;
		uint32_t uiRefCount;
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

		const wchar_t * (*GetModuleName)();
		void (*Call)(const wchar_t * sModule, const wchar_t * sCommand, void* data, int iParam);
		void (*GetCallList)(const wchar_t* sModule, const wchar_t* sCommand, const wchar_t** sCommands, uint32_t& uiCount);

		//version Info:
		//1.0 This initial version
	};

#ifdef __cplusplus
} // extern "C"
#endif // _cplusplus

class I_CommonBase
{
public:
	virtual const char * GetIdentification() = 0;
};

#endif // !COMMON_INCLUDES_H
