/**********************************ģ��ͷ**********************************\
* ģ����:  CppUseADO.cpp
* ��Ŀ��:  CppUseADO
* ��Ȩ (c) Microsoft Corporation.
* 
* CppUseADOʾ����ʾ���������#import��Visual C++��д������ͨ��΢��
* ��ADO��ActiveX Data Objects���������������ݿ⡣��ʾ��չʾ�����ݿ��
* ���Ļ����ṹ������������Դ��ִ��SQL���ʹ��Recordset�����ִ������   
* 
* 
* This source is subject to the Microsoft Public License.
* See http://www.microsoft.com/en-us/openness/licenses.aspx#MPL.
* All other rights reserved.
* 
* THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, 
* EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
* WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
* ***************************************************************************/

#pragma region Includes and Imports
#include "stdafx.h"

#include <atlstr.h>

#import "C:\Program Files\Common Files\System\ADO\msado15.dll" \
	rename("EOF", "EndOfFile")

#include <locale.h>
#pragma endregion


DWORD ReadImage(PCTSTR pszImage, SAFEARRAY FAR **psaChunk);

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale( LC_ALL, "chs" );

	HRESULT hr;
	::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

	ADODB::_ConnectionPtr spConn = NULL;
	ADODB::_RecordsetPtr spRst = NULL;

	try
	{
		/////////////////////////////////////////////////////////////////////
		// ��������Դ
		// 

		_tprintf(_T("�����������ݿ� ...\n"));

		//  ���������ַ��� (������Դ��ʾ�����ݿ�SQLServer2005DB) 
		// 

		// ����1: ʹ�á�Integrated security�� �������ݿ� 
		_bstr_t bstrConn("Provider=SQLOLEDB.1;Integrated Security=SSPI;Data Source=localhost\\SQLEXPRESS;Initial Catalog=SQLServer2005DB;");

		// ������
		hr = spConn.CreateInstance(__uuidof(ADODB::Connection));
		hr = spConn->Open(bstrConn, "","", NULL);

		//// ����2: ��ʹ�á�Integrated security�� �������ݿ�. 
		//_bstr_t bstrUserID("HelloWorld");
		//_bstr_t bstrPassword("111111");
		//_bstr_t bstrConn("Provider=SQLOLEDB.1;Data Source=localhost\\SQLEXPRESS;Initial Catalog=SQLServer2005DB;");
		//// ������
		//hr = spConn.CreateInstance(__uuidof(ADODB::Connection));
		//hr = spConn->Open(bstrConn, bstrUserID, bstrPassword, NULL);
		

		/////////////////////////////////////////////////////////////////////
		// ��д��ִ��ADO����.
		// ������SQLָ��(SELECT/UPDATE/INSERT/DELETE),���ǵ��ô洢���̡�  
		// ������INSERT�����ʾ����
		// 

		_tprintf(_T("����һ����¼����Person��\n"));

		//  1. ����һ���������
		ADODB::_CommandPtr spInsertCmd;
		hr = spInsertCmd.CreateInstance(__uuidof(ADODB::Command));
		
		// 2. �����Ӹ�ֵ���������
		spInsertCmd->ActiveConnection = spConn;	

		// 3.  ���������ı�
		// SQLָ����Ǵ洢������  
		_bstr_t bstrInsertCmd(
			"INSERT INTO Person(LastName, FirstName, EnrollmentDate, Picture)" \
			" VALUES (?, ?, ?, ?)");
		spInsertCmd->CommandText = bstrInsertCmd;

		// 4. ������������
		// ADODB::adCmdText ������ͨ��SQLָ��; 
		// ADODB::adCmdStoredProc ���ڴ洢����.
		spInsertCmd->CommandType = ADODB::adCmdText;

		// 5. ��Ӳ���

		//  LastName (nvarchar(50))���������
		variant_t vtLastName("Ge");
		ADODB::_ParameterPtr spLastNameParam;
		spLastNameParam = spInsertCmd->CreateParameter(
			_bstr_t("LastName"),		// ������
			ADODB::adVarWChar,			// �������� (NVarChar)
			ADODB::adParamInput,		// �������ݷ���
			50,							// ������󳤶� (NVarChar(50)
			vtLastName);				// ����ֵ
		hr = spInsertCmd->Parameters->Append(spLastNameParam);

		//  FirstName (nvarchar(50))���������
		variant_t vtFirstName("Jialiang");
		ADODB::_ParameterPtr spFirstNameParam;
		spFirstNameParam = spInsertCmd->CreateParameter(
			_bstr_t("FirstName"),		//������
			ADODB::adVarWChar,			// �������� (NVarChar)
			ADODB::adParamInput,		// �������ݷ���
			50,							// ������󳤶� (NVarChar(50)
			vtFirstName);				// ����ֵ
		hr = spInsertCmd->Parameters->Append(spFirstNameParam);

		// EnrollmentDate (datetime)���������
		SYSTEMTIME sysNow;
		GetSystemTime(&sysNow);
		double dNow;
		SystemTimeToVariantTime(&sysNow, &dNow);
		variant_t vtEnrollmentDate(dNow, VT_DATE);
		ADODB::_ParameterPtr spEnrollmentParam;
		spEnrollmentParam = spInsertCmd->CreateParameter(
			_bstr_t("EnrollmentDate"),	// ������
			ADODB::adDate,				// ��������(DateTime)
            ADODB::adParamInput,		// �������ݷ���
            -1,							// ������󳤶�(����datetime��Ч)
            vtEnrollmentDate);			// ����ֵ
		hr = spInsertCmd->Parameters->Append(spEnrollmentParam);

		// Image (image)���������

		// ��ͼƬ�ļ�����һ��safe array 
		SAFEARRAY FAR *psaChunk = NULL;
		int cbChunkBytes = ReadImage(_T("MSDN.jpg"), &psaChunk);
		variant_t vtChunk;
		if (cbChunkBytes > 0)	// ���ͼƬ�ɹ���ȡ
		{
			// ��safe array��ֵ��һ��variant����
			vtChunk.vt = VT_ARRAY | VT_UI1;
			// safe array����vtChunk�ͷ�ʱ�ͷţ����û�б�Ҫ����SafeArrayDestroy 
			// ���ͷ�safe array..
			vtChunk.parray = psaChunk;
		}
		else 
		{
			// // ���ò�������󳤶�����Ϊһ����Чֵ
			cbChunkBytes = 1;
			// ���ò���ֵ����ΪDBNull
			vtChunk.ChangeType(VT_NULL);
		}
		ADODB::_ParameterPtr spImageParam;
		spImageParam = spInsertCmd->CreateParameter(
			_bstr_t("Image"),			// ������
			ADODB::adLongVarBinary,		// �������� (Image)
			ADODB::adParamInput,		// �������ݷ���
			cbChunkBytes,				// ��byte�������󳤶�
			vtChunk);					// ����ֵ
		hr = spInsertCmd->Parameters->Append(spImageParam);

		// 6.ִ�и�����
		spInsertCmd->Execute(NULL, NULL, ADODB::adExecuteNoRecords);


		/////////////////////////////////////////////////////////////////////
		// ʹ��Recordset����.
		// http://msdn.microsoft.com/en-us/library/ms681510.aspx
		// Recordset�����ʾһ�ű��м�¼��������ִ�н����ȫ�����ϡ� 
		// ���κ�ʱ��Recordset����ָ�򼯺��еĵ�����¼����������
		// ���κ�ʱ��Recordset����ָ�򼯺��еĵ�����¼���������� 
		// 
		// 

		_tprintf(_T("�оٳ���Person�еļ�¼\n"));

		// 1. ����һ��Recordset����
		hr = spRst.CreateInstance(__uuidof(ADODB::Recordset));
		
		// 2. ��Recordset����
		_bstr_t bstrSelectCmd("SELECT * FROM Person"); // WHERE ...
		hr = spRst->Open(bstrSelectCmd,	// SQL ָ��/ ����ͼ�� /  
										// �洢���̵���/ �ļ���
			spConn.GetInterfacePtr(),	// ���Ӷ���/ �����ַ���
			ADODB::adOpenForwardOnly,	// �α�����. (ֻ���α�)
			ADODB::adLockOptimistic,	// ��������. (��������Update���� 
										// ʱ��������¼��
			ADODB::adCmdText);			// ����һ��������ΪSQL�����
										// �洢���̡�

		// 3. ͨ���α���ǰ�ƶ���ö�ٳ���¼
		spRst->MoveFirst();	// �ƶ���Recordset�еĵ�һ����¼
		while (!spRst->EndOfFile)
		{
			int nPersonId = spRst->Fields->Item["PersonID"]->Value.intVal;
			variant_t vtLastName(spRst->Fields->Item["LastName"]->Value);
			variant_t vtFirstName(spRst->Fields->Item["FirstName"]->Value);
			
			// ���ڱ��ж�����һ���ɿ��ֶΣ���Ҫ���Ը��ֶ�ֵ 
			// �Ƿ�ΪVT_NULL��
			_tprintf(_T("%d\t%s %s\n"), nPersonId,
				vtFirstName.vt == VT_NULL ? _T("(DBNull)") : (PCTSTR)vtFirstName.bstrVal, 
				vtLastName.vt == VT_NULL ? _T("(DBNull)") : (PCTSTR)vtLastName.bstrVal
				);

			// ��ö��Recordset�м�¼ʱ�����µ�ǰ��¼��
			//spRst->Fields->Item["XXXX"]->Value = XXXX
			//spRst->Update(); Or spRst->UpdateBatch(); ѭ����Χ�⡣

			spRst->MoveNext();			// �ƶ�����һ����¼
		}

	}
	catch (_com_error &err)
	{
		_tprintf(_T("Ӧ�ó�����ִ���: %s\n"), 
			err.ErrorMessage());
		_tprintf(_T("���� = %s\n"), (LPCTSTR) err.Description());
	}


	/////////////////////////////////////////////////////////////////////////
	// ���˳�ǰ�������ж���
	// 

	_tprintf(_T("���ڹر����� ...\n"));

	// ���Recordset���ڴ�״̬���رոö���
	if (spRst && spRst->State == ADODB::adStateOpen)
		spRst->Close();

	// ������Ӵ��ڴ�״̬���رո����ӡ�
	if (spConn && spConn->State == ADODB::adStateOpen)
		spConn->Close();

	// ж�ر��߳��е�COM
	::CoUninitialize();

	return 0;
}


