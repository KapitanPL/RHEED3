#include "common_includes.h"

#ifdef _WIN32
#include "windows.h"
#include "io.h"
#else
#include "unistd.h"
#endif
#include "stdlib.h"
#include <stack>
#include <filesystem>

const wchar_t* G_utf8towcs(const char* str)
{
	if (str == nullptr)
		return nullptr;

	//TODO: this probably requires that the locale be
	//configured somehow:

	size_t size = 0;
	mbstowcs_s(&size,nullptr, 0, str, 0);

	if (size == 0)
		return nullptr;

	wchar_t* const val = new wchar_t[size + 1];

	mbstowcs_s(&size, val, size, str, strlen(str));
	val[size] = L'\0';

	return val;
}

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

eRes WalkVariant(S_variant& var, eRes(*callback)(S_variantMeta &varMeta, void* callbackParam), void* callbackparam)
{
	std::stack<S_variantMeta> stack;
	S_variantMeta vm;
	vm.pVar = &var;
	vm.iLevel = 0;
	stack.push(vm);
	eRes res = eOK;
	while (!stack.empty() && res == eOK)
	{
		S_variantMeta meta = stack.top();
		stack.pop();
		if (meta.pVar && meta.pVar->eType == DataEnumType::eVariant && meta.pVar->pData)
		{
			S_variant* pSub = (S_variant*)meta.pVar->pData;
			for (uint32_t ui = 0; ui < meta.pVar->uiCount; ui++)
			{
				S_variantMeta vmSub;
				vmSub.iLevel = meta.iLevel++;
				vmSub.pVar = &pSub[ui];
				stack.push(vmSub);
			}
		}
		if (callback)
		{
			res = callback(meta, callbackparam);
			if (res != eOK)
				break;
		}
	}
	return res;
}

size_t VariantDataSize(S_variant& src)
{
	switch (src.eType)
	{
	case DataEnumType::eInt8:
	{
		return (src.uiCount * sizeof(int8_t));
		
	}
	case DataEnumType::eUInt8:
	{
		return (src.uiCount * sizeof(uint8_t));
		
	}
	case DataEnumType::eInt16:
	{
		return (src.uiCount * sizeof(int16_t));
		
	}
	case DataEnumType::eUInt16:
	{
		return (src.uiCount * sizeof(uint16_t));
		
	}
	case DataEnumType::eInt32:
	{
		return (src.uiCount * sizeof(int32_t));
		
	}
	case DataEnumType::eUInt32:
	{
		return (src.uiCount * sizeof(uint32_t));
		
	}
	case DataEnumType::eInt64:
	{
		return (src.uiCount * sizeof(int64_t));
		
	}
	case DataEnumType::eUInt64:
	{
		return (src.uiCount * sizeof(uint64_t));
		
	}
	case DataEnumType::eFloat:
	{
		return (src.uiCount * sizeof(float));
		
	}
	case DataEnumType::eDouble:
	{
		return (src.uiCount * sizeof(double));
		
	}
	case DataEnumType::eChar:
	{
		return (src.uiCount * sizeof(char));
		
	}
	case DataEnumType::eByte:
	{
		return (src.uiCount * sizeof(unsigned char));

	}
	case DataEnumType::eVariant:
	{
		return (src.uiCount * sizeof(S_variant));

	}
	default:
		return 0;
	}
	return 0;
}

eRes SaveVariantToTextFile(const char* sFileName, S_variant& var, bool bForceRawData, bool bVolatileData)
{
	if (sFileName && !F_FileExists(sFileName))
	{
		S_writeVariantContext context;
		context.target.open(sFileName, std::ios::out | std::ios::binary | std::ios::in | std::ios::trunc);
		context.iLastLevel = 0;

		context.target << u8"#PMSVariantFile Štìpán Test" << u8"\n";

		return WalkVariant(var, &WriteVariantToFile, &context);
	}
	return eOK;
}

std::streampos fileSize(const char* filePath) {

	std::streampos fsize = 0;
	std::ifstream file(filePath, std::ios::binary);

	fsize = file.tellg();
	file.seekg(0, std::ios::end);
	fsize = file.tellg() - fsize;
	file.close();

	return fsize;
}

