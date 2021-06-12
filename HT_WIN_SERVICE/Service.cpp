#pragma once
#include <Windows.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#pragma comment(lib, "..\\Debug\\HT_COM_STATIC_LIB.lib")
#include "../HT_COM_STATIC_LIB/HT_COM_STATIC_LIB.h"
#include "Service.h"

using namespace std;

WCHAR ServiceName[] = SERVICENAME;
SERVICE_STATUS_HANDLE hServiceStatusHandle;
SERVICE_STATUS ServiceStatus;

void trace(const char* msg, int r) {
	std::ofstream out;
	out.open(TRACEPATH, r);
	out << msg << "\n";
	out.close();
}
void trace(const wchar_t* msg, int r) {
	std::wofstream out;
	out.open(TRACEPATH, r);
	out << msg << "\n";
	out.close();
}

HT::HTHANDLE* ht = NULL;

VOID WINAPI ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv) {
	trace("\n==========ServiceMain==========\n");
	char temp[121];
	wchar_t wtemp[121];

	ServiceStatus.dwServiceType = SERVICE_WIN32;
	ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
	ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_PAUSE_CONTINUE;
	ServiceStatus.dwWin32ExitCode = 0;
	ServiceStatus.dwServiceSpecificExitCode = 0;
	ServiceStatus.dwCheckPoint = 0;
	ServiceStatus.dwWaitHint = 0;

	if (!(hServiceStatusHandle = RegisterServiceCtrlHandler(ServiceName, ServiceHandler))) {
		sprintf_s(temp, "\nSetServiceStatusFailed, error code = %d\n", GetLastError());
		trace(temp);
	}
	else {
		ServiceStatus.dwCurrentState = SERVICE_RUNNING;
		ServiceStatus.dwCheckPoint = 0;
		ServiceStatus.dwWaitHint = 0;
		if (!SetServiceStatus(hServiceStatusHandle, &ServiceStatus)) {
			sprintf_s(temp, "\nSetSetviceStatus failed, error code =%d\n", GetLastError());
			trace(temp);
		}
		else {
			trace("==START SERVICE==", std::ofstream::out);
			try
			{
				const wchar_t* fileName = L"D:\\OS\\MAP.ht";
				const wchar_t* uGroup = L"HT";
				HTAPI::Init();
				ht = HTAPI::Start(L"D:\\OS\\MAP.ht", L"user", L"1111");
				if (ht == NULL) {
					trace("HT OPEN ERROR");
					trace("HT TRY CREATE");

					ht = HTAPI::Create(50, 5, 10, 10,fileName,uGroup);
					if(ht != NULL)trace("HT CREATED");
					if(ht == NULL)trace("HT CREATE ERROR");

					if (ht != NULL) HTAPI::Close(ht);
					ht = HTAPI::Start(fileName,L"user",L"1111");
				}
				if (ht != NULL) {
					sprintf_s(temp, "\nHT-Storage Opened snapshotinterval=%d, capacity = %d, maxkeylength = %d, maxdatalength = %d",
						ht->SecSnapshotInterval, ht->Capacity, ht->MaxKeyLength, ht->MaxPayloadLength);
					trace(temp);
					swprintf_s(wtemp, L"user group:%s\n", ht->HTUsersGroup);
					trace(wtemp);
				}
				else
					trace("SOME ERROR");

				while (ServiceStatus.dwCurrentState == SERVICE_RUNNING) {
					Sleep(10000);
					trace("ALLES GUT...");
				}

				if (ht != NULL) {
					sprintf_s(temp, "\nht:%d", ht);
					trace(temp);
					sprintf_s(temp, "\nHT-Storage Closing... snapshotinterval=%d, capacity = %d, maxkeylength = %d, maxdatalength = %d",
						ht->SecSnapshotInterval, ht->Capacity, ht->MaxKeyLength, ht->MaxPayloadLength);
					HTAPI::Stop(ht);
				}
				else
					trace("HT IS NULL\n");
			}
			catch (char* e) {
				trace(e);
			}
		}
	}
}


VOID WINAPI ServiceHandler(DWORD fdwControl) {
	char temp[121];
	switch (fdwControl) {
	case SERVICE_CONTROL_STOP:
		trace("==SERVICE STOP==\n");
	case SERVICE_CONTROL_SHUTDOWN:
		ServiceStatus.dwWin32ExitCode = 0;
		ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		ServiceStatus.dwCheckPoint = 0;
		ServiceStatus.dwWaitHint = 0;
		trace("\n==SERVICE SHUTDOWN==\n");

		break;
	case SERVICE_CONTROL_PAUSE:
		ServiceStatus.dwCurrentState = SERVICE_PAUSED;
		break;
	case SERVICE_CONTROL_CONTINUE:
		ServiceStatus.dwCurrentState = SERVICE_RUNNING;
		break;
	case SERVICE_CONTROL_INTERROGATE: break;
	default:
		sprintf_s(temp, "Unrecognized opcode%d\n", fdwControl);
		trace(temp);
	};
	if (!SetServiceStatus(hServiceStatusHandle, &ServiceStatus)) {
		sprintf_s(temp, "SetServiceStatus failed, error code = %d\n", GetLastError());
		trace(temp);
	}
}
