#include "stdafx.h"
#include "DocumentProvider.h"
#include <dirent.h>
#include<string>
#include<algorithm>
#include <sstream>
#include <sys/types.h>
#include <iostream>
using namespace std;

DocumentProvider::DocumentProvider(string repositoryPath)
{
	DIR *directory = NULL;
	struct dirent *fileEntity;

	directory = opendir(repositoryPath.c_str());

	if (directory != NULL) {
		while ((fileEntity = readdir(directory)) != NULL) {
			if (strcmp(fileEntity->d_name, ".") != 0 && strcmp(fileEntity->d_name, "..") != 0)
				pathList.push_back(repositoryPath + "\\" + string(fileEntity->d_name));
		}
		closedir(directory);
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
			if (token.compare("</DOC>") == 0)
			{
				break;
			}
			else
			{
				token = transformToken(token);
				if (token != "")
					documentText += token + " ";
			}
		}

		//The first string in text is the DOCNO, erase "la" and "-" from it then convert it to unsigned int
		istringstream  iss(documentText);
		iss >> token;
		token = token.erase(0, 2).erase(6, 1);
		documentMetaData.address = stoul(token, nullptr, 10);

		//The second string in text is the DOCID, convert it to unsigned int
		iss >> token;
		documentMetaData.id = stoul(token, nullptr, 10);

		//Store the offset of the current document
		documentMetaData.offset = currentDocumentOffset;

		//Finally construct the document
		document = new Document(documentMetaData, documentText);

		// IF All DOCUMENTS in the current FILE have been processed then close the file and move to the next one
		if (currentFile.eof()) {
			currentFileIndex++;
			currentFile.close();
		}
	}
	return document;
}


// This function eliminates HTML tags, punctuation and convert tokens to lower case
string DocumentProvider::transformToken(string &token)
{
	unsigned i = 0;

	if (token.find("<") == 0 || token.find("</") == 0 || token.find("CHJ=") == 0 || token.find("CVJ=") == 0)
	{
		return "";
	}

	else
	{
		while (i < token.length())
		{
			token[i] = tolower(token[i]);
			if (ispunct(token[i]) && token[i] != '-' && token[i] != '\'')
			{
				token.erase(i, 1);
			}
			else
			{
				i++;
			}
		}

	}
	return token;
}
