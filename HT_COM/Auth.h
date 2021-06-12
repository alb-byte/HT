#include "pch.h"
#include <Windows.h>
#include <iostream>
namespace AUTH {
    bool checkGroupExisting(const wchar_t* groupName);
    bool checkUserGroups(const wchar_t* userName, const wchar_t* groupName);
    bool checkCurrentUserGroup(const wchar_t* groupName);
    bool checkUsersCred(const wchar_t* name, const wchar_t* pass);
}