#include "pch.h"
#include "Hash.h"
namespace HASH {
	int HashFunction(const void* message,const int messageLength, int size, int p)    //Хеш-функция
	{
		char* arrKeyBytes = new char[messageLength];
		memcpy(arrKeyBytes, message, messageLength);
		int sumBytes = 0;
		for (int i = 0; i < messageLength; i++) {
			sumBytes += arrKeyBytes[i];
		}
		double key2 = 5 * ((0.6180339887499 * sumBytes) - int((0.6180339887499 * sumBytes)));
		delete[] arrKeyBytes;
		return (p + sumBytes) % size;
	}
	int Next_hash(int hash, int size, int p) { 
		//return (hash + 5 * p + 3 * p * p) % size;

		return (hash + 1) % size;}
}