#ifndef COMMON_INCLUDES_H
#define COMMON_INCLUDES_H

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

enum eRes
{
	eERR_FAIL = -1,
	eOK = 0,
	eUnknown
};

#endif // !COMMON_INCLUDES_H
