/********************************** ģ��ͷ *********************************\
* ģ����:      CppUACSelfElevation.cpp
* ��Ŀ��:      CppUACSelfElevation
* ��Ȩ (c) Microsoft Corporation.
* 
* �û��˻����� ��UAC����Windows Vista����������ϵͳ�е�һ���°�ȫ�������UAC��
* ��ȫ����ʱ������Ա�Ľ�������ͨ����������ͨ�û�Ȩ���¡����ʾ����ʾ�����ȥ��
* �⵱ǰ���̵�Ȩ�޵ȼ��������ͨ�������֤�Ի�����ȷ�ϲ������������̵߳�Ȩ�޵�
* ����
* 
* This source is subject to the Microsoft Public License.
* See http://www.microsoft.com/opensource/licenses.mspx#Ms-PL.
* All other rights reserved.
* 
* THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, 
* EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
* WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
\***************************************************************************/

#pragma region Includes and Manifest Dependencies
#include <stdio.h>
#include <windows.h>
#include <windowsx.h>
#include <shlobj.h>
#include "Resource.h"

// ��ʼ�Ӿ����
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#pragma endregion


#pragma region "Helper Functions for Admin Privileges and Elevation Status"

//
//   ������IsUserInAdminGroup()
// 
//   ��;����ʹ�ڻ�û��Ϊ��ǰ�û�����Ȩ�޵�ǰ���£��˺������ӵ�д˽��̵�����
//   �����Ƶ��û��Ƿ��Ǳ��ع���Ա��ĳ�Ա��
//
//   ����ֵ�����ӵ�����������Ƶ��û��ǹ���Ա���Ա�򷵻�TRUE����֮�򷵻�FALSE��
//
//   �쳣������˺����������׳�һ������Win32��ش�������C++ DWORD�쳣��
//
//
//   ����ʾ����
//     try 
//     {
//         if (IsUserInAdminGroup())
//             wprintf (L"�û��ǹ���Ա���Ա\n");
//         else
//             wprintf (L"�û����ǹ���Ա���Ա\n");
//     }
//     catch (DWORD dwError)
//     {
//         wprintf(L"IsUserInAdminGroup ����ʧ�� w/err %lu\n", dwError);
//     }
//
BOOL IsUserInAdminGroup()
{
    BOOL fInAdminGroup = FALSE;
    DWORD dwError = ERROR_SUCCESS;
    HANDLE hToken = NULL;
    HANDLE hTokenToCheck = NULL;
    DWORD cbSize = 0;
    OSVERSIONINFO osver = { sizeof(osver) };

    // �򿪴˽��̵����������ƣ�ʹ��TOKEN_QUERY | TOKEN_DUPLICATE��
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY | TOKEN_DUPLICATE, 
        &hToken))
    {
        dwError = GetLastError();
        goto Cleanup;
    }

    // ����Ƿ��ϵͳ��Windows Vista������汾�����汾�� >= 6��������������
    // Windows Vista��ʼ֧���������ƣ�LINKED TOKEN������֮ǰ�İ汾��֧�֡�
    // �����汾 < 6�� 
    if (!GetVersionEx(&osver))
    {
        dwError = GetLastError();
        goto Cleanup;
    }

    if (osver.dwMajorVersion >= 6)
    {
        // ������Windows Vista ������汾�����汾 >= 6��.
        // ����������ͣ����ޣ�limited��,����������elevated, 
        // ����Ĭ�ϣ�default��

        TOKEN_ELEVATION_TYPE elevType;
        if (!GetTokenInformation(hToken, TokenElevationType, &elevType, 
            sizeof(elevType), &cbSize))
        {
            dwError = GetLastError();
            goto Cleanup;
        }

        // ���Ϊ���޵ģ���ȡ������������������Ա���ʹ�á�
        if (TokenElevationTypeLimited == elevType)
        {
            if (!GetTokenInformation(hToken, TokenLinkedToken, &hTokenToCheck, 
                sizeof(hTokenToCheck), &cbSize))
            {
                dwError = GetLastError();
                goto Cleanup;
            }
        }
    }
    
    // CheckTokenMembershipҪ��һ��αװ���ơ�������ǽ��õ�һ���������ƣ���
    // ������һ��αװ���ơ��������û�еõ�һ������ʽ���ƣ�������Ҫ���Ƶ�ǰ
    // �����Ա�õ�һ��αװ���ơ�
    if (!hTokenToCheck)
    {
        if (!DuplicateToken(hToken, SecurityIdentification, &hTokenToCheck))
        {
            dwError = GetLastError();
            goto Cleanup;
        }
    }

    // ��������Ա����ص�SID
    BYTE adminSID[SECURITY_MAX_SID_SIZE];
    cbSize = sizeof(adminSID);
    if (!CreateWellKnownSid(WinBuiltinAdministratorsSid, NULL, &adminSID,  
        &cbSize))
    {
        dwError = GetLastError();
        goto Cleanup;
    }

    // ����Ƿ񱻼������ư�������Ա��SID
    // http://msdn.microsoft.com/en-us/library/aa379596(VS.85).aspx:
    // ����CheckTokenMembership��ȷ���Ƿ�һ��SID��һ�����������ã�������֮��
    // �Ƿ���ӵ��SE_GROUP_ENABLED����
    if (!CheckTokenMembership(hTokenToCheck, &adminSID, &fInAdminGroup)) 
    {
        dwError = GetLastError();
        goto Cleanup;
    }

