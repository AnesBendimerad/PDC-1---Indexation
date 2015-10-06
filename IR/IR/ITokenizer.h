#pragma once
#include <string>
using namespace std;

class ITokenizer {
public:
	virtual string getNextToken() = 0;
};