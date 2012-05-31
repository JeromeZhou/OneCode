/******************************** ģ��ͷ ********************************\
ģ����:  FileDragDropExt.cpp
��Ŀ:    CppShellExtDragDropHandler
��Ȩ (c) Microsoft Corporation.

���ʾ��������ʾ����C++��������ϷŴ������Shell.

���û��һ�Shell�������϶�һ���������û����Է����������ʱ��һ�������Ĳ˵�����ʾ������
����ϷŴ��������һ�������������Ĳ˵��������Ĳ˵���

�������ϷŴ������,����Ӳ˵�����ⴴ��Ӳ���ӡ��������Ĳ˵��С����һ�һ���ļ����Ұ����ϵ�
һ��Ŀ¼��Ӳ�̻������ϣ������Ĳ˵��н���ʾ��������ⴴ��Ӳ���ӡ��˵��ͨ���������˵��
��������ڷ���Ŀ¼�У�����һ���϶��ļ���Ӳ���ӡ�������ӵ������ǡ�����<ԭ�ļ���>��Ӳ���ӡ���


This source is subject to the Microsoft Public License.
See http://www.microsoft.com/opensource/licenses.mspx#Ms-PL.
All other rights reserved.

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, 
EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
\*******************************************************************************/

#include "FileDragDropExt.h"
#include <strsafe.h>
#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")


#define IDM_RUNFROMHERE            0  // ��������ٱ�ʶ����

FileDragDropExt::FileDragDropExt() : m_cRef(1), 
    m_pszMenuText(L"�����ﴴ��Ӳ����")
{
}


FileDragDropExt::~FileDragDropExt()
{
}


void FileDragDropExt::OnCreateHardLink(HWND hWnd)
{      
    // �����õ��϶��ļ��ƶ��Ĳ���·�������磺 "D:\test\file.txt" is reduced to "file.txt".
    wchar_t szExistingFileName[MAX_PATH];
    StringCchCopy(szExistingFileName, ARRAYSIZE(szExistingFileName), m_szSrcFile);
    PathStripPath(szExistingFileName);

    // �����µ�·������
    wchar_t szNewLinkName[MAX_PATH];
    HRESULT hr = StringCchPrintf(szNewLinkName, ARRAYSIZE(szNewLinkName), 
        L"%s\\Ӳ���ӵ� %s", m_szTargetDir, szExistingFileName);
    if (FAILED(hr))
    {
        MessageBox(hWnd, L"���������ƹ���������ʧ��.", 
            L"CppShellExtDragDropHandler", MB_ICONERROR);
        return;
    }

    // �������ļ����Ƿ���ڡ�
    if (PathFileExists(szNewLinkName))
    {
        MessageBox(hWnd, L"�ڴ�λ��,�Ѵ��ھ�����ͬ���Ƶ��ļ�.", 
            L"CppShellExtDragDropHandler", MB_ICONERROR);
        return;
    }

    // �ڴ��ڵ��ļ����µ��ļ�֮�佨��Ӳ���ӡ�
    if (!CreateHardLink(szNewLinkName, m_szSrcFile, NULL))
    {
        wchar_t szMessage[260];
        DWORD dwError = GetLastError();
        
        if (dwError == ERROR_NOT_SAME_DEVICE)
        {
            StringCchCopy(szMessage, ARRAYSIZE(szMessage), 
                L"�޷�������Ӳ���ӣ���Ϊ����Ӳ���ӵ����ļ�," \
                L"��������ͬһ����.");
        }
        else
        {
            StringCchPrintf(szMessage, ARRAYSIZE(szMessage), 
                L"�޷�������Ӳ���� w/err 0x%08lx", dwError);
        }
        
        MessageBox(hWnd, szMessage, L"CppShellExtDragDropHandler", MB_ICONERROR);
    }
}


#pragma region IUnknown

// ��ѯ�����֧�ֵĽ�ڡ�
IFACEMETHODIMP FileDragDropExt::QueryInterface(REFIID riid, void **ppv)
{
    HRESULT hr = S_OK;

    if (IsEqualIID(IID_IUnknown, riid) || 
        IsEqualIID(IID_IContextMenu, riid))
    {
        *ppv = static_cast<IContextMenu *>(this);
    }
    else if (IsEqualIID(IID_IShellExtInit, riid))
    {
        *ppv = static_cast<IShellExtInit *>(this);
    }
    else
    {
        hr = E_NOINTERFACE;
        *ppv = NULL;
    }

    if (*ppv)
    {
        AddRef();
    }

    return hr;
}

// ���Ӷ�һ������ӿڵ����ü�����
IFACEMETHODIMP_(ULONG) FileDragDropExt::AddRef()
{
    return InterlockedIncrement(&m_cRef);
}

//  ���ٶ�һ������ӿڵ����ü�����
IFACEMETHODIMP_(ULONG) FileDragDropExt::Release()
{
    ULONG cRef = InterlockedDecrement(&m_cRef);
    if (0 == cRef)
    {
        delete this;
    }

    return cRef;
}

#pragma endregion


#pragma region IShellExtInit

