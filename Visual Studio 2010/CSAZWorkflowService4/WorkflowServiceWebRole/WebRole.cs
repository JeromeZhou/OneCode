/****************************** ģ��ͷ *************************************\
* Module Name:	WebRole.cs
* Project:		Client
* Copyright (c) Microsoft Corporation.
* 
* ����ĿWebRole��Ŀ�������й�WCF Workflow Service.
* 
* This source is subject to the Microsoft Public License.
* See http://www.microsoft.com/opensource/licenses.mspx#Ms-PL.
* All other rights reserved.
* 
* THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, 
* EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
* WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
\***************************************************************************/

using System;
using System.Collections.Generic;
using System.Linq;
using Microsoft.WindowsAzure;
using Microsoft.WindowsAzure.Diagnostics;
using Microsoft.WindowsAzure.ServiceRuntime;

namespace WorkflowServiceWebRole
{
    public class WebRole : RoleEntryPoint
    {
        public override bool OnStart()
        {
            // ���ڴ������ñ仯����Ϣ
            // �����MSDN�� http://go.microsoft.com/fwlink/?LinkId=166357.

            return base.OnStart();
        }
    }
}
