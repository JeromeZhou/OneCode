using System;
using System.Collections.Generic;
using System.Linq;
using Microsoft.WindowsAzure;
using Microsoft.WindowsAzure.Diagnostics;
using Microsoft.WindowsAzure.ServiceRuntime;

namespace StoryCreatorWebRole
{
    public class WebRole : RoleEntryPoint
    {
        public override bool OnStart()
        {
            // ����AzureLocalStorageTraceListner, ��web.configȡ����ؽڵ� 
            DiagnosticMonitorConfiguration diagnosticConfig = DiagnosticMonitor.GetDefaultInitialConfiguration();
            diagnosticConfig.Directories.ScheduledTransferPeriod = TimeSpan.FromMinutes(1);
            diagnosticConfig.Directories.DataSources.Add(AzureLocalStorageTraceListener.GetLogDirectory());

            return base.OnStart();
        }
    }
}
