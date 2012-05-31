/*********************************** ģ��ͷ ***********************************\
* ģ����:  FlyinTransition.cpp
* ��Ŀ��:  NativeVideoEncoder
* ��Ȩ (c) Microsoft Corporation.
* 
* ʵ�ּ򵥵ķ�����Ч.
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
#include "FlyinTransition.h"


FlyinTransition::FlyinTransition(void)
	: m_direction(Left)
{
}


FlyinTransition::~FlyinTransition(void)
{
}

BYTE* FlyinTransition::GetOutputFrame(float time)
{
	if (this->m_backgroundFrame == nullptr || this->m_foregroundFrame == nullptr)
	{
		return nullptr;
	}
	this->m_outputFrame = new BYTE[this->GetFrameSize()];
	for (int y = 0; y < this->m_frameHeight; y++)
	{
		for (int x = 0; x < this->m_frameWidth; x++)
		{
			int startPixel = (y * this->m_frameWidth + x) * 4;
			if (this->m_direction == Left)
			{
				// Ӧʹ�ö�����������䱳��ͼ��.
				int backgroundCoveredPixels = (int)(time * this->m_frameWidth);

				// ���ٱ���ͼ�����������Ҫת��.
				int xTranslated = (int)((1 - time) * this->m_frameWidth);

				// ����ת����Դ�еĿ�ʼ����.
				int startPixelInSource  = (y * this->m_frameWidth + (x + xTranslated)) * 4;

				// Ӧʹ�ñ���ͼ������������.
				if (x < backgroundCoveredPixels)
				{
					// ����ת������ͼ��.
					this->m_outputFrame[startPixel] = this->m_backgroundFrame[startPixelInSource]; //R
					this->m_outputFrame[startPixel + 1] = this->m_backgroundFrame[startPixelInSource + 1]; //G
					this->m_outputFrame[startPixel + 2] = this->m_backgroundFrame[startPixelInSource + 2]; //B
					this->m_outputFrame[startPixel + 3] = 255; //A
				}

				// Ӧʹ��ǰ��ͼ������������.
				else
				{
					// ǰ��ͼ����Ҫת��.
					this->m_outputFrame[startPixel] = this->m_foregroundFrame[startPixel]; //R
					this->m_outputFrame[startPixel + 1] = this->m_foregroundFrame[startPixel + 1]; //G
					this->m_outputFrame[startPixel + 2] = this->m_foregroundFrame[startPixel + 2]; //B
					this->m_outputFrame[startPixel + 3] = 255; //A
				}
			}

			else if (this->m_direction == Right)
			{
				// Ӧʹ�ö������������ǰ��ͼ��.
				int foregroundCoveredPixels = (int)((1 - time) * this->m_frameWidth);

				// ���ٱ���ͼ�����������Ҫת��.
				int xTranslated = (int)((time - 1) * this->m_frameWidth);

				// ����ת����Դ�еĿ�ʼ����.
				int startPixelInSource  = (y * this->m_frameWidth + (x + xTranslated)) * 4;

				// Ӧʹ�ñ���ͼ������������.
				if (x < foregroundCoveredPixels)
				{
					// ǰ��ͼ����Ҫת��.
					this->m_outputFrame[startPixel] = this->m_foregroundFrame[startPixel]; //R
					this->m_outputFrame[startPixel + 1] = this->m_foregroundFrame[startPixel + 1]; //G
					this->m_outputFrame[startPixel + 2] = this->m_foregroundFrame[startPixel + 2]; //B
					this->m_outputFrame[startPixel + 3] = 255; //A
				}

				// Ӧʹ��ǰ��ͼ������������.
				else
				{
					/// ����ת������ͼ��.						
					this->m_outputFrame[startPixel] = this->m_backgroundFrame[startPixelInSource]; //R
					this->m_outputFrame[startPixel + 1] = this->m_backgroundFrame[startPixelInSource + 1]; //G
					this->m_outputFrame[startPixel + 2] = this->m_backgroundFrame[startPixelInSource + 2]; //B
					this->m_outputFrame[startPixel + 3] = 255; //A
				}
			}

			else if (this->m_direction == Up)
			{
				// Ӧʹ�ö�����������䱳��ͼ��.
				int backgroundCoveredPixels = (int)(time * this->m_frameHeight);

				// ���ٱ���ͼ�����������Ҫת��.
				int yTranslated = (int)((1 - time) * this->m_frameHeight);

				// ����ת����Դ�еĿ�ʼ����.
				int startPixelInSource  = ((y + yTranslated) * this->m_frameWidth + x) * 4;

				// Ӧʹ�ñ���ͼ������������.
				if (y < backgroundCoveredPixels)
				{
					// ����ת������ͼ��.
					this->m_outputFrame[startPixel] = this->m_backgroundFrame[startPixelInSource]; //R
					this->m_outputFrame[startPixel + 1] = this->m_backgroundFrame[startPixelInSource + 1]; //G
					this->m_outputFrame[startPixel + 2] = this->m_backgroundFrame[startPixelInSource + 2]; //B
					this->m_outputFrame[startPixel + 3] = 255; //A
				}

				// Ӧʹ��ǰ��ͼ������������.
				else
				{
					// ǰ��ͼ����Ҫת��.
					this->m_outputFrame[startPixel] = this->m_foregroundFrame[startPixel]; //R
					this->m_outputFrame[startPixel + 1] = this->m_foregroundFrame[startPixel + 1]; //G
					this->m_outputFrame[startPixel + 2] = this->m_foregroundFrame[startPixel + 2]; //B
					this->m_outputFrame[startPixel + 3] = 255; //A
				}
			}

			else if (this->m_direction == Down)
			{
				// Ӧʹ�ö������������ǰ��ͼ��.
				int foregroundCoveredPixels = (int)((1 - time) * this->m_frameHeight);

				// ���ٱ���ͼ�����������Ҫת��.
				int yTranslated = (int)((time - 1) * this->m_frameHeight);

				// ����ת����Դ�еĿ�ʼ����.
				int startPixelInSource  = ((y + yTranslated) * this->m_frameWidth + x) * 4;

				// Ӧʹ�ñ���ͼ������������.
				if (y < foregroundCoveredPixels)
				{
					// ǰ��ͼ����Ҫת��.
					this->m_outputFrame[startPixel] = this->m_foregroundFrame[startPixel]; //R
					this->m_outputFrame[startPixel + 1] = this->m_foregroundFrame[startPixel + 1]; //G
					this->m_outputFrame[startPixel + 2] = this->m_foregroundFrame[startPixel + 2]; //B
					this->m_outputFrame[startPixel + 3] = 255; //A
				}

				// Ӧʹ��ǰ��ͼ������������.
				else
				{
					/// ����ת������ͼ��.				
					this->m_outputFrame[startPixel] = this->m_backgroundFrame[startPixelInSource]; //R
					this->m_outputFrame[startPixel + 1] = this->m_backgroundFrame[startPixelInSource + 1]; //G
					this->m_outputFrame[startPixel + 2] = this->m_backgroundFrame[startPixelInSource + 2]; //B
					this->m_outputFrame[startPixel + 3] = 255; //A
				}
			}
		}
	}
	return this->m_outputFrame;
}

void FlyinTransition::ParseXml(XmlParser* pParser)
{
	wstring direction = pParser->GetAttributes()[L"Direction"];
	if (direction.compare(L"Left") == 0)
	{
		this->m_direction = Left;
	}
	if (direction.compare(L"Right") == 0)
	{
		this->m_direction = Right;
	}
	if (direction.compare(L"Up") == 0)
	{
		this->m_direction = Up;
	}
	if (direction.compare(L"Down") == 0)
	{
		this->m_direction = Down;
	}
}