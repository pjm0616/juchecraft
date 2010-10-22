#ifndef SC_COMPAT_H_
#define SC_COMPAT_H_

// FIXME
#ifdef __WIN32__
#ifndef srand48
# define srand48 srand
#endif
#ifndef lrand48
# define lrand48 rand
#endif
#ifndef usleep
# define usleep(x) Sleep((x) / 1000)
#endif
#ifndef getpid
# define getpid() 0
#endif
#endif

#endif