// ��ʼ������ϷŴ������
IFACEMETHODIMP FileDragDropExt::Initialize(
    LPCITEMIDLIST pidlFolder, LPDATAOBJECT pDataObj, HKEY hKeyProgID)
{
    // �õ��ļ����õ�·����
    if (!SHGetPathFromIDList(pidlFolder, this->m_szTargetDir))
    {
		return E_FAIL;
    }

    // ��ȡ���ڱ��ƶ����ļ���
    if (NULL == pDataObj)
    {
        return E_INVALIDARG;
    }

    HRESULT hr = E_FAIL;

    FORMATETC fe = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
    STGMEDIUM stm;

    // pDataObjָ������Ķ����������С�����������У����ǵõ�һ�������ƶ��ļ����ļ��е�ö�ٵ�HDROP����
    if (SUCCEEDED(pDataObj->GetData(&fe, &stm)))
    {
        //�õ�һ��HDROP����
        HDROP hDrop = static_cast<HDROP>(GlobalLock(stm.hGlobal));
        if (hDrop != NULL)
        {
            // ȷ�����ٸ��ļ�����������б����á����ʾ��������ʾ�˵������һ���ļ�������Ŀ¼�����ƶ�ʱ��
            UINT nFiles = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
            if (nFiles == 1)
            {
                // ��ȡ�ļ���·����
                if (0 != DragQueryFile(hDrop, 0, m_szSrcFile, ARRAYSIZE(m_szSrcFile)))
                {
                    // ���·�����벻��Ŀ¼����ΪӲ���ӽ������ļ�������Ŀ¼��
                    if (!PathIsDirectory(m_szSrcFile))
                    {
                        hr = S_OK;
                    }
                }
            }

            // [-or-]

            // ��׼���ƶ��ļ����ļ��е�ö�١�
            //if (nFiles > 0)
            //{
            //    std::list<std::wstring> draggedFiles;
            //    wchar_t szFileName[MAX_PATH];
            //    for (UINT i = 0; i < nFiles; i++)
            //    {
            //        // ��ȡ��һ���ļ�������.
            //        if (0 != DragQueryFile(hDrop, i, szFileName, ARRAYSIZE(szFileName)))
            //        {
            //            // ·������ΪĿ¼,��ΪӲ����ֻ�������ļ�����Ŀ¼.
            //            if (!PathIsDirectory(szFileName))
            //            {
            //                // ����ļ����Ƶ��б���.
            //                draggedFiles.push_back(szFileName);
            //            }
            //        }
            //    }
            //          
            //    // ��������κ��ܹ������ļ�������S_OK��
            //    if (draggedFiles.size() > 0)
            //    {
            //        hr = S_OK;
            //    }
            //}

            GlobalUnlock(stm.hGlobal);
        }

        ReleaseStgMedium(&stm);
    }

    // �������S_OK���κ�ֵ����������з��أ�����˵����ʾ��
    return hr;
}

#pragma endregion


#pragma region IContextMenu

//
//   ����: FileDragDropExt::QueryContextMenu
//          
//   Ŀ��: ���Shell����IContextMenu::QueryContextMenu�����������Ĳ˵����������Ӳ˵���˵��С�
//	    ��������hmenu������HMENU�������indexMenu�����������õ�һ����ӵĲ˵����������
//
IFACEMETHODIMP FileDragDropExt::QueryContextMenu(
    HMENU hMenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags)
{
    // ���uFlags����CMF_DEFAULTONLY�����ǲ����κ����顣
    if (CMF_DEFAULTONLY & uFlags)
    {
        return MAKE_HRESULT(SEVERITY_SUCCESS, 0, USHORT(0));
    }

    // ��InsertMenu����InsertMenuItem����Ӳ˵��

    MENUITEMINFO mii = { sizeof(mii) };
    mii.fMask = MIIM_ID | MIIM_TYPE | MIIM_STATE;
    mii.wID = idCmdFirst + IDM_RUNFROMHERE;
    mii.fType = MFT_STRING;
    mii.dwTypeData = m_pszMenuText;
    mii.fState = MFS_ENABLED;
    if (!InsertMenuItem(hMenu, indexMenu, TRUE, &mii))
    {
        return HRESULT_FROM_WIN32(GetLastError());
    }

    // ���������Է���һ��HRESULTֵ������SEVERITY_SUCCESS�����ñ���������������������ʶ������һ��1����
    return MAKE_HRESULT(SEVERITY_SUCCESS, 0, USHORT(IDM_RUNFROMHERE + 1));
}


//
//   ����: FileDragDropExt::InvokeCommand
//        
//   Ŀ��:  ��һ���û�����˵��������ߴ��������������ϵ������������������á�lpcmi����ָ��һ��������Ҫ��Ϣ�Ľṹ�塣
//
IFACEMETHODIMP FileDragDropExt::InvokeCommand(LPCMINVOKECOMMANDINFO pici)
{ 
    // pici->lpVerb��high-word������NULL����Ϊ���Ƕ�������û��ʵ��IContextMenu::GetCommandString ��ָ���κα�����
    if (NULL != HIWORD(pici->lpVerb))
	{
		return E_INVALIDARG;
	}

    // Ȼ����lpcmi->lpVerb��low-wordӦ�ð�������ĵ�����ʶ����
    if (LOWORD(pici->lpVerb) == IDM_RUNFROMHERE)
	{
		OnCreateHardLink(pici->hwnd);
	}
	else
	{
		// �������������ܱ�����ϷŴ������ʶ�� �������뷵��E_FAIL���������������������ϷŴ��������Ҳ�����������������
		return E_FAIL;
	}

	return S_OK;
}


//
//  ����: FileDragDropExt::GetCommandString
//  ���һ���û���ͻ����Ŀ֮һ����������Ĳ˵�������������������IContextMenu::GetCommandString
//  ����������������һ������ʾ��Windows Explorer״̬����Help�ı��ַ������������Ҳ�����������󱻷��䵽һ������ı����ַ�����
//  ANSI����Unicode���ܱ�����������Ӳ���ҪΪ����ָ���κα����������������ֱ�ӷ���E_NOTIMPL��
//
IFACEMETHODIMP FileDragDropExt::GetCommandString(UINT_PTR idCommand, 
    UINT uFlags, UINT *pwReserved, LPSTR pszName, UINT cchMax)
{
    return E_NOTIMPL;
}

#pragma endregion
