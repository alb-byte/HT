#include "pch.h"
#include "HT.h"
#include "Auth.h"
#include "LOG.h"

namespace HT {
#pragma region HIDDEN_LAYER_HEADERS
	DWORD WINAPI SnapShotCycle(HTHANDLE* ht);
	wchar_t* GenerateCoreObjectName(const wchar_t* pathToHT,const wchar_t ch);

	BOOL CheckParmLength(HTHANDLE* ht, Element* element);
	BOOL CheckParmLength(HTHANDLE* ht, int payloadLength);
#pragma endregion
#pragma region MEMORY_LAYER_HEADERS
	const int DELETED = -1;
	Element* GetElementFromHT(HTHANDLE* ht, int n);
	BOOL SetElementToHT(HTHANDLE* ht, Element* element, int n);
	BOOL IsDeleted(Element* ptr);
	void SetDeletedFlag(Element* ptr);
	BOOL EqualElementKeys(Element* el1, Element* el2);
	void SetErrorMessage(HTHANDLE* ht, const char* message, int messageLen);
	void UpdateElement(HTHANDLE* ht, Element* el, void* newpayload, int newpayloadlength);
#pragma endregion

#pragma region API_LAYER

	HTHANDLE* Create(
		int	  Capacity,					   // емкость хранилища
		int   SecSnapshotInterval,		   // переодичность сохранения в сек.
		int   MaxKeyLength,                // максимальный размер ключа
		int   MaxPayloadLength,            // максимальный размер данных
		const wchar_t  FileName[512],         // имя файла 
		const wchar_t HTUsersGroup[512]
	) {
		if (!AUTH::checkGroupExisting(HTUsersGroup) ||
			!AUTH::checkCurrentUserGroup(HTUsersGroup) ||
			!AUTH::checkCurrentUserGroup(L"Администраторы")) {
			std::wcout << "401" << std::endl;
			return NULL;
		}
		HANDLE hf = CreateFile(
			FileName,//path
			GENERIC_WRITE | GENERIC_READ,
			NULL,//режим совместного пользования
			NULL,// атрибуты защиты
			CREATE_NEW,// создание 
			FILE_ATTRIBUTE_NORMAL,//атрибуты и флаги
			NULL//файл атрибутов
		);
		if (hf == INVALID_HANDLE_VALUE) return NULL;
		Logger::Write("after create file");

		DWORD HTsize = sizeof(HTHANDLE) + Capacity * (MaxKeyLength + MaxPayloadLength + 2 * sizeof(int));
		HANDLE hm = CreateFileMapping(//создать объект ядра 
			hf,
			NULL,
			PAGE_READWRITE,
			0, HTsize,
			GenerateCoreObjectName(FileName, L'F'));
		if (!hm)return NULL;
		Logger::Write("after create file mapping");

		LPVOID lp = MapViewOfFile(
			hm,
			FILE_MAP_ALL_ACCESS,
			0, 0, 0);
		if (!lp)return NULL;

		ZeroMemory(lp, HTsize);
		HTHANDLE* ht = new(lp) HTHANDLE(
			Capacity,
			SecSnapshotInterval,
			MaxKeyLength,
			MaxPayloadLength,
			FileName);

		ht->File = hf;
		ht->FileMapping = hm;
		ht->Addr = lp;
		ht->lastsnaptime = time(NULL);
		ht->Mutex = CreateMutex(
			NULL,
			FALSE,
			GenerateCoreObjectName(FileName, L'M'));
		memcpy(ht->MutexName, GenerateCoreObjectName(FileName, L'M'), sizeof(GenerateCoreObjectName(FileName, L'M')));
		memcpy(ht->HTUsersGroup, HTUsersGroup, sizeof(HTUsersGroup));

		DWORD SnapShotThread = NULL;
		if (!(ht->SnapshotThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SnapShotCycle, ht, 0, &SnapShotThread))) return NULL;
		return ht;
	}

	HTHANDLE* Start(const wchar_t FileName[512]) {
		HANDLE hf = CreateFile(
			FileName,//path
			GENERIC_WRITE | GENERIC_READ,
			NULL,//режим совместного пользования
			NULL,// атрибуты защиты
			OPEN_EXISTING,//  открытие
			FILE_ATTRIBUTE_NORMAL,//атрибуты и флаги
			NULL//файл атрибутов
		);
		if (hf == INVALID_HANDLE_VALUE) return NULL;
		HANDLE hm = CreateFileMapping(//создать объект ядра 
			hf,
			NULL,
			PAGE_READWRITE,
			0, 0,
			GenerateCoreObjectName(FileName, L'F'));
		DWORD err = _Post_equals_last_error_::GetLastError();
		if (!hm)return NULL;

		LPVOID lp = MapViewOfFile(
			hm,
			FILE_MAP_ALL_ACCESS,
			0, 0, 0);
		if (!lp)return NULL;

		HTHANDLE* ht = (HTHANDLE*)lp;
		if (!AUTH::checkCurrentUserGroup((wchar_t*)ht->HTUsersGroup)) return NULL;
		ht->File = hf;
		ht->FileMapping = hm;
		ht->Addr = lp;
		ht->Mutex = CreateMutex(NULL,FALSE,ht->MutexName);

		DWORD SnapShotThread = NULL;
		if (!(ht->SnapshotThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SnapShotCycle, ht, 0, &SnapShotThread))) return NULL;
		return ht;
	}
	HTHANDLE* Start(const wchar_t FileName[512],
		const wchar_t UserName[256],
		const wchar_t Password[256]) {
		if (!AUTH::checkUsersCred(UserName, Password)) return NULL;
		HANDLE hf = CreateFile(
			FileName,//path
			GENERIC_WRITE | GENERIC_READ,
			NULL,//режим совместного пользования
			NULL,// атрибуты защиты
			OPEN_EXISTING,//  открытие
			FILE_ATTRIBUTE_NORMAL,//атрибуты и флаги
			NULL//файл атрибутов
		);
		if (hf == INVALID_HANDLE_VALUE)return NULL;
		HANDLE hm = CreateFileMapping(//создать объект ядра 
			hf,
			NULL,
			PAGE_READWRITE,
			0, 0,
			GenerateCoreObjectName(FileName, L'F'));
		DWORD err = _Post_equals_last_error_::GetLastError();
		if (!hm)return NULL;
		Logger::Write("after create file mapping OPEN----------------");

		LPVOID lp = MapViewOfFile(
			hm,
			FILE_MAP_ALL_ACCESS,
			0, 0, 0);
		if (!lp)return NULL;

		HTHANDLE* ht = (HTHANDLE*)lp;
		if (!AUTH::checkUserGroups(UserName, (wchar_t*)ht->HTUsersGroup)) return NULL;

		ht->File = hf;
		ht->FileMapping = hm;
		ht->Addr = lp;
		ht->Mutex = CreateMutex(NULL,FALSE,ht->MutexName);

		DWORD SnapShotThread = NULL;
		if (!(ht->SnapshotThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SnapShotCycle, ht, 0, &SnapShotThread))) return NULL;
		return ht;
	}

	BOOL Stop(const HTHANDLE* hthandle) {
		wchar_t filenames[3][15] = { L"HT_INSERT.exe",L"HT_UPDATE.exe",L"HT_DELETE.exe" };

		HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
		PROCESSENTRY32 pEntry;
		pEntry.dwSize = sizeof(pEntry);
		BOOL hRes = Process32First(hSnapShot, &pEntry);
		while (hRes) {
			for (int i = 0; i < 3; i++) {
				if (!wcscmp(filenames[i], pEntry.szExeFile)) {
					HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0, (DWORD)pEntry.th32ProcessID);
					if (hProcess != NULL) {
						TerminateProcess(hProcess, 9);
						CloseHandle(hProcess);
					}
				}
			}
			hRes = Process32Next(hSnapShot, &pEntry);
		}
		CloseHandle(hSnapShot);
		return Close(hthandle);
	}

	HTHANDLE* Open(const wchar_t FileName[512]) {
		HANDLE hm = OpenFileMapping(//создать объект ядра 
			FILE_MAP_ALL_ACCESS,
			FALSE,
			GenerateCoreObjectName(FileName, L'F'));
		DWORD er = _Post_equals_last_error_::GetLastError();
		std::wcout << FileName << std::endl;
		std::cout << er << std::endl;
		std::cout <<"HM "<< hm << std::endl;
		if (!hm)return NULL;

		LPVOID lp = MapViewOfFile(
			hm,
			FILE_MAP_ALL_ACCESS,
			0, 0, 0);
		std::cout << "LP " << lp << std::endl;
		if (!lp)return NULL;

		HTHANDLE* ht = (HTHANDLE*)lp;
		if (!AUTH::checkCurrentUserGroup((wchar_t*)ht->HTUsersGroup)) return NULL;
		ht->elements = (char*)ht + sizeof(HTHANDLE);
		ht->Mutex = CreateMutex(NULL,FALSE, ht->MutexName);
		return ht;
	}

	HTHANDLE* Open(const wchar_t FileName[512],
		const wchar_t UserName[256],
		const wchar_t Password[256]) {

		if (!AUTH::checkUsersCred(UserName, Password)) return NULL;
		HANDLE hm = OpenFileMapping(//создать объект ядра 
			FILE_MAP_ALL_ACCESS,
			FALSE,
			GenerateCoreObjectName(FileName, L'F'));
		DWORD er = _Post_equals_last_error_::GetLastError();
		if (!hm)return NULL;

		LPVOID lp = MapViewOfFile(
			hm,
			FILE_MAP_ALL_ACCESS,
			0, 0, 0);
		if (!lp)return NULL;

		HTHANDLE* ht = (HTHANDLE*)lp;
		if (!AUTH::checkUserGroups(UserName, (wchar_t*)ht->HTUsersGroup)) return NULL;

		ht->elements = (char*)ht + sizeof(HTHANDLE);
		ht->Mutex = CreateMutex(NULL,FALSE, ht->MutexName);
		return ht;
	}

	BOOL Snap(HTHANDLE* hthandle) {
		//WaitForSingleObject(hthandle->Mutex, INFINITE);

		HANDLE hm = CreateMutex(NULL, FALSE, hthandle->MutexName);
		WaitForSingleObject(hm, INFINITE);

		if (!FlushViewOfFile(hthandle->Addr, NULL)) {
			SetErrorMessage(hthandle, "Snapshot error", 15);
			ReleaseMutex(hthandle->Mutex);
			return FALSE;
		}
		std::cout << "-----SNAPSHOT-----" << std::endl;
		Beep(1500, 300);
		hthandle->lastsnaptime = time(NULL);
		ReleaseMutex(hthandle->Mutex);
		return TRUE;
	}

	BOOL Close(const HTHANDLE* hthandle) {
		HANDLE mapping, file, mutex;

		memcpy(&mapping, &hthandle->FileMapping, sizeof(HANDLE));
		memcpy(&file, &hthandle->File, sizeof(HANDLE));
		memcpy(&mutex, &hthandle->Mutex, sizeof(HANDLE));
		TerminateThread(hthandle->SnapshotThread, 0);
		if (!UnmapViewOfFile(hthandle->Addr)) return FALSE;
		if (!CloseHandle(mapping))return FALSE;
		if (!CloseHandle(file))return FALSE;
		if (!CloseHandle(mutex))return FALSE;
		return TRUE;
	}

	BOOL Insert(HTHANDLE* hthandle, Element* element) {
		if (!CheckParmLength(hthandle, element)) return FALSE;
		HANDLE hm = CreateMutex(NULL, FALSE, hthandle->MutexName);
		WaitForSingleObject(hm, INFINITE);
		if (hthandle->N != hthandle->Capacity)
		{
			for (int i = 0, j = HASH::HashFunction(element->key, element->keylength, hthandle->Capacity, 0);
				i != hthandle->Capacity;  j = HASH::Next_hash(j, hthandle->Capacity, ++i)) {
				Element* elFromHT = GetElementFromHT(hthandle, j);
				if (elFromHT == NULL || IsDeleted(elFromHT)) {
					SetElementToHT(hthandle, element, j);
					hthandle->N++;
					ReleaseMutex(hm);
					return TRUE;
				}
				if (elFromHT != NULL && EqualElementKeys(elFromHT, element)) {
					SetErrorMessage(hthandle, "Repeat key\n", 12);
					ReleaseMutex(hm);
					return FALSE;
				}
			}
		}
		ReleaseMutex(hm);
		return FALSE;
	}
	Element* Get(HTHANDLE* hthandle, Element* element) 	//  != NULL успешное завершение 
	{
		if (!CheckParmLength(hthandle, element)) return NULL;
		HANDLE hm = CreateMutex(NULL, FALSE, hthandle->MutexName);
		WaitForSingleObject(hm, INFINITE);

		int indexInHT = -1;
		bool found = false;
		if (hthandle->N != 0)
			for (int i = 0, j = HASH::HashFunction(element->key, element->keylength, hthandle->Capacity, 0);
				GetElementFromHT(hthandle, j) != NULL && i != hthandle->Capacity && !found;
				j = HASH::Next_hash(j, hthandle->Capacity, ++i)) {
			Element* elFromHT = GetElementFromHT(hthandle, j);
			if (!IsDeleted(elFromHT))
				if (EqualElementKeys(elFromHT, element)) {
					indexInHT = j; found = true;
				}
		}
		if (indexInHT < 0) {
			SetErrorMessage(hthandle, "Not found key\n", 15);
			//ReleaseMutex(hthandle->Mutex);
			ReleaseMutex(hm);
			return NULL;
		}
		//ReleaseMutex(hthandle->Mutex);
		ReleaseMutex(hm);
		return GetElementFromHT(hthandle, indexInHT);
	}
	BOOL Delete(HTHANDLE* hthandle, Element* element)	//  == TRUE успешное завершение 
	{
		if (!CheckParmLength(hthandle, element)) return FALSE;
		/*WaitForSingleObject(hthandle->Mutex, INFINITE);*/
		HANDLE hm = CreateMutex(NULL, FALSE, hthandle->MutexName);
		WaitForSingleObject(hm, INFINITE);
		int indexInHT = -1;
		bool deleted = false;
		if (hthandle->N != 0)
			for (int i = 0, j = HASH::HashFunction(element->key, element->keylength, hthandle->Capacity, 0);
				GetElementFromHT(hthandle, j) != NULL && i != hthandle->Capacity && !deleted;
				j = HASH::Next_hash(j, hthandle->Capacity, ++i)) {
			Element* elFromHT = GetElementFromHT(hthandle, j);
			if (!IsDeleted(elFromHT))
				if (EqualElementKeys(elFromHT, element)) {
					indexInHT = j; deleted = true;
				}
		}
		if (indexInHT < 0) {
			SetErrorMessage(hthandle, "Not found key\n", 15);
			ReleaseMutex(hm);
			return FALSE;
		}
		SetDeletedFlag(GetElementFromHT(hthandle, indexInHT));
		hthandle->N--;
		ReleaseMutex(hm);
		return TRUE;
	}
	BOOL Update(HTHANDLE* hthandle, Element* oldelement, void* newpayload, int newpayloadlength) {
		if (!CheckParmLength(hthandle, oldelement) || !CheckParmLength(hthandle, newpayloadlength)) return FALSE;
		HANDLE hm = CreateMutex(NULL, FALSE, hthandle->MutexName);
		WaitForSingleObject(hm, INFINITE);
		int indexInHT = -1;
		bool updated = false;
		if (hthandle->N != 0)
			for (int i = 0, j = HASH::HashFunction(oldelement->key, oldelement->keylength, hthandle->Capacity, 0);
				GetElementFromHT(hthandle, j) != NULL && i != hthandle->Capacity && !updated;
				j = HASH::Next_hash(j, hthandle->Capacity, ++i)) {
			Element* elFromHT = GetElementFromHT(hthandle, j);
			if (!IsDeleted(elFromHT))
				if (EqualElementKeys(elFromHT, oldelement)) {
					indexInHT = j; updated = true;
				}
		}
		if (indexInHT < 0) {
			SetErrorMessage(hthandle, "Not found key\n", 15);
			ReleaseMutex(hm);
			return FALSE;
		}
		UpdateElement(hthandle, GetElementFromHT(hthandle, indexInHT), newpayload, newpayloadlength);
		ReleaseMutex(hm);
		return TRUE;
	}
	char* GetLastError(HTHANDLE* ht) { return ht->LastErrorMessage; }
