#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>

void* malloc(size_t sz)
{
    char *p = NULL;
    void *(*libc_malloc)(size_t) = dlsym(RTLD_NEXT, "malloc");
    p = libc_malloc(sz);
    printf("malloc\t 0x%x (%d) \n",p,sz);
    return p;
}

void free(void *p)
{
    void (*libc_free)(void*) = dlsym(RTLD_NEXT, "free");
    if(p != NULL)
        printf("free\t 0x%x \n",p);
        
    libc_free(p);
}
