#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <pthread.h>

static __thread int no_hook_malloc = 0;
static __thread int no_hook_free = 0;

#define MIN_ALLOC 4*1024


/*
 *	MALLOC 
 * 
 */
#if 1
void *(*libc_malloc)(size_t) = 0;
void* malloc(size_t sz)
{
    void *p = NULL;
    void *caller = NULL;
    if(libc_malloc == 0)
        libc_malloc = dlsym(RTLD_NEXT, "malloc");

    if(no_hook_malloc) {
        return libc_malloc(sz);
    }
    no_hook_malloc = 1;
    caller = __builtin_return_address(0);
    p = libc_malloc(sz);
    if(MIN_ALLOC <= sz) {
        printf("malloc:\t caller=0x%x ptr=0x%x sz=%d \n",caller,p,sz);
        fflush(stdout);
    }
    no_hook_malloc = 0;
    return p;
}
#endif
#if 1
static void (*libc_free)(void*)=0;
void free(void *p)
{
    if(libc_free == 0)
        libc_free = dlsym(RTLD_NEXT, "free");

    if(no_hook_free) {
        return libc_free(p);
    }
    no_hook_free = 1;
    printf("free:\t 0x%x \n",p);
    fflush(stdout);
    libc_free(p);
    no_hook_free = 0;
}
#endif

/*
 *	MMAP
 * 
 */
#include <sys/mman.h>
void *(*libc_mmap)(void*,size_t,int,int,int,off_t) = 0;
void *mmap (void *addr, size_t len, int prot,
       int flags, int fd, off_t offset)
{
    void *ret = NULL;
    if(libc_mmap == 0)
        libc_mmap = dlsym(RTLD_NEXT, "mmap");

    ret = libc_mmap(addr,len,prot,flags,fd,offset);
    printf("mmap:\t addr=0x%x len=0x%x fd=0x%x (returned: 0x%x) \n",addr,len,fd,ret);
    return ret;
}

static void *(*libc_munmap)(void*,size_t) = 0;
int munmap(void *start, size_t length)
{
    if(libc_munmap == 0)
        libc_munmap = dlsym(RTLD_NEXT, "munmap");

    printf("munmap:\t 0x%x 0x%x \n",start,length);
    return libc_munmap(start,length);
}
