#pragma once
#include <Windows.h>
namespace HASH{
	int HashFunction(const void* message, int messageLength, int size, int p);
	int Next_hash(int hash, int size, int p);
}