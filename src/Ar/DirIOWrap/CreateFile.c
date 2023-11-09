/*
 * File: CreateFile.c
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
#ifdef __cplusplus
	};
#endif
/* Create a file and write data to it */
void CreateFile(struct CreateFile* inst)
{

	FileCreate(&inst->internal.FileCreate);
		
		
	if(inst->internal.FileCreate.status == ERR_OK){

		//We are done opening
		inst->internal.FileCreate.enable= 	0;
		
		inst->internal.FileWrite.enable=	1;
		inst->internal.FileWrite.ident=		inst->internal.FileCreate.ident;
		inst->internal.FileClose.ident=		inst->internal.FileCreate.ident;

		inst->internal._Active= 1;
		inst->internal._Open=	1;
	}
	else if(inst->internal.FileCreate.status == ERR_FUB_ENABLE_FALSE
	){
		if(inst->Execute 
			&& !inst->internal._Execute
			&& !inst->internal._Active
			&& inst->pData	!=	0
		){

			inst->internal._Execute=	1;
			
			inst->internal.FileCreate.enable=	1;
			inst->Status= ERR_FUB_BUSY;

			inst->internal.FileCreate.pDevice=	(UDINT)inst->FileDevice;
			inst->internal.FileCreate.pFile=	(UDINT)inst->FileName;
				
			inst->internal.FileWrite.len=		inst->DataLen;
			inst->internal.FileWrite.pSrc=		inst->pData;
			inst->internal.FileWrite.offset=	0;
		
		}
		else if(!inst->Execute){
			
			inst->Status= ERR_FUB_ENABLE_FALSE;
			
		}
	}
	else if(inst->internal.FileCreate.status != ERR_FUB_BUSY){

		//Tell the user we failed
		inst->internal.FileCreate.enable= 0;
		inst->Status= inst->internal.FileCreate.status;		

	}
	
	
	FileWrite(&inst->internal.FileWrite);

	if(inst->internal.FileWrite.status == ERR_OK){
		inst->internal.FileWrite.enable= 0;
		inst->internal._Active=	0;
	}
	else if(inst->internal.FileWrite.status != ERR_FUB_BUSY
	&&		inst->internal.FileWrite.status != ERR_FUB_ENABLE_FALSE
	){
		inst->internal.FileWrite.enable= 0;
		inst->Status=	inst->internal.FileWrite.status;		
		inst->internal._Active=	0;
	}

	//Free up any resources
	if( inst->internal._Open && inst->internal._Active == 0){
		inst->internal.FileClose.enable=	1;		
	}

	FileClose(&inst->internal.FileClose);
	
	if(	inst->internal.FileClose.status	!= ERR_FUB_BUSY 
	&&	inst->internal.FileClose.status	!= ERR_FUB_ENABLE_FALSE
	){

		inst->Status=	inst->internal.FileClose.status;		
		inst->internal.FileClose.enable=	0;
		inst->internal._Open=	0;		
	}
	
	if( !inst->Execute ){

		inst->internal._Execute=	0;

	}
	
}
