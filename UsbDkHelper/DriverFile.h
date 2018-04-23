/**********************************************************************
* Copyright (c) 2013-2014  Red Hat, Inc.
*
* Developed by Daynix Computing LTD.
*
* Authors:
*     Dmitry Fleytman <dmitry@daynix.com>
*     Pavel Gurvich <pavel@daynix.com>
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
**********************************************************************/

#pragma once

#include "UsbDkData.h"

#define DRIVER_FILE_EXCEPTION_STRING TEXT("Driver file operation error. ")

class MutexT {
public:
	MutexT() {
		_usbdk_mutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, TEXT("usbdk_mutex"));
		if (_usbdk_mutex == NULL)
			_usbdk_mutex = CreateMutex(NULL, FALSE, TEXT("usbdk_mutex"));
	}
	~MutexT() {
		CloseHandle(_usbdk_mutex);
	}
	void lock() {
		WaitForSingleObject(_usbdk_mutex, INFINITE);
	}
	void unlock() {
		ReleaseMutex(_usbdk_mutex);
	}
private:
	void* _usbdk_mutex;
	// no copy
	MutexT(const MutexT&);
	void operator=(const MutexT&);
};

class UsbDkDriverFileException : public UsbDkW32ErrorException
{
public:
    UsbDkDriverFileException() : UsbDkW32ErrorException(DRIVER_FILE_EXCEPTION_STRING){}
    UsbDkDriverFileException(LPCTSTR lpzMessage) : UsbDkW32ErrorException(tstring(DRIVER_FILE_EXCEPTION_STRING) + lpzMessage){}
    UsbDkDriverFileException(LPCTSTR lpzMessage, DWORD dwErrorCode) : UsbDkW32ErrorException(tstring(DRIVER_FILE_EXCEPTION_STRING) + lpzMessage, dwErrorCode){}
    UsbDkDriverFileException(tstring errMsg) : UsbDkW32ErrorException(tstring(DRIVER_FILE_EXCEPTION_STRING) + errMsg){}
    UsbDkDriverFileException(tstring errMsg, DWORD dwErrorCode) : UsbDkW32ErrorException(tstring(DRIVER_FILE_EXCEPTION_STRING) + errMsg, dwErrorCode){}
};
class UsbDkDriverFile
{
public:
    UsbDkDriverFile(LPCTSTR lpFileName, bool bOverlapped = false, bool bLocked = false);
    UsbDkDriverFile(HANDLE ObjectHandle, bool bOverlapped = false)
                    : m_bOverlapped(bOverlapped), m_hDriver(ObjectHandle)
    {}
	virtual ~UsbDkDriverFile();

    TransferResult Ioctl(DWORD Code,
               bool ShortBufferOk = false,
               LPVOID InBuffer = nullptr,
               DWORD InBufferSize = 0,
               LPVOID OutBuffer = nullptr,
               DWORD OutBufferSize = 0,
               LPDWORD BytesReturned = nullptr,
               LPOVERLAPPED Overlapped = nullptr);

    TransferResult Read(LPVOID Buffer,
              DWORD BufferSize,
              LPDWORD BytesRead,
              LPOVERLAPPED Overlapped = nullptr);

    TransferResult Write(LPVOID Buffer,
               DWORD BufferSize,
               LPDWORD BytesWritten,
               LPOVERLAPPED Overlapped = nullptr);

protected:
    HANDLE m_hDriver;

private:
    bool m_bOverlapped;
	bool m_locked;
	//mutex_t _mutex;
};
