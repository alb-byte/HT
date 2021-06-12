#include "pch.h"
#include "Auth.h"
#include <Windows.h>
#include <iostream>
#include <iostream>
#include <LMaccess.h>
#include <Lm.h>
#include "LOG.h"
#pragma comment (lib,"netapi32.lib")

using namespace std;

bool AUTH::checkGroupExisting(const wchar_t* groupName) {
	GROUP_INFO_0* groupsInfo;
	DWORD readed = 0, total = 0;
	NetLocalGroupEnum(
		NULL, 0,
		(LPBYTE*)&groupsInfo,
		MAX_PREFERRED_LENGTH,
		&readed, &total, NULL
	);
	bool exosts = false;
	for (int i = 0; i < readed; i++) {
		int res = lstrcmpW(groupName, groupsInfo[i].grpi0_name);
		if (res == 0) {
			exosts = true;
			break;
		}
	}
	NetApiBufferFree((LPVOID)groupsInfo);
	return exosts;
}
bool AUTH::checkUserGroups(const wchar_t* userName, const wchar_t* groupName) {
	GROUP_USERS_INFO_0* groupUsersInfo;
	DWORD uc = 0, tc = 0;
	NET_API_STATUS ns = NetUserGetLocalGroups(
		NULL,
		userName,
		0,
		LG_INCLUDE_INDIRECT,
		(LPBYTE*)&groupUsersInfo,
		MAX_PREFERRED_LENGTH,
		&uc,
		&tc
	);
	bool exosts = false;
	if (ns == NERR_Success) {
		for (int i = 0; i < uc; i++) {
			int res = lstrcmpW(groupName, groupUsersInfo[i].grui0_name);
			if (res == 0) {
				exosts = true;
				break;
			}
		}
		NetApiBufferFree((LPVOID)groupUsersInfo);
	}
	return exosts;
}
bool AUTH::checkCurrentUserGroup(const wchar_t* groupName) {
	WCHAR currentUserName[512];
	DWORD lenUserName = 512;
	bool resGetUser = GetUserName(currentUserName, &lenUserName);
	return checkUserGroups(currentUserName, groupName);
}
bool AUTH::checkUsersCred(const wchar_t* name, const wchar_t* pass) {
	wstring ws(name);
	wstring ws2(pass);
	string s1(ws.begin(), ws.end());
	string s2(ws2.begin(), ws2.end());
	Logger::Write(s1);
	Logger::Write(s2);
	bool res;
	HANDLE hToken = 0;
	res = LogonUserW(
		name,
		L".",
		pass,
		LOGON32_LOGON_INTERACTIVE,
		LOGON32_PROVIDER_DEFAULT,
		&hToken
	);
	CloseHandle(hToken);
	if (res) Logger::Write("SUCCESS LOGIN");
	return res;
}

