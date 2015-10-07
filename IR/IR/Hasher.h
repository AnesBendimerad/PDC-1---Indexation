#pragma once
#include "IHasher.h"
class Hasher : public IHasher {
public:
	int hash(string token);
};
