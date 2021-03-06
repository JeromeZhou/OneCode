================================================================================
       Windows 应用程序：CSIEExplorerBar 概述                        
===============================================================================
/////////////////////////////////////////////////////////////////////////////
概要：
本例展示了如何创建并部署一个列出网页所有图片的IE浏览器工具条。


/////////////////////////////////////////////////////////////////////////////
安装和卸载：

一、安装

对于安装在x86或x64操作系统上32位的IE，安装CSIEExplorerBarSetup(x86).msi。
这个文件由CSIEExplorerBarSetup(x86)项目执行获得。

对于安装在x64操作系统上的64位的IE，安装CSIEExplorerBarSetup(x64).msi。这个
文件由CSIEExplorerBarSetup(x64)项目执行获得。

二、卸载

对于安装在x86或x64操作系统上32位的IE，卸载CSIEExplorerBarSetup(x86).msi。
这个文件由CSIEExplorerBarSetup(x86)项目执行获得。

对于安装在x64操作系统上的64位的IE，卸载CSIEExplorerBarSetup(x64).msi。这个
文件由CSIEExplorerBarSetup(x64)项目执行获得。



////////////////////////////////////////////////////////////////////////////////
演示：
 
第一步：在VS2010中打开这个项目并将解决方案的platform设置为x86.请确保CSIEExplorerBar和
        CSIEExplorerBarSetup(x86)两个项目在配置管理器中是被选中为build的。

	    注意：如果你希望在64位的IE中运行这个例子，请将platform设置为x64并选中CSIEExplorerBar
              和CSIEExplorerBarSetup(x64)为build的。
        
第二步：生成解决方案。
 
第三步：在解决方案资源管理器中右键点击CSIEExplorerBarSetup(x86)项目，并选择安装。

	    安装完成后，打开32位的IE并点击工具栏=>管理加载项,在管理加载项对话框中，
        你可以找到"图片列举浏览器工具条"项。

第四步：打开32位IE并点击工具栏=>浏览器栏，然后选中"图片列举浏览器栏"。 你就会在IE中
        发现一个浏览器栏。

第五步：访问http://www.microsoft.com/. 点击浏览器栏中的“获取全部图片”按钮。你将会看到在浏览器栏
        中有许多图片的url。点击其中的一项，IE就会在一个新的选项卡中打开这图片。


/////////////////////////////////////////////////////////////////////////////
实现：

一、 创建一个项目并添加引用。

在Visual Studio 2010中,创建一个Visual C# / Windows / Class Library项目，
并命名为 “CSIEExplorerBar”。

在解决方案管理器中右键点击这个项目并选择“添加引用”。添加 .NET tab中的
“Microsoft.mshtml”和COM 选项卡中的 “Microsoft 网际网路控制”。
-----------------------------------------------------------------------------

二、创建IE浏览器栏目。

想要在IE中添加一个浏览器栏目，你需要做以下几步：

1. 为ComVisible 类创建一个有效的GUID。

2. 实现IObjectWithSite、IDeskBand、IDockingWindow、IOleWindow和IInputObject接口。
   
3. 将这个程序集注册到COM中。

4. 创建一个新的密钥，并用你创建的浏览器栏的类型的类别标识符 (CATID)作为这个密钥的名称
   可能会是以下值中的一个：
   {00021494-0000-0000-C000-000000000046} 定义一个横向的浏览器栏。
   {00021493-0000-0000-C000-000000000046} 定义一个垂直的浏览器栏。
   
   结果类似于：

   HKEY_CLASSES_ROOT\CLSID\<Your GUID>\Implemented Categories\{00021494-0000-0000-C000-000000000046}
   或
   HKEY_CLASSES_ROOT\CLSID\<Your GUID>\Implemented Categories\{00021493-0000-0000-C000-000000000046}
   
5.删除以下的注册表项，因为这些注册表项用于缓存 浏览器栏的枚举。

   HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Explorer\Discardable\PostSetup\
   Component Categories\{00021493-0000-0000-C000-000000000046}\Enum
   或
   HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Explorer\Discardable\PostSetup\
   Component Categories\{00021494-0000-0000-C000-000000000046}\Enum

6. 在注册表中设置浏览器栏的大小。

   HKEY_LOCAL_MACHINE\Software\Microsoft\Internet Explorer\Explorer Bars\<Your GUID>\BarSize
    

-----------------------------------------------------------------------------

三、 用安装项目部署IE浏览器栏。

  (1)在CSIEExplorerBar项目中，添加一个Installer类（在示例代码中被命名为IEExplorerBarInstaller ）用于定义
