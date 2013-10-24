#include "XmlParser.h"


XmlParser::XmlParser(std::string file_path)
{
	doc.load_file(file_path.c_str());
	

}

XmlParser::XmlParser(void)
{
}


XmlParser::~XmlParser(void)
{
}


