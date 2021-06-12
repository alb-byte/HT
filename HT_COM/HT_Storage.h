#pragma once
#include <Windows.h>
#include "HT_Element.h"

namespace HT {
	struct HTHANDLE {
		HT_API HTHANDLE();
		HT_API HTHANDLE(int Capacity, int SecSnapshotInterval, int MaxKeyLength, int MaxPayloadLength, const wchar_t FileName[512]);
		int     Capacity;               // ������� ��������� � ���������� ��������� 
		int     SecSnapshotInterval;    // ������������� ���������� � ���. 
		int     MaxKeyLength;           // ������������ ����� �����
		int     MaxPayloadLength;       // ������������ ����� ������
		char    FileName[512];          // ��� ����� 
		HANDLE  File;                   // File HANDLE != 0, ���� ���� ������
		HANDLE  FileMapping;            // Mapping File HANDLE != 0, ���� mapping ������  
		LPVOID  Addr;                   // Addr != NULL, ���� mapview ��������  
		char    LastErrorMessage[512];  // ��������� �� ��������� ������ ��� 0x00  
		time_t  lastsnaptime;           // ���� ���������� snap'a (time())  

		void* elements;
		HANDLE Mutex;
		wchar_t MutexName[512];
		wchar_t HTUsersGroup[512];
		int N;							//������� ����������
		HANDLE SnapshotThread;
	};
}
