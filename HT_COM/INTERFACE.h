#pragma once
#include <objbase.h>
#include "HT.h"

#define FNAME L"AHT.OS.COM"
#define VINDX L"AHT.OS.1"
#define PRGID L"AHT.OS"

// {E49A949B-91B5-42D7-97FF-338D07D812BC}
static const GUID CLSID_CA =
{ 0xe49a949b, 0x91b5, 0x42d7, { 0x97, 0xff, 0x33, 0x8d, 0x7, 0xd8, 0x12, 0xbc } };

// {6047F974-CCF1-44E9-B922-6ADEC211A28E}
static const GUID IID_IHT =
{ 0x6047f974, 0xccf1, 0x44e9, { 0xb9, 0x22, 0x6a, 0xde, 0xc2, 0x11, 0xa2, 0x8e } };

interface IHT :IUnknown
{
	virtual HRESULT STDMETHODCALLTYPE Create(
		int Capacity, int SecSnapshotInterval, int MaxKeyLength,
		int MaxPayloadLength, const wchar_t FileName[512], const wchar_t HTUsersGroup[512], HT::HTHANDLE** result
	) = 0;

	virtual HRESULT STDMETHODCALLTYPE Open(const wchar_t FileName[512], HT::HTHANDLE** result) = 0;
	virtual HRESULT STDMETHODCALLTYPE Open(const wchar_t FileName[512],
		const wchar_t UserName[256],
		const wchar_t Password[256],
		HT::HTHANDLE** result) = 0;
	virtual HRESULT STDMETHODCALLTYPE Snap(HT::HTHANDLE** hthandle) = 0;
	virtual HRESULT STDMETHODCALLTYPE Close(HT::HTHANDLE** hthandle) = 0;
	virtual HRESULT STDMETHODCALLTYPE Start(const wchar_t FileName[512], HT::HTHANDLE** result) = 0;
	virtual HRESULT STDMETHODCALLTYPE Start(const wchar_t FileName[512],
		const wchar_t UserName[256],
		const wchar_t Password[256], 
		HT::HTHANDLE** result) = 0;
	virtual HRESULT STDMETHODCALLTYPE Stop(HT::HTHANDLE** hthandle) = 0;

	virtual HRESULT STDMETHODCALLTYPE Get(HT::HTHANDLE** hthandle, HT::Element* element, HT::Element& result) = 0;
	virtual HRESULT STDMETHODCALLTYPE Insert(HT::HTHANDLE** hthandle, HT::Element* element) = 0;
	virtual HRESULT STDMETHODCALLTYPE Update(HT::HTHANDLE** hthandle, HT::Element* element, void* newpayload, int newpayloadlength) = 0;
	virtual HRESULT STDMETHODCALLTYPE Delete(HT::HTHANDLE** hthandle, HT::Element* element) = 0;

	virtual HRESULT STDMETHODCALLTYPE GetLastError(HT::HTHANDLE** ht, char** result) = 0;
};