Cleanup:
    // �������������ѷ�����ڴ���Դ
    if (hToken)
    {
        CloseHandle(hToken);
        hToken = NULL;
    }
    if (hTokenToCheck)
    {
        CloseHandle(hTokenToCheck);
        hTokenToCheck = NULL;
    }

    // һ�����κ��쳣�������׳�����
    if (ERROR_SUCCESS != dwError)
    {
        throw dwError;
    }

    return fInAdminGroup;
}


// 
//   ������IsRunAsAdmin()
//
//   ��;���˺�����⵱ǰ�����Ƿ��Թ���Ա������С�������֮���˽���Ҫ���û���
//   ӵ�����������Ƶ��û��ǹ���Ա���Ա�����Ѿ�ִ����Ȩ��������
//
//   ����ֵ�����ӵ�����������Ƶ��û��ǹ���Ա���Ա���Ѿ�ִ����Ȩ�������򷵻�
//   TRUE����֮�򷵻�FALSE��
//
//   �쳣������˺����������׳�һ������Win32��ش�������C++ DWORD�쳣��
//
//   ����ʾ����
//     try 
//     {
//         if (IsRunAsAdmin())
//             wprintf (L"�����Թ���Ա�������\n");
//         else
//             wprintf (L"����û���Թ���Ա�������\n");
//     }
//     catch (DWORD dwError)
//     {
//         wprintf(L"IsRunAsAdmin ʧ�� w/err %lu\n", dwError);
//     }
//
BOOL IsRunAsAdmin()
{
    BOOL fIsRunAsAdmin = FALSE;
    DWORD dwError = ERROR_SUCCESS;
    PSID pAdministratorsGroup = NULL;

    // �ڴ���估��ʼ��һ������Ա���SID
    SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
    if (!AllocateAndInitializeSid(
        &NtAuthority, 
        2, 
        SECURITY_BUILTIN_DOMAIN_RID, 
        DOMAIN_ALIAS_RID_ADMINS, 
        0, 0, 0, 0, 0, 0, 
        &pAdministratorsGroup))
    {
        dwError = GetLastError();
        goto Cleanup;
    }

    // ����Ƿ����Ա���SID���̵߳���������������Ч��
    if (!CheckTokenMembership(NULL, pAdministratorsGroup, &fIsRunAsAdmin))
    {
        dwError = GetLastError();
        goto Cleanup;
    }

Cleanup:
     // �������������ѷ�����ڴ���Դ
    if (pAdministratorsGroup)
    {
        FreeSid(pAdministratorsGroup);
        pAdministratorsGroup = NULL;
    }

    // һ�����κ��쳣�������׳�����
    if (ERROR_SUCCESS != dwError)
    {
        throw dwError;
    }

    return fIsRunAsAdmin;
}


