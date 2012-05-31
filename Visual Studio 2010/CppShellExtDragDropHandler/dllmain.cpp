/****************************** ģ��ͷ******************************\
ģ����:  dllmain.cpp
��Ŀ:      CppShellExtPropSheetHandler
��Ȩ (c) Microsoft Corporation.

����ļ�ʵ����DllMain,�� DllGetClassObject, DllCanUnloadNow, 
DllRegisterServer, DllUnregisterServer��������Щ��������COM DLL�Ǳ�Ҫ�ġ�

DllGetClassObject �����๤��������๤��������ClassFactory.h/cpp ��ָ���Ľӿڡ�

DllCanUnloadNow��飬������ǿ��Դ��ڴ�ж�������.

COM��������DllRegisterServer�Ĵ���������ϷŴ������ͨ�����ö���Reg.h/cpp�ĺ�����ע��ġ�
������ϷŴ��������.exe���ļ�����ϵ��


DllUnregisterServerû��ע����  COM ������������ϷŴ������. 

This source is subject to the Microsoft Public License.
See http://www.microsoft.com/opensource/licenses.mspx#Ms-PL.
All other rights reserved.

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, 
EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
\***************************************************************************/

#include <windows.h>
#include <Guiddef.h>
#include "ClassFactory.h"           // ����๤��
#include "Reg.h"


// {F574437A-F944-4350-B7E9-95B6C7008029}
const CLSID CLSID_FileDragDropExt = 
{ 0xF574437A, 0xF944, 0x4350, { 0xB7, 0xE9, 0x95, 0xB6, 0xC7, 0x00, 0x80, 0x29 } };


HINSTANCE   g_hInst     = NULL;
long        g_cDllRef   = 0;


BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
        // ����� DLL ģ���ʵ�������ǽ���������ȡ��DLL��·����ע�����.
        g_hInst = hModule;
        DisableThreadLibraryCalls(hModule);
        break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}


//
//   ����: DllGetClassObject
//
//   Ŀ�ģ������๤������ѯ�ض��Ľӿڡ�

//
//   ����:
//   * rclsid - CLSID����ϵ��ȷ�����ݺʹ���.
//   * riid - �ӿڵı�ʶ�Ĳο�������ӿڵĵ������������������Ĺ�ͨ��
//   * ppv -ָ������ĵ�ַ������������շ���riid�ڵĽӿ�ָ�롣�ɹ����غ� *ppv�����������Ľӿ�ָ�롣������ִ��󣬸ýӿ�ָ��
//   ΪNULL��

      

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void **ppv)
{
    HRESULT hr = CLASS_E_CLASSNOTAVAILABLE;

    if (IsEqualCLSID(CLSID_FileDragDropExt, rclsid))
    {
        hr = E_OUTOFMEMORY;

        ClassFactory *pClassFactory = new ClassFactory();
        if (pClassFactory)
        {
            hr = pClassFactory->QueryInterface(riid, ppv);
            pClassFactory->Release();
        }
    }

    return hr;
}


//
//  ����: DllCanUnloadNow
//
//   Ŀ��:��� ��������ܴ��ڴ���ж�����.
//
//   ע��: �����õ���������ʱ�������ܴ��ڴ���ж�أ�û���κ���һֱ����ʹ����������
//
STDAPI DllCanUnloadNow(void)
{
    return g_cDllRef > 0 ? S_FALSE : S_OK;
}


//
// ����: DllRegisterServer
//
//  Ŀ��: ע�� COM ���� ������ϷŴ������.
// 
STDAPI DllRegisterServer(void)
{
    HRESULT hr;

    wchar_t szModule[MAX_PATH];
    if (GetModuleFileName(g_hInst, szModule, ARRAYSIZE(szModule)) == 0)
    {
        hr = HRESULT_FROM_WIN32(GetLastError());
        return hr;
    }

    // ע��������.
    hr = RegisterInprocServer(szModule, CLSID_FileDragDropExt, 
        L"CppShellExtDragDropHandler.FileDragDropExt Class", 
        L"Apartment");
    if (SUCCEEDED(hr))
    {
        // ע�������괦�����. 
        hr = RegisterShellExtDragDropHandler(CLSID_FileDragDropExt, 
            L"CppShellExtDragDropHandler.FileDragDropExt");
    }

    return hr;
}


//
//   ����: DllUnregisterServer
//
//   Ŀ��:û��ע�� COM ���������ϷŴ������.
// 
STDAPI DllUnregisterServer(void)
{
    HRESULT hr = S_OK;

    wchar_t szModule[MAX_PATH];
    if (GetModuleFileName(g_hInst, szModule, ARRAYSIZE(szModule)) == 0)
    {
        hr = HRESULT_FROM_WIN32(GetLastError());
        return hr;
    }

    // ע�����.
    hr = UnregisterInprocServer(CLSID_FileDragDropExt);
    if (SUCCEEDED(hr))
    {
        // ע����괦�����.
        hr = UnregisterShellExtDragDropHandler(CLSID_FileDragDropExt);
    }

    return hr;
}