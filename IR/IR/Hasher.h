#pragma once
#include "IHasher.h"
class Hasher : public IHasher {
public:
	unsigned int hash(string token);
};
