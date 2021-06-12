#include <iostream>
#include "../HT_COM_STATIC_LIB/HT_COM_STATIC_LIB.h"
#pragma comment(lib, "../Debug/HT_COM_STATIC_LIB.lib")
int main()
{
    int capacity = 10;
    int snapshotIntervalSec = 5;
    int maxKeyLength = 10;
    int maxPayloadLength = 10;
    const wchar_t* fileName = L"C:\\Users\\alber\\OneDrive\\3course\\6semestr\\OS\\labs\\lab13\\Debug\\storage2.ht";
    const wchar_t* uGroup = L"HT";

    HTAPI::Init();
    HT::HTHANDLE* HT = HTAPI::Create(capacity, snapshotIntervalSec, maxKeyLength, maxPayloadLength, fileName, uGroup);
    HTAPI::Dispose();
    system("pause");
    return 0;
}