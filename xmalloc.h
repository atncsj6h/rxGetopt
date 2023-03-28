#ifndef xmalloc_h_included
#define xmalloc_h_included

#if defined(__cplusplus) && __cplusplus
 extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>

static inline void *xmalloc(const size_t size) {
  void *ret = malloc(size);
  if (!ret && size)
    fprintf(stderr, "%s: error: cannot allocate %zu bytes", "rxgetopt", size);
  return ret ;
}

static inline void *xrealloc(void *ptr, const size_t size)
{
  void *ret = realloc(ptr, size);
  if (!ret && size)
    fprintf(stderr, "%s: error: cannot allocate %zu bytes", "rxgetopt", size);
  return ret;
}

#if defined(__cplusplus) && __cplusplus
 }
#endif

#endif // xmalloc_h_included
