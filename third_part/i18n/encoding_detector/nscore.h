#ifndef nsCore_h__
#define nsCore_h__
#include <stdio.h>
#include <malloc.h>

typedef int PRInt32;
typedef unsigned int PRUint32;
typedef bool PRBool;
typedef short PRInt16;
typedef PRUint32 nsresult;
#define PR_FALSE false
#define PR_TRUE true
#define nsnull NULL
#define PR_MALLOC malloc
#define PR_FREEIF(x) do { if(x) free(x); } while(0)
#endif


