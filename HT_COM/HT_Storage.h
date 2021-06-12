#pragma once
#include <Windows.h>
#include "HT_Element.h"

namespace HT {
	struct HTHANDLE {
		HT_API HTHANDLE();
		HT_API HTHANDLE(int Capacity, int SecSnapshotInterval, int MaxKeyLength, int MaxPayloadLength, const wchar_t FileName[512]);
		int     Capacity;               // емкость хранилища в количестве элементов 
		int     SecSnapshotInterval;    // переодичность сохранения в сек. 
		int     MaxKeyLength;           // максимальная длина ключа
		int     MaxPayloadLength;       // максимальная длина данных
		char    FileName[512];          // имя файла 
		HANDLE  File;                   // File HANDLE != 0, если файл открыт
		HANDLE  FileMapping;            // Mapping File HANDLE != 0, если mapping создан  
		LPVOID  Addr;                   // Addr != NULL, если mapview выполнен  
		char    LastErrorMessage[512];  // сообщение об последней ошибке или 0x00  
		time_t  lastsnaptime;           // дата последнего snap'a (time())  

		void* elements;
		HANDLE Mutex;
		wchar_t MutexName[512];
		wchar_t HTUsersGroup[512];
		int N;							//текущее заполнение
		HANDLE SnapshotThread;
	};
}
