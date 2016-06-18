
#ifndef _Base64_H
#define _Base64_H

//#include "ScoMsgParserDll.h"
//#include <stdio.h>
#include <string>

using std::string;

class Base64
{
public:
	static void encode(string& str);
	static void decode(string& str);

private:
	static char codes[];
	static char values[];
};
#endif