#pragma endregion

#pragma region MEMORY_LAYER

	Element* GetElementFromHT(HTHANDLE* ht, int n) {
		int defaultElementSize = ht->MaxKeyLength + ht->MaxPayloadLength + 2 * sizeof(int);
		void* elementAddr = (char*)ht + sizeof(HTHANDLE) + defaultElementSize * n;

		Element* element = new Element();
		element->key = elementAddr;
		element->keylength = *(int*)((char*)elementAddr + ht->MaxKeyLength);
		element->payload = ((char*)elementAddr + ht->MaxKeyLength + sizeof(int));
		element->payloadlength = *(int*)((char*)elementAddr + ht->MaxKeyLength + sizeof(int) + ht->MaxPayloadLength);
		if (element->keylength == 0) {
			delete element;
			return NULL;
		}
		return element;
	}
	BOOL SetElementToHT(HTHANDLE* ht, Element* element, int n) {
		int defaultElementSize = ht->MaxKeyLength + ht->MaxPayloadLength + 2 * sizeof(int);
		void* elementAddr = (char*)ht + sizeof(HTHANDLE) + defaultElementSize * n;

		memcpy(elementAddr, element->key, element->keylength);
		memcpy(((char*)elementAddr + ht->MaxKeyLength), &element->keylength, sizeof(int));
		memcpy(((char*)elementAddr + ht->MaxKeyLength + sizeof(int)), element->payload, element->payloadlength);
		memcpy(((char*)elementAddr + ht->MaxKeyLength + sizeof(int) + +ht->MaxPayloadLength), &element->payloadlength, sizeof(int));
		return TRUE;
	}
	BOOL IsDeleted(Element* ptr) { return (*(int*)ptr->key == -1); }
	BOOL EqualElementKeys(Element* el1, Element* el2) {
		return memcmp(el1->key, el2->key, el2->keylength) == NULL;
	}
	void SetErrorMessage(HTHANDLE* ht, const char* message, int messageLen) {
		memcpy(ht->LastErrorMessage, (char*)message, messageLen);
	}
	void SetDeletedFlag(Element* ptr) {
		memcpy(ptr->key, &DELETED, sizeof(DELETED));
	}
	void UpdateElement(HTHANDLE* ht, Element* el, void* newpayload, int newpayloadlength) {
		ZeroMemory(el->payload, ht->MaxPayloadLength + sizeof(int));
		memcpy(el->payload, newpayload, newpayloadlength);
		memcpy((char*)el->payload + ht->MaxPayloadLength, &newpayloadlength, sizeof(int));
	}
