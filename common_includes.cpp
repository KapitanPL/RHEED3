#include "common_includes.h"

#ifdef _WIN32
#include <windows.h>
#endif

eRes InitVariant(S_variant& var, DataEnumType::eDataType eType, uint32_t uiCount, const char* sName, bool bZeroMemory)
{
	if (uiCount > 0)
	{
		var.sName = sName;
		var.eType = eType;
		switch (eType)
		{
		case DataEnumType::eInt8:
		{
			var.pData = malloc(sizeof(int8_t) * uiCount);
			if (var.pData && bZeroMemory)
				memset(var.pData, 0, uiCount * sizeof(int8_t));
			break;
		}
		case DataEnumType::eUInt8:
		{
			var.pData = malloc(sizeof(uint8_t) * uiCount);
			if (var.pData && bZeroMemory)
				memset(var.pData, 0, uiCount * sizeof(uint8_t));
			break;
		}
		case DataEnumType::eInt16:
		{
			var.pData = malloc(sizeof(int16_t) * uiCount);
			if (var.pData && bZeroMemory)
				memset(var.pData, 0, uiCount * sizeof(int16_t));
			break;
		}
		case DataEnumType::eUInt16:
		{
			var.pData = malloc(sizeof(uint16_t) * uiCount);
			if (var.pData && bZeroMemory)
				memset(var.pData, 0, uiCount * sizeof(uint16_t));
			break;
		}
		case DataEnumType::eInt32:
		{
			var.pData = malloc(sizeof(int32_t) * uiCount);
			if (var.pData && bZeroMemory)
				memset(var.pData, 0, uiCount * sizeof(int32_t));
			break;
		}
		case DataEnumType::eUInt32:
		{
			var.pData = malloc(sizeof(uint32_t) * uiCount);
			if (var.pData && bZeroMemory)
				memset(var.pData, 0, uiCount * sizeof(uint32_t));
			break;
		}
		case DataEnumType::eInt64:
		{
			var.pData = malloc(sizeof(int64_t) * uiCount);
			if (var.pData && bZeroMemory)
				memset(var.pData, 0, uiCount * sizeof(int64_t));
			break;
		}
		case DataEnumType::eUInt64:
		{
			var.pData = malloc(sizeof(uint64_t) * uiCount);
			if (var.pData && bZeroMemory)
				memset(var.pData, 0, uiCount * sizeof(uint64_t));
			break;
		}
		case DataEnumType::eFloat:
		{
			var.pData = malloc(sizeof(float) * uiCount);
			if (var.pData && bZeroMemory)
				memset(var.pData, 0, uiCount * sizeof(float));
			break;
		}
		case DataEnumType::eDouble:
		{
			var.pData = malloc(sizeof(double) * uiCount);
			if (var.pData && bZeroMemory)
				memset(var.pData, 0, uiCount * sizeof(double));
			break;
		}
		case DataEnumType::eChar:
		{
			var.pData = malloc(sizeof(char) * uiCount);
			if (var.pData && bZeroMemory)
				memset(var.pData, 0, uiCount * sizeof(char));
			break;
		}
		case DataEnumType::eByte:
		{
			var.pData = malloc(sizeof(unsigned char) * uiCount);
			if (var.pData && bZeroMemory)
				memset(var.pData, 0, uiCount * sizeof(unsigned char));
			break;
		}
		case DataEnumType::eWChar:
		{
			var.pData = malloc(sizeof(wchar_t) * uiCount);
			if (var.pData && bZeroMemory)
				memset(var.pData, 0, uiCount * sizeof(wchar_t));
			break;
		}
		case DataEnumType::eVariant:
		{
			var.pData = malloc(sizeof(S_variant) * uiCount);
			if (var.pData && bZeroMemory)
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
			break;
		}
		default:
			return eERR_NOT_IMPLEMENTED;
		}
		if (var.pData)
		{
			var.uiCount = uiCount;
		}
		return eOK;
	}
	else
	{
		var.sName = sName;
		var.eType = eType;
		var.uiCount = 0;
		var.pData = NULL;
		return eOK;
	}
}

eRes ReleaseVariant(S_variant& var)
{
	if (var.eType > DataEnumType::eUnknown && var.eType < DataEnumType::eVariant && var.pData)
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

eRes CopyVariant(S_variant& dst, S_variant& src)
{
	ReleaseVariant(dst);
	InitVariant(dst, src.eType, src.uiCount, src.sName, false);

	switch (src.eType)
	{
	case DataEnumType::eInt8:
	{
		memcpy(dst.pData, src.pData, src.uiCount * sizeof(int8_t));
		break;
	}
	case DataEnumType::eUInt8:
	{
		memcpy(dst.pData, src.pData, src.uiCount * sizeof(uint8_t));
		break;
	}
	case DataEnumType::eInt16:
	{
		memcpy(dst.pData, src.pData, src.uiCount * sizeof(int16_t));
		break;
	}
	case DataEnumType::eUInt16:
	{
		memcpy(dst.pData, src.pData, src.uiCount * sizeof(uint16_t));
		break;
	}
	case DataEnumType::eInt32:
	{
		memcpy(dst.pData, src.pData, src.uiCount * sizeof(int32_t));
		break;
	}
	case DataEnumType::eUInt32:
	{
		memcpy(dst.pData, src.pData, src.uiCount * sizeof(uint32_t));
		break;
	}
	case DataEnumType::eInt64:
	{
		memcpy(dst.pData, src.pData, src.uiCount * sizeof(int64_t));
		break;
	}
	case DataEnumType::eUInt64:
	{
		memcpy(dst.pData, src.pData, src.uiCount * sizeof(uint64_t));
		break;
	}
	case DataEnumType::eFloat:
	{
		memcpy(dst.pData, src.pData, src.uiCount * sizeof(float));
		break;
	}
	case DataEnumType::eDouble:
	{
		memcpy(dst.pData, src.pData, src.uiCount * sizeof(double));
		break;
	}
	case DataEnumType::eChar:
	{
		memcpy(dst.pData, src.pData, src.uiCount * sizeof(char));
		break;
	}
	case DataEnumType::eByte:
	{
		memcpy(dst.pData, src.pData, src.uiCount * sizeof(unsigned char));
		break;
	}
	case DataEnumType::eWChar:
	{
		memcpy(dst.pData, src.pData, src.uiCount * sizeof(wchar_t));
		break;
	}
	case DataEnumType::eVariant:
	{
		S_variant* pDstSubVariant = (S_variant*)dst.pData;
		S_variant* pSrcSubVariant = (S_variant*)src.pData;
		eRes res = eOK;
		for (uint32_t ui = 0; ui < src.uiCount; ui++, pDstSubVariant++, pSrcSubVariant++)
		{
			res = CopyVariant(*pDstSubVariant, *pSrcSubVariant);
			if (res != eOK)
				return res;
		}
		break;
	}
	default:
		return eERR_NOT_IMPLEMENTED;
	}
	return eOK;
}
