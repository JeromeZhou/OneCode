Imports Microsoft.WindowsAzure.Diagnostics
Imports Microsoft.WindowsAzure.ServiceRuntime

Public Class WebRole
    Inherits RoleEntryPoint

    Public Overrides Function OnStart() As Boolean

        DiagnosticMonitor.Start("DiagnosticsConnectionString")

        ' ���ڴ������õı仯��鿴MSDN http://go.microsoft.com/fwlink/?LinkId=166357.
        AddHandler RoleEnvironment.Changing, AddressOf RoleEnvironmentChanging

        Return MyBase.OnStart()

    End Function

    Private Sub RoleEnvironmentChanging(ByVal sender As Object, ByVal e As RoleEnvironmentChangingEventArgs)

        ' �������������ֵ�仯
        If (e.Changes.Any(Function(change) TypeOf change Is RoleEnvironmentConfigurationSettingChange)) Then
            ' ��e.Cancel��Ϊtrue���������ɫʵ��
            e.Cancel = True
        End If

    End Sub

End Class

