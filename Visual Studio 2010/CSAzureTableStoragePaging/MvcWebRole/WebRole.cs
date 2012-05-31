using System.Linq;
using Microsoft.WindowsAzure.Diagnostics;
using Microsoft.WindowsAzure.ServiceRuntime;

namespace MvcWebRole
{
    public class WebRole : RoleEntryPoint
    {
        public override bool OnStart()
        {
            DiagnosticMonitor.Start("DiagnosticsConnectionString");

            // ���ڴ������õı仯��鿴MSDN http://go.microsoft.com/fwlink/?LinkId=166357.
            RoleEnvironment.Changing += RoleEnvironmentChanging;

            return base.OnStart();
        }

        private void RoleEnvironmentChanging(object sender, RoleEnvironmentChangingEventArgs e)
        {
            // �������������ֵ�仯
            if (e.Changes.Any(change => change is RoleEnvironmentConfigurationSettingChange))
            {
                // ��e.Cancel��Ϊtrue���������ɫʵ��
                e.Cancel = true;
            }
        }
    }
}
