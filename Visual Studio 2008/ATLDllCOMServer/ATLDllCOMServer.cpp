/********************************* ģ��ͷ *********************************\
* ģ����:      ATLDllCOMServer.cpp
* ��Ŀ��:      ATLDllCOMServer
* ��Ȩ (c) Microsoft Corporation.
* 
* ATL��ּ�ڿ�����Ч�ģ����ģ���������COM����Լ�������������̡�
* ATLDllCOMServer�ṩ��һ��ʵ���ڶ�̬���ӿ⣨DLL���еĽ����ڷ��������󡣲�����
* ��һ�����ÿ��ƴ�����Ľ��̿ռ������С�
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
#include "resource.h"
#include "ATLDllCOMServer_i.h"
#include "dllmain.h"
#include "dlldatax.h"
#pragma endregion


//���ڼ���Ƿ��DLL�ܱ�OLEж��
STDAPI DllCanUnloadNow(void)
{
#ifdef _MERGE_PROXYSTUB
    HRESULT hr = PrxDllCanUnloadNow();
    if (hr != S_OK)
        return hr;
#endif
    return _AtlModule.DllCanUnloadNow();
}


// ����һ���๤�������๤��������һ������ѯ���͵Ķ���
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
#ifdef _MERGE_PROXYSTUB
    if (PrxDllGetClassObject(rclsid, riid, ppv) == S_OK)
        return S_OK;
#endif
    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}



// DllRegisterServer ��ϵͳע����������Ŀ
STDAPI DllRegisterServer(void)
{
    // ע�������⣬����������еĽӿ�
    HRESULT hr = _AtlModule.DllRegisterServer();
#ifdef _MERGE_PROXYSTUB
    if (FAILED(hr))
        return hr;
    hr = PrxDllRegisterServer();
#endif
	return hr;
}


// DllUnregisterServer ��ϵͳע������Ƴ���Ŀ
STDAPI DllUnregisterServer(void)
{
	HRESULT hr = _AtlModule.DllUnregisterServer();
#ifdef _MERGE_PROXYSTUB
    if (FAILED(hr))
        return hr;
    hr = PrxDllRegisterServer();
    if (FAILED(hr))
        return hr;
    hr = PrxDllUnregisterServer();
#endif
	return hr;
}

// DllInstall - ����ÿ̨�����е�ÿ���û���ע��������/ɾ����Ŀ
STDAPI DllInstall(BOOL bInstall, LPCWSTR pszCmdLine)
{
    HRESULT hr = E_FAIL;
    static const wchar_t szUserSwitch[] = _T("user");

    if (pszCmdLine != NULL)
    {
    	if (_wcsnicmp(pszCmdLine, szUserSwitch, _countof(szUserSwitch)) == 0)
    	{
    		AtlSetPerUserRegistration(true);
    	}
    }

    if (bInstall)
    {	
    	hr = DllRegisterServer();
    	if (FAILED(hr))
    	{	
    		DllUnregisterServer();
    	}
    }
    else
    {
    	hr = DllUnregisterServer();
    }

    return hr;
}


