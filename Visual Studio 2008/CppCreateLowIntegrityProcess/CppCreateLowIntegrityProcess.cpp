/****************************** ģ��ͷ ******************************\
* ģ����:  CppCreateLowIntegrityProcess.cpp
* ��Ŀ��:      CppCreateLowIntegrityProcess
* ��Ȩ (c) Microsoft Corporation.
* 
* �������ʾ����ʾ���������һ���������Խ��̡��������������С��Ե������ȼ�ִ
* �б����򡱰�ť����Ӧ�ó���ʹ�õ��������ٴ�����һ��������ʵ�����������Խ���ֻ
* ���ڵ�������������д�����ݣ�����%USERPROFILE%\AppData\LocalLow�ļ��л���ע��
* ���е�HKEY_CURRENT_USER\Software\AppDataLow��ֵ����ʹ��ǰ�û���SID�����ɷ���
* �����б�discretionary access control list����ӵ��д��Ȩ�ޣ��������Ҫ����һ
* �������ԸߵĶ�����Ҳ�����յ�һ���޷����ʵĴ���
*
* Ĭ������£��ӽ��̼̳��丸���̵������Եȼ���Ҫ����һ���������Խ��̣������ʹ��
* CreateProcessAsUser�͵������Է�����������һ���µ��ӽ��̡���ϸ��Ϣ��ο�ʾ��
* CreateLowIntegrityProcess�е���غ�����
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
#include <sddl.h>
#include <shlobj.h>
#include "Resource.h"

// Enable Visual Style
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


#pragma region Helper Functions related to Process Integrity Level

// 
//   ������CreateLowIntegrityProcess(PCWSTR)
//
//   ��;���˺����Ե������Լ�������һ��Ӧ�ó���
//
//   ������
//   * pszCommandLine - ��Ҫ��ִ�е������С����ַ����Ϊ32K���ַ������ַ���
//     ������һ��ָ��ֻ���ڴ��ָ�루���糣�����߳����ַ�������������������
//     һ�������ַ������˺����ᵼ��һ�����ʳ�ͻ��access violation���Ĵ���
//
//   ����ֵ������˺����ɹ����򷵻�ֵ��TRUE�����ʧ�ܣ���Ϊ0������GetLastError
//   ��ȡ����Ĵ�����Ϣ��
//
//   ע�ͣ�
//   ����һ���������Խ���
//   1) ���Ƶ�ǰ���̵ľ������ӵ���������Լ���
//   2��ʹ��SetTokenInformation���÷��ʽ��̵������Լ���Ϊ�͡�
//   3��ʹ��CreateProcessAsUser���������Լ���ķ������ƴ���һ���µĽ��̡�
//

BOOL CreateLowIntegrityProcess(PWSTR pszCommandLine)
{
    DWORD dwError = ERROR_SUCCESS;
    HANDLE hToken = NULL;
    HANDLE hNewToken = NULL;
    wchar_t szIntegritySid[] = L"S-1-16-4096";  // ��������SID�ַ���
    PSID pIntegritySid = NULL;
    TOKEN_MANDATORY_LABEL tml = { 0 };
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi = { 0 };

    // �򿪽��̵�����������
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_DUPLICATE | TOKEN_QUERY |
        TOKEN_ADJUST_DEFAULT | TOKEN_ASSIGN_PRIMARY, &hToken))
    {
        dwError = GetLastError();
        goto Cleanup;
    }

    // ���Ƶ�ǰ���̵�������
    if (!DuplicateTokenEx(hToken, 0, NULL, SecurityImpersonation, 
        TokenPrimary, &hNewToken))
    {
        dwError = GetLastError();
        goto Cleanup;
    }

    // ������������SID
    if (!ConvertStringSidToSid(szIntegritySid, &pIntegritySid))
    {
        dwError = GetLastError();
        goto Cleanup;
    }

    tml.Label.Attributes = SE_GROUP_INTEGRITY;
    tml.Label.Sid = pIntegritySid;

    // ���÷������Ƶ������Լ���Ϊ��
    if (!SetTokenInformation(hNewToken, TokenIntegrityLevel, &tml, 
        (sizeof(tml) + GetLengthSid(pIntegritySid))))
    {
        dwError = GetLastError();
        goto Cleanup;
    }

    // �Ե������Լ��𴴽�һ���½��̡�
    if (!CreateProcessAsUser(hNewToken, NULL, pszCommandLine, NULL, NULL, 
        FALSE, 0, NULL, NULL, &si, &pi))
    {
        dwError = GetLastError();
        goto Cleanup;
    }

Cleanup:
    // ���������ѷ������Դ
    if (hToken)
    {
        CloseHandle(hToken);
        hToken = NULL;
    }
    if (hNewToken)
    {
        CloseHandle(hNewToken);
        hNewToken = NULL;
    }
    if (pIntegritySid)
    {
        LocalFree(pIntegritySid);
        pIntegritySid = NULL;
    }
    if (pi.hProcess)
    {
        CloseHandle(pi.hProcess);
        pi.hProcess = NULL;
    }
    if (pi.hThread)
    {
        CloseHandle(pi.hThread);
        pi.hThread = NULL;
    }

    if (ERROR_SUCCESS != dwError)
    {
        // ʧ��ʱȷ�����ܹ���ȡ�˴������
        SetLastError(dwError);
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}


//
//   ������ GetProcessIntegrityLevel(PDWORD)
//
//   ��;���˺�����ȡ��ǰ�̵߳������Լ��������Լ���ֻ����Windows Vista����
//   ���汾��Windows����Ч��������Windows Vista֮ǰ�İ汾��ִ��GetProcessIntegrityLevel�� 
//   ������FALSE��
//
//   ������
//   * pdwIntegrityLevel - �����ǰ���̵������Լ���������������ĳһ��ֵ��
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
//   ����ֵ: ����������óɹ���������TRUE�����ʧ����ΪFALSE������GetLastError
//   ��ȡ����Ĵ�����Ϣ���������GetProcessIntegrityLevel��Windows Vista֮ǰϵͳ
//   ����pdwIntegrityLevelΪ��ʱ��ERROR_INVALID_PARAMETER�����Ĵ�����Ϣ
//
//   ����ʾ����
//     DWORD dwIntegrityLevel;
//     if (!GetProcessIntegrityLevel(&dwIntegrityLevel))
//     {
//         wprintf(L"GetProcessIntegrityLevel failed w/err %lu\n", 
//             GetLastError());
//     }
//
BOOL GetProcessIntegrityLevel(PDWORD pdwIntegrityLevel)
{
    DWORD dwError = ERROR_SUCCESS;
    HANDLE hToken = NULL;
    DWORD cbTokenIL = 0;
    PTOKEN_MANDATORY_LABEL pTokenIL = NULL;

    if (pdwIntegrityLevel == NULL)
    {
        dwError = ERROR_INVALID_PARAMETER;
        goto Cleanup;
    }

    // ��TOKEN_QUERY�������̵߳����������ơ�
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
    *pdwIntegrityLevel = *GetSidSubAuthority(pTokenIL->Label.Sid, 0);

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

    if (ERROR_SUCCESS != dwError)
    {
        // ʧ��ʱȷ�����ܹ���ȡ�˴������
        SetLastError(dwError);
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

#pragma endregion


//
//   ������ReportError(LPWSTR, DWORD)
//
//   ��;��Ϊ����ʧ�ܵĺ�����ʾһ��������Ϣ�Ի���
//
//   ������
//   * pszFunction - ����ʧ�ܵĺ���������
//   * dwError - Win32������롣 ����Ĭ��ֵΪ�߳����Ĵ�����롣
//
//   ע�⣺�����û����ȷָ��dwError��ֵ��������ں���ʧ�ܺ����̵���ReportError
//   ����ȷ�����ڵ��õĽ��̵����������û�б�����API��д��
//
void ReportError(LPCWSTR pszFunction, DWORD dwError = GetLastError())
{
    wchar_t szMessage[200];
    if (-1 != swprintf_s(szMessage, ARRAYSIZE(szMessage), 
        L"%s ʧ�� w/err 0x%08lx", pszFunction, dwError))
    {
        MessageBox(NULL, szMessage, L"����", MB_ICONERROR);
    }
}


// 
//   ������OnInitDialog(HWND, HWND, LPARAM)
//
//   ��;������ WM_INITDIALOG��Ϣ.
//
BOOL OnInitDialog(HWND hWnd, HWND hwndFocus, LPARAM lParam)
{
    // ��ȡ����ʾ���̵������Լ���
    HWND hILLabel = GetDlgItem(hWnd, IDC_IL_STATIC);
    DWORD dwIntegrityLevel;
    if (GetProcessIntegrityLevel(&dwIntegrityLevel))
    {
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
    else
    {
        ReportError(L"GetProcessIntegrityLevel");
        SetWindowText(hILLabel, L"N/A");
    }

    return TRUE;
}


// 
//   ������CreateTestFileInKnownFolder(REFKNOWNFOLDERID)
//
//   ��;���˺���������ָ����Windows��֪�ļ����д���һ�������ļ�(testfile.txt)��
//   ������ʾ����Ϣ�Ի����в��Խ����
//
//   ������
//   * rfid - KNOWNFOLDERID�ṹ������ָ���ļ���
//
void CreateTestFileInKnownFolder(REFKNOWNFOLDERID rfid) 
{
    HRESULT hr = S_OK;
    PWSTR pszFolder = NULL;
    wchar_t szPath[MAX_PATH];
    HANDLE hFile = INVALID_HANDLE_VALUE;
    wchar_t szMessage[1024];

    // ��ȡ��֪�ļ���
    hr = SHGetKnownFolderPath(rfid, 0, NULL, &pszFolder);
    if (FAILED(hr))
    {
        goto Cleanup;
    }

    // ���ļ�����ӵ��ļ���·�����Ա����������ļ�·����
    wcscpy_s(szPath, ARRAYSIZE(szPath), pszFolder);
    wcscat_s(szPath, ARRAYSIZE(szPath), L"\\testfile.txt");

    // ���������ļ�
    hFile = CreateFile(szPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 
        FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        hr = HRESULT_FROM_WIN32(GetLastError());
    }
    
    // ��ʾ���Խ��
    if (SUCCEEDED(hr))
    {
        swprintf_s(szMessage, ARRAYSIZE(szMessage), 
            L"�ɹ�д������ļ�: %s", szPath);
        MessageBox(NULL, szMessage, L"���Խ��", MB_ICONINFORMATION);
    }
    else if (hr == E_ACCESSDENIED)
    {
        swprintf_s(szMessage, ARRAYSIZE(szMessage), 
            L"�ܾ����� '%s'.", szPath);
        MessageBox(NULL, szMessage, L"���Խ��", MB_ICONERROR);
    }
    else
    {
        swprintf_s(szMessage, ARRAYSIZE(szMessage), 
            L"д������ļ� '%s'ʧ�� w/err 0x%08lx", 
            szPath, hr);
        MessageBox(NULL, szMessage, L"���Խ��", MB_ICONERROR);
    }

Cleanup:
    // �������������ѷ�����ڴ���Դ
    if (pszFolder)
    {
        CoTaskMemFree(pszFolder);
        pszFolder = NULL;
    }
    if (hFile != INVALID_HANDLE_VALUE)
    {
        CloseHandle(hFile);
        hFile = INVALID_HANDLE_VALUE;
    }
}


//
//   ����:OnCommand(HWND, int, HWND, UINT)
//
//   ��;������WM_COMMAND��Ϣ
//
void OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify)
{
    switch (id)
    {
    case IDC_CREATELOWPROCESS_BN:
        {
            wchar_t szPath[MAX_PATH];
            if (GetModuleFileName(NULL, szPath, ARRAYSIZE(szPath)))
            {
                // �Ե������Լ�������Ӧ�ó���
                if (!CreateLowIntegrityProcess(szPath))
                {
                    ReportError(L"CreateLowIntegrityProcess");
                }
            }
        }
        break;

    case IDC_WRITELOCALAPPDATA_BN:
        // ������LocalAppData�д��������ļ�
        CreateTestFileInKnownFolder(FOLDERID_LocalAppData);
        break;

    case IDC_WRITELOCALAPPDATALOW_BN:
        // ������LocalAppDataLow�д��������ļ�
        CreateTestFileInKnownFolder(FOLDERID_LocalAppDataLow);
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
//   ��;:����WM_CLOSE��Ϣ
//
void OnClose(HWND hWnd)
{
    EndDialog(hWnd, 0);
}


//
//  ������DialogProc(HWND, UINT, WPARAM, LPARAM)
//
//  ��;�� �������Ի������Ϣѭ��
//
INT_PTR CALLBACK DialogProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        // ��OnInitDialog�д���WM_INITDIALOG��Ϣ
        HANDLE_MSG (hWnd, WM_INITDIALOG, OnInitDialog);

        // ��OnCommand�д���WM_COMMAND��Ϣ
        HANDLE_MSG (hWnd, WM_COMMAND, OnCommand);

        // ��OnClose�д���WM_CLOSE��Ϣ
        HANDLE_MSG (hWnd, WM_CLOSE, OnClose);

    default:
        return FALSE;
    }
    return 0;
}


//
//  ������wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
//
//  ��;��Ӧ�ó�����ڵ�
//
int APIENTRY wWinMain(HINSTANCE hInstance,
                      HINSTANCE hPrevInstance,
                      LPWSTR    lpCmdLine,
                      int       nCmdShow)
{
    return DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAINDIALOG), NULL, DialogProc);
}