eRes SaveVariantToBinFile(const char* sFileName, S_variant& var, bool bVolatileData)
{
	if (sFileName && !F_FileExists(sFileName))
	{
		S_writeVariantContext context;
		context.target.open(std::filesystem::u8path(sFileName), std::ios::out | std::ios::binary | std::ios::in | std::ios::trunc);
		std::string sTmp(sFileName);
		if (bVolatileData)
		{
			sTmp.append(u8".tmp");
			context.tmpData.open(std::filesystem::u8path(sFileName), std::ios::out | std::ios::binary | std::ios::in | std::ios::trunc);
		}
		context.iLastLevel = 0;
		WalkVariant(var, &WriteVariantToFile, &context);
		if (bVolatileData && context.tmpData.is_open())
		{
			//go through target and increase all offsets + size of target
			std::streampos targetSize = fileSize(sFileName);
			for (auto itOffset : context.listOffsets)
			{
				context.target.seekp(itOffset.first, std::ios::beg);
				uint32_t uiOffset = 0;
				context.target.read((char*)&uiOffset, sizeof(uint32_t));
				uiOffset += (uint32_t)targetSize;
				context.target.seekp(-4, std::ios::cur);
				context.target.write((char*)&uiOffset, 4);
			}
			context.target.seekp(0, std::ios::end);
			context.tmpData.seekg(0, std::ios::beg);
			
			char buffer[1024];
			std::streamsize read = 0;
			read = context.tmpData.readsome(&buffer[0], 1024);
			while (read > 0)
			{
				context.target.write(&buffer[0], read);
				read = context.tmpData.readsome(&buffer[0], 1024);
			}
			
			context.tmpData.close();

			std::filesystem::remove(sTmp);

		}
		else
		{
			std::streampos targetSize = fileSize(sFileName);
			for (auto itOffset : context.listOffsets)
			{
				context.target.seekp(itOffset.first, std::ios::beg);
				uint32_t uiOffset = 0;
				context.target.read((char*)&uiOffset, 4);
				uiOffset += (uint32_t)targetSize;
				context.target.seekp(-4, std::ios::cur);
				context.target.write((char*)&uiOffset, 4);

				context.target.seekp(0, std::ios::end);
				context.target.write((char*)itOffset.second.first, itOffset.second.second);
			}
		}
		context.target.flush();
		context.target.close();
	}
	return eERR_FAIL;
}
eRes WriteVariantToFile(S_variantMeta& varMeta, /*S_writeVariantContext&*/ void* context)
{
	S_writeVariantContext* pContext = static_cast<S_writeVariantContext*>(context);
	if (!pContext || !pContext->target.is_open())
		return eERR_FAIL;
	//if (!varMeta.pVar) !!will use this as terminating level
		//return eERR_FAIL;
	if (pContext->target.is_open() && pContext->target.flags() & std::ios::binary)
	{
		const char* sLevelBegin = "<{>";
		const char* sLevelEnd = "<}>";
		for (int8_t iL = 0; iL < pContext->iLastLevel - varMeta.iLevel; iL++)
		{
			pContext->target << sLevelEnd;
		}
		for (int8_t iL = 0; iL < varMeta.iLevel - pContext->iLastLevel +1; iL++)
		{
			pContext->target << sLevelBegin;
		}
		if (varMeta.pVar)
		{
			pContext->target.write((char*)&varMeta.pVar->eType, 4);
			pContext->target.write((char*)&varMeta.pVar->uiCount, 4);
			uint32_t uiZero = 0;
			uint64_t uiMult = 0;
			if (varMeta.pVar->eType != DataEnumType::eVariant)
				pContext->listOffsets.push_back(std::pair<std::streampos, std::pair<void*, size_t>>(pContext->target.tellp(), std::pair<void*, size_t>(varMeta.pVar->pData, VariantDataSize(*varMeta.pVar))));
			pContext->target.write((char*)&uiZero, 4); //ofset of data block in file
			pContext->target.write(varMeta.pVar->sName, sizeof(varMeta.pVar->sName) + 1); //to include the /0?
		}
		pContext->iLastLevel = varMeta.iLevel;
		
	}
	else if (pContext->target.is_open())
	{
		const char* sTab = u8"\t";
		const char* sLevelBegin = u8"{";
		const char* sLevelEnd = u8"}";
		const char* sNewLine = u8"\n";

		std::function<void(std::fstream*, int8_t)> fNewLine = [sNewLine, sTab](std::fstream *file, int8_t iLevel)
		{
			*file << sNewLine;
			for (int8_t i = 0; i < iLevel; i++)
				*file << sTab;
		};

		for (int8_t iL = 0; iL < varMeta.iLevel - pContext->iLastLevel +1; iL++)
		{
			pContext->target << sLevelBegin;
			fNewLine(&(pContext->target), pContext->iLastLevel + iL);
		}
		if (varMeta.pVar)
		{
			pContext->target << u8"Name: " << varMeta.pVar->sName;
			fNewLine(&(pContext->target), varMeta.iLevel);

			pContext->target << u8"Type: " << varMeta.pVar->eType;
			fNewLine(&(pContext->target), varMeta.iLevel);

			pContext->target << u8"Count: " << varMeta.pVar->uiCount;
			fNewLine(&(pContext->target), varMeta.iLevel);

			pContext->target << u8"Data: {";

			switch (varMeta.pVar->eType)
			{
			case DataEnumType::eInt8:
			{
				int8_t* p = (int8_t*)varMeta.pVar->pData;
				for (uint32_t ui = 0; ui < varMeta.pVar->uiCount; ui++, p++)
				{
					pContext->target << *p << u8" ";
				}
				break;
			}
			case DataEnumType::eByte:
			case DataEnumType::eUInt8:
			{
				uint8_t* p = (uint8_t*)varMeta.pVar->pData;
				for (uint32_t ui = 0; ui < varMeta.pVar->uiCount; ui++, p++)
				{
					pContext->target << *p << u8" ";
				}
				break;
			}
			case DataEnumType::eInt16:
			{
				int16_t* p = (int16_t*)varMeta.pVar->pData;
				for (uint32_t ui = 0; ui < varMeta.pVar->uiCount; ui++, p++)
				{
					pContext->target << *p << u8" ";
				}
				break;
			}
			case DataEnumType::eUInt16:
			{
				uint16_t* p = (uint16_t*)varMeta.pVar->pData;
				for (uint32_t ui = 0; ui < varMeta.pVar->uiCount; ui++, p++)
				{
					pContext->target << *p << u8" ";
				}
				break;
			}
			case DataEnumType::eInt32:
			{
				int32_t* p = (int32_t*)varMeta.pVar->pData;
				for (uint32_t ui = 0; ui < varMeta.pVar->uiCount; ui++, p++)
				{
					pContext->target << *p << u8" ";
				}
				break;
			}
			case DataEnumType::eUInt32:
			{
				uint32_t* p = (uint32_t*)varMeta.pVar->pData;
				for (uint32_t ui = 0; ui < varMeta.pVar->uiCount; ui++, p++)
				{
					pContext->target << *p << u8" ";
				}
				break;
			}
			case DataEnumType::eInt64:
			{
				int64_t* p = (int64_t*)varMeta.pVar->pData;
				for (uint32_t ui = 0; ui < varMeta.pVar->uiCount; ui++, p++)
				{
					pContext->target << *p << u8" ";
				}
				break;
			}
			case DataEnumType::eUInt64:
			{
				uint64_t* p = (uint64_t*)varMeta.pVar->pData;
				for (uint32_t ui = 0; ui < varMeta.pVar->uiCount; ui++, p++)
				{
					pContext->target << *p << u8" ";
				}
				break;
			}
			case DataEnumType::eFloat:
			{
				float* p = (float*)varMeta.pVar->pData;
				for (uint32_t ui = 0; ui < varMeta.pVar->uiCount; ui++, p++)
				{
					pContext->target << *p << u8" ";
				}
				break;
			}
			case DataEnumType::eDouble:
			{
				double* p = (double*)varMeta.pVar->pData;
				for (uint32_t ui = 0; ui < varMeta.pVar->uiCount; ui++, p++)
				{
					pContext->target << *p << u8" ";
				}
				break;
			}
			case DataEnumType::eChar:
			{
				char* p = (char*)varMeta.pVar->pData;
				pContext->target << *p;
				break;
			}
			}
			pContext->target << u8"}";

		}
	}
	else
	{
	return eERR_NOT_IMPLEMENTED;
	}
	return eOK;
}


bool F_FileExists(const char* sName)
{
#ifdef _WIN32
	if (_waccess_s(G_utf8towcs(sName), 00) == 0)
		return true;
	else
		return false; 
#else
	if (access(sName, F_OK) == 0)
		return true;
	else
		return false;
#endif
}
/*
C_variant::C_variant(S_variant* pVar)
{
	if (pVar)
	{
		if (pVar->eType == DataEnumType::eVariant)
		{
			S_variant* pSub = (S_variant*)pVar->pData;
			for (uint32_t ui = 0; ui < pVar->uiCount; ui++)
			{
				switch (pSub[ui].eType)
				{
				default:
				{
					m_mapSubVar.
				}
				}
			}
		}
		else
		{
			//isthere something to do here?
		}
	}
}*/
