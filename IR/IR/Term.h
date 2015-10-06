#pragma once
#include <string>
using namespace std;

typedef struct Term {
	string token;
	unsigned int totalFrequency;
	unsigned int documentNumber;
	void* postingList;
} Term;