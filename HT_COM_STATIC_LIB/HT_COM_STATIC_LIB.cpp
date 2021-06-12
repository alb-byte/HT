#include "pch.h"
#include "HT_COM_STATIC_LIB.h"
#include <iostream>
#include <fstream>
namespace HTAPI
{
	IHT* pHT = nullptr;
	IUnknown* pIUnknown = NULL;

	bool Init() {
		CoInitialize(NULL);                        // инициализация библиотеки OLE32
		HRESULT hr = CoCreateInstance(CLSID_CA, NULL, CLSCTX_INPROC_SERVER, IID_IUnknown, (void**)&pIUnknown);
		if (SUCCEEDED(hr))
			if (SUCCEEDED(pIUnknown->QueryInterface(IID_IHT, (void**)&pHT)))
				return true;
		return false;
	}
	bool Dispose() {
		bool res = true;
		if (!SUCCEEDED(pHT->Release()))
			res = false;
		if (!SUCCEEDED(pIUnknown->Release()))
			res = false;
		CoFreeUnusedLibraries();
		return true;
	}
	HT::HTHANDLE* Create(
		int Capacity, int SecSnapshotInterval, int MaxKeyLength,
		int MaxPayloadLength, const wchar_t FileName[512], const wchar_t HTUsersGroup[512]
	) {
		HT::HTHANDLE* tempHT = nullptr;
		bool r = SUCCEEDED(pHT->Create(Capacity, SecSnapshotInterval, MaxKeyLength, MaxPayloadLength, FileName, HTUsersGroup, &tempHT));

		return tempHT;
	}
	HT::HTHANDLE* Open(const wchar_t FileName[512]) {
		HT::HTHANDLE* tempHT = nullptr;
		SUCCEEDED(pHT->Open(FileName, &tempHT));
		return tempHT;
	}
	HT::HTHANDLE* Open(const wchar_t FileName[512],
		const wchar_t UserName[256],
		const wchar_t Password[256]) {
		HT::HTHANDLE* tempHT = nullptr;
		SUCCEEDED(pHT->Open(FileName, UserName, Password, &tempHT));
		return tempHT;
	}
	bool Snap(HT::HTHANDLE* hthandle) {
		return SUCCEEDED(pHT->Snap(&hthandle));
	}
	bool Close(HT::HTHANDLE* hthandle) {
		return SUCCEEDED(pHT->Close(&hthandle));
	}
	HT::HTHANDLE* Start(const wchar_t FileName[512]) {
		HT::HTHANDLE* tempHT = nullptr;
		SUCCEEDED(pHT->Start(FileName, &tempHT));
		return tempHT;
	}
	HT::HTHANDLE* Start(const wchar_t FileName[512],
		const wchar_t UserName[256],
		const wchar_t Password[256]){
		HT::HTHANDLE* tempHT = nullptr;
		SUCCEEDED(pHT->Start(FileName, UserName, Password, &tempHT));
		return tempHT;
	}
	bool Stop(HT::HTHANDLE* hthandle) {
		return SUCCEEDED(pHT->Stop(&hthandle));
	}
	HT::Element* Get(HT::HTHANDLE* hthandle, HT::Element* element) {
		HT::Element* result = new HT::Element();
		ZeroMemory(result, sizeof(result));
		SUCCEEDED(pHT->Get(&hthandle, element, *result));
		if (!result->key) {
			delete result;
			return NULL;
		}
		return result;
	}
	bool Insert(HT::HTHANDLE* hthandle, HT::Element* element) {
		return SUCCEEDED(pHT->Insert(&hthandle, element));
	}
	bool Update(HT::HTHANDLE* hthandle, HT::Element* oldelement, void* newpayload, int newpayloadlength) {
		return SUCCEEDED(pHT->Update(&hthandle, oldelement, newpayload, newpayloadlength));
	}
	bool Delete(HT::HTHANDLE* hthandle, HT::Element* element) {
		return SUCCEEDED(pHT->Delete(&hthandle, element));
	}

	char* GetLastError(HT::HTHANDLE* hthandle) {
		char* lastError = nullptr;
		pHT->GetLastError(&hthandle, &lastError);
		return lastError;
	}
}
