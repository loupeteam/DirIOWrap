/*
 * File: DataObjToFile.c
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
/* Convert dataobject to file */
void DataObjToFile(struct DataObjToFile* inst)
{

	ReadDataObjectVolatile(&inst->internal.ReadDataObject);
	
	switch (inst->internal.ReadDataObject.Status)
	{
		case ERR_OK:

			if( inst->internal.CreateFile.Execute == 0 ){
				inst->internal.CreateFile.Execute=	1;
				
				if(strcmp(((DataObjectFileHeader_typ*)inst->internal.ReadDataObject.pData)->HEAD,"File") == 0 ){
					inst->internal.CreateFile.pData=	inst->internal.ReadDataObject.pData + sizeof(DataObjectFileHeader_typ);
					inst->internal.CreateFile.DataLen=	((DataObjectFileHeader_typ*)inst->internal.ReadDataObject.pData)->FileSize;
					strcpy(inst->internal.CreateFile.FileName,((DataObjectFileHeader_typ*)inst->internal.ReadDataObject.pData)->FileName);
					strcpy(inst->ExportFileName,((DataObjectFileHeader_typ*)inst->internal.ReadDataObject.pData)->FileName);					
				}
				else{
					inst->internal.CreateFile.pData=	inst->internal.ReadDataObject.pData;
					inst->internal.CreateFile.DataLen=	inst->internal.ReadDataObject.DataLen;
					strcpy(inst->ExportFileName,inst->internal.CreateFile.FileName);										
				}
			}
       
			break;

		case ERR_FUB_ENABLE_FALSE:
			
			if( !inst->Execute ){
				inst->Status=	ERR_FUB_ENABLE_FALSE;
				strcpy(inst->ExportFileName,"");
			}
						
			if( inst->Execute && !inst->internal._Execute ){
			
				strcpy(inst->internal.ReadDataObject.DataObjectName,inst->DataObjectName);

				strcpy(inst->internal.CreateFile.FileName,inst->FileName);
				strcpy(inst->internal.CreateFile.FileDevice,inst->FileDevice);
			
				inst->internal.ReadDataObject.Enable= 	1;
				inst->Status=						ERR_FUB_BUSY;
				inst->internal._Execute= 			1;
			}
			break;
		
		case ERR_FUB_BUSY:		
			inst->Status=	inst->internal.ReadDataObject.Status;
			break;
		
		default:

			inst->Status=	inst->internal.ReadDataObject.Status;

			if(!inst->Execute){
			
				inst->internal.ReadDataObject.Enable= 	0;
				
			}			

			break;
	}
     
	CreateFile(&inst->internal.CreateFile );
	
	switch (inst->internal.CreateFile.Status)
	{
		
		case ERR_FUB_ENABLE_FALSE:	

			break;

		case ERR_FUB_BUSY:		

			//Use this status
			inst->Status=	inst->internal.CreateFile.Status;
			break;
		
		default:

			//Any other status should grab the status and disable both function blocks
			inst->Status=	inst->internal.CreateFile.Status;

			inst->internal.CreateFile.Execute=		0;
			inst->internal.ReadDataObject.Enable= 	0;

			break;
	}
     
	if( !inst->Execute ){
		inst->internal._Execute= 0;
	}

}
