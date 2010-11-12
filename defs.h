// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#ifndef SC_DEFS_H_
#define SC_DEFS_H_


#ifdef __GNUC__
# define likely(expr_) __builtin_expect((expr_), 1)
# define unlikely(expr_) __builtin_expect((expr_), 0)
# define prefetch_read(addr_) __builtin_prefetch((addr_), 0)
# define prefetch_write(addr_) __builtin_prefetch((addr_), 1)
# define ATTRIBUTE_UNUSED __attribute__((unused))
# define ATTRIBUTE_ALWAYS_INLINE __attribute__((always_inline))
#else
# define likely(expr_) (expr_r)
# define unlikely(expr_) (expr_)
# define prefetch_read(addr_) do{}while(0)
# define prefetch_write(addr_) do{}while(0)
# define ATTRIBUTE_UNUSED
# define ATTRIBUTE_ALWAYS_INLINE
#endif


#ifndef M_PI
# define M_PI 3.14159265358979323846
#endif

#ifdef __WIN32__
# define snprintf _snprintf
#endif

#ifdef __WIN32__
#define GAME_ROOT_DIR "../../"
#define GAME_DATA_DIR "../../../mini_sc_data/"
#else
#define GAME_ROOT_DIR "./"
#define GAME_DATA_DIR "../mini_sc_data/"
#endif

#endif

