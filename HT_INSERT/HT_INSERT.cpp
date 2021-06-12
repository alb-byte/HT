#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "../HT_COM_STATIC_LIB/HT_COM_STATIC_LIB.h"
#pragma comment(lib, "../Debug/HT_COM_STATIC_LIB.lib")
#define SEC 1000

char randomSymbol();
void WriteRowInLog(HANDLE hf, HT::Element e, bool success);

int wmain(int argc, wchar_t* argv[])
{
	SetConsoleTitle(L"HT_INSERT");
	std::cout << std::setfill('=') << std::setw(64) << '|' << std::endl;
	for (int count = 0; count < argc; ++count)
		std::wcout << count << ": " << argv[count] << '\n';
	std::cout << std::setfill('=') << std::setw(64) << '|' << std::endl;
	wchar_t* file = nullptr;
	wchar_t* userName = nullptr;
	wchar_t* password = nullptr;
	wchar_t* log = nullptr;

	if (argc == 2) {
		file = argv[1];
		log = new wchar_t[] {L"D:\\OS\\INSERT.log"};
	}
	else if (argc == 4) {
		file = argv[1];
		userName = argv[2];
		password = argv[3];
		log = new wchar_t[] {L"D:\\OS\\INSERT.log"};
	}
	else if (argc == 5) {
		file = argv[1];
		log = argv[4];
	}
	else
		std::cout << "ERROR: Invalid params" << std::endl
		<< "---->HT_INSERT {fileName}{username?}{password?}{logfile?}" << std::endl;
	if (file != nullptr && log != nullptr) {
		char key[10] = "key";
		int payload = 0;

		HT::HTHANDLE* hthandle;
		HTAPI::Init();

		if (userName != nullptr && password != nullptr)
			hthandle = HTAPI::Open(file, userName, password);
		else hthandle = HTAPI::Open(file);

		if (!hthandle) {
			HANDLE hOut;
			//  Get the handle of the output stream
			hOut = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hOut,
				FOREGROUND_RED |        //  Foreground_Red
				FOREGROUND_INTENSITY);      //  Background color_blue
			std::cout << "ERROR -- START HT OR CHECK LOGIN PASSWORD!!!" << std::endl;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			HTAPI::Dispose();
			return 0;
		}
		HANDLE hf = CreateFile(
			log,//path
			GENERIC_WRITE,
			NULL,//режим совместного пользования
			NULL,// атрибуты защиты
			OPEN_ALWAYS,// создание 
			FILE_ATTRIBUTE_NORMAL,//атрибуты и флаги
			NULL//файл атрибутов
		);
		int but = MessageBox(NULL, L"START?", L"HT_INSERT", MB_OKCANCEL | MB_ICONINFORMATION);
		if (but == IDCANCEL) {
			HTAPI::Close(hthandle);
			HTAPI::Dispose();
			system("pause");
			return 0;
		}
		while (true) {
			key[3] = randomSymbol();
			HT::Element element(key, 5, &payload, 4);
			bool res = HTAPI::Insert(hthandle, &element);
			std::cout << key << '\t' << res << std::endl;
			WriteRowInLog(hf, element, res);

			Sleep(SEC);
		}
		HTAPI::Dispose();
		CloseHandle(hf);
	}

	system("pause");
	return 0;
}
char randomSymbol()
{
	int minValue = 49;
	int d = 50;//1 - c
	return  minValue + rand() % d;
}
void WriteRowInLog(HANDLE hf, HT::Element e, bool success = false) {
	char buffer[80]{ 0 };
	DWORD n = NULL;
	time_t seconds = time(NULL);
	tm* timeinfo = localtime(&seconds);
	const char* format = "%e.%m.%Y %I:%M:%S";
	strftime(buffer, 80, format, timeinfo);

	strcat(buffer, " key = ");
	strcat(buffer, (char*)e.key);
	success ? strcat(buffer, " TRUE\n") : strcat(buffer, " FALSE\n");

	LARGE_INTEGER newPointer;
	newPointer.QuadPart = 0;
	DWORD positionMode = FILE_END;

	if (!SetFilePointerEx(hf, newPointer, NULL, positionMode))
		throw std::exception("SetFilePointerEx return False");
	if (!SetEndOfFile(hf))
		throw std::exception("SetEndOfFile return FALSE");

	WriteFile(
		hf,
		buffer,
		strlen(buffer),
		&n,
		NULL);
}
