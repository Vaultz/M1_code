/* support_thread_posix.c
 * programme testant les possibilites d’ordonnancement des threads posix
 * n 2006 */
#define _POSIX_SOURCE
#include <unistd.h>
#include <stdio.h>

int main(void) {
    printf("_POSIX_VERSION=%ld\n", _POSIX_VERSION);
#if _POSIX_VERSION < 199506L
    printf("sans thread posix\n");
#else
    printf("avec thread posix\n");
#ifdef _POSIX_PTHREAD_PRIORITY_SCHEDULING
    printf("avec ordonnancement des threads\n");
#else
    printf("sans ordonnancement des threads\n");
#endif /* _POSIX_PTHREAD_PRIORITY_SCHEDULING */
#endif /* _POSIX_VERSION < 199506L */
    return 0;
}
