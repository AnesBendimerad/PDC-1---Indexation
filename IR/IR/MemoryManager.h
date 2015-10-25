#pragma once
#include <windows.h>
#include "psapi.h"
class MemoryManager {
private:
	
public:
	static unsigned long long getWorkingSetSize();
	static unsigned int getDiskSectorSize(); //in Bytes
};