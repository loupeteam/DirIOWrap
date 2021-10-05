
#include <bur/plctypes.h>
#ifdef __cplusplus
	extern "C"
	{
#endif
	#include "DirIOWrap.h"
#ifdef __cplusplus
	};
#endif
void DeleteDataObject(struct DeleteDataObject* inst)
{
	inst->internal.DatObjInfo.pName= 	(UDINT)inst->DataObjectName;
	DatObjInfo(&inst->internal.DatObjInfo);

	if(inst->internal.DatObjInfo.status == ERR_OK){

		//We are done opening
		inst->internal.DatObjInfo.enable= 0;

		inst->internal.DatObjDelete.ident=	inst->internal.DatObjInfo.ident;
		inst->internal.DatObjDelete.enable=	1;
		inst->internal._Active=				1;
	}
	else if(inst->internal.DatObjInfo.status == ERR_FUB_ENABLE_FALSE
	){
		if(inst->Execute 
			&& inst->internal._Execute	==	0
			&& !inst->internal._Active
		){
			inst->internal._Execute= 			1;
			inst->internal.DatObjInfo.enable=	1;
			inst->Status= 						ERR_FUB_BUSY;
		}
		else if( !inst->Execute ){
			inst->Status= 						ERR_FUB_ENABLE_FALSE;		
		}
	}
	else if(inst->internal.DatObjInfo.status != ERR_FUB_BUSY){

		//Tell the user we failed
		inst->internal.DatObjInfo.enable= 0;
		inst->Status= inst->internal.DatObjInfo.status;		

	}
		
	DatObjDelete(&inst->internal.DatObjDelete);

	if(inst->internal.DatObjDelete.status == ERR_OK){
		inst->internal.DatObjDelete.enable= 0;
		inst->Status=	0;
		inst->internal._Active=	0;
	}
	else if(inst->internal.DatObjDelete.status != ERR_FUB_BUSY
		&&		inst->internal.DatObjDelete.status != ERR_FUB_ENABLE_FALSE
	){
		inst->internal.DatObjDelete.enable= 0;
		inst->Status=	inst->internal.DatObjDelete.status;		
		inst->internal._Active=	0;
	}
	
	if( !inst->Execute ){
		inst->internal._Execute= 0;
	}

}
