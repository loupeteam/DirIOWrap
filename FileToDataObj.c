
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
/* Convert a file to a dataobject */
void FileToDataObj(struct FileToDataObj* inst)
{

	
	DataObjectFileHeader_typ fileheader;
		
	ReadFileVolatile(&inst->internal.ReadFile);
	
	switch (inst->internal.ReadFile.Status)
	{
		case ERR_OK:

			inst->internal.ReadFile.Enable=	0;
			
			inst->internal.DataLength=	inst->internal.ReadFile.DataLen + sizeof(fileheader);	
			strcpy(fileheader.HEAD,	"File");
			fileheader.FileSize=		inst->internal.ReadFile.DataLen;
			strcpy(fileheader.FileName,	inst->internal.ReadFile.FileName);
				
			//Get some temp memory for the file to be read into
			inst->internal.AllocStatus= TMP_alloc(inst->internal.DataLength, (void*)&inst->internal.pData);

			//Check if we got the memory
			if(inst->internal.AllocStatus==0){

				inst->internal.CreateDataObject.Execute= 	1;

				memcpy((void*)inst->internal.pData,&fileheader,sizeof(fileheader));
				memcpy((void*)inst->internal.pData + sizeof(fileheader),(void*)inst->internal.ReadFile.pData,inst->internal.ReadFile.DataLen);
			
				inst->internal.CreateDataObject.DataLen=	inst->internal.DataLength;
				inst->internal.CreateDataObject.pData=	inst->internal.pData;
							
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
			
				strcpy(inst->internal.ReadFile.FileDevice,inst->FileDevice);
				strcpy(inst->internal.ReadFile.FileName,inst->FileName);

				strcpy(inst->internal.CreateDataObject.DataObjectName,inst->DataObjectName);

				inst->internal.ReadFile.Enable= 	1;
				inst->Status=						ERR_FUB_BUSY;
				inst->internal._Execute= 			1;
			}
			break;
		
		case ERR_FUB_BUSY:		
			inst->Status=	inst->internal.ReadFile.Status;
			break;
		
		default:

			inst->Status=	inst->internal.ReadFile.Status;

			if(!inst->Execute){
			
				inst->internal.ReadFile.Enable= 	0;
				
			}			

			break;
	}
	
	CreateDataObject(&inst->internal.CreateDataObject);
	
	switch (inst->internal.CreateDataObject.Status)
	{
		
		case ERR_FUB_ENABLE_FALSE:	

			break;

		case ERR_FUB_BUSY:		

			//Use this status
			inst->Status=	inst->internal.CreateDataObject.Status;
			break;
		
		default:

			//Any other status should grab the status and disable both function blocks
			inst->Status=	inst->internal.CreateDataObject.Status;

			inst->internal.CreateDataObject.Execute=0;
			inst->internal._Active=					0;
			
			break;
	}
     
	//Free up any resources
	if( inst->internal.pData!=0 && inst->internal._Active == 0){
		inst->internal.AllocStatus=TMP_free(inst->internal.DataLength, (void*)inst->internal.pData);	
		inst->internal.pData=			0;
		inst->internal.DataLength=		0;
//		inst->internal.ReadFile.Enable= 0;		
	}
	
	if( !inst->Execute ){
			inst->internal._Execute= 0;
	}
	
}
