/*
 * File: ReadFileVolatile.c
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


void ReadFileVolatile(struct  ReadFileVolatile* inst)
{
		
	inst->internal.FileOpen.mode=		fiREAD_ONLY;
	inst->internal.FileOpen.pDevice= 	(UDINT)inst->FileDevice;
	inst->internal.FileOpen.pFile=		(UDINT)inst->FileName;
	FileOpen(&inst->internal.FileOpen);

	if(inst->internal.FileOpen.status == ERR_OK){

		
		//We are done opening
		inst->internal.FileOpen.enable= 0;

		inst->internal._Open=	1;
		inst->internal.FileClose.ident=	inst->internal.FileOpen.ident;

		//Grab the read file length
		inst->DataLen=	inst->internal.FileOpen.filelen;
		
		//Get some temp memory for the file to be read into
		inst->internal.AllocStatus= TMP_alloc(inst->DataLen, (void*)&inst->pData);

		//Check if we got the memory
		if(inst->internal.AllocStatus==0){

			//If we got the memory, read the file into it
			inst->internal.FileRead.enable= 1;		
			inst->internal.FileRead.len=	inst->internal.FileOpen.filelen;
			inst->internal.FileRead.ident=	inst->internal.FileOpen.ident;
			inst->internal.FileRead.pDest=	inst->pData;
			inst->internal.FileRead.offset=	0;
			inst->internal._Active= 1;
		}
		else{

			//Tell the user we failed
			inst->Status= inst->internal.AllocStatus;

		}
	}
	else if(inst->internal.FileOpen.status == ERR_FUB_ENABLE_FALSE
	){
		if(inst->Enable 
			&& inst->pData	==	0
			&& inst->Status	== 	ERR_FUB_ENABLE_FALSE
		){
			inst->internal.FileOpen.enable=	1;
			inst->Status= ERR_FUB_BUSY;
		}
	}
	else if(inst->internal.FileOpen.status != ERR_FUB_BUSY){

		//Tell the user we failed
		inst->internal.FileOpen.enable= 0;
		inst->Status= inst->internal.FileOpen.status;		

	}
		
	FileRead(&inst->internal.FileRead);

	if(inst->internal.FileRead.status == ERR_OK){
		inst->internal.FileRead.enable= 0;
		inst->Status=	0;
		inst->internal._Active=	0;
	}
	else if(inst->internal.FileRead.status != ERR_FUB_BUSY
	&&		inst->internal.FileRead.status != ERR_FUB_ENABLE_FALSE
	){
		inst->internal.FileRead.enable= 0;
		inst->Status=	inst->internal.FileRead.status;		
		inst->internal._Active=	0;
	}

	//Free up any resources
	if( !inst->Enable && inst->pData!=0 && inst->internal._Active == 0){
		inst->internal.AllocStatus=TMP_free(inst->DataLen, (void*)inst->pData);	
		inst->pData=	0;
		inst->DataLen=	0;
	}
	
	if( !inst->Enable 
		&& inst->pData==0
		&& inst->internal.FileOpen.status == ERR_FUB_ENABLE_FALSE
		&& inst->internal.FileRead.status == ERR_FUB_ENABLE_FALSE
	){
		inst->Status= ERR_FUB_ENABLE_FALSE;
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
		
}
