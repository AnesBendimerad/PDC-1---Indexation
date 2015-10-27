#include<unordered_set>

std::unordered_set<std::string> * getStopWords(){

	std::unordered_set<std::string> * stopWords = new std::unordered_set<std::string>();
	 
	stopWords->insert("");
	stopWords->insert("<p>");
	stopWords->insert("</p>");
	stopWords->insert("and");
	stopWords->insert("about");
	stopWords->insert("a");
	stopWords->insert("an");
	stopWords->insert("are");
	stopWords->insert("as");
	stopWords->insert("at");
	stopWords->insert("be");
	stopWords->insert("by");
	stopWords->insert("com");
	stopWords->insert("for");
	stopWords->insert("from");
	stopWords->insert("how");
	stopWords->insert("in");
	stopWords->insert("is");
	stopWords->insert("it");
	stopWords->insert("of");
	stopWords->insert("on");
	stopWords->insert("or");
	stopWords->insert("that");
	stopWords->insert("the");
	stopWords->insert("this");
	stopWords->insert("to");
	stopWords->insert("was");
	stopWords->insert("what");
	stopWords->insert("when");
	stopWords->insert("where");
	stopWords->insert("who");
	stopWords->insert("will");
	stopWords->insert("with");
	stopWords->insert("the");
	stopWords->insert("www");

	return stopWords;
}