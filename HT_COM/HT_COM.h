#pragma once
#include "INTERFACE.h"

class HT_COM :public IHT
{
public:
	HT_COM();
	~HT_COM();

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv);
	virtual ULONG STDMETHODCALLTYPE AddRef(void);
	virtual ULONG STDMETHODCALLTYPE Release(void);

	virtual HRESULT STDMETHODCALLTYPE Create(
		int Capacity, int SecSnapshotInterval, int MaxKeyLength,
		int MaxPayloadLength, const wchar_t FileName[512], const wchar_t HTUsersGroup[512], HT::HTHANDLE** result
	);
	virtual HRESULT STDMETHODCALLTYPE Open(const wchar_t FileName[512], HT::HTHANDLE** result);
	virtual HRESULT STDMETHODCALLTYPE Open(const wchar_t FileName[512],
		const wchar_t UserName[256],
		const wchar_t Password[256], 
		HT::HTHANDLE** result);
	virtual HRESULT STDMETHODCALLTYPE Snap(HT::HTHANDLE** hthandle);
	virtual HRESULT STDMETHODCALLTYPE Close(HT::HTHANDLE** hthandle);
	virtual HRESULT STDMETHODCALLTYPE Start(const wchar_t FileName[512], HT::HTHANDLE** result);
	virtual HRESULT STDMETHODCALLTYPE Start(const wchar_t FileName[512],
		const wchar_t UserName[256],
		const wchar_t Password[256], 
		HT::HTHANDLE** result);
	virtual HRESULT STDMETHODCALLTYPE Stop(HT::HTHANDLE** hthandle);

	virtual HRESULT STDMETHODCALLTYPE Get(HT::HTHANDLE** hthandle, HT::Element* element, HT::Element& result);
	virtual HRESULT STDMETHODCALLTYPE Insert(HT::HTHANDLE** hthandle, HT::Element* element);
	virtual HRESULT STDMETHODCALLTYPE Update(HT::HTHANDLE** hthandle, HT::Element* element, void* newpayload, int newpayloadlength);
	virtual HRESULT STDMETHODCALLTYPE Delete(HT::HTHANDLE** hthandle, HT::Element* element);

	virtual HRESULT STDMETHODCALLTYPE GetLastError(HT::HTHANDLE** ht, char** result);
private:
	volatile ULONG m_Ref;
};