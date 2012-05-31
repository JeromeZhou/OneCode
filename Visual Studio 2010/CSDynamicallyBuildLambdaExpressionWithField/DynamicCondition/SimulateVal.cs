/********************************** ģ��ͷ *****************************************\
* ģ����:  SimulateVal.cs
* ��Ŀ��:  CSDynamicallyBuildLambdaExpressionWithField
* ��Ȩ (c) Microsoft Corporation.
* 
* SimulateVal.cs �ļ������˴���DateTime��Boolean�Ĳ�ͬ����.
*
*
* This source is subject to the Microsoft Public License.
* See http://www.microsoft.com/opensource/licenses.mspx#Ms-PL.
* All other rights reserved.
*
* THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER 
* EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF 
* MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
\***********************************************************************************/

public static class SimulateVal
{
    public static double Val(string expression)
    {
        if (expression == null)
        {
            return 0;
        }
        // �ȳ��������ַ�����Ȼ���𽥴����С�����ַ�����
        // �Ա�ģ��VB�С�Val������Ϊ.
        // ��������һ����ʶ��ֵ�����β���ַ�.
        for (int size = expression.Length; size > 0; size--)
        {
            double testDouble;
            if (double.TryParse(expression.Substring(0, size), out testDouble))
                return testDouble;
        }

        // û��ʶ���κ�ֵ�����Է���0.
        return 0;
    }

    public static double Val(object expression)
    {
        if (expression == null)
        {
            return 0;
        }

        double testDouble;
        if (double.TryParse(expression.ToString(), out testDouble))
        {
            return testDouble;
        }
        //C#��Val������ true ʱ������-1.
        bool testBool;

        if (bool.TryParse(expression.ToString(), out testBool))
        {
            return testBool ? -1 : 0;
        }
        // C#��Val����Ϊ���ڷ������е���.
        System.DateTime testDate;
        if (System.DateTime.TryParse(expression.ToString(), out testDate))
        {
            return testDate.Day;
        }
        // û��ʶ���κ�ֵ�����Է���0.
        return 0;

    }

    /// <summary>
    /// ���ַ�ת��Ϊ�ַ���.
    /// </summary>
    public static int Val(char expression)
    {
        int testInt;
        if (int.TryParse(expression.ToString(), out testInt))
        {
            return testInt;
        }
        else
        {
            return 0;
        }
    }
}