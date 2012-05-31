=============================================================================
          Ӧ�ó���  : VBOneNoteRibbonAddIn ��Ŀ����
=============================================================================

/////////////////////////////////////////////////////////////////////////////
ժҪ: 

��ʵ����ʾ��һ��ʵ����IDTExtensibility2 �� OneNote 2010 COM ��ӳ���.
����ӳ���֧��ͨ��ʵ�� IRibbonExtensibility �ӿڣ����ƹ�����.

VBOneNoteRibbonAddIn: Ϊ VBOneNoteRibbonAddInSetup ��Ŀ���� 
VBOneNoteRibbonAddIn.dll ����Ŀ.

VBOneNoteRibbonAddInSetup: �������� OneNote 2010 �� setup.exe ��
VBOneNoteRibbonAddInSetup.msi �İ�װ��Ŀ.

/////////////////////////////////////////////////////////////////////////////
ǰ��������

�������ڰ�װ�� Microsoft OneNote 2010 �ļ���������д˴���ʾ��.

/////////////////////////////////////////////////////////////////////////////
��ʾ:

���в��轫ָ��ͨ�� VBOneNoteRibbonAddIn ����ʾʵ��.

���� 1. �Թ���Ա�����,�򿪽�������ļ� VBOneNoteRibbonAddIn.sln;

���� 2. ����,�� Visual Studio 2010��,���� VBOneNoteRibbonAddIn��Ȼ�����ɰ�װ��
Ŀ VBOneNoteRibbonAddInSetup, ��ô�����õ��������� setup.exe ��Ӧ�ó��� 
VBOneNoteRibbonAddInSetup.msi;

����3. ��װ setup.exe;

����4. �� OneNote 2010, ��������������Ϣ��:
MessageBox.Show("VBOneNoteRibbonAddIn OnConnection")
MessageBox.Show("VBOneNoteRibbonAddIn OnAddInsUpdate")
MessageBox.Show("VBOneNoteRibbonAddIn OnStartupComplete");

����5. ����"����"ѡ�,��������"ͳ����"��,��������ӳ���ӵ��������İ�ť ShowForm.
���� ShowForm ��ť�����崰�ڵ����������Ե����ô����ϵİ�ť��ȡ��ǰҳ�ı���.

����6. ���ر� OneNote 2010 ʱ, ��������������Ϣ��:
MessageBox.Show("VBOneNoteRibbonAddIn OnBeginShutdown")
MessageBox.Show("VBOneNoteRibbonAddIn OnDisconnection")

/////////////////////////////////////////////////////////////////////////////
����:

����1. ������ʾ,����һ��������ӳ������չ��,�͹������ӳ�����:

	�Թ���Ա�����,�� Visual Studio 2010;
	����һ���������ӳ��� (Other Project Types->Extensibility) 

	ʹ�� Visual Basic.NET ����һ����ӳ���; 

	ѡ�� Microsoft Access ����Ϊ������Microsoft OneNote �ɹ�ѡ��,��һ��������
	ѡ�� Microsoft Access ѡ��,���ǵ�Ҫ�޸İ�װ��Ŀ�ĵ�ע�����HKCUΪOneNote);
    
	��д��ӳ�������ƺ�˵��;
	��ѡ����ӳ���ѡ����ѡ��������ѡ��

����2. �޸� VBOneNoteRibbonAddInSetup ע���
(right click Project->View->Registry) 
[HKEY_CLASSES_ROOT\AppID\{Your GUID}]
"DllSurrogate"=""
[HKEY_CLASSES_ROOT\CLSID\{Your GUID}]
"AppID"="{Your GUID}"

[HKEY_CURRENT_USER\Software\Microsoft\Office\OneNote\AddIns\
VBOneNoteRibbonAddIn.Connect]
"LoadBehavior"=dword:00000003
"FriendlyName"="OneNoteRibbonAddInSample"
"Description"="OneNote2010 Ribbon AddIn Sample"

[HKEY_LOCAL_MACHINE\SOFTWARE\Classes\AppID\{Your GUID}]
"DllSurrogate"=""
[HKEY_LOCAL_MACHINE\SOFTWARE\Classes\CLSID\{Your GUID}]
"AppID"="{Your GUID}"

����3. ��� customUI.xml �� showform.png ��Դ�ļ�����Ŀ VBOneNoteRibbonAddIn ��.

����4. �����̳��� IRibbonExtensibility ���� Connect,��ʵ�ַ��� GetCustomUI.

    ''' <summary>
    '''    ���Զ��幦�����û������ XML �Զ����ļ���,���ص� XML ���.
    ''' </summary>
    ''' <param name="RibbonID">RibbonX �û������ ID</param>
    ''' <returns>string</returns>
    Public Function GetCustomUI(ByVal RibbonID As String) As String _
        Implements IRibbonExtensibility.GetCustomUI

        Return Resources.customUI
    End Function

����5. ���� customUI.xml ������,ʵ�ַ��� OnGetImage �� ShowForm.

    ''' <summary>
    '''  ʵ�� customUI.xml �е� OnGetImage ����
    ''' </summary>
    ''' <param name="imageName">customUI.xml �е�ͼ����</param>
    ''' <returns> װ��ͼ�����ݵ��ڴ��� </returns>
    Public Function OnGetImage(ByVal imageName As String) As IStream
        Dim stream As New MemoryStream()

        If imageName = "showform.png" Then
            Resources.showform.Save(stream, ImageFormat.Png)
        End If

        Return New ReadOnlyIStreamWrapper(stream)
    End Function

    ''' <summary>
    '''  ������ʾ����ķ���
    ''' </summary>
    ''' <param name="control">��ʾ���ݸ�ÿ���������û����� (UI) �ؼ���	        
    '''  �ص����̵Ķ���. </param>
    Public Sub ShowForm(ByVal control As IRibbonControl)
        Dim context As OneNote.Window = TryCast(control.Context, OneNote.Window)
        Dim owner As New CWin32WindowWrapper(CType(context.WindowHandle, IntPtr))
        Dim form As New TestForm(TryCast(applicationObject, OneNote.Application))
        form.ShowDialog(owner)

        form.Dispose()
        form = Nothing
        context = Nothing
        owner = Nothing
        GC.Collect()
        GC.WaitForPendingFinalizers()
        GC.Collect()
    End Sub

����6. ��� ReadOnlyIStreamWrapper ��� CWin32WindowWrapper �ൽ��Ŀ
VBOneNoteRibbonAddIn �в����һ�����ڲ����д򿪵Ĵ���.

����7. ע���������Ϊ COM ���.

Ҫִ�д˲������뵥������Ŀ��->����Ŀ����...����ť. ����Ŀ����ҳ������
��"����"ѡ�,��ѡ�и�ѡ��"Ϊ COM Interop ע��".

����8. �������� VBOneNoteRibbonAddIn ��Ŀ, 
Ȼ������ VBOneNoteRibbonAddInSetup ��Ŀ�Բ��� setup.exe �� 
VBOneNoteRibbonAddInSetup.msi.


/////////////////////////////////////////////////////////////////////////////
�ο�:

MSDN: Creating OneNote 2010 Extensions with the OneNote Object Model
http://msdn.microsoft.com/en-us/magazine/ff796230.aspx

Jeff Cardon's Blog
http://blogs.msdn.com/b/onenotetips/

/////////////////////////////////////////////////////////////////////////////

