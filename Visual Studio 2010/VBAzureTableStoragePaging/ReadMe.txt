﻿========================================================================
    WINDOWSAZURE : VBAzureTableStoragePaging 解决方案概述
========================================================================

/////////////////////////////////////////////////////////////////////////////
用途:

经常有这样的情况,我们想对表存储使用分页.然而基于目前表存储的限制,它仅仅支持
连续的记号,那样帮助我们实现一个简单的前一个(后一个)分页.这个实例阐述了怎样
在一个MVC应用程序中实现前一个(后一个)分页.TableStoragePagingUtility.vb文件中
的类可以被其他应用程序重复利用.如果你想再利用那些代码,你需要做的就是实现自定
义的ICachedDataProvider<T>类去存储TableStoragePagingUtility<T>所需要的数据.

请注意由于表存储的限制我们只能向前读,这就意味着如果我们想读取最新的数据,我们必
须从最开始的地方来开始读.在这个例子中我们只从表存储中获取数据并将它存在缓存中.
所有的数据都是从缓存中读取的.

/////////////////////////////////////////////////////////////////////////////
先决条件:

Microsoft Visual Studio的Windows Azure工具
http://www.microsoft.com/downloads/en/details.aspx?FamilyID=7a1089b6-4050-4307-86c4-9dadaa5ed018

/////////////////////////////////////////////////////////////////////////////
代码测试:

A. 确保CloudService被设置为启动项目.按F5开始调试.
B. 在页面上点击"首先添加数据测试"链接.
C. 观察页面上表的显示.
D. 点击"下一页" 或 "上一页"链接测试分页功能.

/////////////////////////////////////////////////////////////////////////////
相关资料:

查询超时和分页
http://msdn.microsoft.com/zh-cn/library/dd135718.aspx

/////////////////////////////////////////////////////////////////////////////