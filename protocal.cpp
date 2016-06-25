#include "protocal.h"
#ifdef CURLPP
#else
unsigned char bufff[40960];
int l = 0;
int writer(unsigned char *data, size_t size, size_t nmemb, unsigned char *writerData)
{
  int realsize = size * nmemb;
  memcpy(bufff + l, data, realsize);
  l += realsize;
  return realsize;
}
#endif

