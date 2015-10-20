#pragma once
#include <string>
#include "Term.h"
#include "IIterator.h"
using namespace std;

class IDictionary {
public:
	virtual Term* addTerm(string token) = 0;
	virtual void addTerm(Term* term) = 0;
	virtual Term* getTerm(string token) = 0;
	virtual IIterator * getIterator()=0;
	virtual unsigned long long& getTermsNumber() = 0;
	virtual unsigned long long getMemorySize() = 0;

	virtual unsigned long long getTokenId(string token) = 0;
	virtual Term* getTermById(unsigned long long id) = 0;
};