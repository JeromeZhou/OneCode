/****************************** ģ��ͷ ******************************\
ģ����:  Reg.h
��Ŀ:      CppShellExtPropSheetHandler
��Ȩ(c) Microsoft Corporation.

����ļ�ʵ�����ڽ�����COM�����Shell����ϷŴ�������ע���ע�����ظ�ʹ�õĺ�����

RegisterInprocServer - ��ע�����ע������ڵ����.
UnregisterInprocServer - ��ע�����ע�������ڵ����.
RegisterShellExtDragDropHandler - ע������ϷŴ������.
UnregisterShellExtDragDropHandler - ע������ϷŴ������.

This source is subject to the Microsoft Public License.
See http://www.microsoft.com/opensource/licenses.mspx#Ms-PL.
All other rights reserved.

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, 
EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
\***************************************************************************/

#pragma once

#include <windows.h>


//
//   ����: RegisterInprocServer
//
//   Ŀ��: ��ע�����ע������ڵ����.
//
//   ����:
//   * pszModule - ·����ģ����������

//   * clsid -�������ID
//   * pszFriendlyName -�Ѻõ�����
//   * pszThreadModel - �߳̿�
//
//   ע��: ��ע����д���HKCR\CLSID\{<CLSID>}��ֵ��
// 
//   HKCR
//   {
//      NoRemove CLSID
//      {
//          ForceRemove {<CLSID>} = s '<Friendly Name>'
//          {
//              InprocServer32 = s '%MODULE%'
//              {
//                  val ThreadingModel = s '<Thread Model>'
//              }
//          }
//      }
//   }
//
HRESULT RegisterInprocServer(PCWSTR pszModule, const CLSID& clsid, 
    PCWSTR pszFriendlyName, PCWSTR pszThreadModel);


//
//   ����: UnregisterInprocServer
//
//   Ŀ¼: ��ע�����ע������������.
//
//   ����:
//   * clsid - �������ID��
//
//   ע��: ��ע�����ɾ�� HKCR\CLSID\{<CLSID>}��ֵ��
//
HRESULT UnregisterInprocServer(const CLSID& clsid);


//
//   ����: RegisterShellExtDragDropHandler
//
// Ŀ��: ע������ϷŴ������.
//
//   ����:
//   * clsid -�齨����ID��
//   * pszFriendlyName - �Ѻ�����
//
//   ע��: ��ע����д������м�ֵ��.
//
//   HKCR
//   {
//      NoRemove Directory
//      {
//          NoRemove shellex
//          {
//              NoRemove DragDropHandlers
//              {
//                  {<CLSID>} = s '<Friendly Name>'
//              }
//          }
//      }
//      NoRemove Folder
//      {
//          NoRemove shellex
//          {
//              NoRemove DragDropHandlers
//              {
//                  {<CLSID>} = s '<Friendly Name>'
//              }
//          }
//      }
//      NoRemove Drive
//      {
//          NoRemove shellex
//          {
//              NoRemove DragDropHandlers
//              {
//                  {<CLSID>} = s '<Friendly Name>'
//              }
//          }
//      }
//   }
//
HRESULT RegisterShellExtDragDropHandler(const CLSID& clsid, PCWSTR pszFriendlyName);


//
//  ����: UnregisterShellExtDragDropHandler
//
//   Ŀ��: ע����괦�����.
//
//   ����:
//   * clsid -�������ID��
//
//   ע��: ��������Ƴ�����ע����е�HKCR\Directory\shellex\DragDropHandlers, 
//   HKCR\Folder\shellex\DragDropHandlers 
//   HKCR\Drive\shellex\DragDropHandlers �µ� {<CLSID>}��ֵ��
//
HRESULT UnregisterShellExtDragDropHandler(const CLSID& clsid);