/****************************** ģ��ͷ ******************************\
ģ����:  Reg.cpp
��Ŀ��:  CppShellExtPropSheetHandler
��Ȩ(c) Microsoft Corporation.

����ļ�ʵ�����ڽ�����COM�����Shell����ϷŴ�������ע���ע�����ظ�ʹ�õĺ�����


RegisterInprocServer - ��ע�����ע������ڵ����.
UnregisterInprocServer - ��ע�����ע�������ڵ����.
RegisterShellExtDragDropHandler - ע������ϷŴ������.
UnregisterShellExtDragDropHandler - ע������ϷŴ������.

This source is subject to the Microsoft Public License.
See http://www.microsoft.com/opensource/licenses.mspx#Ms-PL.
All other rights reserved.

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, 
EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
\***************************************************************************/

#include "Reg.h"
#include <strsafe.h>


#pragma region Registry Helper Functions

//
//  ����: SetHKCRRegistryKeyAndValue
//
//  Ŀ��:�ú�������һ��ע�����HKCR������ָ��ע���ֵ��
//
//
//  ����:
//   * pszSubKey -��HKCR��ָ��ע��������������ڣ�����������������ע����
//   * pszValueName -ָ����ע���ֵ�����á����pszValueName
//    ΪNULL���ú���������Ĭ��ֵ��

//   * pszData -ָ��ע���ֵ���ַ�������
//
//   ����ֵ��
//��������ɹ�������S_OK��������������һ��
// HRESULT������롣

// 
HRESULT SetHKCRRegistryKeyAndValue(PCWSTR pszSubKey, PCWSTR pszValueName, 
    PCWSTR pszData)
{
    HRESULT hr;
    HKEY hKey = NULL;

    //����һ��ָ����ע���ֵ. ������ֵ�Ѿ����ڣ���������ͽ������� 
    //
    hr = HRESULT_FROM_WIN32(RegCreateKeyEx(HKEY_CLASSES_ROOT, pszSubKey, 0, 
        NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL));

    if (SUCCEEDED(hr))
    {
        if (pszData != NULL)
        {
            //����ָ���ļ�ֵ
            DWORD cbData = lstrlen(pszData) * sizeof(*pszData);
            hr = HRESULT_FROM_WIN32(RegSetValueEx(hKey, pszValueName, 0, 
                REG_SZ, reinterpret_cast<const BYTE *>(pszData), cbData));
        }

        RegCloseKey(hKey);
    }

    return hr;
}


//
//   ����: GetHKCRRegistryKeyAndValue
//
//   Ŀ��: �ú�����һ��ע�����HKCR���õ���
//         ָ����ע���ֵ���������ݡ�
//
//   PARAMETERS:
//   * pszSubKey - ��HKCR��ָ��ע���ֵ��������ֵ�����ڣ��������������һ������
//   * pszValueName -ָ����������ע���ֵ.���
//     pszValueName �ǿյ�, ������������Ĭ��ֵ.
//   * pszData - ��������ָ������ַ������ݵ�ֵ��
//   * cbData - ָ���������Ĵ�С��
//
//   ����ֵ:
//     ��������ɹ�������S_OK��������������һ��
//     HRESULT������롣���磬���ָ����ע����
//     ���ڻ�ָ��ֵ�����Ƶ����ݻ�û�����ã�����
//     ����COR_E_FILENOTFOUND��0x80070002����

// 
HRESULT GetHKCRRegistryKeyAndValue(PCWSTR pszSubKey, PCWSTR pszValueName, 
    PWSTR pszData, DWORD cbData)
{
    HRESULT hr;
    HKEY hKey = NULL;

    // ���Դ�ָ����ע����
    hr = HRESULT_FROM_WIN32(RegOpenKeyEx(HKEY_CLASSES_ROOT, pszSubKey, 0, 
        KEY_READ, &hKey));

    if (SUCCEEDED(hr))
    {
        // ��ȡָ����ֵ�����Ƶ����ݡ�

        hr = HRESULT_FROM_WIN32(RegQueryValueEx(hKey, pszValueName, NULL, 
            NULL, reinterpret_cast<LPBYTE>(pszData), &cbData));

        RegCloseKey(hKey);
    }

    return hr;
}

#pragma endregion


