======================================================================================
      Windows Ӧ�ó���: VBDynamicallyBuildLambdaExpressionWithField ����                        
======================================================================================

//////////////////////////////////////////////////////////////////////////////////////
ժҪ:  

���ʵ����ʾ����ζ�̬����lambda���ʽ,�ͽ�������ʾ�� DataGridView �ؼ���.

���ʵ����ʾ����ν��������������һ�𣬲���̬����LINQ��SQL. LINQ�Ƿǳ��õķ���������
���Ͱ�ȫ��ֱ�ۡ����ױ��ֵķ�ʽ���������������Ͳ�ѯ����. ���磬��Ӧ��
�����е��������ܣ������ÿͻ��ҵ���������������������һ�м�¼.
 
   
//////////////////////////////////////////////////////////////////////////////////////
��ʾ:

����1. ��Visual Studio 2010�����������Ŀ.
 
����2. ����������վ����ʾ������ SQL2000SampleDb.msi:
       http://www.microsoft.com/downloads/en/details.aspx?FamilyID=06616212-0356-46a0-8da2-eebc53a68034&displaylang=en
      
����3. ������װ������ϵͳĿ¼������SQL Server Management Studio.

����4. �Ҽ�����д��"Databases"�����ڵ㣬Ȼ���������"Attach...".

����5. ��ȷ��Northwind.MDF ��Northwind.LDF�Ѱ�װ��ϵͳĿ¼�в��Ҷ���������ȫ�ķ��ʿ���Ȩ.
       �����������£��ڡ�Attach Databases���Ի����ڣ�ѡ��"Add..."��ť���Ը���Northwind��
	   �ݿ�.
	  
����6. �� VBDynamicallyBuildLambdaExpressionWithField ��Ŀ�ѡ�� app.config�����޸�����
       �ַ���������˫�� VBDynamicallyBuildLambdaExpressionWithField ��Ŀ�е� 
	   Settings.settings���޸���Ϊ��Value�����е�ֵ.
	
����7. �һ� VBDynamicallyBuildLambdaExpressionWithField ��Ŀ�����"Set as StartUp Project"
       �˵���.

����8. ��F5��ݼ��������򣬲�ѡ�������ֶΡ� ���������������ֵ.

����9. �������������ť���鿴��LINQ��SQL�����Ľ��.


//////////////////////////////////////////////////////////////////////////////////////
�����߼�:
1. ��������Ҫ����: Condition, Condition(Of T), and Condition(Of T,S)

   a. Condition ��һ��������,���ڹ��췺�Ͱ汾.ͨ�������������ǿ��Ի�÷������Ͳ�������
   �ĺô��������������ǲ���Ϊ���ݷ������Ͳ������������ģ�����������Ϊ�����Ƶ�����.

   b. Condition(Of T) �����������������ӵ�һ��. T ��Ԫ�����ͣ������������ʾ���е�Order).

   c. Condition(Of T,S) ����򵥵���; ����ʾ��һ��"object.propery<comparison> value"���
   ʽ.���Ͳ��� S �����ƶ�Ϊ����ֵ������(����String,Date,Boolean...).

2. ���ڱ���ִ�еĲ�ѯ,���ǽ�Lambda���ʽ���뵽һ��ί���У��Ա����ڴ���ִ����. �û�����ͨ
   ������ Matches ������������ί��.
   
  	 ' �� lambda ���ʽ���뵽һ��ί����.
  	del = CType(LambdaExpr.Compile(), Func(Of T, Boolean))

3. �ײ�����չ��������Ϊ����Զ��ִ�е� IQueryable(Of T)�������ڱ���ִ�е� IEnumerable(Of T).


//////////////////////////////////////////////////////////////////////////////////////
References:

Implementing Dynamic Searching Using LINQ 
http://blogs.msdn.com/b/vbteam/archive/2007/08/29/implementing-dynamic-searching-using-linq.aspx

LINQ to SQL (Part 9 - Using a Custom LINQ Expression with the <asp:LinqDatasource> control) 
http://weblogs.asp.net/scottgu/archive/2007/09/07/linq-to-sql-part-9-using-a-custom-linq-expression-with-the-lt-asp-linqdatasource-gt-control.aspx


//////////////////////////////////////////////////////////////////////////////////////