#pragma endregion
#pragma region HIDDEN_LAYER
	DWORD WINAPI SnapShotCycle(HTHANDLE* ht) {
		while (true)
		{
			if (ht) {
				if (time(NULL) >= ht->lastsnaptime + ht->SecSnapshotInterval)
				{
					HANDLE hm = CreateMutex(NULL, FALSE, ht->MutexName);
					WaitForSingleObject(hm, INFINITE);

					if (!FlushViewOfFile(ht->Addr, NULL)) {
						SetErrorMessage(ht, "Snapshot error", 15);
						return FALSE;
					}
					else {
						ht->lastsnaptime = time(NULL);
						Beep(500, 300);
						Logger::Write("snap");
					}
					ReleaseMutex(hm);
				}
			}
			else
				break;
		}
		return TRUE;
	}
	wchar_t* GenerateCoreObjectName(const wchar_t* pathToHT, const wchar_t ch) {
		/*std::wstring path = std::wstring(pathToHT);
		std::replace(path.begin(), path.end(), '\\', ch);
		std::wcout << L"Core objct created: " << (wchar_t*)path.c_str()<<' '<<path.length();
		return (wchar_t*)path.c_str();*/
		wchar_t* objectName = new wchar_t[MAX_PATH];
		ZeroMemory(objectName, MAX_PATH);
		int j = 0;
		const wchar_t* global = { L"Global\\" };
		for (int i = 0; i < wcslen(global); i++) {
				objectName[j++] = global[i];
		}
		objectName[j++] = ch;
		for (int i = 0; i < wcslen(pathToHT); i++) {
			if (pathToHT[i] == L'\\')
				continue;
			else 
				objectName[j++] = pathToHT[i];
		}
		std::wcout << objectName << std::endl;
		return objectName;
	}
	BOOL CheckParmLength(HTHANDLE* ht, Element* element) {
		if (element->keylength > ht->MaxKeyLength) {
			SetErrorMessage(ht, "KeyLength > maxKeyLength\n", strlen("KeyLength > maxKeyLength\n") + 1);
			return FALSE;
		}
		else if (element->payloadlength > ht->MaxPayloadLength) {
			SetErrorMessage(ht, "PayloadLength > maxPayloadLength\n", strlen("PayloadLength > maxPayloadLength\n") + 1);
			return FALSE;
		}
		return TRUE;
	}
	BOOL CheckParmLength(HTHANDLE* ht, int payloadLength) {
		if (payloadLength > ht->MaxPayloadLength) {
			SetErrorMessage(ht, "PayloadLength > maxPayloadLength\n", strlen("PayloadLength > maxPayloadLength\n") + 1);
			return FALSE;
		}
		return TRUE;
	}

#pragma endregion
}