#include "pch.h"
#include "CFactory.h"
#include "LOG.h"
#include "INTERFACE.h"
ULONG g_Components;
ULONG g_ServerLocks;

STDAPI DllCanUnloadNow() {

	HRESULT rc = E_UNEXPECTED;
	if ((g_ServerLocks == 0) && (g_Components == 0)) rc = S_OK;
	else rc = S_FALSE;
	Logger::Write("DllCanUnloadNow");
	return rc;
}
STDAPI DllGetClassObject(const CLSID& clsid, const IID& iid, void** ppv) {
	HRESULT rc = E_UNEXPECTED;
	CFactory* pF;
	if (clsid != CLSID_CA) rc = CLASS_E_CLASSNOTAVAILABLE;
	else if ((pF = new CFactory()) == NULL) rc = E_OUTOFMEMORY;
	else {
		rc = pF->QueryInterface(iid, ppv);
		pF->Release();
	}
	Logger::Write("DllGetClassObject");
	return rc;
}