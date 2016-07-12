
#ifndef _Base64_H
#define _Base64_H

//#include "ScoMsgParserDll.h"
//#include <stdio.h>
#include <string>
#include "config.h"

using std::string;

class APIDEC Base64
{
public:
	static void encode(string& str);
	static void encode(const char *buf, int buflen, string & out);
	static void decode(string& str);

private:
	static char codes[];
	static char values[];
};
#endif
