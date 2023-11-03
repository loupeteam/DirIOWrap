/*
 * File: CreateDataObject.c
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
/* Create Data object from data */
void CreateDataObject(struct CreateDataObject* inst)
{
	DatObjCreate(&inst->internal.CreateDataObject);
	
	switch (inst->internal.CreateDataObject.status)
	{		
		case ERR_FUB_ENABLE_FALSE:	
			if( inst->Execute && !inst->internal._Execute ){
				inst->internal._Execute=					1;
				inst->internal.CreateDataObject.enable=		1;
				inst->internal.CreateDataObject.grp=		0;
				inst->internal.CreateDataObject.MemType=	doUSRROM;	
//				inst->internal.CreateDataObject.Option=		doNO_CS;
				inst->internal.CreateDataObject.pName=		(UDINT)inst->DataObjectName;
				inst->internal.CreateDataObject.pCpyData=	inst->pData;
				inst->internal.CreateDataObject.len=		inst->DataLen;
				inst->Status=	ERR_FUB_BUSY;
			}	
			else if(!inst->Execute){
				inst->Status=	ERR_FUB_ENABLE_FALSE;			
			}
			break;

		case ERR_FUB_BUSY:		

			//Use this status
			inst->Status=	inst->internal.CreateDataObject.status;
			break;
		
		default:

			//Any other status should grab the status and disable both function blocks
			inst->Status=	inst->internal.CreateDataObject.status;

			inst->internal.CreateDataObject.enable=		0;

			break;
	}
	
	if( !inst->Execute){
		inst->internal._Execute=					0;
	}
}
