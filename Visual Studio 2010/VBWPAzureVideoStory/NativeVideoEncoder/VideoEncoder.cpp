/*********************************** ģ��ͷ ***********************************\
* ģ����:  VideoEncoder.cpp
* ��Ŀ��:  NativeVideoEncoder
* ��Ȩ (c) Microsoft Corporation.
* 
* ����Ƶ���б�������� .
* 
* This source is subject to the Microsoft Public License.
* See http://www.microsoft.com/opensource/licenses.mspx#Ms-PL.
* All other rights reserved.
* 
* THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, 
* EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
* WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
\***************************************************************************/

#pragma once
#include "StdAfx.h"
#include "VideoEncoder.h"
#include "XmlParser.h"
#include "ErrorCodes.h"

VideoEncoder::VideoEncoder(void) :
	m_pIWICFactory(nullptr),
	m_COMInitializedInDll(false),
	m_inputFile(L""),
	m_outputFile(L""),
	m_outputVideoFormat(MFVideoFormat_H264),
	m_frameWidth(800),
	m_frameHeight(600),
	m_videoBitRate(1500000),
	m_fps(30),
	m_pSinkWriter(nullptr)
{
	this->m_photos = list<Photo*>();
	this->m_frameStride = this->m_frameWidth * 4;
	this->m_frameBufferSize = this->m_frameStride * this->m_frameHeight;
}

VideoEncoder::~VideoEncoder(void)
{
	this->Dispose();
}

void VideoEncoder::Dispose()
{
	SafeRelease(&this->m_pIWICFactory);
	if (this->m_COMInitializedInDll)
	{
		CoUninitialize();
		this->m_COMInitializedInDll = false;
	}
	for (list<Photo*>::iterator it = this->m_photos.begin(); it != this->m_photos.end(); it++)
	{
		if (*it != nullptr)
		{
			delete *it;
			*it = nullptr;
		}
	}
	this->m_photos.clear();
	this->m_logFileStream.close();
	SafeRelease(&this->m_pSinkWriter);
}

HRESULT VideoEncoder::Encode()
{
	HRESULT hr = S_OK;
	Photo* pPhoto = nullptr;
	DWORD streamIndex = 0;
	BYTE* pCurrentBitmap = nullptr;
	BYTE* pPreviousBitmap = nullptr;
	BYTE* pFrameBuffer = nullptr;
	LONGLONG startTime = 0;
	UINT64 sampleDuration = 0;
	UINT64 transitionSampleDuration = 0;

	CheckHR(this->Initialize());
	this->m_logFileStream << "�ѳɹ���ʼ�����й���Ƶ������" << endl;
	CheckHR(this->ParseInputXml());
	this->m_logFileStream << "Xml�����ļ������ɹ�." << endl;

	CheckHR(this->CreateSinkWriter(&this->m_pSinkWriter, &streamIndex));
	this->m_logFileStream << "Sink writer�����ɹ�." << endl;

	// ѭ��������Ƭ.
	for (list<Photo*>::iterator it = this->m_photos.begin(); it != this->m_photos.end(); it++)
	{
		pPhoto = *it;
		int frameSize = 0;
		// ����λͼ.
		CheckHR(this->DecodeFrame(pPhoto, &pCurrentBitmap, &frameSize));
		this->m_logFileStream << "��Ƭ " << pPhoto->GetFile().c_str() << " �ɹ�����." << endl;

		// ��д��Чʾ��.
		if (pPreviousBitmap != nullptr)
		{
			transitionSampleDuration = (UINT64)(pPhoto->GetTransitionDuration() * this->m_fps);
			if (pPhoto->GetTransition() != nullptr)
			{
				pPhoto->GetTransition()->SetForegroundFrame(pPreviousBitmap);
				pPhoto->GetTransition()->SetBackgroundFrame(pCurrentBitmap);
				CheckHR(this->WriteTransitionSample(transitionSampleDuration, pPhoto->GetTransition(), streamIndex, &startTime));
				this->m_logFileStream << "��Ƭ " << pPhoto->GetFile().c_str() << " ����Чʾ����д�ɹ�." << endl;
			}
		}

		// ��д��Ƭʾ��.
		sampleDuration = (UINT64)(pPhoto->GetPhotoDuration() * this->m_fps);
		CheckHR(this->WritePhotoSample(sampleDuration, pCurrentBitmap, streamIndex, &startTime));
		this->m_logFileStream << "��Ƭ " << pPhoto->GetFile().c_str() << " �ľ�̬ʾ����д�ɹ�." << endl;

		// �ͷ������Ƭ��������Դ.
		delete pPreviousBitmap;
		pPreviousBitmap = pCurrentBitmap;
	}
	hr = this->m_pSinkWriter->Finalize();
	this->m_logFileStream << "�ύ�ɹ�." << endl;
	CheckHR(hr);
	CheckHR(MFShutdown());

cleanup:
	if (pPreviousBitmap != pCurrentBitmap && pPreviousBitmap != nullptr)
	{
		delete pPreviousBitmap;
		pPreviousBitmap = nullptr;
	}
	if (pCurrentBitmap != nullptr)
	{
		delete pCurrentBitmap;
		pCurrentBitmap = nullptr;
	}
	this->Dispose();
	return hr;
}

