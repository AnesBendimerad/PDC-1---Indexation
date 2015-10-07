#pragma once
#include <string>
using namespace std;

typedef struct Term {
	string token="";
	unsigned int totalFrequency=1;
	unsigned int documentNumber=0;
	void* postingList=NULL;
} Term;