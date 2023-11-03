/*
 * File: ValidDirectoryName.c
 * Copyright (c) 2023 Loupe
 * https://loupe.team
 * 
 * This file is part of DirIOWrap, licensed under the MIT License.
 */

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
