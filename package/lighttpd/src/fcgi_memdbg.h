#define DBG_MEMORY_ALLOC
#ifdef DBG_MEMORY_ALLOC
#include <string.h>
static void*__malloc_dbg_ptr = NULL;
#warning "Using dbg malloc ------------------------------------------------------------------------ "
static char __memdbgbuf[256];
static int __memdbgbuf_strlen = 0;
#define __ax_malloc(__SZ__) ((__memdbgbuf_strlen = sprintf(__memdbgbuf, "%s(%d) malloc:%p\n", __func__, __LINE__, \
                (__malloc_dbg_ptr = (void*)malloc(__SZ__)))) && write(2, __memdbgbuf, __memdbgbuf_strlen) ?\
                 __malloc_dbg_ptr : NULL)
#define __ax_calloc(__NUM__, __SZ__) ((__memdbgbuf_strlen = sprintf(__memdbgbuf, "%s(%d) calloc:%p\n", __func__, __LINE__, \
                (__malloc_dbg_ptr = (void*)calloc(__NUM__, __SZ__)))) && write(2, __memdbgbuf, __memdbgbuf_strlen) ? \
                 __malloc_dbg_ptr : NULL)
#define __ax_realloc(__PTR__, __SZ__) ((__memdbgbuf_strlen = sprintf(__memdbgbuf, "%s(%d) realloc:%p\n", __func__, __LINE__, \
                (__malloc_dbg_ptr = (void*)realloc(__PTR__, __SZ__)))) && write(2, __memdbgbuf, __memdbgbuf_strlen) ? \
                 __malloc_dbg_ptr : NULL)
#define __ax_strdup(__STR__) ((__memdbgbuf_strlen = sprintf(__memdbgbuf, "%s(%d) strdup alloc:%p\n", __func__, __LINE__, \
                (__malloc_dbg_ptr = (void*)strdup(__STR__)))) && write(2, __memdbgbuf, __memdbgbuf_strlen) ? \
                 __malloc_dbg_ptr : NULL)
#define __ax_strndup(__STR__, __SZ__) ((__memdbgbuf_strlen = sprintf(__memdbgbuf, "%s(%d) strndup alloc:%p\n", __func__, __LINE__, \
                (__malloc_dbg_ptr = (void*)strndup(__STR__, __SZ__)))) && write(2, __memdbgbuf, __memdbgbuf_strlen) ? \
                 __malloc_dbg_ptr : NULL)
#define __ax_asprintf(__STRBUF__, __FMT__, ...) { asprintf(__STRBUF__, __FMT__, ##__VA_ARGS__); \
                                                  __memdbgbuf_strlen = sprintf(__memdbgbuf, "%s(%d) asprintf alloc:%p\n", __func__, __LINE__, * __STRBUF__); \
                                                  write(2, __memdbgbuf, __memdbgbuf_strlen);}
#define __ax_free(__PTR__) { __memdbgbuf_strlen = sprintf(__memdbgbuf, "%s(%d) freed:%p\n", __func__, __LINE__, __PTR__); \
                             write(2, __memdbgbuf, __memdbgbuf_strlen); \
                             free(__PTR__); \
                             __PTR__ = NULL; }
#else
#define __ax_malloc malloc
#define __ax_calloc calloc
#define __ax_realloc realloc
#define __ax_strdup strdup
#define __ax_strndup strndup
#define __ax_asprintf asprintf
#define __ax_free free
#endif