/*!
 * \��Ҫ
 * ��ͼƬ�ļ�����safe array.
 * 
 * \���� pszImage
 * ͼƬ�ļ��ĵ�ַ.
 * 
 * \���� ppsaChunk
 * ���safe array.
 * 
 * \����ֵ
 * *ͼƬ�ļ���byte��ֵ.0��ʾʧ��
 */
DWORD ReadImage(PCTSTR pszImage, SAFEARRAY FAR **ppsaChunk)
{
	// ��ͼƬ�ļ�
	HANDLE hImage = CreateFile(pszImage, GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hImage == INVALID_HANDLE_VALUE) 
	{
		_tprintf(_T("�޷���ͼƬ�ļ� %s\n"), pszImage);
		return 0;
	}

	// �õ��ļ���byte��ʾ�Ĵ�С��
	LARGE_INTEGER liSize;
	if (!GetFileSizeEx(hImage, &liSize))
	{
		_tprintf(_T("�޷��õ�ͼƬ�Ĵ�С w/err 0x%08lx\n"), 
			GetLastError());
		CloseHandle(hImage);
		return 0;
	}
	if (liSize.HighPart != 0)
	{
		_tprintf(_T("ͼƬ̫��\n"));
		CloseHandle(hImage);
		return 0;
	}
	DWORD dwSize = liSize.LowPart, dwBytesRead;

	// ͨ�� cbChunkBytesԪ������һ��safe array
	*ppsaChunk = SafeArrayCreateVector(VT_UI1, 1, dwSize);

	// ��ʼ��safe array�е�����
	BYTE *pbChunk;
	SafeArrayAccessData(*ppsaChunk, (void **)&pbChunk);

	// ��ȡͼƬ�ļ�
	if (!ReadFile(hImage, pbChunk, dwSize, &dwBytesRead, NULL) 
		|| dwBytesRead == 0 || dwBytesRead != dwSize)
	{
		_tprintf(_T("�޷��Ӹ��ļ��ж�ȡ w/err 0x%08lx\n"),
			GetLastError());
		CloseHandle(hImage);
		// �ͷ� safe array
		SafeArrayUnaccessData(*ppsaChunk);
		SafeArrayDestroy(*ppsaChunk);
        return 0;
	}

	SafeArrayUnaccessData(*ppsaChunk);

	CloseHandle(hImage);

	return dwSize;
}