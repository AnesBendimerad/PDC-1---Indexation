#include "stdafx.h"
#include "DocumentProvider.h"
#include <dirent.h>
#include<string>
#include<algorithm>
#include <sstream>
#include <sys/types.h>
#include <iostream>
#include "FileManager.h"
using namespace std;

DocumentProvider::DocumentProvider(string repositoryPath)
{
	DIR *directory = NULL;
	struct dirent *fileEntity;

	directory = FileManager::openDirectory(repositoryPath);
	if (directory != NULL) {
		while ((fileEntity = readdir(directory)) != NULL) {
			if (strcmp(fileEntity->d_name, ".") != 0 && strcmp(fileEntity->d_name, "..") != 0)
				pathList.push_back(repositoryPath + "\\" + string(fileEntity->d_name));
		}
		closedir(directory);
	}
	if (pathList.empty()) {
		throw runtime_error("The directory is empty");
	}
}



DocumentProvider::~DocumentProvider()
{
	//dtor
}

Document* DocumentProvider::getNextDocument()
{
	Document *document = nullptr ;
	DocumentMetaData documentMetaData;
	unsigned int currentDocumentOffset;
	string documentText;
	string token;
	
	//If all FILES have been processed then we return null
	if (currentFileIndex != pathList.size()) 
	{
		if (!currentFile.is_open())
		{
			currentFile.open(pathList.at(currentFileIndex));
		}

		//Save the offset of the current document in the file
		currentDocumentOffset = (unsigned int) currentFile.tellg();

		while (currentFile >> token) {
			//The end of the current DOCUMENT has been reached
			if (token.compare("<DOCNO>") == 0)
			{
				currentFile >> token;
				token = token.erase(0, 2).erase(6, 1);
				documentMetaData.address= stoul(token, nullptr, 10);
			}
			else if(token.compare("<DOCID>") == 0)
			{
				currentFile >> token;
				documentMetaData.id = stoul(token, nullptr, 10);
			}
			else if (token.compare("<HEADLINE>") == 0)
			{
				while (currentFile >> token && token.compare("</HEADLINE>") != 0)
					documentText += token + " ";
			}
			else if (token.compare("<TEXT>") == 0)
			{
				while (currentFile >> token && token.compare("</TEXT>") != 0)
					documentText += token + " ";
			}
			else if (token.compare("<GRAPHIC>") == 0)
			{
				while (currentFile >> token && token.compare("</GRAPHIC>") != 0)
					documentText += token + " ";
			}
			else if (token.compare("</DOC>") == 0)
			    { 
					currentFile >> token;
					// IF All DOCUMENTS in the current FILE have been processed then close the file and move to the next one
					if (currentFile.eof())
					{
						cout << "numero:" << currentFileIndex << endl;
						currentFileIndex++;
						currentFile.close();
					}
					break;
				}
		}
		

		//Store the offset of the current document
		documentMetaData.offset = currentDocumentOffset;

		//Finally construct the document
		document = new Document(documentMetaData, documentText);
	}
	return document;
}
