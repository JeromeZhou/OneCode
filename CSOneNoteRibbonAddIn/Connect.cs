/****************************** ģ��ͷ ************************************\
ģ����:  Connect.cs
��Ŀ��:  CSOneNoteRibbonAddIn
��Ȩ (c) Microsoft Corporation.

���޷�������ӳ����ϵ��¼�֪ͨ������,�����Ǳ����ء�ж�ء����µȵ�.

This source is subject to the Microsoft Public License.
See http://www.microsoft.com/opensource/licenses.mspx#Ms-PL.
All other rights reserved.

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, 
EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
\***************************************************************************/

namespace CSOneNoteRibbonAddIn
{
    #region ���õ������ռ�
    using System;
    using Extensibility;
    using System.Runtime.InteropServices;
    using System.Windows.Forms;
    using Microsoft.Office.Core;
    using System.Runtime.InteropServices.ComTypes;
    using System.IO;
    using CSOneNoteRibbonAddIn.Properties;
    using System.Drawing.Imaging;
    using OneNote = Microsoft.Office.Interop.OneNote; 
    #endregion

    #region �����װ����Ϣ���õ���ʾ
    // ������ʱ����ӳ�����׼��ע������.
    // ���Ժ��ʱ�䣬�����ӳ�������ĳЩԭ��,��Ϊ������,����:
    //   1) ���ƶ�����Ŀ��һ̨�������,�ü���������������������Ŀ�Ļ���.
    //   2) ����ʾһ����Ϣѯ�����Ƿ�Ҫɾ������ӳ���,��ѡ����"��".
    //   3) ע�����.
    // ������Ҫͨ������ CSOneNoteRibbonAddInSetup ��Ŀ������ע��,�ڡ����������Դ��������
    // ���һ�����Ŀ,ѡ��װ.
    #endregion

    /// <summary>
    ///  ����ʵ��һ����ӳ���Ķ���.
    /// </summary>
    /// <seealso class='IDTExtensibility2' />
    [GuidAttribute("0BE84534-48A5-48A7-A9BD-0B5CAE7E12A0"),
    ProgId("CSOneNoteRibbonAddIn.Connect")]
    public class Connect : Object, Extensibility.IDTExtensibility2, IRibbonExtensibility
    {
        /// <summary>
        /// ʵ�ָ���Ӷ���Ĺ�����.
        ///	�����Խ����Լ��ĳ�ʼ��������ӵ��÷�����.
        /// </summary>
        public Connect()
        {
        }

        /// <summary>
        /// ʵ�� IDTExtensibility2 �ӿڵ�OnConnection ����.
        /// ����ӳ��򱻼���ʱ,����֪ͨ.
        /// </summary>
        /// <param term='application'>
        ///  ����Ӧ�ó���ĸ�����
        /// </param>
        /// <param term='connectMode'>
        ///  ������ӳ������ڱ����ص����.
        /// </param>
        /// <param term='addInInst'>
        ///  ��ʾ����ӳ���Ķ���.
        /// </param>
        /// <seealso class='IDTExtensibility2' />
        public void OnConnection(object application, Extensibility.ext_ConnectMode connectMode, 
            object addInInst, ref System.Array custom)
        {
            MessageBox.Show("CSOneNoteRibbonAddIn OnConnection");
            applicationObject = application;
            addInInstance = addInInst;
        }

        /// <summary>
        ///  ʵ�� IDTExtensibility2 �ӿڵ�OnDisconnection ����.
        ///  ����ӳ���ж��ʱ,����֪ͨ.
        /// </summary>
        /// <param term='disconnectMode'>
        ///  ������ӳ������ڱ�ж�ص����.
        /// </param>
        /// <param term='custom'>
        ///  ����Ӧ�ó����ض��Ĳ���������.
        /// </param>
        /// <seealso class='IDTExtensibility2' />
        public void OnDisconnection(Extensibility.ext_DisconnectMode disconnectMode, 
            ref System.Array custom)
        {
            MessageBox.Show("CSOneNoteRibbonAddIn OnDisconnection");
            this.applicationObject = null;
            GC.Collect();
            GC.WaitForPendingFinalizers();
        }

        /// <summary>
        /// ʵ�� IDTExtensibility2 �ӿڵ� OnAddInsUpdate ����.   
        /// ����ӳ���ļ��ϱ��޸�ʱ,����֪ͨ.
        /// </summary>
        /// <param term='custom'>
        /// ����Ӧ�ó����ض��Ĳ���������.
        /// </param>
        /// <seealso class='IDTExtensibility2' />
        public void OnAddInsUpdate(ref System.Array custom)
        {
            MessageBox.Show("CSOneNoteRibbonAddIn OnAddInsUpdate");
        }

        /// <summary>
        ///  ʵ�� IDTExtensibility2 �ӿڵ� OnStartupComplete ����. 
        ///  ������Ӧ�ó���ļ��ϱ��޸�ʱ,����֪ͨ.
        /// </summary>
        /// <param term='custom'>
        ///  ����Ӧ�ó����ض��Ĳ���������.
        /// </param>
        /// <seealso class='IDTExtensibility2' />
        public void OnStartupComplete(ref System.Array custom)
        {
            MessageBox.Show("CSOneNoteRibbonAddIn OnStartupComplete");
        }

        /// <summary>
        ///  ʵ�� IDTExtensibility2 �ӿڵ� OnBeginShutdown ����.
        ///  ������Ӧ�ó���ж��ʱ,����֪ͨ.
        /// </summary>
        /// <param term='custom'>
        ///  ����Ӧ�ó����ض��Ĳ���������.
        /// </param>
        /// <seealso class='IDTExtensibility2' />
        public void OnBeginShutdown(ref System.Array custom)
        {
            MessageBox.Show("CSOneNoteRibbonAddIn OnBeginShutdown");

            if (this.applicationObject != null)
            {
                this.applicationObject = null;
            }
        }

        private object applicationObject;
        private object addInInstance;

        /// <summary>		     
        /// ���Զ��幦�����û������ XML �Զ����ļ���,���ص� XML ��� 
        /// </summary>
        /// <param name="RibbonID"> RibbonX �û������ ID </param>
        /// <returns>string</returns>
        public string GetCustomUI(string RibbonID)
        {
            return Properties.Resources.customUI;
        }

        /// <summary>
        ///  ʵ�ַ��� customUI.xml �е� OnGetImage
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
    }
}