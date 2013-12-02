#include "expatxml.h"
#include <stdlib.h>
#include <algorithm>
#include <cstring>
#include <sstream>

namespace com {
namespace xiaonei {
namespace talk {
namespace util {

bool ExpatTool::IsValid(std::string xml){
	XML_Parser parser = XML_ParserCreate(NULL);               
	if (!parser) {                  
	//	MCE_WARN("XMLGenerator::ValidCheck   Message---->    Fail to create Parser..");
		return false;                             
	}
	if (!XML_Parse(parser, xml.c_str(), xml.size(), 1)) {
	//MCE_INFO("XMLGenerator::ValidCheck  Message---->    xml error:"<<xml << " line:" << XML_GetCurrentLineNumber(parser)
	//	<< "   errorstring:" <<XML_ErrorString(XML_GetErrorCode(parser)) );                              
	XML_ParserFree(parser);                                   
	return false;
	}
	XML_ParserFree(parser);
	return true;
}


}
}
}
}
