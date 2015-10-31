#include "stdafx.h"
#include "MemoryManager.h"

unsigned long long MemoryManager::getCurrentWorkingSetSize()
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
	return 0;
}

unsigned long long MemoryManager::getPeakWorkingSetSize()
{
	PROCESS_MEMORY_COUNTERS pmc;
	if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
	{
		return pmc.PeakWorkingSetSize;
	}
	CloseHandle(GetCurrentProcess());
	return 0;
}


unsigned int MemoryManager::getDiskSectorSize()
{
	LPCWSTR pszDrive = NULL;
	DWORD lpSectorsPerCluster, lpBytesPerSector, lpNumberOfFreeClusters, lpTotalNumberOfClusters;
	GetDiskFreeSpace(pszDrive, &lpSectorsPerCluster, &lpBytesPerSector, &lpNumberOfFreeClusters, &lpTotalNumberOfClusters);
	return lpSectorsPerCluster*lpBytesPerSector;
}