安装过程中自定义的行为。这个类继承了System.Configuration.Install.Installer。当用户安装/卸载组件时，我们
用自定义的行为来添加/删除在注册表中的上下文菜单项并在当前托管程序集中注册/注销COM可见类。


    [RunInstaller(true), ComVisible(false)]
    public partial class IEExplorerBarInstaller : System.Configuration.Install.Installer
    {
        public IEExplorerBarInstaller()
        {
            InitializeComponent();
        }
  
        public override void Install(System.Collections.IDictionary stateSaver)
        {
            base.Install(stateSaver);

            RegistrationServices regsrv = new RegistrationServices();
            if (!regsrv.RegisterAssembly(this.GetType().Assembly,
            AssemblyRegistrationFlags.SetCodeBase))
            {
                throw new InstallException("注册到COM失败");
            }
        }

        public override void Uninstall(System.Collections.IDictionary savedState)
        {
            base.Uninstall(savedState);

            RegistrationServices regsrv = new RegistrationServices();
            if (!regsrv.UnregisterAssembly(this.GetType().Assembly))
            {
                throw new InstallException("注册到COM失败");
            }
        }
    }
	

  (2)若要添加部署项目，在文件菜单上，指向“添加”，然后单击“新建项目”。在添加新项目
对话框中，展开"其他项目类型",再展开“安装和部署项目”，
然后点击Visual Studio 安装程序，最后点击安装项目。在名称框中，输入CSIEExplorerBarSetup(x86)。
单击“确定”以创建项目。在安装项目的属性对话框中，请确保目标平台 属性被设置为x86。这个安装
项目是为x86或x64的windows操作系统的32位IE浏览器部署的。

右击安装项目，选择“添加/项目输出 ... ”。在添加项目输出组对话框中，CSIEExplorerBar 会
显示在项目列表中。选择主输出并点击确定。VS将检测CSIEExplorerBar的依赖关系，
包括.Net  Framework 4.0 (Client Profile)。

右击安装项目，并选择“查看/自定义操作“。在自定义操作编辑器中，右击自定义操作根节点。在操作菜单上，
点击添加自定义操作。在 在项目中选项 的对话框中，双击应用程序文件夹。从CSIEExplorerBar中选择主输出。
这样会添加我们定义在CSIEExplorerBar的IEExplorerBarInstaller 中的自定义操作。
  
创建安装项目。如果创建成功，你会获得一个.msi文件和一个Setup.exe文件。你可以将它们分发给用户，用于
安装和卸载这个浏览器栏。


  (3) 若要部署这个浏览器栏到x64操作系统的64位IE中，你必需创建一个新的安装项目（例如 本例中得
CSIEExplorerBarSetup(x64)），并将其TargetPlatform 属性设置为x64。

   虽然TargetPlatform 属性被设置为x64，但是本机包装的.msi文件仍然是一个32位的可执行文件。
Visual Studio 将32位版本的InstallUtilLib.dll 嵌入到了二进制表中来取代InstallUtil。因此自定义操作会在
32位下运行，这并不是本例所期望的。要解决这个问题，保证自定义操作在64位模式下运行，你可以选择为InstallUtil 
记录导入适应位数的 InstallUtilLib.dll到二进制表中，或者如果你将32位托管自定义操作作为一项新的记录添加
到二进制表中，并调整CustomAction 表为64位托管自定义操作使用64位二进制表记录。这篇博客介绍了如何手动地
去这样做：
http://blogs.msdn.com/b/heaths/archive/2006/02/01/64-bit-managed-custom-actions-with-visual-studio.aspx。

在本此示例代码中，我们使用了一个后期生成的javascript：Fix64bitInstallUtilLib.js来自东修改InstallUtil。
你可以在 CSIEExplorerBarSetup(x64)项目文件夹中找到这个文件。若要配置该脚本以使它运行在后期生成事件中，
你可以在解决方案资源管理器中点击CSIEExplorerBarSetup(x64)项目，并在属性窗口中找到PostBuildEvent 属性，将其
值指定为：  
	"$(ProjectDir)Fix64bitInstallUtilLib.js" "$(BuiltOuputPath)" "$(ProjectDir)"

  重复在（2）中步骤，设置自定义操作，配置先决条件并生成安装项目。


/////////////////////////////////////////////////////////////////////////////
诊断：

若要调试浏览器栏，你可以附加到iexplorer.exe。


///////////////////////////////////////////////////////////////////////////// 
 
参考文献：

创建自定义浏览器栏，工具带和桌面带
http://msdn.microsoft.com/en-us/library/bb776819(VS.85).aspx

添加浏览器栏
http://msdn.microsoft.com/en-us/library/aa753590(VS.85).aspx

/////////////////////////////////////////////////////////////////////////////

