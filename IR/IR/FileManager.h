#pragma once
#include <string>
#include <fstream>
#include <dirent.h>
using namespace::std;
class FileManager {
private:
	
public:
	static ifstream* openIfstream(string fileName);
	static ofstream* openOfstream(string fileName);
	static DIR * openDirectory(string directoryName);
};