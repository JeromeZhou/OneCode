/****************************** ģ��ͷ ******************************\
* ģ����:	FadeTransition.h
* ��Ŀ��: NativeVideoEncoder
* ��Ȩ (c) Microsoft Corporation.
* 
* ʵ�ּ򵥵ĵ��뵭����Ч.
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
#include "transitionbase.h"

class FadeTransition :
	public TransitionBase
{
public:
	FadeTransition(void);
	~FadeTransition(void);

	BYTE* GetOutputFrame(float time);
};

