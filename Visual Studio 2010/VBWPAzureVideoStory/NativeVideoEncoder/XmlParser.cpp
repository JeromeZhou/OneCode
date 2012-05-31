/*********************************** ģ��ͷ ***********************************\
* ģ����:  XmlParser.cpp
* ��Ŀ��:  NativeVideoEncoder
* ��Ȩ (c) Microsoft Corporation.
* 
* �򵥵� XML ������.��֧�ָ߼��Ĺ��ܣ����������ռ�ͼܹ���֤��.
* 
* This source is subject to the Microsoft Public License.
* See http://www.microsoft.com/opensource/licenses.mspx#Ms-PL.
* All other rights reserved.
* 
* THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, 
* EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
* WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
\***************************************************************************/

#include "StdAfx.h"
#include "XmlParser.h"
#include "codecvt" 
#include "cstdlib"
#include "locale" 
#include "fstream"
#include "ErrorCodes.h"

using namespace std;

XmlParser::XmlParser() :
	m_fileContent(nullptr),
	m_fileSize(0),
	m_currentChar(1),
	m_currentElementName(),
	m_isSelfCloseTag(false)
{
	this->m_attributes = map<wstring, wstring>();
}

XmlParser::~XmlParser(void)
{
	if (this->m_fileContent != nullptr)
	{
		delete this->m_fileContent;
	}
	this->m_attributes.clear();
}

// ���� XmlParser������ȡԴ�ļ�.
// ���ǻ���ļ�ִ��2�� I/O����.
// ��һ��ʹ�� Win32 API ��ȡ�ļ����ȣ�ʹ���ǿ��Է��仺����.
// �ڶ���ʹ�� c++ STL ��ȡ���ļ�.
// ʹ�� Win32 API ���԰������ǻ�ȡ�ļ����ԣ�
// �� STL ���԰������ǽ� utf-8 �ļ�ת��Ϊ���ַ�.
// ����Ӱ����С��
// ��Ϊ Win32 API ֻ�Ƕ�ȡ�ļ�����û�ж�ȡ�ļ�����.
HRESULT XmlParser::Create(XmlParser** parser, wstring fileName)
{
	HRESULT hr = S_OK;

	*parser = new XmlParser();

	// ���ļ�.
	HANDLE hFileHandle = CreateFile(
		fileName.c_str(),
		GENERIC_READ,
		FILE_SHARE_READ,
		nullptr,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		nullptr);

	if (hFileHandle == INVALID_HANDLE_VALUE)
	{
		return ERROR_FILE_INVALID;
	}

	// ��ȡ�ļ���С.
	(*parser)->m_fileSize = GetFileSize(hFileHandle, nullptr);
	if ((*parser)->m_fileSize == INVALID_FILE_SIZE)
	{
		return INVALID_FILE_SIZE;
	}

	// �رո��ļ�.���ǽ�ʹ�� c++ STL ����ȡ.
	CloseHandle(hFileHandle);

	(*parser)->m_fileContent = new wchar_t[(*parser)->m_fileSize];
	for (int i = 0; i < (*parser)->m_fileSize; i++)
	{
		(*parser)->m_fileContent[i] = 0;
	}
	
	// ��ʹ�� STL�򿪸��ļ�.
	wifstream fileStream;
	fileStream.open(fileName);

	// �� utf-8 ��ת��Ϊ���ַ���.
	codecvt_utf8<wchar_t>* converter = new codecvt_utf8<wchar_t>(); 
	const locale empty_locale = locale::empty(); 
	const locale utf8_locale = locale(empty_locale, converter);
	fileStream.imbue(utf8_locale);

	// ��ȡ�ļ�����.
	fileStream.read((*parser)->m_fileContent, (*parser)->m_fileSize);

	fileStream.close();
	return hr;
}

// ��ȡ xml ����.
// <?xml version="1.0" encoding="utf-8"?>
// �ڴ˰汾�У�����ֻ�������ַ�.
HRESULT XmlParser::ReadDeclaration()
{
	if (this->m_fileContent == nullptr)
	{
		return ERROR_FILE_INVALID;
	}

	//  �ڴ˰汾�У����ǲ����ܶ�Ĵ�����.
	// �����򵥽��ַ��ƶ�����һ�� ' >'.
	while (this->m_fileContent[this->m_currentChar] != '>')
	{
		this->m_currentChar++;
	}
	return S_OK;
}

