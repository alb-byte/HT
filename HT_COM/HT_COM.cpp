#include "pch.h"
#include "HT.h"
#include "HT_COM.h"
#include "LOG.h"

extern ULONG g_Components;

HT_COM::HT_COM() :m_Ref(1) {
	InterlockedIncrement((LONG*)&g_Components);
	Logger::Write("HT_COM::Adder");
};
HT_COM::~HT_COM() {
	InterlockedDecrement((LONG*)&g_Components);
	Logger::Write("OS12::~Adder");
};

HRESULT STDMETHODCALLTYPE HT_COM::QueryInterface(REFIID riid, void** ppv)
{
	HRESULT rc = S_OK;
	*ppv = NULL;
	if (riid == IID_IUnknown)	*ppv = static_cast<IHT*>(this);
	else if (riid == IID_IHT)	*ppv = static_cast<IHT*>(this);
	else rc = E_NOINTERFACE;

	if (rc == S_OK) this->AddRef();
	Logger::Write("OS12::QueryInterface ");
	return rc;
};
ULONG STDMETHODCALLTYPE HT_COM::AddRef(void) {
	InterlockedIncrement((LONG*)&(this->m_Ref));
	Logger::Write("OS12::AddRef");
	return this->m_Ref;
};
ULONG STDMETHODCALLTYPE HT_COM::Release(void) {
	ULONG rc = this->m_Ref;
	if ((rc = InterlockedDecrement((LONG*)&(this->m_Ref))) == 0) delete this;
	Logger::Write("OS12::Release");
	return rc;
};

HRESULT STDMETHODCALLTYPE HT_COM::Create(
	int Capacity, int SecSnapshotInterval, int MaxKeyLength,
	int MaxPayloadLength, const wchar_t FileName[512], const wchar_t HTUsersGroup[512], HT::HTHANDLE** result
)
{
	*result = HT::Create(Capacity, SecSnapshotInterval, MaxKeyLength, MaxPayloadLength, FileName, HTUsersGroup);
	if (*result != NULL) return S_OK;
	return E_FAIL;
}
HRESULT STDMETHODCALLTYPE HT_COM::Open(const wchar_t FileName[512], HT::HTHANDLE** result) {
	std::wcout << FileName << std::endl;
	HT::HTHANDLE* temp = HT::Open(FileName);
	std::cout << temp << std::endl;
	Logger::Write("Open file handle", temp);
	if (temp == NULL) return E_FAIL;
	*result = temp;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE HT_COM::Open(const wchar_t FileName[512],
	const wchar_t UserName[256],
	const wchar_t Password[256],
	HT::HTHANDLE** result) {
	HT::HTHANDLE* temp = HT::Open(FileName, UserName, Password);
	std::cout << temp << std::endl;
	Logger::Write("Open file handle", temp);
	if (temp == NULL) Logger::Write("HHHHHHHHHHHHHHHHHH");
	if (temp == NULL) return E_FAIL;
	*result = temp;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE HT_COM::Snap(HT::HTHANDLE** hthandle) {
	if (HT::Snap(*hthandle))
		return S_OK;
	else
		return E_FAIL;
}

HRESULT STDMETHODCALLTYPE HT_COM::Close(HT::HTHANDLE** hthandle) {
	Logger::Write("Close");

	bool res = HT::Close(*hthandle);
	if (res) return S_OK;
	else
		return E_FAIL;
}

HRESULT STDMETHODCALLTYPE HT_COM::Start(const wchar_t FileName[512], HT::HTHANDLE** result) {
	Logger::Write("START file");
	std::wcout << FileName << std::endl;
	HT::HTHANDLE* temp = HT::Start(FileName);
	if (temp == NULL) return E_FAIL;
	*result = temp;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE HT_COM::Start(const wchar_t FileName[512],
	const wchar_t UserName[256],
	const wchar_t Password[256],
	HT::HTHANDLE** result) {
	std::wcout << UserName << "  :  " << Password << std::endl;
	HT::HTHANDLE* temp = HT::Start(FileName, UserName, Password);
	if (temp == NULL) return E_FAIL;
	*result = temp;
	return S_OK;
}
HRESULT STDMETHODCALLTYPE HT_COM::Stop(HT::HTHANDLE** hthandle) {
	bool res = HT::Stop(*hthandle);
	Logger::Write("STOP file");

	if (res)
		return S_OK;
	else
		return E_FAIL;
}

HRESULT STDMETHODCALLTYPE HT_COM::Get(HT::HTHANDLE** hthandle, HT::Element* element, HT::Element& result) {
	HT::Element* temp = HT::Get(*hthandle, element);
	Logger::Write("GET");

	if (temp == NULL) return E_FAIL;
	result = *temp;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE HT_COM::Insert(HT::HTHANDLE** hthandle, HT::Element* element) {
	if (HT::Insert(*hthandle, element))
		return S_OK;
	else
		return E_FAIL;
}
HRESULT STDMETHODCALLTYPE HT_COM::Update(HT::HTHANDLE** hthandle, HT::Element* element, void* newpayload, int newpayloadlength) {
	if (HT::Update(*hthandle, element, newpayload, newpayloadlength))
		return S_OK;
	else
		return E_FAIL;
}

HRESULT STDMETHODCALLTYPE HT_COM::Delete(HT::HTHANDLE** hthandle, HT::Element* element) {
	if (HT::Delete(*hthandle, element))
		return S_OK;
	else
		return E_FAIL;
}

HRESULT STDMETHODCALLTYPE HT_COM::GetLastError(HT::HTHANDLE** ht, char** result) {
	*result = HT::GetLastError(*ht);
	Logger::Write("STOP file");
	return S_OK;
}
