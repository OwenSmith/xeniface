/* Copyright (c) Citrix Systems Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, 
 * with or without modification, are permitted provided 
 * that the following conditions are met:
 *
 * *   Redistributions of source code must retain the above 
 *     copyright notice, this list of conditions and the 
 *     following disclaimer.
 * *   Redistributions in binary form must reproduce the above 
 *     copyright notice, this list of conditions and the 
 *     following disclaimer in the documentation and/or other 
 *     materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF 
 * SUCH DAMAGE.
 */

#ifndef __XENAGENT_SERVICE_H__
#define __XENAGENT_SERVICE_H__

#include <version.h>

#define SVC_NAME "xensvc"
#define SVC_DISPLAYNAME PRODUCT_NAME_STR ## "Interface Service"
#define SVC_DESC "Monitors and provides various metrics to XenStore"

#include "devicelist.h"
#include "xenifacedevice.h"

class CXenAgent : public IDeviceCreator
{
public: // statics
    static void Log(const char* fmt, ...);

    static int ServiceInstall();
    static int ServiceUninstall();
    static int ServiceEntry();

    static void WINAPI ServiceMain(int argc, char** argv);
    static DWORD WINAPI ServiceControlHandlerEx(DWORD, DWORD, LPVOID, LPVOID);

public: // ctor/dtor
    CXenAgent();
    ~CXenAgent();

public: // IDeviceCreator
    virtual CDevice* Create(const wchar_t* path);
    virtual void OnDeviceAdded(CDevice* dev);
    virtual void OnDeviceRemoved(CDevice* dev);

private: // service events
    void OnServiceStart();
    void OnServiceStop();
    void OnDeviceEvent(DWORD, LPVOID);
    bool ServiceMainLoop();

private: // service support
    void SetServiceStatus(DWORD state, DWORD exit = 0, DWORD hint = 0);
    void WINAPI __ServiceMain(int argc, char** argv);
    DWORD WINAPI __ServiceControlHandlerEx(DWORD, DWORD, LPVOID, LPVOID);

    SERVICE_STATUS          m_status;
    SERVICE_STATUS_HANDLE   m_handle;
    HANDLE                  m_evtlog;
    HANDLE                  m_svc_stop;

    CDeviceList             m_devlist;
    CXenIfaceDevice*        m_device;
    CRITICAL_SECTION        m_crit;
};

#endif
