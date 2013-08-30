#ifndef CONFIG_H
#define CONFIG_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
# define BUILD_WIN32
#endif

#if defined(BUILD_WIN32) && !defined(BUILD_NO_DLL)
# ifdef CLASS_IMPLEMENTATION
# define CLASS_EXPORT __declspec(dllexport)
#else
# define CLASS_EXPORT __declspec(dllimport)
# endif
# else
# define CLASS_EXPORT
#endif

#endif

