#pragma once
#include <string>
using namespace std;

class IHasher {
public:
	virtual int hash(string token) = 0;
};