//
//   ������IsProcessElevated
//
//   ��;���˺�����ȡ��ǰ���̵�Ȩ��������Ϣ�����ɴ˽����Ƿ������Ȩ��������
//   ����������Ȩ������ֻ����Windows Vista�������汾��Windows����Ч��������
//   Windows Vista֮ǰ�İ汾��ִ��IsProcessElevated�� �����׳�һ��C++�쳣��
//   �˺������������ڼ���Ƿ�˽����Թ���Ա������С�
//
//   ����ֵ������˽��̵�Ȩ���ѱ�����������TRUE����֮�򷵻�FALSE��
//
//
//   �쳣������˺����������׳�һ������Win32��ش�������C++ DWORD�쳣��
//   ������Windows Vista֮ǰ��Windows�е���IsProcessElevated�����׳��Ĵ���
//   ����ΪERROR_INVALID_PARAMETER��
//
//   ע�⣺TOKEN_INFORMATION_CLASS�ṩ��TokenElevationType�Ա�Ե�ǰ���̵�����
//   ���ͣ�TokenElevationTypeDefault / TokenElevationTypeLimited /
//   TokenElevationTypeFull�����м�⡣ ����TokenElevation�Ĳ�֮ͬ�����ڣ���UAC
//   �ر�ʱ����ʹ��ǰ�����Ѿ�������(�����Լ��� == ��)��Ȩ�������������Ƿ���
//   TokenElevationTypeDefault��������֮���Դ���ȷ�ϵ�ǰ�̵߳����������ǲ���ȫ�ġ�
//   ��Եģ�����Ӧ��ʹ��TokenElevation��
//
//   ����ʾ����
//     try 
//     {
//         if (IsProcessElevated())
//             wprintf (L"�����Ѿ�����\n");
//         else
//             wprintf (L"������δ����\n");
//     }
//     catch (DWORD dwError)
//     {
//         wprintf(L"IsProcessElevated ʧ�� w/err %lu\n", dwError);
//     }
//
BOOL IsProcessElevated()
{
    BOOL fIsElevated = FALSE;
    DWORD dwError = ERROR_SUCCESS;
    HANDLE hToken = NULL;

    // ʹ��TOKEN_QUERY�򿪽�������������
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
    {
        dwError = GetLastError();
        goto Cleanup;
    }

    // ��ȡ����������Ϣ
    TOKEN_ELEVATION elevation;
    DWORD dwSize;
    if (!GetTokenInformation(hToken, TokenElevation, &elevation, 
        sizeof(elevation), &dwSize))
    {
        // ������������Windows Vista֮ǰ��ϵͳ�У�GetTokenInformation����
        // FALSE�ʹ�����ERROR_INVALID_PARAMETER������������Щ����ϵͳ��֧
        // ��TokenElevation��
        dwError = GetLastError();
        goto Cleanup;
    }

    fIsElevated = elevation.TokenIsElevated;

Cleanup:
    // �������������ѷ�����ڴ���Դ
    if (hToken)
    {
        CloseHandle(hToken);
        hToken = NULL;
    }

    // һ�����κ��쳣�������׳�����
    if (ERROR_SUCCESS != dwError)
    {
        throw dwError;
    }

    return fIsElevated;
}


