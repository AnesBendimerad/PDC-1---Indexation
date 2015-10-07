#pragma once
#include "Document.h"
class IDocumentProvider {
public:
	virtual Document* getNextDocument() = 0;
};