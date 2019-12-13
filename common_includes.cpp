#include "common_includes.h"

#ifdef _WIN32
#include <windows.h>
#endif

eRes InitVariant(S_variant& var, DataEnumType::eDataType eType, uint32_t uiCount, const char* sName, bool bZeroMemory)
{
	if (uiCount > 0)
	{
		bool bOK = false;
		var.sName = sName;
		var.eType = eType;
		uint32_t uiMultiplicator = 0;
		if (eType & DataEnumType::eInt8)
			uiMultiplicator = 1;
		else if (eType & DataEnumType::eInt16)
			uiMultiplicator = 2;
		else if (eType & DataEnumType::eInt32 || eType & DataEnumType::eFloat)
			uiMultiplicator = 4;
		else if (eType & DataEnumType::eInt64 || eType & DataEnumType::eDouble)
			uiMultiplicator = 8;
		if (uiMultiplicator)
		{
			var.pData = malloc(uiMultiplicator * uiCount);
			if (var.pData)
			{
				var.uiCount = uiCount;
				bOK = true;
			}
			else
			{
				return eERR_FAIL;
			}
		}
		else if (eType & DataEnumType::eVariant)
		{
			var.pData = malloc(uiCount * sizeof(S_variant));
			bOK = true;
		}
		if (bOK && bZeroMemory)
		{
			if (eType & DataEnumType::eInt8)
			{
				if (eType & DataEnumType::eSigned)
				{
					int8_t* pD = (int8_t*)(var.pData);
					for (uint32_t ui = 0; ui < uiCount; ui++)
					{
						*pD = 0;
						pD++;
					}
				}
				else
				{
					uint8_t* pD = (uint8_t*)(var.pData);
					for (uint32_t ui = 0; ui < uiCount; ui++)
					{
						*pD = 0;
						pD++;
					}
				}
			}
			else if (eType & DataEnumType::eInt16)
			{
				if (eType & DataEnumType::eSigned)
				{
					int16_t* pD = (int16_t*)(var.pData);
					for (uint32_t ui = 0; ui < uiCount; ui++)
					{
						*pD = 0;
						pD++;
					}
				}
				else
				{
					uint16_t* pD = (uint16_t*)(var.pData);
					for (uint32_t ui = 0; ui < uiCount; ui++)
					{
						*pD = 0;
						pD++;
					}
				}
			}
			else if (eType & DataEnumType::eInt32)
			{
				if (eType & DataEnumType::eSigned)
				{
					int32_t* pD = (int32_t*)(var.pData);
					for (uint32_t ui = 0; ui < uiCount; ui++)
					{
						*pD = 0;
						pD++;
					}
				}
				else
				{
					uint32_t* pD = (uint32_t*)(var.pData);
					for (uint32_t ui = 0; ui < uiCount; ui++)
					{
						*pD = 0;
						pD++;
					}
				}
			}
			else if (eType & DataEnumType::eInt64)
			{
				if (eType & DataEnumType::eSigned)
				{
					int64_t* pD = (int64_t*)(var.pData);
					for (uint32_t ui = 0; ui < uiCount; ui++)
					{
						*pD = 0;
						pD++;
					}
				}
				else
				{
					uint64_t* pD = (uint64_t*)(var.pData);
					for (uint32_t ui = 0; ui < uiCount; ui++)
					{
						*pD = 0;
						pD++;
					}
				}
			}
			else if (eType & DataEnumType::eFloat)
			{
				float* pD = (float*)(var.pData);
				for (uint32_t ui = 0; ui < uiCount; ui++)
				{
					*pD = 0.0;
					pD++;
				}
			}
			else if (eType & DataEnumType::eDouble)
			{
				double* pD = (double*)(var.pData);
				for (uint32_t ui = 0; ui < uiCount; ui++)
				{
					*pD = 0.0;
					pD++;
				}
			}
			else if (eType & DataEnumType::eVariant)
			{
				S_variant* pD = (S_variant*)(var.pData);
				for (uint32_t ui = 0; ui < uiCount; ui++)
				{
					pD->eType = DataEnumType::eUnknown;
					pD->pData = NULL;
					pD->sName = NULL;
					pD->uiCount = 0;
					pD++;
				}
			}
			return eOK;
		}
	}
	else
		return eERR_FAIL;
	return eERR_FAIL;
}

eRes ReleaseVariant(S_variant& var)
{
	uint32_t uiMultiplicator = 0;
	if (var.eType & DataEnumType::eInt8)
		uiMultiplicator = 1;
	else if (var.eType & DataEnumType::eInt16)
		uiMultiplicator = 2;
	else if (var.eType & DataEnumType::eInt32 || var.eType & DataEnumType::eFloat)
		uiMultiplicator = 4;
	else if (var.eType & DataEnumType::eInt64 || var.eType & DataEnumType::eDouble)
		uiMultiplicator = 8;
	if (uiMultiplicator)
	{
		free(var.pData);
		var.pData = 0;
		var.eType = DataEnumType::eUnknown;
		var.uiCount = 0;
		var.sName = 0;
		return eOK;
	}
	else if (var.eType & DataEnumType::eVariant)
	{
		S_variant* pSubVariant = (S_variant*)var.pData;
		eRes res = eOK;
		for (uint32_t ui = 0; ui < var.uiCount; ui++, pSubVariant++)
		{
			res = ReleaseVariant(*pSubVariant);
			if (res != eOK)
				return res;
		}
		free(var.pData);
		var.pData = 0;
		var.eType = DataEnumType::eUnknown;
		var.uiCount = 0;
		var.sName = 0;
		return eOK;
	}
	return eOK;
}