//
//   ������ GetProcessIntegrityLevel()
//
//   ��;���˺�����ȡ��ǰ�̵߳������Լ��������Լ���ֻ����Windows Vista����
//   ���汾��Windows����Ч��������Windows Vista֮ǰ�İ汾��ִ��GetProcessIntegrityLevel�� 
//   �����׳�һ��C++�쳣��
//
//   ����ֵ�����ص�ǰ���̵������Լ���������������ĳһ��ֵ��
//
//     SECURITY_MANDATORY_UNTRUSTED_RID (SID: S-1-16-0x0)
//     ��ʾ�������εļ�����������һ���������Ա�𶯵Ľ��̡���ʱ�����
//     д���������ֹ��
//
//     SECURITY_MANDATORY_LOW_RID (SID: S-1-16-0x1000)
//     ��ʾ�������Լ����������ڱ���ģʽ�µ�IE���������ʱ�����ϵͳ�ж�
//     �󣨰����ļ���ע����ֵ����д���������ֹ��
//
//     SECURITY_MANDATORY_MEDIUM_RID (SID: S-1-16-0x2000)
//     ��ʾ�������Լ�������������UAC����ʱ������ͨӦ�ó���
//
//
//     SECURITY_MANDATORY_HIGH_RID (SID: S-1-16-0x3000)
//     ��ʾ�������Լ������������û�ͨ��UAC����Ȩ������һ������ԱӦ�ó���
//     ����UAC�ر�ʱ������Ա�û�����һ����ͨ����
//
//
//     SECURITY_MANDATORY_SYSTEM_RID (SID: S-1-16-0x4000)
//     ��ʾϵͳ�����Լ����������ڷ����������ϵͳ�����Ӧ�ó��򣨱���
//     Wininit, Winlogon, Smss���ȵȣ�
//
//
//   �쳣������˺����������׳�һ������Win32��ش�������C++ DWORD�쳣��
//   ������Windows Vista֮ǰ��Windows�е���GetProcessIntegrityLevel������
//   ���Ĵ������ΪERROR_INVALID_PARAMETER��
// 
//   ����ʾ����
//     try 
//     {
//         DWORD dwIntegrityLevel = GetProcessIntegrityLevel();
//     }
//     catch (DWORD dwError)
//     {
//         wprintf(L"GetProcessIntegrityLevel ʧ�� w/err %lu\n", dwError);
//     }
//
DWORD GetProcessIntegrityLevel()
{
    DWORD dwIntegrityLevel = 0;
    DWORD dwError = ERROR_SUCCESS;
    HANDLE hToken = NULL;
    DWORD cbTokenIL = 0;
    PTOKEN_MANDATORY_LABEL pTokenIL = NULL;

    // ʹ��TOKEN_QUERY���̵߳����������ơ�
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
    {
        dwError = GetLastError();
        goto Cleanup;
    }

    // ��ѯ���������Լ�����Ϣ�Ĵ�С��ע�⣺����Ԥ�ڵõ�һ��FALSE���������
    // ERROR_INSUFFICIENT_BUFFER�� ��������������GetTokenInformation����һ��
    // �ջ��塣ͬʱ����cbTokenIL�����ǻ��֪�����Լ�����Ϣ�Ĵ�С��
    if (!GetTokenInformation(hToken, TokenIntegrityLevel, NULL, 0, &cbTokenIL))
    {
        if (ERROR_INSUFFICIENT_BUFFER != GetLastError())
        {
            // ������������Windows Vista֮ǰ��ϵͳ�У�GetTokenInformation����
            // FALSE�ʹ�����ERROR_INVALID_PARAMETER������������Щ����ϵͳ��֧
            // ��TokenElevation��
            dwError = GetLastError();
            goto Cleanup;
        }
    }

    // ��������Ϊ�����Լ�����Ϣ����һ�����档
    pTokenIL = (TOKEN_MANDATORY_LABEL *)LocalAlloc(LPTR, cbTokenIL);
    if (pTokenIL == NULL)
    {
        dwError = GetLastError();
        goto Cleanup;
    }

    // ������������Լ�����Ϣ��
    if (!GetTokenInformation(hToken, TokenIntegrityLevel, pTokenIL, 
        cbTokenIL, &cbTokenIL))
    {
        dwError = GetLastError();
        goto Cleanup;
    }

    // �����Լ���SIDΪS-1-16-0xXXXX��ʽ�������磺S-1-16-0x1000��ʾΪ��������
    // �����SID�����������ҽ���һ���μ���Ȩ��Ϣ��
    dwIntegrityLevel = *GetSidSubAuthority(pTokenIL->Label.Sid, 0);

Cleanup:
    // �������������ѷ�����ڴ���Դ

    if (hToken)
    {
        CloseHandle(hToken);
        hToken = NULL;
    }
    if (pTokenIL)
    {
        LocalFree(pTokenIL);
        pTokenIL = NULL;
        cbTokenIL = 0;
    }

    // һ�����κ��쳣�������׳�����
    if (ERROR_SUCCESS != dwError)
    {
        throw dwError;
    }

    return dwIntegrityLevel;
}

#pragma endregion