// ȡ��ǰ�ڵ㲢����ȡ���ƽ�����һ���ڵ�.
HRESULT XmlParser::ReadStartElement()
{
	if (this->m_fileContent == nullptr)
	{
		return ERROR_FILE_INVALID;
	}

	this->SaveState();

	// ����ֱ����������һ����ʼԪ�� .
	while (this->m_fileContent[this->m_currentChar] != '<')
	{
		this->m_currentChar++;
	}

	// ������Ҫ���� ' <'���������ǿ��Կ�ʼʹ�ø�Ԫ�ص�����.
	this->m_currentChar++;
	
	// ���������Ľ���Ԫ�أ����������޷���ȡ��ʼԪ��.
	if (this->m_fileContent[this->m_currentChar] == '/')
	{
		this->RollbackState();
		return E_NOTSTARTELEMENT;
	}

	bool spaceFound = false;
	wstring attributeName = L"";
	wstring attributeValue = L"";
	bool quoteFound = false;

	while (this->m_fileContent[this->m_currentChar] != '>')
	{
		// δ���ֿո�.
		if (!spaceFound)
		{
			// ��һ���ո�ָʾԪ�����ƽ���.
			if (this->m_fileContent[this->m_currentChar] == ' ')
			{
				spaceFound = true;
			}
			else
			{
				this->m_currentElementName.append(1, this->m_fileContent[this->m_currentChar]);
			}
		}
		// ����ȥ���ԡ����Ǽٶ� xml �ļ���ʽ���õģ�
		// ��˿ո��ʾ����һ����������.
		else
		{			
			if (this->m_fileContent[this->m_currentChar] == '\"')
			{
				// ����ֵ�Ŀ�ʼ.
				if (!quoteFound)
				{
					quoteFound = true;
				}
				// ����ֵ�Ľ�β������Ե�����ӳ�䣬���ñ���.
				else
				{
					// TODO: c++ ӳ���Լ��Զ�������Ŀ.
					// ����Ŀǰ����Ҫ����ԭʼ���Ե�˳��.
					// �������˳�����Ҫ�����ǿ���Ҫʹ�öԣ�������ӳ��.
					// ���������û�ж���������֧���е���.
					this->m_attributes[attributeName] = attributeValue;
					attributeName.clear();
					attributeValue.clear();
					quoteFound = false;
				}
			}
			else
			{
				// �����Ķ���������.
				if (!quoteFound)
				{
					// ���Ǽٶ� xml ��ʽ��ȷ����˸�Ԫ�ص����Ʋ��ܰ��� '=' �� ' '.
					// ����Ӧ����'"'ǰ��'='.
					// ' ' ���������Խ��з�������������ʱ���ǿ�ʼһ��������.
					// ���������������ҲӦ����.
					if (this->m_fileContent[this->m_currentChar] != '='
						&& this->m_fileContent[this->m_currentChar] != ' ')
					{
						attributeName.append(1, this->m_fileContent[this->m_currentChar]);
					}
				}
				//  ��ȡ����ֵ .
				else
				{
					attributeValue.append(1, this->m_fileContent[this->m_currentChar]);
				}
			}
		}

		this->m_currentChar++;

		// ����Чxml �ļ�.
		// ��Ȼ���Ǽ��� xml �ļ�����Ч�����ǻ�����һЩ�򵥵ļ��.
		if (this->m_currentChar > this->m_fileSize)
		{
			return E_UNEXPECTED;
		}
	}

	// ����ǩ�Ƿ����ҷ��.
	if (this->m_fileContent[this->m_currentChar - 1] == '/')
	{
		this->m_isSelfCloseTag = true;
	}

	// While ѭ��û�ж� ' >'.�������ڶ�.
	this->m_currentChar++;
	return S_OK;
}

// �Ķ�����Ԫ��.
// ��Ϊ���Ǽ�������Ч�� xml �ļ������ǿ��Լ򵥵��ƽ�ָ�뵽'>'.
HRESULT XmlParser::ReadEndElement(void)
{
	if (this->m_fileContent == nullptr)
	{
		return ERROR_FILE_INVALID;
	}

	// ��Ԫ�������ҷ�յģ�������������.
	if (this->m_isSelfCloseTag)
	{
		this->m_isSelfCloseTag = false;
		return S_OK;
	}

	this->SaveState();

	while (this->m_fileContent[this->m_currentChar] != '>')
	{
		if (this->m_fileContent[this->m_currentChar] == '<')
		{
			// ������һ����ʼԪ�أ�������ǲ����Ķ�����Ԫ��.
			if (this->m_fileContent[this->m_currentChar + 1] != '/')
			{
				this->RollbackState();
				return E_NOTENDELEMENT;
			}
		}

		this->m_currentChar++;
		
		
		// ����Чxml �ļ�.
		// ��Ȼ���Ǽ��� xml �ļ�����Ч�����ǻ�����һЩ�򵥵ļ��.
		if (this->m_currentChar > this->m_fileSize)
		{
			return E_UNEXPECTED;
		}
	}

	// �ƽ�ָ����ǰ��'>'.
	this->m_currentChar++;
	return S_OK;
}

// ���浱ǰ��״̬��������������˴������ǿ��Իع�.
// TODO: �����������Կ��ܻ�ǳ���ʱ.
// ��ǰ�汾�����ǵ�ÿ��Ԫ��ֻ�м������ԣ�
// ����Ӧ���Ǵ����ް�.
// ���ڽ��������ǵ���������������û�б�Ҫ���Ƶ���������.
void XmlParser::SaveState()
{
	this->m_parserState.currentChar = this->m_currentChar;
	this->m_parserState.attributes = map<wstring, wstring>(this->m_attributes);
	this->m_attributes.clear();
	this->m_parserState.currentElementName = this->m_currentElementName;
	this->m_currentElementName.clear();
	this->m_parserState.isSelfCloseTag = this->m_isSelfCloseTag;
	this->m_isSelfCloseTag = false;
}

//  �ع���ǰ��״̬ .
void XmlParser::RollbackState()
{
	this->m_currentChar = this->m_parserState.currentChar;
	this->m_attributes = map<wstring, wstring>(this->m_parserState.attributes);
	this->m_currentElementName = this->m_parserState.currentElementName;
	this->m_isSelfCloseTag = this->m_parserState.isSelfCloseTag;
}