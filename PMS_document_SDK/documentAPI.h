#ifndef PMS_DOCUMENT_API_H
#define PMS_DOCUMENT_API_H

#include "./../common_includes.h"

namespace DocumentAPI
{

	enum eDataType
	{
		eByte = 0,
		eInt32,
		eUInt32,
		eInt64,
		eUInt64,
		eFloat,
		eDouble,
		eWChar,
		eChar,
		eUnknown
	};

	struct S_Data
	{
		size_t uiDescriptionSize;
		void* pDescription;
		size_t uiSize;
		void* pData;
	};

	struct S_Document
	{
		char sName;
		S_Data* pData;
		size_t	uiDataCount;
	};

#ifdef __cplusplus

	int DLL_CALL testCall(int a, int b);

	class DLL_CALL C_document
	{
	public:
		C_document();
		virtual ~C_document();
	private:
		S_Document m_sDocument;
	};

	extern "C" {
#endif
		eRes SaveFile(S_Document* pDocument, char sPath);
		eRes LoadFile(S_Document* pDocument, char sPath);
#ifdef __cplusplus
	}
#endif
	
};

#endif // !PMS_DOCUMENT_API_H
