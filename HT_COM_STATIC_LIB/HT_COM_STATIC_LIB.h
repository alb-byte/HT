#pragma once
#include <iostream>
#include <objbase.h>
#include "../HT_COM/INTERFACE.h"

namespace HTAPI{
	bool  Init();
	bool  Dispose();
	HT::HTHANDLE* Create(
		int Capacity, int SecSnapshotInterval, int MaxKeyLength,
		int MaxPayloadLength, const wchar_t FileName[512], const wchar_t HTUsersGroup[512]
	);
	HT::HTHANDLE* Open(const wchar_t FileName[512]);
	HT::HTHANDLE* Open(const wchar_t FileName[512],
		const wchar_t UserName[256],
		const wchar_t Password[256]);
	bool Snap(HT::HTHANDLE* hthandle);
	bool Close(HT::HTHANDLE* hthandle);

	HT::HTHANDLE* Start(const wchar_t FileName[512]);
	HT::HTHANDLE* Start(const wchar_t FileName[512],
		const wchar_t UserName[256],
		const wchar_t Password[256]);
	bool Stop(HT::HTHANDLE* hthandle);

	HT::Element* Get(HT::HTHANDLE* hthandle,HT::Element* element);
	bool Insert(HT::HTHANDLE* hthandle,HT::Element* element);
	bool Update(HT::HTHANDLE* hthandle,HT::Element* oldelement, void* newpayload, int newpayloadlength);
	bool Delete(HT::HTHANDLE* hthandle,HT::Element* element);

	char* GetLastError(HT::HTHANDLE* hthandle);

}