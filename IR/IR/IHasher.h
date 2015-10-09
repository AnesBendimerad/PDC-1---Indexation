#pragma once
#include <string>
using namespace std;

class IHasher {
public:
	virtual unsigned int hash(string token) = 0;
};