HRESULT VideoEncoder::Initialize()
{
	HRESULT hr = S_OK;

	this->m_logFileStream = ofstream(this->m_logFile);

	if (this->m_inputFile == L"")
	{
		this->m_logFileStream << "�����Ӱ�����ļ���Ч" << endl;
		return ERROR_FILE_INVALID;
	}

	// ��ʼ�� COM.
	hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	// COM δ�����ô����ʼ��. ������ǳ�ʼ����.
	if (SUCCEEDED(hr))
	{
		this->m_logFileStream << "COM��ʼ���ɹ�." << endl;
		this->m_COMInitializedInDll = true;
	}

	// COM�����ô����ʼ��.
	else if (hr == RPC_E_CHANGED_MODE || hr == S_FALSE)
	{
		this->m_COMInitializedInDll = false;
	}

	// COM��ʼ��ʧ��.
	else
	{
		return hr;
	}

	// ����WIC ����
	CheckHR(CoCreateInstance(
		CLSID_WICImagingFactory,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&this->m_pIWICFactory)
		));

	// ����Media Foundation.
	CheckHR(MFStartup(MF_VERSION));
		
cleanup:
	if (!SUCCEEDED(hr))
	{
		DWORD error = GetLastError();
		this->m_logFileStream << "�������: " << error << endl;
	}
	return hr;
}

// ����sink writer. ����������.
HRESULT VideoEncoder::CreateSinkWriter(IMFSinkWriter** ppSinkWriter, DWORD* pStreamIndex)
{
	HRESULT hr = S_OK;
	if (this->m_outputFile == L"")
	{
		return ERROR_FILE_INVALID;
	}

	// ����sink writer.
	*ppSinkWriter = nullptr;	
	IMFSinkWriter *pSinkWriter = nullptr;
	IMFMediaType* pOutputMediaType = nullptr;
	IMFMediaType *pInMediaType = nullptr;   
	CheckHR(MFCreateSinkWriterFromURL(this->m_outputFile.c_str(), nullptr, nullptr, &pSinkWriter));

	// �������������ý������.
	CheckHR(MFCreateMediaType(&pOutputMediaType));
	CheckHR(pOutputMediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video));
	CheckHR(pOutputMediaType->SetGUID(MF_MT_SUBTYPE, this->m_outputVideoFormat));
	CheckHR(pOutputMediaType->SetUINT32(MF_MT_AVG_BITRATE, this->m_videoBitRate));
	CheckHR(pOutputMediaType->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive));

	CheckHR(MFSetAttributeSize(pOutputMediaType, MF_MT_FRAME_SIZE, this->m_frameWidth, this->m_frameHeight));
	CheckHR(MFSetAttributeRatio(pOutputMediaType, MF_MT_FRAME_RATE, (UINT32)this->m_fps, 1));
	CheckHR(MFSetAttributeRatio(pOutputMediaType, MF_MT_PIXEL_ASPECT_RATIO, 1, 1));
	DWORD streamIndex;
	CheckHR(pSinkWriter->AddStream(pOutputMediaType, &streamIndex));

	// ���������ý������.
    CheckHR(MFCreateMediaType(&pInMediaType));   
    CheckHR(pInMediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video));   
    CheckHR(pInMediaType->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_RGB32));     
    CheckHR(pInMediaType->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive)); 

	// ����Ĳ�����Ϣ��Ϊ������������������Ҫ.��ĳЩ���������Ҫ������ H.264.
	// ���������ȥ����������Ϊ��ֵ��H.264 �����µ��ϴ���ͼ��.
	CheckHR(pInMediaType->SetUINT32(MF_MT_DEFAULT_STRIDE, this->m_frameStride));
    CheckHR(MFSetAttributeSize(pInMediaType, MF_MT_FRAME_SIZE, this->m_frameWidth, this->m_frameHeight));
    CheckHR(MFSetAttributeRatio(pInMediaType, MF_MT_FRAME_RATE, (UINT32)this->m_fps, 1));   
    CheckHR(MFSetAttributeRatio(pInMediaType, MF_MT_PIXEL_ASPECT_RATIO, 1, 1));
    CheckHR(pSinkWriter->SetInputMediaType(streamIndex, pInMediaType, nullptr));   

	// ��ʼ��д.
	CheckHR(pSinkWriter->BeginWriting());

	*ppSinkWriter = pSinkWriter;
	(*ppSinkWriter)->AddRef();
	*pStreamIndex = streamIndex;

