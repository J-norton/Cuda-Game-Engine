#pragma once
#include <string>
#include <pugixml.hpp>

class XmlParser
{
private:	
	pugi::xml_document doc;
public:
	void loadResourceFile(std::string filepath);
	XmlParser(void);
	//Creates an XML parser and loads all resources specified by the file
	XmlParser(std::string resource_path);
	XmlParser& operator=(const XmlParser& rhs);
	~XmlParser(void);
};

