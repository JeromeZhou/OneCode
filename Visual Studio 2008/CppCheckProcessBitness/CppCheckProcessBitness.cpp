/****************************** ģ��ͷ ******************************\
* ģ����:  CppCheckProcessBitness.cpp
* ��Ŀ��:  CppCheckProcessBitness
* ��Ȩ (c) Microsoft Corporation.
* 
*���ʵ��������ʾ�����ȷ��һ�������Ľ�����64λ�Ļ���32λ��  
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
#include <stdio.h>
#include <windows.h>
#pragma endregion

//
//   FUNCTION: DoesWin32MethodExist(PCWSTR, PCSTR)
//
//   PURPOSE: �ú���ȷ���Ƿ񷽷�������һ���ض���ģ�鵼����. .
//
//   PARAMETERS:
//   * pszModuleName - ģ�������.
//   * pszMethodName - ����������.
//
//   RETURN VALUE: �����methodNameָ���ĵķ���������ģ����ָ����ģ�鵼�����ú�������true.
//
BOOL DoesWin32MethodExist(PCWSTR pszModuleName, PCSTR pszMethodName)
{
    HMODULE hModule = GetModuleHandle(pszModuleName);
    if (hModule == NULL)
    {
        return FALSE;
    }
    return (GetProcAddress(hModule, pszMethodName) != NULL);
}


//
//   FUNCTION: Is64BitOperatingSystem()
//
//   PURPOSE: �ú����жϵ�ǰ����ϵͳ�Ƿ���64λ����ϵͳ.
//
//   RETURN VALUE: �������ϵͳ��64λ���ú�������true�����򷵻�false.
//
BOOL Is64BitOperatingSystem()
{
#if defined(_WIN64)
    return TRUE;   // 64λ����ֻ��������windows x64��
#elif defined(_WIN32)
    // 32λ����������32λ��64λwindows��.
    BOOL fIs64bitOS = FALSE;
    return (DoesWin32MethodExist(L"kernel32.dll", "IsWow64Process") && 
        (IsWow64Process(GetCurrentProcess(), &fIs64bitOS) && fIs64bitOS));
#else
    return FALSE;  // 64λWindows��֧��win16.
#endif
}


//
//   FUNCTION: Is64bitProcess(HANDLE hProcess)
//
//   PARAMETERS:
//   * hProcess - �ý��̵ľ��.
//   
//   PURPOSE: �ú����ж�ָ���Ľ����Ƿ���64λ��.
//
//   RETURN VALUE: ���ָ���Ľ�����64λ�ģ���������TRUE��
        ///  ���򷵻�FALSE.
//
BOOL Is64BitProcess(HANDLE hProcess)
{
	BOOL fIs64bitProcess = FALSE;

    if (Is64BitOperatingSystem())
	{
		// ��64λ����ϵͳ�����һ��������û����WOW64ģʽ�����У��������һ����һ��64λ��.
		fIs64bitProcess = !(IsWow64Process(hProcess, &fIs64bitProcess) && 
			fIs64bitProcess);
	}

	return fIs64bitProcess;
}


int wmain(int argc, wchar_t* argv[])
{
	if (argc > 1)
	{
		// ���ָ��һ�����̵�ID��Ϊ����.
		DWORD dwProcessId = _wtoi(argv[1]); 
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, 
			dwProcessId);

		if(hProcess != NULL)
		{
			if(Is64BitProcess(hProcess))
			{
				printf("�ý�����һ��64λ����.\n");
			}
			else
			{
				printf("�ý�����һ��32λ����.\n");
			}

			CloseHandle(hProcess);
		}
		else
		{
			DWORD errorCode = GetLastError();
			printf("�������ڵõ����� %d, ���������:0x%08lx\n", 
				dwProcessId, errorCode);
		}
	}
	else
	{
		// ���û��ָ������ID����ʹ�õ�ǰ���̵�ID.
		HANDLE hProcess = GetCurrentProcess();
		if(Is64BitProcess(hProcess))
		{
			printf("��ǰ������64λ��.\n");
		}
		else
		{
			printf("��ǰ������32λ��.\n");
		}
	}

	return 0;
}

