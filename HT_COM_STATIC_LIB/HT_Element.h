#pragma once
namespace HT {
#define HT_EXPORTS

#ifdef HT_EXPORTS
#define HT_API __declspec(dllexport)
#else
#define HT_API __declspec(dllimport)
#endif

	struct Element {
		Element();
		Element(const void* key, int keylength);                                             // for Get
		HT_API Element(const void* key, int keylength, const void* payload, int  payloadlength);    // for Insert
		Element(Element* oldelement, const void* newpayload, int  newpayloadlength);         // for update
		void* key;                 // �������� ����� 
		int   keylength;           // ������ �����
		void* payload;             // ������ 
		int   payloadlength;       // ������ ������
	};
	HT_API void print(const Element* element);
}