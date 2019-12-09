#include "documentAPI.h"

namespace DocumentAPI
{

	C_document::C_document()
	{
	}
	C_document::~C_document()
	{
	}
	int DLL_CALL testCall(int a, int b)
	{
		return a * b;
	}
	eRes SaveFile(S_Document* pDocument, wchar_t sPath)
	{
		return eOK;
	}
	eRes LoadFile(S_Document* pDocument, wchar_t sPath)
	{
		return eOK;
	}
}
