#ifndef PMS_DOCUMENT_API_H
#define PMS_DOCUMENT_API_H

#include "./../common_includes.h"

namespace DocumentAPI
{
#ifdef __cplusplus
	extern "C" {
#endif

#ifdef __cplusplus
	}
#endif
	int DLL_CALL testCall(int a, int b);

	class DLL_CALL C_document
	{
	public:
		C_document();
		virtual ~C_document();
	};
};

#endif // !PMS_DOCUMENT_API_H
