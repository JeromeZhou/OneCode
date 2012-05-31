/*********************************** ģ��ͷ ***********************************\
* ģ����:  TransitionFactory.h
* ��Ŀ��:  NativeVideoEncoder
* ��Ȩ (c) Microsoft Corporation.
* 
* �ṩ��һЩĬ��ʵ�ֵĻ���. ���ڴ�����ЧЧ���Ĺ�����.
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
#include "TransitionFactory.h"
#include "FadeTransition.h"
#include "FlyinTransition.h"

TransitionFactory::TransitionFactory(void)
{
}


TransitionFactory::~TransitionFactory(void)
{
}

//  ��Ϊ c++ ��֧�ַ��䣬���ǲ��ò�ʹ�� if �� ...
void TransitionFactory::CreateTransition(wstring transitionName, TransitionBase** output, int frameWidth, int frameHeight)
{
	*output = nullptr;
	if (transitionName == L"Fade Transition")
	{
		*output = new FadeTransition();
	}
	else if (transitionName == L"Fly In Transition")
	{
		*output = new FlyinTransition();
	}
	// ����Ӹ�����Ч.

	if ((*output) != nullptr)
	{
		(*output)->SetFrameWidth(frameWidth);
		(*output)->SetFrameHeight(frameHeight);
	}
}