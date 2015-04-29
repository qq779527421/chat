/*************************************************************************
	> File Name: debug.h
	> Author: 
	> Mail: 
	> Created Time: 2015年04月29日 星期三 21时43分24秒
 ************************************************************************/

#ifndef QG_DEBUG_H
#define QG_DEBUG_H

#ifdef DEBUG3
#ifndef DEBUG2
#define DEBUG2
#endif
#define debug3(...) fprintf(stderr, __VA_ARGS__)
#else
#define debug3(...)
#endif

#ifdef DEBUG2
#ifndef DEBUG1
#define DEBUG1
#endif
#define debug2(...) fprintf(stderr, __VA_ARGS__)
#else
#define debug2(...)
#endif

#ifdef DEBUG
#ifndef DEBUG1
#define DEBUG1
#endif
#endif

#ifdef DEBUG1
#ifndef DEBUG
#define DEBUG
#endif
#define debug1(...) fprintf(stderr, __VA_ARGS__)
#else
#define debug1(...)
#endif

#ifdef DEBUG
#define debug(...) fprintf(stderr, __VA_ARGS__)
#else
#define debug(...)
#endif

#endif 