//
//   ����: RegisterInprocServer
//
//   Ŀ��:��ע�����ע����������.
//
//   ����:
//   * pszModule - ·����ģ����������
//   * clsid -�������ID
//   * pszFriendlyName -�Ѻõ�����
//   * pszThreadModel -�߳̿�
//
//   NOTE: The function creates the HKCR\CLSID\{<CLSID>} key in the registry.
// 
//   HKCR
//   {
//      NoRemove CLSID
//      {
//          ForceRemove {<CLSID>} = s '<Friendly Name>'
//          {
//              InprocServer32 = s '%MODULE%'
//              {
//                  val ThreadingModel = s '<Thread Model>'
//              }
//          }
//      }
//   }
//
HRESULT RegisterInprocServer(PCWSTR pszModule, const CLSID& clsid, 
    PCWSTR pszFriendlyName, PCWSTR pszThreadModel)
{
    if (pszModule == NULL || pszThreadModel == NULL)
    {
        return E_INVALIDARG;
    }

    HRESULT hr;

    wchar_t szCLSID[MAX_PATH];
    StringFromGUID2(clsid, szCLSID, ARRAYSIZE(szCLSID));

    wchar_t szSubkey[MAX_PATH];

    //����HKCR\CLSID\{<CLSID>} �ļ�ֵ.
    hr = StringCchPrintf(szSubkey, ARRAYSIZE(szSubkey), L"CLSID\\%s", szCLSID);
    if (SUCCEEDED(hr))
    {
        hr = SetHKCRRegistryKeyAndValue(szSubkey, NULL, pszFriendlyName);

        // ���� HKCR\CLSID\{<CLSID>}\InprocServer32 ��ֵ.
        if (SUCCEEDED(hr))
        {
            hr = StringCchPrintf(szSubkey, ARRAYSIZE(szSubkey), 
                L"CLSID\\%s\\InprocServer32", szCLSID);
            if (SUCCEEDED(hr))
            {
                // ����COMģ���·����InprocServer32��Ĭ��ֵ��
                hr = SetHKCRRegistryKeyAndValue(szSubkey, NULL, pszModule);
                if (SUCCEEDED(hr))
                {
                    // ����������߳̿�.
                    hr = SetHKCRRegistryKeyAndValue(szSubkey, 
                        L"ThreadingModel", pszThreadModel);
                }
            }
        }
    }

    return hr;
}


//
//   ����: UnregisterInprocServer
//
//   Ŀ��:��ע�����ע�������ڵ����.
//
//   ����:
//   * clsid - �������ID��
//
//  ע��: ���������ע�����ɾ�����HKCR\CLSID\{<CLSID>}��ֵ��
//
HRESULT UnregisterInprocServer(const CLSID& clsid)
{
    HRESULT hr = S_OK;

    wchar_t szCLSID[MAX_PATH];
    StringFromGUID2(clsid, szCLSID, ARRAYSIZE(szCLSID));

    wchar_t szSubkey[MAX_PATH];

    // ɾ�� HKCR\CLSID\{<CLSID>} ��ֵ.
    hr = StringCchPrintf(szSubkey, ARRAYSIZE(szSubkey), L"CLSID\\%s", szCLSID);
    if (SUCCEEDED(hr))
    {
        hr = HRESULT_FROM_WIN32(RegDeleteTree(HKEY_CLASSES_ROOT, szSubkey));
    }

    return hr;
}


