#include <iostream>
#pragma comment(lib, "..\\Debug\\HT_COM_STATIC_LIB.lib")
#include "../HT_COM_STATIC_LIB/HT_COM_STATIC_LIB.h"

int wmain(int argc, wchar_t* argv[])
{
	std::cout << std::setfill('=') << std::setw(64) << '|' << std::endl;
	for (int count = 0; count < argc; ++count)
		std::wcout << count << ": " << argv[count] << '\n';
	std::cout << std::setfill('=') << std::setw(64) << '|' << std::endl;

	if (argc == 7) {
		int capacity = _wtoi(argv[1]);
		int snapshotIntervalSec = _wtoi(argv[2]);
		int maxKeyLength = _wtoi(argv[3]);
		int maxPayloadLength = _wtoi(argv[4]);

		const wchar_t* fileName = argv[5];
		const wchar_t* uGroup = argv[6];
		HTAPI::Init();
		HT::HTHANDLE* HT = HTAPI::Create(capacity, snapshotIntervalSec, maxKeyLength, maxPayloadLength, fileName, uGroup);
		if (HT != NULL) {
			HTAPI::Close(HT);
			std::wcout << "HT - Storage Created filename = " << argv[5];
			std::cout <<
				", snapshotinterval = " << snapshotIntervalSec <<
				",capacity = " << capacity <<
				", maxkeylength = " << maxKeyLength <<
				", maxdatalength = " << maxPayloadLength <<
				std::endl;
		}
		else std::cout << "ERROR file already exists or uGroup not found" << std::endl;
		HTAPI::Dispose();
	}
	else 
		std::cout << "ERROR: Invalid params"<<
		std::endl<<
		"---->OS_CREATE {capacity}{snapshotInterval}{maxKeyLength}{maxPayloadLength}{fileName}{uGroup}" <<
		std::endl;

	system("pause");
	return 0;
}