cleanup:
	if (!SUCCEEDED(hr))
	{
		DWORD error = GetLastError();
		this->m_logFileStream << "�������: " << error << endl;
	}
	SafeRelease(&pSinkWriter);
	SafeRelease(&pOutputMediaType);
	return hr;
}

// ��������xml�ļ�.
// TODO: Ŀǰ����δִ�����ͼ��.
// �����¸��汾�м���.
HRESULT VideoEncoder::ParseInputXml()
{
	HRESULT hr = S_OK;
	wstring photoCountString = wstring();
	int photoCount = 0;
	XmlParser* pXmlParser;

	CheckHR(XmlParser::Create(&pXmlParser, this->m_inputFile));
	CheckHR(pXmlParser->ReadDeclaration());
	CheckHR(pXmlParser->ReadStartElement());
	photoCountString = pXmlParser->GetAttributes()[L"PhotoCount"];
	photoCount = _wtoi(photoCountString.c_str());
	for (int i = 0; i < photoCount; i++)
	{
		CheckHR(pXmlParser->ReadStartElement());
		Photo* photo = new Photo();
		photo->SetFile(pXmlParser->GetAttributes()[L"Name"]);
		photo->SetPhotoDuration(_wtoi(pXmlParser->GetAttributes()[L"PhotoDuration"].c_str()));

	    // ����Ƿ�����Ƭû�а�����Ч.���ȳ��Զ�ȡ����Ԫ�ص�ͼƬ.
		HRESULT hrTemp = pXmlParser->ReadEndElement();

		// E_NOTENDELEMENT ��ζ�����ǲ��ܶ�ȡ������Ϊ����Ԫ�أ���ЧԪ�أ���Ԫ��.
		// ������������������ЧԪ��.
		// �������Ԫ�ص���Ƭ�ѱ��ɹ���ȡ .
		if (hrTemp == E_NOTENDELEMENT)
		{
			CheckHR(pXmlParser->ReadStartElement());
			wstring transitionName = pXmlParser->GetAttributes()[L"Name"];
			photo->SetTransitionName(transitionName);
			photo->SetTransitionDuration(_wtoi(pXmlParser->GetAttributes()[L"Duration"].c_str()));
			TransitionBase* pTransition = nullptr;
			TransitionFactory::CreateTransition(
				transitionName,
				&pTransition,
				this->m_frameWidth,
				this->m_frameHeight);
			if (pTransition != nullptr)
			{
				pTransition->SetFrameWidth(this->m_frameWidth);
				pTransition->SetFrameHeight(this->m_frameHeight);
				pTransition->ParseXml(pXmlParser);
				photo->SetTransition(pTransition);
			}

			// ��ЧԪ�ؽ���.
			CheckHR(pXmlParser->ReadEndElement());

			// ��ƬԪ�ؽ���.
			CheckHR(pXmlParser->ReadEndElement());
		}
		this->m_photos.push_back(photo);
	}

cleanup:
	if (!SUCCEEDED(hr))
	{
		DWORD error = GetLastError();
		this->m_logFileStream << "�������: " << error << endl;
	}
	if (pXmlParser != nullptr)
    {
        delete pXmlParser;
        pXmlParser = nullptr;
    }
	return hr;
}

