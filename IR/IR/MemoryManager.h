#pragma once
#include <windows.h>
#include "psapi.h"
class MemoryManager {
private:
	
public:
	static unsigned long long getCurrentWorkingSetSize();
	static unsigned long long getPeakWorkingSetSize();
	static unsigned int getDiskSectorSize(); //in Bytes
};