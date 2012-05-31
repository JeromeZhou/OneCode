'******************************** ģ��ͷ *************************************************'
' ģ����:  SimulateVal.vb
' ��Ŀ��:  VBDynamicallyBuildLambdaExpressionWithField
' ��Ȩ (c) Microsoft Corporation.
' 
' SimulateVal.vb �ļ������˴���DateTime��Boolean�Ĳ�ͬ����.
'
' This source is subject to the Microsoft Public License.
' See http://www.microsoft.com/opensource/licenses.mspx#Ms-PL.
' All other rights reserved.
'
' THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER 
' EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF 
' MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
'*****************************************************************************************'

Public NotInheritable Class SimulateVal
    Private Sub New()
    End Sub
    Public Shared Function Val(ByVal expression As String) As Double
        If expression Is Nothing Then
            Return 0
        End If
        ' �ȳ��������ַ�����Ȼ���𽥴����С�����ַ�����
        ' �Ա�ģ��VB�С�Val������Ϊ.
        ' ��������һ����ʶ��ֵ�����β���ַ�.
        For size As Integer = expression.Length To 1 Step -1
            Dim testDouble As Double
            If Double.TryParse(expression.Substring(0, size), testDouble) Then
                Return testDouble
            End If
        Next size

        ' û��ʶ���κ�ֵ�����Է���0.
        Return 0
    End Function

    Public Shared Function Val(ByVal expression As Object) As Double
        If expression Is Nothing Then
            Return 0
        End If

        Dim testDouble As Double
        If Double.TryParse(expression.ToString(), testDouble) Then
            Return testDouble
        End If

        Dim testBool As Boolean

        If Boolean.TryParse(expression.ToString(), testBool) Then
            Return If(testBool, -1, 0)
        End If

        Dim testDate As Date
        If Date.TryParse(expression.ToString(), testDate) Then
            Return testDate.Day
        End If
        ' û��ʶ���κ�ֵ�����Է���0.
        Return 0

    End Function

    ''' <summary>
    ''' ���ַ�ת��Ϊ�ַ���.
    ''' </summary>
    Public Shared Function Val(ByVal expression As Char) As Integer
        Dim testInt As Integer
        If Integer.TryParse(expression.ToString(), testInt) Then
            Return testInt
        Else
            Return 0
        End If
    End Function
End Class