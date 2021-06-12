#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>

#define SERVICENAME L"AHT_SERVICE"
#define SERVICEPATH L"D:\\OS\\lab13\\Debug\\HT_WIN_SERVICE.exe"

char* errortxt(const char* msg, int code) {
	char* buf = new char[512];
	sprintf_s(buf, 512, "%s: error code = %d\n", msg, code);
	return buf;
}

int main()
{
	SC_HANDLE schService = NULL, schSCManager = NULL;
	try {
		schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
		if (!schSCManager) throw errortxt("OpenSCManager", GetLastError());
		else {
			schService = CreateService(
				schSCManager,
				SERVICENAME,SERVICENAME,
				SERVICE_ALL_ACCESS,
				SERVICE_WIN32_OWN_PROCESS,
				SERVICE_AUTO_START,
				SERVICE_ERROR_NORMAL,
				SERVICEPATH,
				NULL,NULL,NULL,
				NULL,NULL
			);
			if (!schService) throw errortxt("CreateService", GetLastError());
		}
	}
	catch (char* txt) { std::cout << txt << "\n"; }
	if (schSCManager) CloseServiceHandle(schSCManager);
	if (schService) CloseServiceHandle(schSCManager);
	system("pause");
	return 0;
}