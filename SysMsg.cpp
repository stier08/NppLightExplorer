//this file is part of notepad++
//Copyright (C)2003 Don HO ( donho@altern.org )
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#include "stdafx.h"

#include "SysMsg.h"
#include <memory>
#include <string>
#include <algorithm>

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

#pragma warning ( disable : 4996 )

void systemMessage(const TCHAR *title)
{
  LPVOID lpMsgBuf;
  TCHAR	 message[1024];


  FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                 NULL,
				 ::GetLastError(),
                 MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
                 (LPTSTR) &lpMsgBuf,
                 0,
                 NULL );// Process any inserts in lpMsgBuf.

  _stprintf(message, _T("%s\r\n\r\n%s"), title, lpMsgBuf);

  ::MessageBox(NULL, message, _T("Light Explorer"), MB_OK | MB_ICONSTOP);
  ::LocalFree(lpMsgBuf);
}

void systemMessageEx(const TCHAR *title, const TCHAR *fileName, int lineNumber)
{
	try {
		LPVOID lpMsgBuf;
		TCHAR	 message[1024];
		FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
						 NULL,
						 ::GetLastError(),
						 MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
						 (LPTSTR) &lpMsgBuf,
						 0,
						 NULL );// Process any inserts in lpMsgBuf.

		_stprintf(message, _T("%s\r\n\r\n%s (%d)\r\n\r\n%s"), title, fileName, lineNumber, lpMsgBuf);

		::MessageBox(NULL, message, _T("Light Explorer"), MB_OK | MB_ICONSTOP);
		::LocalFree(lpMsgBuf);
	}
	catch (...) {
		systemMessage(title);
	}
}

void printInt(int int2print)
{
	TCHAR str[32];
	_itot(int2print, str, 10);
	::MessageBox(NULL, str, _T(""), MB_OK);
}

void printStr(const TCHAR *str2print)
{
	::MessageBox(NULL, str2print, _T(""), MB_OK);
}
