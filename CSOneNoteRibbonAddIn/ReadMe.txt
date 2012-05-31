=============================================================================
          Ӧ�ó��� : CSOneNoteRibbonAddIn ��Ŀ����
=============================================================================
/////////////////////////////////////////////////////////////////////////////
ժҪ: 
��ʵ����ʾ��һ��ʵ����IDTExtensibility2 �� OneNote 2010 COM ��ӳ���.

����ӳ���֧��ͨ��ʵ�� IRibbonExtensibility �ӿڣ����ƹ�����.

CSOneNoteRibbonAddIn: Ϊ CSOneNoteRibbonAddInSetup ��Ŀ���� 
CSOneNoteRibbonAddIn.dll ����Ŀ.

CSOneNoteRibbonAddInSetup: �������� OneNote 2010 �� setup.exe ��
CSOneNoteRibbonAddInSetup.msi �İ�װ��Ŀ.

/////////////////////////////////////////////////////////////////////////////
ǰ��������

�������ڰ�װ�� Microsoft OneNote 2010 �ļ���������д˴���ʾ��.

/////////////////////////////////////////////////////////////////////////////
��ʾ:

���в��轫ָ��ͨ�� CSOneNoteRibbonAddIn ����ʾʵ��.

���� 1. �Թ���Ա�����,�򿪽�������ļ� CSOneNoteRibbonAddIn.sln;

���� 2. ����,�� Visual Studio 2010��,���� CSOneNoteRibbonAddIn��Ȼ�����ɰ�װ��
Ŀ CSOneNoteRibbonAddInSetup, ��ô�����õ��������� setup.exe ��Ӧ�ó��� 
CSOneNoteRibbonAddInSetup.msi;

����3. ��װ setup.exe;

����4. �� OneNote 2010, ��������������Ϣ��:
MessageBox.Show("CSOneNoteRibbonAddIn OnConnection"),
MessageBox.Show("CSOneNoteRibbonAddIn OnAddInsUpdate"),
MessageBox.Show("CSOneNoteRibbonAddIn OnStartupComplete");

����5. ����"����"ѡ�,��������"ͳ����"��,��������ӳ���ӵ��������İ�ť ShowForm.
���� ShowForm ��ť�����崰�ڵ����������Ե����ô����ϵİ�ť��ȡ��ǰҳ�ı���.

����6. ���ر� OneNote 2010 ʱ, ��������������Ϣ��:
MessageBox.Show("CSOneNoteRibbonAddIn OnBeginShutdown"),
MessageBox.Show("CSOneNoteRibbonAddIn OnDisconnection")

/////////////////////////////////////////////////////////////////////////////
����:

����1. ������ʾ,����һ��������ӳ������չ��,�͹������ӳ�����:
  
	�Թ���Ա�����,�� Visual Studio 2010;
	����һ���������ӳ��� (Other Project Types->Extensibility) 

	ʹ�� Visual C# ����һ����ӳ���; 

	ѡ�� Microsoft Access ����Ϊ������Microsoft OneNote �ɹ�ѡ��,��һ��������
	ѡ�� Microsoft Access ѡ��,���ǵ�Ҫ�޸İ�װ��Ŀ�ĵ�ע�����HKCUΪOneNote);
    
	��д��ӳ�������ƺ�˵��;
	��ѡ����ӳ���ѡ����ѡ��������ѡ��
	
����2. �޸� CSOneNoteRibbonAddInSetup ע��� 
(�һ�"��Ŀ"->��ͼ->ע���) 
[HKEY_CLASSES_ROOT\AppID\{Your GUID}]
"DllSurrogate"=""
[HKEY_CLASSES_ROOT\CLSID\{Your GUID}]
"AppID"="{Your GUID}"

[HKEY_CURRENT_USER\Software\Microsoft\Office\OneNote\AddIns\
CSOneNoteRibbonAddIn.Connect]
"LoadBehavior"=dword:00000003
"FriendlyName"="CSOneNoteRibbionAddIn"
"Description"="OneNote2010 Ribbon AddIn Sample"

[HKEY_LOCAL_MACHINE\SOFTWARE\Classes\AppID\{Your GUID}]
"DllSurrogate"=""
[HKEY_LOCAL_MACHINE\SOFTWARE\Classes\CLSID\{Your GUID}]
"AppID"="{Your GUID}"

����3. ��� customUI.xml �� showform.png ��Դ�ļ�����Ŀ CSOneNoteRibbonAddIn ��.

����4. �����̳��� IRibbonExtensibility ���� Connect,��ʵ�ַ��� GetCustomUI.
       
	/// <summary>		     
        /// ���Զ��幦�����û������ XML �Զ����ļ���,���ص� XML ��� 
        /// </summary>
        /// <param name="RibbonID"> RibbonX �û������ ID </param>
        /// <returns>string</returns>
        public string GetCustomUI(string RibbonID)
        {
            return Properties.Resources.customUI;
        }

����5. ���� customUI.xml ������,ʵ�ַ��� OnGetImage �� ShowForm.

        /// <summary>
        ///  ʵ�� customUI.xml �е� OnGetImage ����
        /// </summary>
        /// <param name="imageName"> customUI.xml �е�ͼ���� </param>
        /// <returns> װ��ͼ�����ݵ��ڴ��� </returns>
        public IStream OnGetImage(string imageName)
        {
            MemoryStream stream = new MemoryStream();
            if (imageName == "showform.png")
            {
                Resources.ShowForm.Save(stream, ImageFormat.Png);
            }

            return new ReadOnlyIStreamWrapper(stream);
        }

        /// <summary>
        ///  ������ʾ����ķ���
        /// </summary>
        /// <param name="control"> ��ʾ���ݸ�ÿ���������û����� (UI) �ؼ���	
        /// �ص����̵Ķ���. </param>
        public void ShowForm(IRibbonControl control)
        {
            OneNote.Window context = control.Context as OneNote.Window;
            CWin32WindowWrapper owner =
                new CWin32WindowWrapper((IntPtr)context.WindowHandle);
            TestForm form = new TestForm(applicationObject as OneNote.Application);
            form.ShowDialog(owner);

            form.Dispose();
            form = null;
            context = null;
            owner = null;           
            GC.Collect();
            GC.WaitForPendingFinalizers();
            GC.Collect();
        }

����6. ��� ReadOnlyIStreamWrapper ��� CWin32WindowWrapper �ൽ��Ŀ
CSOneNoteRibbonAddIn �в����һ�����ڲ����д򿪵Ĵ���.

����7. ע���������Ϊ COM ���.

��Ҫִ�д˲������뵥������Ŀ��->����Ŀ����...����ť. ����Ŀ����ҳ������
��"����"ѡ�,��ѡ�и�ѡ��"Ϊ COM Interop ע��".

����8. �������� CSOneNoteRibbonAddIn ��Ŀ, 
Ȼ������ CSOneNoteRibbonAddInSetup ��Ŀ�Բ��� setup.exe �� 
CSOneNoteRibbonAddInSetup.msi.

/////////////////////////////////////////////////////////////////////////////
�ο�:

MSDN: Creating OneNote 2010 Extensions with the OneNote Object Model
http://msdn.microsoft.com/en-us/magazine/ff796230.aspx

Jeff Cardon's Blog
http://blogs.msdn.com/b/onenotetips/

/////////////////////////////////////////////////////////////////////////////

