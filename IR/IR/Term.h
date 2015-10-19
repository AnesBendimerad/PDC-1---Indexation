#pragma once
#include <string>
#include <fstream>
using namespace std;

typedef struct Term {
	string token="";
	unsigned int totalFrequency=1;
	unsigned int documentNumber=0;
	void* postingList=nullptr;
} Term;

ofstream & operator << (std::ofstream & os, Term * rec);
ifstream & operator >> (std::ifstream & is, Term * rec);