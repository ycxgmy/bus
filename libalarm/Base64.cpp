#ifdef WIN32
#include "stdafx.h"
#endif

#include "Base64.h"

char
Base64::values[] = {
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,62,-1,-1,-1,63,
	52,53,54,55,56,57,58,59,60,61,-1,-1,-1, -2,-1,-1,
	-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,
	15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1,
	-1,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,
	41,42,43,44,45,46,47,48,49,50,51,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
};

char
Base64::codes[] = {
	'A','B','C','D','E','F','G','H','I','J',
	'K','L','M','N','O','P','Q','R','S','T',
	'U','V','W','X','Y','Z','a','b','c','d',
	'e','f','g','h','i','j','k','l','m','n',
	'o','p','q','r','s','t','u','v','w','x',
	'y','z','0','1','2','3','4','5','6','7',
	'8','9','+','/'
};

void
Base64::encode(string& str)
{
        string tmpStr  = str;
	str.erase();
	int    len   = tmpStr.length();
	int    j     = 0;
	for(int i = 0; i < len;)
	{
		char   ech[] = {0,0,0,0};
		unsigned char   vch[] = {0,0,0};
		for(j = 0; (j < 3) && (i < len); i++)
			vch[j++] = tmpStr[i];
		if(j > 0)
		{
		    ech[0] = codes[vch[0] >> 2];
		    ech[1] = codes[((vch[0] & 3) << 4)   | (vch[1] >> 4)];
			ech[2] = codes[((vch[1] & 0xF) << 2) | (vch[2] >> 6)];
			ech[3] = codes[vch[2] & 0x3F];
			while(j < 3)
				ech[++j] = '=';
			str.append(ech,4);
		}
	}
}

void Base64::encode(const char *buf, int buflen, string &out)
{
  out.erase();
  int j = 0;
  for(int i = 0; i < buflen; ) {
      char   ech[] = {0,0,0,0};
      unsigned char   vch[] = {0,0,0};
      for(j = 0; (j < 3) && (i < buflen); i++)
	vch[j++] = buf[i];
      if(j > 0)
	{
	  ech[0] = codes[vch[0] >> 2];
	  ech[1] = codes[((vch[0] & 3) << 4)   | (vch[1] >> 4)];
	  ech[2] = codes[((vch[1] & 0xF) << 2) | (vch[2] >> 6)];
	  ech[3] = codes[vch[2] & 0x3F];
	  while(j < 3)
	    ech[++j] = '=';
	  out.append(ech,4);
	}
    }
}

void
Base64::decode(string& str)
{
	string tmpStr  = str.c_str();
	str.erase();
	int    len   = tmpStr.length();
	char   tmp;
	int    j     = 0;
	for(int i = 0; i < len;)
	{
		unsigned char   vch[] = {0,0,0,0};
		char   dch[] = {0,0,0};
		for(j = 0; (j < 4) && (i < len); i++)
		{
			tmp = values[(unsigned char)tmpStr[i]];
			if(tmp >= 0)
				vch[j++] = tmp;			
		}
		dch[0] = (vch[0] << 2) | (vch[1] >> 4);
		dch[1] = (vch[1] << 4) | (vch[2] >> 2);
		dch[2] = (vch[2] << 6) | vch[3];
		if(j > 0)
			str.append(dch,--j);
	}
}
