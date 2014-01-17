#include <stdlib.h>
#include <stdio.h>

int main (int argc, char* argv[]) {
    char *p;
    int mem_mb = 500;
    int timeout = 2000000;
    if(argc == 3)
    {
        mem_mb = atoi(argv[1]);
        timeout = atoi(argv[2]);
    }
    printf("%s: allocating %d MB and sleeping for %d seconds. \n",argv[0],mem_mb,timeout);
    fflush(stdout);
    p = (char*) malloc(mem_mb*1024*1024);
    if(NULL == p) {
        printf("failed to allocatte memory.. exit. \n");
        return 0;
    }
    memset(p,0xff,mem_mb*1024*1024);
    sleep(timeout);
    free(p);
    return 1;
}
