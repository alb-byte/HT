#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "../HT_COM_STATIC_LIB/HT_COM_STATIC_LIB.h"
#pragma comment(lib, "../Debug/HT_COM_STATIC_LIB.lib")
#define SEC 1000
#define DEFAULT_INSERT_LOG_PATH L"D:\\OS\\INSERT.log"
#define DEFAULT_UPDATE_LOG_PATH L"D:\\OS\\UPDATE.log"
#define DEFAULT_DELETE_LOG_PATH L"D:\\OS\\DELETE.log"


void printProgrammArgs(int argc, wchar_t* argv[]);
char generateRandomSymbol();
void writeRowInLog(HANDLE hf, HT::Element e, bool success);
void showMenu();
int wmain(int argc, wchar_t* argv[])
{
	printProgrammArgs(argc, argv);

	wchar_t* file = nullptr;
	wchar_t* userName = nullptr;
	wchar_t* password = nullptr;
	wchar_t* log = nullptr;

	if (argc == 2) {
		file = argv[1];
	}
	else if (argc == 4) {
		file = argv[1];
		userName = argv[2];
		password = argv[3];
	}
	else {
		std::cout << "INVALID PARAMS: {fileName}{username?}{password?}" << std::endl;
		return 0;
	}
	showMenu();
	std::cout << "ENTER COMMAND(DEFAULT 1): " << std::endl;
	char command = '1';
	std::cin >> command;
	switch (command) {
	case '1':
		std::cout << "1";
		break;
	case '2':
		std::cout << "2";
		break;
	case '3':
		std::cout << "3";
		break;
	}

	system("pause");
	return 0;
}

#define SHORTLINE "----------"
void showMenu() {
	std::cout << SHORTLINE << SHORTLINE << std::endl
		<< "1. INSERT" << std::endl
		<< "2. UPDATE" << std::endl
		<< "3. DELETE" << std::endl
		<< SHORTLINE << SHORTLINE << std::endl;
}
void printProgrammArgs(int argc, wchar_t* argv[]) {
	std::cout << std::setfill('=') << std::setw(64) << '|' << std::endl;
	for (int count = 1; count < argc; ++count)
		std::wcout << count << ": " << argv[count] << '\n';
	std::cout << std::setfill('=') << std::setw(64) << '|' << std::endl;
}
char generateRandomSymbol()
{
	int minValue = 49;
	int d = 50;
	return  minValue + rand() % d;//1 - c
}
void writeRowInLog(HANDLE hf, HT::Element e, bool success = false) {
	char buffer[80]{ 0 };
	DWORD n = NULL;
	time_t seconds = time(NULL);
	tm* timeinfo = localtime(&seconds);
	const char* format = "%e.%m.%Y %I:%M:%S";
	strftime(buffer, 80, format, timeinfo);

	strcat(buffer, " key = ");
	strcat(buffer, (char*)e.key);
	strcat(buffer, " value = ");
	strcat(buffer, "0 ");
	success ? strcat(buffer, "TRUE\n") : strcat(buffer, "FALSE\n");

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