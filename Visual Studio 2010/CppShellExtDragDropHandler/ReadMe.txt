=============================================================================
    ��̬���ӿ� : CppShellExtDragDropHandler��Ŀ����
=============================================================================

/////////////////////////////////////////////////////////////////////////////
��Ҫ:

��ʾ��������ʾ����C++����һ��Shell����϶��Ĵ���

���û��Ҽ����һ��Shell�������϶��ö���ʱ�����û���ͼ��ɾ�����õĶ���ʱһ������
�Ĳ˵�����ʾ��һ������϶����������һ�������Ĳ˵�������򣬿��Խ���Ŀ��ӵ���
�������Ĳ˵���


�������϶������������Ӿ���Class ID(CLSID): 
    {F574437A-F944-4350-B7E9-95B6C7008029}

���������Ĳ˵������һ���˵������Ӳ���ӡ��������Ҽ�����һ���ļ����϶�����ļ���
һ��Ŀ¼��һ�������������棬�����Ĳ˵�����Ϊ������Ӳ���ӡ��Ĳ˵����ʾ��ͨ�����
�˵���������������϶�λ�ô���һ���϶��ļ���Ӳ���ӡ������ӵ������ǡ���<Դ��
����>��Ӳ���ӡ���(һ��Ӳ������һ���ļ����ļ�ϵͳ�Ĵ������и��ļ�����ͬ������ڳ�
��һ�����õ�һ�ļ���·�����ļ����κθı䱻��ʾ�������ļ���Ӳ���ӵ�Ӧ�ó����У�����
Ӳ���ӵĸ�����Ϣ���ο�MSDN�ĵ���http://msdn.microsoft.com/en-us/library/aa365006.aspx.��


/////////////////////////////////////////////////////////////////////////////
��װ���Ƴ�:

A. ��װ

�����Ҫʹ��Windowsϵͳ��һ��X64��Shell��չ����ʹ����Ŀ
���ã�http://msdn.microsoft.com/en-us/library/9yb4317s.aspx��Visual C++��Ŀ����64λƽ̨��
ֻ��64λ��չ��DLL���Ա����ص�64λWindows Shell�����С��������չ�����ص���32λWindowsϵͳ��
�����ʹ��Ĭ��Win32��Ŀ���ã������ɸ���Ŀ��


��������ʾ�����Թ���Ա������У��������������ɵ�CppShellExtDragDropHandler.dll ���ļ��в�
�������

    Regsvr32.exe CppShellExtDragDropHandler.dll

��������ק������򽫱��ɹ�ע�ᣬ�������һ����Ϣ�����Ϣ�ǣ�

    "DllRegisterServer in CppShellExtDragDropHandler.dll succeeded."

B. �Ƴ�

��������ʾ�����Թ���Ա������У��������������ɵ�CppShellExtDragDropHandler.dll ���ļ��в�
�������

    Regsvr32.exe /u CppShellExtDragDropHandler.dll

��������ק������򽫱��ɹ��Ƴ����������һ����Ϣ�����Ϣ�ǣ�

    "DllUnregisterServer in CppShellExtDragDropHandler.dll succeeded."


/////////////////////////////////////////////////////////////////////////////
��ʾ:

����Ĳ����������ק����������ʾ������ʾ��

����1. �����Ҫʹ��Windowsϵͳ��һ��X64��Shell��չ����ʹ����Ŀ
���ã�http://msdn.microsoft.com/en-us/library/9yb4317s.aspx��Visual C++��Ŀ����64λƽ̨��
ֻ��64λ��չ��DLL���Ա����ص�64λWindows Shell�����С��������չ�����ص���32λWindowsϵͳ��
�����ʹ��Ĭ��Win32��Ŀ���ã������ɸ���Ŀ��

����2.Ȼ������Visual Studio 2010�ɹ��������������Ŀʾ�����㽲���һ��DLL�ļ�
CppShellExtDragDropHandler.dll����Ϊ����Ա���������һ��������ʾ������������������
��CppShellExtDragDropHandler.dll ���ļ��в��������

 
    Regsvr32.exe CppShellExtDragDropHandler.dll

��������ק������򽫱��ɹ�ע�ᣬ�������һ����Ϣ�����Ϣ�ǣ�

    "DllRegisterServer in CppShellExtDragDropHandler.dll succeeded."

����3. ����Դ�������в���һ���ļ���������������ļ����е�ReadMe.txt�������ƵĴ�С���һ����
�ļ������϶�����һ��Ŀ¼��һ�������Ĳ˵�������Ϊ��Ӳ���ӡ��Ĳ˵����ʾ��ͨ���������˵��
�������Ӧ�ó������Ϸŵ�λ���е���ק���Ǹ��ļ��ϴ���һ��Ӳ���ӡ�������ӵ������ǡ�Ӳ����<��
Դ�ļ���>�����硰Ӳ���ӵ�ReadMe.txt����.�ļ����κθı䱻��ʾ�������ļ���Ӳ���ӵ�Ӧ�ó����У����
���Ϸŵ�λ������һ����ͬ���ļ������㽫����һ���������Ϣ�������Ѿ���һ����ͬ���ļ�������ͨ��Windows Shell ��ʾ��


����4. ����ͬ��������ʾ���£���������

    Regsvr32.exe /u CppShellExtDragDropHandler.dll

Ϊ��ע������϶���������Shell��


/////////////////////////////////////////////////////////////////////////////
ʵʩ:

A. ���������������Ŀ

�� Visual Studio 2010��, ����һ�� Visual C++ / Win32 / Win32 ����Ŀ��Ϊ
"CppShellExtDragDropHandler". ��Win32Ӧ�ó����򵼵� "Ӧ�ó�������" ҳ�У�ѡ������Ϊ"DLL����Ӧ��
�����Ҳ鿴������Ŀ��ѡ�Ȼ��������ɰ�ť��һ���յ�Win32 DLL ��Ŀ���������ˣ�

-----------------------------------------------------------------------------

B. ʵ�ֻ����������ģ�ͣ�COM����DLL

Shell��չ��������ڽ����ڵ�COM�����DLL��ʵ�ֵġ�
��DLL�У��ʹ��⵼�����ǣ�����һ��������COM����ʵʩ��DllGetClassObject, DllCanUnloadNow, 
DllRegisterServer,�� DllUnregisterServer in (and exporting them from) the 
DLL, ���һ��IUnknown�ӿڵĻ���ʵ�ֵ�COM�࣬�������COM��׼���๤������ص��ļ��ڴ�ʾ��������

  dllmain.cpp - ʵ�� DllMain �����DllGetClassObject, DllCanUnloadNow, 
    DllRegisterServer, DllUnregisterServer �Ĺ��ܶ���COM DLL�Ǳ�Ҫ�� 

  GlobalExportFunctions.def -ͨ��module-definition�ļ���DLL�е���DllGetClassObject, DllCanUnloadNow, 
    DllRegisterServer, DllUnregisterServer. �����ͨ��.def�ļ�����������Ŀ�����Ե�����ҳ��ͨ��������
��Module Definition File����

  Reg.h/cpp - ��ע����ж���COM�����ע�����ע���Ĺ��ܵ��ظ�ʹ��
    RegisterInprocServer, UnregisterInprocServer

  FileDragDropExt.h/cpp - ����COM��. ������ļ������ܷ���IUnknown�ӿڵĻ���ʵ��

  ClassFactory.h/cpp - ����COM�ඨ���๤��. 

-----------------------------------------------------------------------------

C. ʵ������϶��Ĵ����ע��.

-----------
ʵ������϶�����:

һ������϶�������һ�������Ĳ˵��Ĵ�����������Ĳ˵���������������Ĳ˵�����Ӷ���� . ʵ��
��������ϷŴ������ͳ��������Ĳ˵����������ͬ

���FileDragDropExt.h/cpp�ļ�����һ������ϷŴ������. �������ϷŴ���������ʵ
��IShellExtInit �� IContextMenu�ӿڡ� 

    class FileDragDropExt : IShellExtInit, IContextMenu
    {
    public:
        // IShellExtInit
        IFACEMETHODIMP Initialize(LPCITEMIDLIST pidlFolder, LPDATAOBJECT 
            pDataObj, HKEY hKeyProgID);

        // IContextMenu
        IFACEMETHODIMP QueryContextMenu(HMENU hMenu, UINT indexMenu, 
            UINT idCmdFirst, UINT idCmdLast, UINT uFlags);
        IFACEMETHODIMP InvokeCommand(LPCMINVOKECOMMANDINFO pici);
        IFACEMETHODIMP GetCommandString(UINT_PTR idCommand, UINT uFlags, 
            UINT *pwReserved, LPSTR pszName, UINT cchMax);
    };

  1.ʵ��IShellExtInit

������Ϸ���չCOM����ʵ������IShellExtInit::Initialize���������ã��ڷ����Ĳ���֮�䣬pidlFolder��
�ļ��е�PIDL������ļ��������ļ����ŵĵط��������ͨ������SHGetPathFromIDList��PIDL�л��Ŀ¼
  

  pDataObj��һ��IDataObject�ӿ�ָ��ͨ�������ǿ��Լ����������ļ������֡�
  
��IShellExtInit::Initialize������������κ�S_OK��ֵ�����أ�������˵�����ᱻʹ�á�


 �ڴ���ʾ����.���FileDragDropExt����Initialize��������
  Ŀ��Ŀ¼�������ڳ�Ա������Ŀ¼·���У� m_szTargetDir
  
    //�������ļ����ŵ�Ŀ¼.
    if (!SHGetPathFromIDList(pidlFolder, this->m_szTargetDir))
    {
		return E_FAIL;
    }

  �����о���ѡ�����ļ����ļ��С����ֻ*һ*�ļ���
  �϶������Ҹ��ļ�����Ŀ¼Ҫ����Ӳ���Ӳ�����
  ���÷����洢����ļ������Ժ�ʹ�ã��������ļ���
  S_OK��
.
  2. ʵ�� IContextMenu

  IShellExtInit::Initialize returns S_OK֮��IContextMenu::QueryContextMenu ������������ȡ�˵������
����϶�����չ�����ӡ�QueryContextMenu �Ǽ��׶��á���InsertMenuItem�������Ƶķ������������϶�
����չ�Ĳ˵���˵������ʶ��������ڻ��ߵ���idCmdFirst���ұ���С��idCmdLast��QueryContextMenu���뷵
���������ֱ�ʶ����ӵ��˵����һ����õķ���˵�����ķ�ʽ����0��ʼ���ҽ����������С��������Ϸ���
չ����Ҫ����κ���˵��У���Ӧ�ü򵥵Ĵ�QueryContextMenu����0.�����ʾ�������У����ǲ�������˵����
���ﴴ��Ӳ���ӡ���
IContextMenu::GetCommandString �������������˵���ԭ�������ݣ������ڲ˵�������ʾ�İ����ı�������������
�Ų��������У������������ǲ���Ҫ�����ַ������߱����ַ������������Ǽ򵥵Ĵ�GetCommandString����E_NOTIMP��
��ͨ��ѡ����չ��װ�˵���ʱIContextMenu::InvokeCommand�����á������������Ӧ�������ﴴ��Ӳ���ӡ�����ʱ������
�����ļ������ļ�֮��ķŵ�λ�ý���Ӳ���ӡ�


-----------
ע�ᴦ�����:

��괦�����ͨ��ע��������������У�

    HKEY_CLASSES_ROOT\Directory\shellex\DragDropHandlers\

��ĳЩ����£�����Ҫ��HKCR\Folder��ע���������������ϴ�������drop�����������ڸ�Ŀ¼��
HKCR\Drive���������drop������

����ϷŴ�������ע����ʵ���� dllmain.cpp��DllRegisterServer�����С� DllRegisterServer������Reg.h/cpp�е�
�� DllRegisterServer������ע��COM������������� ������RegisterShellExtDragDropHandler��HKEY_CLASSES_ROOT\Directory
\shellex\DragDropHandlers\��HKEY_CLASSES_ROOT\Folder\shellex\DragDropHandlers\��HKEY_CLASSES_ROOT\Drive\shellex\
DragDropHandlers\��ע������ϷŴ������   

����ļ�ֵ�������ʾ�������ע�������
 

    HKCR
    {
        NoRemove CLSID
        {
            ForceRemove {F574437A-F944-4350-B7E9-95B6C7008029} = 
                s 'CppShellExtDragDropHandler.FileDragDropExt Class'
            {
                InprocServer32 = s '<Path of CppShellExtDragDropHandler.DLL file>'
                {
                    val ThreadingModel = s 'Apartment'
                }
            }
        }
        NoRemove Directory
        {
            NoRemove shellex
            {
                NoRemove DragDropHandlers
                {
                    {F574437A-F944-4350-B7E9-95B6C7008029} = 
                        s 'CppShellExtDragDropHandler.FileDragDropExt'
                }
            }
        }
        NoRemove Folder
        {
            NoRemove shellex
            {
                NoRemove DragDropHandlers
                {
                    {F574437A-F944-4350-B7E9-95B6C7008029} = 
                        s 'CppShellExtDragDropHandler.FileDragDropExt'
                }
            }
        }
        NoRemove Drive
        {
            NoRemove shellex
            {
                NoRemove DragDropHandlers
                {
                    {F574437A-F944-4350-B7E9-95B6C7008029} = 
                        s 'CppShellExtDragDropHandler.FileDragDropExt'
                }
            }
        }
    }

ע����ʵ����dllmain.cpp��DllUnregisterServer�����С� ����������HKCR\Directory\shellex\ContextMenuHandlers, 
HKCR\Folder\shellex\ContextMenuHandlers��HKCR\Drive\shellex\ContextMenuHandlers�µ�HKCR\CLSID\{<CLSID>} ���� {<CLSID>} ��



/////////////////////////////////////////////////////////////////////////////
�ο�:

MSDN: ��������ϷŴ������
http://msdn.microsoft.com/en-us/library/bb776881.aspx#dragdrop

The Complete Idiot's Guide to Writing Shell Extensions - ���Ĳ���
http://www.codeproject.com/KB/shell/shellextguide4.aspx


/////////////////////////////////////////////////////////////////////////////