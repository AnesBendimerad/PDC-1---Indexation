#include "stdafx.h"
#include "FileManager.h"

ifstream * FileManager::openIfstream(string fileName)
{
	ifstream *inputStream=new ifstream(fileName, ios::in | ios::binary);
	if (inputStream->fail()) {
		throw runtime_error("Failed when openning file");
	}
	return inputStream;
}

ofstream * FileManager::openOfstream(string fileName)
{
	ofstream *outputStream = new ofstream(fileName, ios::out | ios::binary);
	if (outputStream->fail()) {
		throw runtime_error("Failed when openning file");
	}
	return outputStream;
}

DIR * FileManager::openDirectory(string directoryName)
{
	DIR *directory = opendir(directoryName.c_str());
	if (directory == NULL) {
		throw runtime_error("Failed when openning directory");
	}
	return directory;
}