//
//   ����: RegisterShellExtDragDropHandler
//
//   Ŀ��: ע������ϷŴ������.
//
//   ����:
//   * clsid - �������ID
//   * pszFriendlyName - �Ѻõ�����
//
//   ע��: ���������ע����д���������ļ�ֵ.
//
//   HKCR
//   {
//      NoRemove Directory
//      {
//          NoRemove shellex
//          {
//              NoRemove DragDropHandlers
//              {
//                  {<CLSID>} = s '<Friendly Name>'
//              }
//          }
//      }
//      NoRemove Folder
//      {
//          NoRemove shellex
//          {
//              NoRemove DragDropHandlers
//              {
//                  {<CLSID>} = s '<Friendly Name>'
//              }
//          }
//      }
//      NoRemove Drive
//      {
//          NoRemove shellex
//          {
//              NoRemove DragDropHandlers
//              {
//                  {<CLSID>} = s '<Friendly Name>'
//              }
//          }
//      }
//   }
//
HRESULT RegisterShellExtDragDropHandler(const CLSID& clsid, PCWSTR pszFriendlyName)
{
    wchar_t szCLSID[MAX_PATH];
    StringFromGUID2(clsid, szCLSID, ARRAYSIZE(szCLSID));

    wchar_t szSubkey[MAX_PATH];

    // ����ϷŴ������ͨ��������ע�� 
    //����: HKCR\Directory\shellex\DragDropHandlers.
    // ���������ֵ HKCR\Directory\shellex\DragDropHandlers\{<CLSID>}
    HRESULT hr = StringCchPrintf(szSubkey, ARRAYSIZE(szSubkey), 
        L"Directory\\shellex\\DragDropHandlers\\%s", szCLSID);
    if (SUCCEEDED(hr))
    {
        // ����Ĭ�ϵļ�ֵ.
        hr = SetHKCRRegistryKeyAndValue(szSubkey, NULL, pszFriendlyName);
    }

    //��һЩ����£��������HKCR\Folder��ע�ᣬΪ�˴������棬��HKCR\Drive�ĸ������� 
    if (SUCCEEDED(hr))
    {
        hr = StringCchPrintf(szSubkey, ARRAYSIZE(szSubkey), 
            L"Folder\\shellex\\DragDropHandlers\\%s", szCLSID);
        if (SUCCEEDED(hr))
        {
            // ����Ĭ�ϵļ�ֵ
            hr = SetHKCRRegistryKeyAndValue(szSubkey, NULL, pszFriendlyName);
        }
    }

    if (SUCCEEDED(hr))
    {
        hr = StringCchPrintf(szSubkey, ARRAYSIZE(szSubkey), 
            L"Drive\\shellex\\DragDropHandlers\\%s", szCLSID);
        if (SUCCEEDED(hr))
        {
            // ����Ĭ�ϵļ�ֵ.
            hr = SetHKCRRegistryKeyAndValue(szSubkey, NULL, pszFriendlyName);
        }
    }

    return hr;
}


//
//   ����: UnregisterShellExtDragDropHandler
//
//  Ŀ��: Unregister the drag-and-drop handler.
//
//  ����:
//   * clsid - �������ID
//
//   ע��: ��������Ƴ�����ע����е�HKCR\Directory\shellex\DragDropHandlers, 
//   HKCR\Folder\shellex\DragDropHandlers 
//   HKCR\Drive\shellex\DragDropHandlers �µ� {<CLSID>}��ֵ��
//
HRESULT UnregisterShellExtDragDropHandler(const CLSID& clsid)
{
    wchar_t szCLSID[MAX_PATH];
    StringFromGUID2(clsid, szCLSID, ARRAYSIZE(szCLSID));

    wchar_t szSubkey[MAX_PATH];

    // �Ƴ���� HKCR\Directory\shellex\DragDropHandlers\{<CLSID>} ��ֵ.
    HRESULT hr = StringCchPrintf(szSubkey, ARRAYSIZE(szSubkey), 
        L"Directory\\shellex\\DragDropHandlers\\%s", szCLSID);
    if (SUCCEEDED(hr))
    {
        hr = HRESULT_FROM_WIN32(RegDeleteTree(HKEY_CLASSES_ROOT, szSubkey));
    }

    if (SUCCEEDED(hr))
    {
        // �Ƴ� HKCR\Folder\shellex\DragDropHandlers\{<CLSID>} ��ֵ.
        hr = StringCchPrintf(szSubkey, ARRAYSIZE(szSubkey), 
            L"Folder\\shellex\\DragDropHandlers\\%s", szCLSID);
        if (SUCCEEDED(hr))
        {
            hr = HRESULT_FROM_WIN32(RegDeleteTree(HKEY_CLASSES_ROOT, szSubkey));
        }
    }

    if (SUCCEEDED(hr))
    {
        //�Ƴ� HKCR\Drive\shellex\DragDropHandlers\{<CLSID>} ��ֵ.
        hr = StringCchPrintf(szSubkey, ARRAYSIZE(szSubkey), 
            L"Drive\\shellex\\DragDropHandlers\\%s", szCLSID);
        if (SUCCEEDED(hr))
        {
            hr = HRESULT_FROM_WIN32(RegDeleteTree(HKEY_CLASSES_ROOT, szSubkey));
        }
    }

    return hr;
}