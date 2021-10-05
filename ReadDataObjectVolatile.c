/********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Library: AxisMap
 * File: LoadAxisMapFile.c
 * Author: Josh
 * Created: October 02, 2015
 ********************************************************************
 * Implementation of library AxisMap
 ********************************************************************/

#include <bur/plctypes.h>
#ifdef __cplusplus
	extern "C"
	{
#endif

#include "DirIOWrap.h"

#ifdef __cplusplus
	};
#endif


void ReadDataObjectVolatile(struct  ReadDataObjectVolatile* inst)
{
		
	inst->internal.DatObjInfo.pName= 	(UDINT)inst->DataObjectName;
	DatObjInfo(&inst->internal.DatObjInfo);

	if(inst->internal.DatObjInfo.status == ERR_OK){

		//We are done opening
		inst->internal.DatObjInfo.enable= 0;

		//Grab the read file length
		inst->DataLen=	inst->internal.DatObjInfo.len;
		
		//Get some temp memory for the file to be read into
		inst->internal.AllocStatus= TMP_alloc(inst->DataLen, (void*)&inst->pData);

		//Check if we got the memory
		if(inst->internal.AllocStatus==0){

			//If we got the memory, read the file into it
			inst->internal.DatObjRead.enable= 1;		
			inst->internal.DatObjRead.len=		inst->internal.DatObjInfo.len;
			inst->internal.DatObjRead.ident=	inst->internal.DatObjInfo.ident;
			inst->internal.DatObjRead.pDestination=	inst->pData;
			inst->internal.DatObjRead.Offset=	0;
			inst->internal._Active= 1;
		}
		else{

			//Tell the user we failed
			inst->Status= inst->internal.AllocStatus;

		}
	}
	else if(inst->internal.DatObjInfo.status == ERR_FUB_ENABLE_FALSE
	){
		if(inst->Enable 
			&& inst->pData	==	0
			&& inst->Status	== 	ERR_FUB_ENABLE_FALSE
		){
			inst->internal.DatObjInfo.enable=	1;
			inst->Status= ERR_FUB_BUSY;
		}
	}
	else if(inst->internal.DatObjInfo.status != ERR_FUB_BUSY){

		//Tell the user we failed
		inst->internal.DatObjInfo.enable= 0;
		inst->Status= inst->internal.DatObjInfo.status;		

	}
		
	DatObjRead(&inst->internal.DatObjRead);

	if(inst->internal.DatObjRead.status == ERR_OK){
		inst->internal.DatObjRead.enable= 0;
		inst->Status=	0;
		inst->internal._Active=	0;
	}
	else if(inst->internal.DatObjRead.status != ERR_FUB_BUSY
	&&		inst->internal.DatObjRead.status != ERR_FUB_ENABLE_FALSE
	){
		inst->internal.DatObjRead.enable= 0;
		inst->Status=	inst->internal.DatObjRead.status;		
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
		&& inst->internal.DatObjInfo.status == ERR_FUB_ENABLE_FALSE
		&& inst->internal.DatObjRead.status == ERR_FUB_ENABLE_FALSE
	){
		inst->Status= ERR_FUB_ENABLE_FALSE;
	}
}
