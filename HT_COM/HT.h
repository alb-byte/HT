#pragma once
#include "pch.h"
#include "HT_Storage.h"
#include "Hash.h"

namespace HT {

	HT_API HTHANDLE* Create(
		int Capacity, int SecSnapshotInterval, int MaxKeyLength,
		int MaxPayloadLength, const wchar_t  FileName[512], const wchar_t HTUsersGroup[512]
	);
	HT_API HTHANDLE* Open(const wchar_t FileName[512]);
	HT_API HTHANDLE* Open(const wchar_t FileName[512],
		const wchar_t UserName[256],
		const wchar_t Password[256]
	);
	HT_API BOOL Snap(HTHANDLE* hthandle);
	HT_API BOOL Close(const HTHANDLE* hthandle);

	HT_API HTHANDLE* Start(const wchar_t FileName[512]);
	HT_API HTHANDLE* Start(const wchar_t FileName[512],
		const wchar_t UserName[256],
		const wchar_t Password[256]
	);
	HT_API BOOL Stop(const HTHANDLE* hthandle);

	HT_API Element* Get(HTHANDLE* hthandle, Element* element);
	HT_API BOOL Insert(HTHANDLE* hthandle, Element* element);
	HT_API BOOL Update(HTHANDLE* hthandle, Element* oldelement, void* newpayload, int newpayloadlength);
	HT_API BOOL Delete(HTHANDLE* hthandle, Element* element);

	HT_API char* GetLastError(HTHANDLE* ht);
};
