#include "stdafx.h"
#include "MemoryManager.h"

unsigned long long MemoryManager::getWorkingSetSize()
{
	PROCESS_MEMORY_COUNTERS pmc;
	if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
	{
		return pmc.WorkingSetSize;
		/*
		std::cout << "\tPageFaultCount:\t" << pmc.PageFaultCount / 1024 << "KB" << std::endl;
		std::cout << "\tPeakWorkingSetSize:\t" << pmc.PeakWorkingSetSize / 1024 << "KB" << std::endl;
		std::cout << "\tQuotaPeakPagedPoolUsage: \t" << pmc.QuotaPeakPagedPoolUsage / 1024 << "KB" << std::endl;
		std::cout << "\tQuotaPagedPoolUsage: \t" << pmc.QuotaPagedPoolUsage / 1024 << "KB" << std::endl;
		std::cout << "\tQuotaPeakNonPagedPoolUsage:\t" << pmc.QuotaPeakNonPagedPoolUsage / 1024 << "KB" << std::endl;
		std::cout << "\tQuotaNonPagedPoolUsage:\t" << pmc.QuotaNonPagedPoolUsage / 1024 << "KB" << std::endl;
		std::cout << "\tPagefileUsage:\t" << pmc.PagefileUsage / 1024 << "KB" << std::endl;
		std::cout << "\tPeakPagefileUsage:\t" << pmc.PeakPagefileUsage / 1024 << "KB" << std::endl;
		*/
	}
	CloseHandle(GetCurrentProcess());
}
