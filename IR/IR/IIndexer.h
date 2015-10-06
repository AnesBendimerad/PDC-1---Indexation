#pragma once
#include "IIndex.h"
class IIndexer {
public:
	virtual IIndex createIndex() = 0;
};