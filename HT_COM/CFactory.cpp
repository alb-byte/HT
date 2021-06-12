#pragma once
#include "pch.h"
#include "CFactory.h"
#include "HT_COM.h"
#include "LOG.h"

extern ULONG g_ServerLocks;

CFactory::CFactory() :m_Ref(1) { Logger::Write("CFactory::CFactory"); };
CFactory::~CFactory() { Logger::Write("CFactory::~CFactory"); };


HRESULT STDMETHODCALLTYPE CFactory::QueryInterface(REFIID riid, void** ppv)
{
	HRESULT rc = S_OK;
	*ppv = NULL;
	if (riid == IID_IUnknown)	*ppv = (IUnknown*)this;
	else if (riid == IID_IClassFactory)	*ppv = (IClassFactory*)this;
	else rc = E_NOINTERFACE;

	if (rc == S_OK) this->AddRef();
	Logger::Write("Cfactory::QueryInterface");
	return rc;
};
ULONG STDMETHODCALLTYPE CFactory::AddRef(void) {
	InterlockedIncrement((LONG*)&(this->m_Ref));
	Logger::Write("CFactory::AddRef m_Ref");
	return this->m_Ref;
};
ULONG STDMETHODCALLTYPE CFactory::Release(void) {
	ULONG rc = this->m_Ref;
	if ((rc = InterlockedDecrement((LONG*)&(this->m_Ref))) == 0) delete this;
	Logger::Write("CFactory::Release rc");
	return rc;
};


HRESULT STDMETHODCALLTYPE CFactory::CreateInstance(IUnknown* pUO,const IID& id,void** ppv) {
	Logger::Write("CFactory::CreateInstance call ");

	HRESULT rc = E_UNEXPECTED;
	HT_COM* pHTCOM;

	if (pUO != NULL)	rc = CLASS_E_NOAGGREGATION;
	else if ((pHTCOM = new HT_COM()) == NULL)	rc = E_OUTOFMEMORY;
	else {
		rc = pHTCOM->QueryInterface(id, ppv);
		pHTCOM->Release();
	}

	Logger::Write("CFactory::CreateInstance rc ");
	return rc;
}
HRESULT STDMETHODCALLTYPE CFactory::LockServer(BOOL b) {
	HRESULT rc = S_OK;

	if (b) InterlockedIncrement((LONG*)&(g_ServerLocks));
	else InterlockedDecrement((LONG*)&(g_ServerLocks));

	Logger::Write("CFactory::LockServer");
	return rc;
}
