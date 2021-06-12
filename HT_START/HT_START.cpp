#include <iostream>
#include "../HT_COM_STATIC_LIB/HT_COM_STATIC_LIB.h"
#pragma comment(lib, "../Debug/HT_COM_STATIC_LIB.lib")

int wmain(int argc, wchar_t* argv[])
{
	std::cout << std::setfill('=') << std::setw(64) << '|' << std::endl;
	for (int count = 0; count < argc; ++count)
		std::wcout << count << ": " << argv[count] << '\n';
	std::cout << std::setfill('=') << std::setw(64) << '|' << std::endl;


	wchar_t* file = nullptr;
	wchar_t* userName = nullptr;
	wchar_t* password = nullptr;

	if (argc == 2) {
		file = argv[1];
	}
	else if (argc == 4) {
		file = argv[1];
		userName = argv[2];
		password = argv[3];
	}
	else {
		std::cout << "Invalid params" << std::endl <<
			"---->HT_START {fileName}{UserName}{Password}" << std::endl;
		return 0;
	}
	HT::HTHANDLE* HT = NULL;
	HTAPI::Init();
	if (userName != nullptr && password != nullptr)
		HT = HTAPI::Start(file, userName, password);
	else
		HT = HTAPI::Start(file);

	if (HT != NULL) {
		std::wcout << "HT - Storage Created filename = " << argv[1];
		std::cout <<
			", snapshotinterval = " << HT->SecSnapshotInterval <<
			",capacity = " << HT->Capacity <<
			", maxkeylength = " << HT->MaxKeyLength <<
			", maxdatalength = " << HT->MaxPayloadLength <<
			std::endl;
		std::cout << "Press key to close" << std::endl;
		getchar();
		HTAPI::Stop(HT);
	}
	else
		std::cout << "ERROR" << std::endl;
	HTAPI::Dispose();
	system("pause");
	return 0;
}