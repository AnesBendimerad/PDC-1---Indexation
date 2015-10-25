#pragma once
#include <string>
#define SIMPLE_TOKENIZER 0
#define STRTK_TOKENIZER 1
using namespace std;

class ITokenizer {
public:
	virtual string getNextToken() = 0;
};