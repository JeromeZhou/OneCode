/******************************ģ��ͷ ******************************\
ģ����:  FileDragDropExt.h
��Ŀ:      CppShellExtDragDropHandler
��Ȩ(c) Microsoft Corporation.

�������ʾ����ʾ����ΪC++����һ��Shell����ϷŴ������. 

���û��Ҽ����һ��Shell�����϶�һ������ʱ�����û���ͼ�Ƴ��������ʱ�����Ĳ˵�����
ʾ.һ����괦�������һ�������Ĳ˵�������򣬸������Ĳ˵����������������Ĳ˵��������

�����괦��������Ӷ��������Ĳ˵��������˵�����ⴴ��Ӳ���ӡ��������Ҽ�����һ���ļ���
�϶��ļ���Ŀ¼��������������ʱ�������Ĳ˵�����ʾ�����ﴴ��Ӳ���ӡ��˵��ͨ������˵��
����������ļ��Ϸŵ�λ���д���һ��Ӳ����������ӵ������ǡ�Ӳ����<��Դ�ļ���>����


This source is subject to the Microsoft Public License.
See http://www.microsoft.com/opensource/licenses.mspx#Ms-PL.
All other rights reserved.

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, 
EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
\***************************************************************************/

#pragma once

#include <windows.h>
#include <shlobj.h>     // For IShellExtInit and IContextMenu


class FileDragDropExt : IShellExtInit, IContextMenu
{
public:
    // IUnknown
    IFACEMETHODIMP QueryInterface(REFIID riid, void **ppv);
    IFACEMETHODIMP_(ULONG) AddRef();
    IFACEMETHODIMP_(ULONG) Release();

    // IShellExtInit
    IFACEMETHODIMP Initialize(LPCITEMIDLIST pidlFolder, LPDATAOBJECT pDataObj, HKEY hKeyProgID);

    // IContextMenu
    IFACEMETHODIMP QueryContextMenu(HMENU hMenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags);
    IFACEMETHODIMP InvokeCommand(LPCMINVOKECOMMANDINFO pici);
    IFACEMETHODIMP GetCommandString(UINT_PTR idCommand, UINT uFlags, UINT *pwReserved, LPSTR pszName, UINT cchMax);
	
    FileDragDropExt();

protected:
    ~FileDragDropExt();

private:
    // �������������.
    long m_cRef;

    // �϶����ļ�.
    wchar_t m_szSrcFile[MAX_PATH];

    // ���Ŀ¼���ļ��ŵĵط�.
    wchar_t m_szTargetDir[MAX_PATH];

    // ��������Ǵ������ﴴ��Ӳ���ӡ�����.
    void OnCreateHardLink(HWND hWnd);

    PWSTR m_pszMenuText;
};