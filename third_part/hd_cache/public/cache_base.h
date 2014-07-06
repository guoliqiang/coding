#ifndef CACHE_BASE_H
#define CACHE_BASE_H

#include <errno.h>
#include <pthread.h>
#include <assert.h>

#define RT_NOTICE_PROCESSED              1
#define RT_NOTICE_NONE_PROCESSED         0
#define RT_ERROR_BEYOND_LIMIT           -1
#define RT_ERROR_GENERAL_ERROR          -2
#define RT_ERROR_GETLOCK_ERROR			    -3
typedef unsigned int u_int;
#endif

