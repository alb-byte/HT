#include "pch.h"
#include "HT_Storage.h"

namespace HT {
	HTHANDLE::HTHANDLE() {}
	HTHANDLE::HTHANDLE(
		int Capacity, int SecSnapshotInterval, int MaxKeyLength,
		int MaxPayloadLength, const wchar_t FileName[512]
	) {
		this->Capacity = Capacity; this->SecSnapshotInterval = SecSnapshotInterval;
		this->MaxKeyLength = MaxKeyLength; this->MaxPayloadLength = MaxPayloadLength;
		this->N = 0;
		memcpy(this->FileName, FileName, sizeof(this->FileName));
	}
}