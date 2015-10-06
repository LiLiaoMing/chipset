#include "StdAfx.h"
#include "ApplicationFileLocator.h"

CApplicationFileLocator::CApplicationFileLocator(void)
{
	ZeroMemory(data, sizeof(data));
}

CApplicationFileLocator::CApplicationFileLocator(BYTE* pdata)
{
	ZeroMemory(data, sizeof(data));
	memcpy(data, pdata, 4);
}

CApplicationFileLocator::~CApplicationFileLocator(void)
{
}

void CApplicationFileLocator::SetValue(BYTE* pdata)
{
	ZeroMemory(data, sizeof(data));
	memcpy(data, pdata, 4);
}
