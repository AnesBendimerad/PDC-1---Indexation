#pragma once
#include <string>
#include "Term.h"
using namespace std;

class IDictionary {
public:
	virtual Term addTerm(string token) = 0;
	virtual Term getTerm(string token) = 0;
};