// ������Ƭ�Ļ���λͼ�ļ���.
HRESULT VideoEncoder::DecodeFrame(Photo* pPhoto, BYTE** ppOutputBitmap, int* pBitmapSize)
{
	HRESULT hr = S_OK;

	IWICBitmapDecoder *pDecoder  = nullptr;
	IWICBitmapFrameDecode *pFrame  = nullptr;
	IWICBitmap* pSourceBitmap = nullptr;
	IWICBitmapLock* pLock = nullptr;
	BYTE* pSourceBuffer = nullptr;
	BYTE* pDestinationBuffer = nullptr;
	UINT pixelWidth;
	UINT pixelHeight;
	WICRect lockRect;

	*ppOutputBitmap = nullptr;
		hr = m_pIWICFactory->CreateDecoderFromFilename(
		pPhoto->GetFile().c_str(),
		nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnDemand,
		&pDecoder
		);
	CheckHR(hr);
	this->m_logFileStream << "WIC�����������ɹ�." << endl;
	GUID containerFormat;
	CheckHR(pDecoder->GetContainerFormat(&containerFormat));

	// ���ǽ�֧��jpg �ļ�.
	if (containerFormat != GUID_ContainerFormatJpeg)
	{
		this->m_logFileStream << "��֧��jpeg�ļ�." << endl;
		return E_NOTSUPPORTEDFORMAT;
	}

	// ���ǽ�֧��jpg �ļ�. ���ֻ��һ��.
	CheckHR(pDecoder->GetFrame(0, &pFrame));

	// TODO: Ŀǰ������Ҫ������Ƭ����ͬ�Ĵ�С.
	// ��������ڽ��������仯���޸Ĵ���.
	pFrame->GetSize(&pixelWidth, &pixelHeight);
	if (pixelWidth != this->m_frameWidth || pixelHeight != this->m_frameHeight)
	{
		this->m_logFileStream << "���е���Ƭ����ʹ�ù̶��Ĵ�С." << endl;
		return E_IMAGESIZEINCORRECT;
	}

	// ����Դλͼ����.
	CheckHR(this->m_pIWICFactory->CreateBitmapFromSource(pFrame, WICBitmapCacheOnLoad, &pSourceBitmap));
	this->m_logFileStream << "λͼ��Դ�����ɹ�." << endl;

	lockRect.X = 0;
	lockRect.Y = 0;
	lockRect.Width = pixelWidth;
	lockRect.Height = pixelHeight;
	CheckHR(pSourceBitmap->Lock(&lockRect, WICBitmapLockWrite, &pLock));
	UINT sourceBufferSize;
	CheckHR(pLock->GetDataPointer(&sourceBufferSize, &pSourceBuffer));

	// Jpg BGR λͼת�� RGBA ��ʽ.
	UINT destinationBufferSize = sourceBufferSize / 3 * 4;
	pDestinationBuffer = new BYTE[destinationBufferSize];
	for (UINT i = 0, j = 0; i < sourceBufferSize; i+=3, j+=4)
	{
		pDestinationBuffer[j] = pSourceBuffer[i]; // R
		pDestinationBuffer[j + 1] = pSourceBuffer[i + 1]; // G
		pDestinationBuffer[j + 2] = pSourceBuffer[i + 2]; // B
		pDestinationBuffer[j + 3] = 255; // A
	}

	*ppOutputBitmap = pDestinationBuffer;
	*pBitmapSize = destinationBufferSize;

cleanup:
	if (!SUCCEEDED(hr))
	{
		DWORD error = GetLastError();
		this->m_logFileStream << "�������: " << error << endl;
		this->m_logFileStream << "HResult��: " << hr << endl;
	}
	if (pSourceBuffer != nullptr)
	{
		// ɾ��pSourceBuffer;
	}
	SafeRelease(&pDecoder);
	SafeRelease(&pFrame);
	SafeRelease(&pSourceBitmap);
	SafeRelease(&pLock);

	return hr;
}

