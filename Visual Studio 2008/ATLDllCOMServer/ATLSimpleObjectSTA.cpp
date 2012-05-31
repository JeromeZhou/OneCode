/********************************** ģ��ͷ *********************************\
* ģ����:      ATLSimpleObjectSTA.cpp
* ��Ŀ��:      ATLDllCOMServer
* ��Ȩ (c) Microsoft Corporation.
* 
* ���������ʵ����CATLSimpleObjectSTA
* 
* This source is subject to the Microsoft Public License.
* See http://www.microsoft.com/opensource/licenses.mspx#Ms-PL.
* All other rights reserved.
* 
* THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, 
* EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
* WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
\***************************************************************************/

#pragma region Includes
#include "stdafx.h"
#include "ATLSimpleObjectSTA.h"
#pragma endregion


STDMETHODIMP CATLSimpleObjectSTA::get_FloatProperty(FLOAT* pVal)
{
	// TODO: Add your implementation code here

	*pVal = m_fField;
	return S_OK;
}

STDMETHODIMP CATLSimpleObjectSTA::put_FloatProperty(FLOAT newVal)
{
	// TODO: Add your implementation code here

    // �����¼���FloatPropertyChanging
	VARIANT_BOOL cancel = VARIANT_FALSE; 
	Fire_FloatPropertyChanging(newVal, &cancel);

	if (cancel == VARIANT_FALSE)
	{
		m_fField = newVal;	// ������ֵ
	}
    // ����֮�⣬�����κ�����
	return S_OK;
}

STDMETHODIMP CATLSimpleObjectSTA::HelloWorld(BSTR* pRet)
{
	// TODO: Add your implementation code here

    // Ϊstring�����ڴ�
	*pRet = ::SysAllocString(L"HelloWorld");
	if (pRet == NULL)
		return E_OUTOFMEMORY;

    // �ͻ������ڸ���ΪBSTR�ͷ��ڴ�
	return S_OK;
}

STDMETHODIMP CATLSimpleObjectSTA::GetProcessThreadID(LONG* pdwProcessId, LONG* pdwThreadId)
{
	// TODO: Add your implementation code here

	*pdwProcessId = GetCurrentProcessId();
	*pdwThreadId = GetCurrentThreadId();
	return S_OK;
}
