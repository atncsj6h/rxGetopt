#ifndef xstringops_h_included
#define xstringops_h_included

#if defined(__cplusplus) && __cplusplus
 extern "C" {
#endif

#include <stdlib.h>
#include <string.h>

static inline char * strtolower(const char *str)
{
  char * res = (char *)malloc( strlen(str) + 1 ) ;
  const char *s = str;
  char *r = res;
  for (; *s; s++, r++ ) {
    *r = ( *s < 'A' || *s > 'Z' ) ? *s : *s - 32 ;
  }
  *r = 0;
  return res;
}

static inline char * strtoupper(const char *str)
{
  char * res = (char *)malloc( strlen(str) + 1 ) ;
  const char *s = str;
  char *r = res;
  for (; *s; s++, r++ ) {
    *r = ( *s < 'a' || *s > 'z' ) ? *s : *s - 32 ;
  }
  *r = 0;
  return res;
}

#if defined(__cplusplus) && __cplusplus
 }
#endif

#endif  //  xstringops_h_included