HRESULT VideoEncoder::WritePhotoSample(UINT64 sampleDuration, BYTE* pBitmap, DWORD streamIndex, LONGLONG* startTime)
{
	HRESULT hr = S_OK;
	IMFMediaBuffer* pMediaBuffer = nullptr;
	BYTE* pFrameBuffer = nullptr;
	IMFSample* pSample = nullptr;

	for (DWORD i = 0; i < sampleDuration; i++)
    {
		CheckHR(MFCreateMemoryBuffer(this->m_frameBufferSize, &pMediaBuffer));
		pMediaBuffer->Lock(&pFrameBuffer, nullptr, nullptr);
		CheckHR(MFCopyImage(pFrameBuffer, this->m_frameStride, pBitmap, this->m_frameStride, this->m_frameStride, this->m_frameHeight));
		CheckHR(pMediaBuffer->Unlock());
		CheckHR(pMediaBuffer->SetCurrentLength(this->m_frameBufferSize));
		CheckHR(MFCreateSample(&pSample));
		CheckHR(pSample->AddBuffer(pMediaBuffer));
		CheckHR(pSample->SetSampleTime(*startTime));
		CheckHR(pSample->SetSampleDuration(this->GetFrameDuration()));
		CheckHR(this->m_pSinkWriter->WriteSample(streamIndex, pSample));
		(*startTime) += this->GetFrameDuration();

	    // �ͷ�ʾ����Դ.
		SafeRelease(&pMediaBuffer);
		SafeRelease(&pSample);
	}

cleanup:
	if (!SUCCEEDED(hr))
	{
		DWORD error = GetLastError();
		this->m_logFileStream << "�������: " << error << endl;
	}
	SafeRelease(&pMediaBuffer);
	SafeRelease(&pSample);
	return hr;
}

HRESULT VideoEncoder::WriteTransitionSample(UINT64 sampleDuration, TransitionBase* pTransition, DWORD streamIndex, LONGLONG* startTime)
{
	HRESULT hr = S_OK;
	IMFMediaBuffer* pMediaBuffer = nullptr;
	BYTE* pFrameBuffer = nullptr;
	IMFSample* pSample = nullptr;
	BYTE* pOutputFrame = nullptr;

	for (DWORD i = 0; i < sampleDuration; i++)
    {
		CheckHR(MFCreateMemoryBuffer(this->m_frameBufferSize, &pMediaBuffer));
		pMediaBuffer->Lock(&pFrameBuffer, nullptr, nullptr);
		float time = (float)i / (float)sampleDuration;
		pOutputFrame = pTransition->GetOutputFrame(time);
		CheckHR(MFCopyImage(pFrameBuffer, this->m_frameStride, pOutputFrame, this->m_frameStride, this->m_frameStride, this->m_frameHeight));
		CheckHR(pMediaBuffer->Unlock());
		CheckHR(pMediaBuffer->SetCurrentLength(this->m_frameBufferSize));
		CheckHR(MFCreateSample(&pSample));
		CheckHR(pSample->AddBuffer(pMediaBuffer));
		CheckHR(pSample->SetSampleTime(*startTime));
		CheckHR(pSample->SetSampleDuration(this->GetFrameDuration()));
		CheckHR(this->m_pSinkWriter->WriteSample(streamIndex, pSample));
		(*startTime) += this->GetFrameDuration();

		// �ͷ�ʾ����Դ.
		SafeRelease(&pMediaBuffer);
		SafeRelease(&pSample);
		if (pOutputFrame != nullptr)
		{
			delete pOutputFrame;
			pOutputFrame = nullptr;
		}
	}

cleanup:
	if (!SUCCEEDED(hr))
	{
		DWORD error = GetLastError();
		this->m_logFileStream << "�������: " << error << endl;
	}
	SafeRelease(&pMediaBuffer);
	SafeRelease(&pSample);
	if (pOutputFrame != nullptr)
	{
		delete pOutputFrame;
		pOutputFrame = nullptr;
	}
	return hr;
}