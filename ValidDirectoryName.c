/********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Library: DirIOWrap
 * File: ValidDirectoryName.c
 * Author: Josh
 * Created: April 24, 2015
 ********************************************************************
 * Implementation of library DirIOWrap
 ********************************************************************/

#include <bur/plctypes.h>
#ifdef __cplusplus
	extern "C"
	{
#endif

	#include "DirIOWrap.h"
	#include <string.h>

#ifdef __cplusplus
	};
#endif

/* TODO: Add your comment here */
plcbit ValidDirectoryName(unsigned long Name)
{
	if(Name==0) return 0;
	return strcmp((char*)Name,"")!=0;	
}