//
//   ������ ReportError(LPWSTR, DWORD)
//
//   ��;����һ�������������һ���Ի�����ʾ������Ϣ
//
//   ������
//   * pszFunction - ����ʧ�ܵĺ��������ơ�
//   * dwError - Win32������롣����Ĭ��ֵΪ���̵�GetLastError�ķ���ֵ��
//
//   ע�⣺���û����ȷָ��dwError��ֵ�����Ǳ����ں���ʧ�ܺ����̵���ReportError
//   ����ȷ�����ڵ��õĽ��̵����������û�б�����API��д��
void ReportError(LPCWSTR pszFunction, DWORD dwError = GetLastError())
{
    wchar_t szMessage[200];
    if (-1 != swprintf_s(szMessage, ARRAYSIZE(szMessage), 
        L"%s failed w/err 0x%08lx", pszFunction, dwError))
    {
        MessageBox(NULL, szMessage, L"Error", MB_ICONERROR);
    }
}


// 
//   ������ OnInitDialog(HWND, HWND, LPARAM) 
//   
//   ��;������WM_INITDIALOG��Ϣ����⼰��ʾ���Թ���Ա�˻�������С�״̬��
//   Ȩ��������Ϣ�͵�ǰ���̵������Լ���

BOOL OnInitDialog(HWND hWnd, HWND hwndFocus, LPARAM lParam)
{
    // ��ȡ����ʾ��ʹ�ڻ�û��Ϊ��ǰ�û�����Ȩ�޵�ǰ���£�ӵ�д˽��̵�����
    // �����Ƶ��û��Ƿ��Ǳ��ع���Ա��ĳ�Ա��(IsUserInAdminGroup)��
    HWND hInAdminGroupLabel = GetDlgItem(hWnd, IDC_INADMINGROUP_STATIC);
    try
    {
        BOOL const fInAdminGroup = IsUserInAdminGroup();
        SetWindowText(hInAdminGroupLabel, fInAdminGroup ? L"��" : L"��");
    }
    catch (DWORD dwError)
    {
        SetWindowText(hInAdminGroupLabel, L"N/A");
        ReportError(L"IsUserInAdminGroup", dwError);
    }

     // ��ȡ����ʾ�Ƿ�˽����Թ���Ա������С���IsRunAsAdmin����
    HWND hIsRunAsAdminLabel = GetDlgItem(hWnd, IDC_ISRUNASADMIN_STATIC);
    try
    {
        BOOL const fIsRunAsAdmin = IsRunAsAdmin();
        SetWindowText(hIsRunAsAdminLabel, fIsRunAsAdmin ? L"��" : L"��");
    }
    catch (DWORD dwError)
    {
        SetWindowText(hIsRunAsAdminLabel, L"N/A");
        ReportError(L"IsRunAsAdmin", dwError);
    }
    

     // ��ȡ����ʾ����Ȩ��������Ϣ(IsProcessElevated)�������Լ���GetProcessIntegrityLevel��
    // ע�⣺��Щ��Ϣ��Windows Vista֮ǰ��Windows�в����ڡ�

    HWND hIsElevatedLabel = GetDlgItem(hWnd, IDC_ISELEVATED_STATIC);
    HWND hILLabel = GetDlgItem(hWnd, IDC_IL_STATIC);

    OSVERSIONINFO osver = { sizeof(osver) };
    if (GetVersionEx(&osver) && osver.dwMajorVersion >= 6)
    {
        // ������Windows Vista������汾�����汾�� >= 6����
        try
        {
            // ��ȡ����ʾ����Ȩ��������Ϣ
            BOOL const fIsElevated = IsProcessElevated();
            SetWindowText(hIsElevatedLabel, fIsElevated ? L"��" : L"��");

            // ���������δ�����������¡���������Ȩ�ޡ���ť����UI����ʾUAC����
            // ͼ�ꡣ��Button_SetElevationRequiredState����Commctrl.h�ж��壩��
            // ����ʾ�����ذ�ť�ϵĶ���ͼ�ꡣ��Ҳ����ͨ������SHGetStockIconInfo
            // ������SIID_SHIELD������ȡ��ͼ�ꡣ
            HWND hElevateBtn = GetDlgItem(hWnd, IDC_ELEVATE_BN);
            Button_SetElevationRequiredState(hElevateBtn, !fIsElevated);
        }
        catch (DWORD dwError)
        {
            SetWindowText(hIsElevatedLabel, L"N/A");
            ReportError(L"IsProcessElevated", dwError);
        }

        try
        {
            // ��ȡ����ʾ���̵������Լ���
            DWORD const dwIntegrityLevel = GetProcessIntegrityLevel();
            switch (dwIntegrityLevel)
            {
            case SECURITY_MANDATORY_UNTRUSTED_RID: SetWindowText(hILLabel, L"������"); break;
            case SECURITY_MANDATORY_LOW_RID: SetWindowText(hILLabel, L"��"); break;
            case SECURITY_MANDATORY_MEDIUM_RID: SetWindowText(hILLabel, L"��"); break;
            case SECURITY_MANDATORY_HIGH_RID: SetWindowText(hILLabel, L"��"); break;
            case SECURITY_MANDATORY_SYSTEM_RID: SetWindowText(hILLabel, L"ϵͳ"); break;
            default: SetWindowText(hILLabel, L"δ֪"); break;
            }
        }
        catch (DWORD dwError)
        {
            SetWindowText(hILLabel, L"N/A");
            ReportError(L"GetProcessIntegrityLevel", dwError);
        }
    }
    else
    {
        SetWindowText(hIsElevatedLabel, L"N/A");
        SetWindowText(hILLabel, L"N/A");
    }

    return TRUE;
}


