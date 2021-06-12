#include "HT_Element.h"
#include <iostream>
#include <iomanip>

namespace HT {
	Element::Element() {
		this->key = nullptr; this->keylength = NULL;
		this->payload = nullptr; this->payloadlength = NULL;
	}
	Element::Element(const void* key, int keylength) {
		Element(); this->key = (void*)key; this->keylength = keylength;
	}
	Element::Element(const void* key, int keylength, const void* payload, int  payloadlength) {
		this->key = (void*)key; this->keylength = keylength;
		this->payload = (void*)payload; this->payloadlength = payloadlength;
	}
	Element::Element(Element* oldelement, const void* newpayload, int  newpayloadlength) {
		this->key = oldelement->key; this->keylength = oldelement->keylength;
		this->payload = (void*)payload; this->payloadlength = payloadlength;
	}

	void print(const Element* element) {
		std::cout << std::right << std::setfill('=') << std::setw(30) << '|' << std::endl;
		std::cout << std::left << std::setfill(' ') << std::setw(10) << "KEY";
		std::cout << (char*)element->key;
		std::cout << std::endl << std::right << std::setfill('-') << std::setw(6) << '|' << std::endl;
		std::cout << std::left << std::setfill(' ') << std::setw(10) << "VALUE";
		std::cout << (char*)element->payload;
		std::cout << std::endl;
		std::cout << std::right << std::setfill('=') << std::setw(30) << '|' << std::endl;
	}
}