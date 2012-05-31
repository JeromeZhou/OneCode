/*********************************** ģ��ͷ ***********************************\
* ģ����:  TransitionFactory.h
* ��Ŀ��:  NativeVideoEncoder
* ��Ȩ (c) Microsoft Corporation.
* 
* �ṩ��һЩĬ��ʵ�ֵĻ���.���ڴ�����ЧЧ���Ĺ�����.
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
#include "string"
#include "TransitionBase.h"

using namespace std;

class TransitionFactory
{
public:
	TransitionFactory(void);
	~TransitionFactory(void);

	static void CreateTransition(wstring transitionName, TransitionBase** output, int frameWidth, int frameHeight);
};

