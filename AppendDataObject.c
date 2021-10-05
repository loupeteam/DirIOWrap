
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
void AppendDataObject(struct AppendDataObject* inst)
{

	ReadDataObjectVolatile(&inst->internal.ReadDataObject);
	
	switch (inst->internal.ReadDataObject.Status)
	{
		case ERR_OK:

			inst->internal.ReadDataObject.Enable=	0;
			
			inst->internal.DataLength=	inst->internal.ReadDataObject.DataLen + inst->DataLen;
						
			//Get some temp memory for the file to be read into
			inst->internal.AllocStatus= TMP_alloc(inst->internal.DataLength, (void*)&inst->internal.pData);

			//Check if we got the memory
			if(inst->internal.AllocStatus==0){

				memcpy((void*)inst->internal.pData,(void*)inst->internal.ReadDataObject.pData,inst->internal.ReadDataObject.DataLen);
				memcpy((void*)inst->internal.pData + inst->internal.ReadDataObject.DataLen-1,(void*)inst->pData,inst->DataLen);
			
				inst->internal.WriteDataObject.DataLen=	inst->internal.DataLength;
				inst->internal.WriteDataObject.pData=	inst->internal.pData;

				inst->internal.DeleteDataObject.Execute=	1;			
				
				//If we got the memory, read the file into it
				inst->internal._Active= 1;
			}
			else{

				//Tell the user we failed
				inst->Status= inst->internal.AllocStatus;

			}
			
			break;

		case ERR_FUB_ENABLE_FALSE:
			
			if( !inst->Execute ){
				inst->Status=	ERR_FUB_ENABLE_FALSE;
			}
						
			if( inst->Execute && !inst->internal._Execute ){
			
				strcpy(inst->internal.ReadDataObject.DataObjectName,	inst->DataObjectName);
				strcpy(inst->internal.WriteDataObject.DataObjectName,	inst->DataObjectName);
				strcpy(inst->internal.DeleteDataObject.DataObjectName,	inst->DataObjectName);
			
				inst->internal.ReadDataObject.Enable= 	1;
				inst->Status=						ERR_FUB_BUSY;
				inst->internal._Execute= 			1;
			}
			break;
		
		case ERR_FUB_BUSY:		
			inst->Status=	inst->internal.ReadDataObject.Status;

		default:

			inst->Status=	inst->internal.ReadDataObject.Status;

			if(!inst->Execute){
			
				inst->internal.ReadDataObject.Enable= 	0;
				
			}			

			break;
		}

	DeleteDataObject(&inst->internal.DeleteDataObject);
	if(inst->internal.DeleteDataObject.Status == 0){
		inst->internal.DeleteDataObject.Execute=	0;
		inst->internal.WriteDataObject.Execute=		1;		
	}
	else if(inst->internal.DeleteDataObject.Status != ERR_FUB_BUSY 
	&&		inst->internal.DeleteDataObject.Status != ERR_FUB_ENABLE_FALSE	
	){
		inst->internal._Active=	0;
		inst->Status=			inst->internal.DeleteDataObject.Status;
	}

	CreateDataObject(&inst->internal.WriteDataObject);
	if(inst->internal.WriteDataObject.Status==0){
		inst->internal.WriteDataObject.Execute=		0;
		inst->internal._Active=	0;
		inst->Status=			ERR_OK;
	}
	else if(inst->internal.WriteDataObject.Status != ERR_FUB_BUSY 
	&&		inst->internal.WriteDataObject.Status != ERR_FUB_ENABLE_FALSE
	){	
		inst->Status=			inst->internal.WriteDataObject.Status;
		inst->internal._Active=	0;	
	}
	
	//Free up any resources
	if( inst->internal.pData !=0 && inst->internal._Active == 0){
		inst->internal.AllocStatus=TMP_free(inst->internal.DataLength, (void*)inst->internal.pData);	
		inst->internal.pData=	0;
		inst->internal.DataLength=	0;
	}
	
	if(!inst->Execute){
		inst->internal._Execute=0;
	}
		
}
