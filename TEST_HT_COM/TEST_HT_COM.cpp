#include <iostream>
#include <objbase.h>
#include "../HT_COM/INTERFACE.h"

IHT* pHT = nullptr;
int main()
{
	int capacity = 10;
	int snapshotIntervalSec = 5;
	int maxKeyLength = 10;
	int maxPayloadLength = 10;
	const wchar_t* fileName = L"C:\\Users\\alber\\OneDrive\\3course\\6semestr\\OS\\labs\\lab13\\Debug\\storage.ht";
	HT::HTHANDLE* HT = nullptr;

	IUnknown* pIUnknown = NULL;
	CoInitialize(NULL);                        // инициализация библиотеки OLE32
	HRESULT hr = CoCreateInstance(CLSID_CA, NULL, CLSCTX_INPROC_SERVER, IID_IUnknown, (void**)&pIUnknown);
	if (SUCCEEDED(hr))
	{
		std::cout << "CoCreateInstance succeeded" << std::endl;
		if (SUCCEEDED(pIUnknown->QueryInterface(IID_IHT, (void**)&pHT)))
		{
			/*if (SUCCEEDED(pHT->Create(capacity, snapshotIntervalSec, maxKeyLength, maxPayloadLength, fileName,&HT))) std::cout<<"IHT::CREATE"<<std::endl;
			else std::cout << "IHT::CREATE ERROR" << std::endl;*/

			if (SUCCEEDED(pHT->Start(fileName,&HT))) std::cout<<"IHT::Start"<<std::endl;
			else std::cout << "IHT::start ERROR" << std::endl;
			Sleep(1000);
			//HT::HTHANDLE* HTOPEN = nullptr;
			//if (SUCCEEDED(pHT->Open(fileName,&HTOPEN))) std::cout<<"IHT::OPEN"<<std::endl;
			//else std::cout << "IHT::OPEN ERROR" << std::endl;
			//Sleep(3000);
			//if (SUCCEEDED(pHT->Snap(&HT, &res))) std::cout << "IHT::snap" << std::endl;
			//else std::cout << "IHT::snap ERROR" << std::endl;
			/*if (SUCCEEDED(pHT->Close(&HTOPEN,&res))) std::cout << "IHT::CLOSE" << std::endl;
			else std::cout << "IHT::CLOSE ERROR" << std::endl;
			Sleep(3000);*/
			HT::Element* el1 = new HT::Element("str3", 5, "albert3", 8);
			HT::Element* el2 = new HT::Element("str", 4, "value2", 7);
			HT::Element elres;
			if (SUCCEEDED(pHT->Insert(&HT,el1))) std::cout << "IHT::insert" << std::endl;
			else std::cout << "IHT::insert ERROR" << std::endl;

			if (SUCCEEDED(pHT->Get(&HT,el1,elres))) std::cout << "IHT::get" << std::endl;
			else std::cout << "IHT::get ERROR" << std::endl;

			if (SUCCEEDED(pHT->Update(&HT, el1, el2->payload,el2->payloadlength))) std::cout << "IHT::update" << std::endl;
			else std::cout << "IHT::update ERROR" << std::endl;

			if (SUCCEEDED(pHT->Delete(&HT, el1))) std::cout << "IHT::update" << std::endl;
			else std::cout << "IHT::update ERROR" << std::endl;

			if (SUCCEEDED(pHT->Get(&HT, el1, elres))) std::cout << "IHT::get" << std::endl;
			else std::cout << "IHT::get ERROR" << std::endl;

			if (SUCCEEDED(pHT->Stop(&HT))) std::cout << "IHT::stop" << std::endl;
			else std::cout << "IHT::stop ERROR" << std::endl;

		}
		else  std::cout << "IHT::ERROR" << std::endl;
		pIUnknown->Release();
	}
	else  std::cout << "CoCreateInstance error" << std::endl;
	CoFreeUnusedLibraries();

    system("pause");
    return 0;
}