//
//   ������OnCommand(HWND, int, HWND, UINT)
//
//   ��;������WM_COMMAND��Ϣ
//
void OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify)
{
    switch (id)
    {
    case IDC_ELEVATE_BN:
        {
            // ��鵱ǰ�����Ƿ��Թ���ԱȨ�����У����������������           
            BOOL fIsRunAsAdmin;
            try
            {
                fIsRunAsAdmin = IsRunAsAdmin();
            }
            catch (DWORD dwError)
            {
                ReportError(L"IsRunAsAdmin", dwError);
                break;
            }

             // ����˽��̲����Թ���Ա������У�����Ȩ�޵ȼ���
            if (!fIsRunAsAdmin)
            {
                wchar_t szPath[MAX_PATH];
                if (GetModuleFileName(NULL, szPath, ARRAYSIZE(szPath)))
                {
                    // �Թ���Ա�������������
                    SHELLEXECUTEINFO sei = { sizeof(sei) };
                    sei.lpVerb = L"runas";
                    sei.lpFile = szPath;
                    sei.hwnd = hWnd;
                    sei.nShow = SW_NORMAL;

                    if (!ShellExecuteEx(&sei))
                    {
                        DWORD dwError = GetLastError();
                        if (dwError == ERROR_CANCELLED)
                        {
                                // �û��ܾ�����
                                // ʲô������...
                        }
                    }
                    else
                    {
                        EndDialog(hWnd, TRUE);  // �˳�
                    }
                }
            }
            else
            {
                MessageBox(hWnd, L"�˽������Թ���Ա�������", L"UAC", MB_OK);
            }
        }
        break;

    case IDOK:
    case IDCANCEL:
        EndDialog(hWnd, 0);
        break;
    }
}


//
//   ������OnClose(HWND)
//
//   ��;������WM_CLOSE��Ϣ
//
void OnClose(HWND hWnd)
{
    EndDialog(hWnd, 0);
}


//
//  ������DialogProc(HWND, UINT, WPARAM, LPARAM)
//
//  ��;�� Ϊ���Ի�������Ϣ
//
INT_PTR CALLBACK DialogProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        // ����OnInitDialog����WM_INITDIALOG��Ϣ
        HANDLE_MSG (hWnd, WM_INITDIALOG, OnInitDialog);

        // ����OnCommand����WM_COMMAND��Ϣ
        HANDLE_MSG (hWnd, WM_COMMAND, OnCommand);

        // ����OnClose����WM_CLOSE��Ϣ
        HANDLE_MSG (hWnd, WM_CLOSE, OnClose);

    default:
        return FALSE;
    }
    return 0;
}


//
//  ������wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
//
//  ��;������ں���
//
int APIENTRY wWinMain(HINSTANCE hInstance,
                      HINSTANCE hPrevInstance,
                      LPWSTR    lpCmdLine,
                      int       nCmdShow)
{
    return DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAINDIALOG), NULL, DialogProc);
}