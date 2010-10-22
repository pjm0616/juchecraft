#ifndef SC_DEFS_H_
#define SC_DEFS_H_

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

