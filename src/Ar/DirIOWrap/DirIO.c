/*
 * File: DirIO.c
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

unsigned short DirIOSetError(struct DIOWrap_typ* t, UINT ErrorID );

/* Wrapper for common directory functions */
unsigned short DirIO(struct DIOWrap_typ* t)
{	
	
	STRING stringIndex[16]; 
	plcbit Valid;
	
	if(!t->OUT.STAT.Busy && !t->OUT.STAT.Done && !t->OUT.STAT.Error){
		if(t->IN.CMD.Move && !t->Internal.CMD.Move){
			//save move command internally
			t->Internal.CMD.Move = 1;
			
			///// set up copy and delete commands /////
			strcpy(t->Internal.SrcName,t->IN.PAR.SrcPath);
			strcat(t->Internal.SrcName,t->IN.PAR.SrcName);

			strcpy(t->Internal.DstName,t->IN.PAR.DestPath);
			strcat(t->Internal.DstName,t->IN.PAR.DestName);			

			Valid = ValidDirectoryName(t->Internal.FUB.Copy.pDestDir)
				&&ValidDirectoryName(t->Internal.FUB.Copy.pSrcDir)
				&&ValidDirectoryName(t->Internal.FUB.Copy.pSrcDev)
				&&ValidDirectoryName(t->Internal.FUB.Copy.pDestDev);

			t->Internal.FUB.CopyFile.enable = Valid && t->IN.PAR.SrcName[0]!='\\';
			t->Internal.FUB.Copy.enable   = Valid && t->IN.PAR.SrcName[0]=='\\'; 
			
			if(t->Internal.FUB.Copy.enable==0 && t->Internal.FUB.CopyFile.enable==0){
				DirIOSetError(t,fiERR_INVALID_DIRECTORY);
			}	
		}
		else if(t->IN.CMD.Copy){
			
			strcpy(t->Internal.SrcName,t->IN.PAR.SrcPath);
			strcat(t->Internal.SrcName,t->IN.PAR.SrcName);

			strcpy(t->Internal.DstName,t->IN.PAR.DestPath);
			strcat(t->Internal.DstName,t->IN.PAR.SrcName);			

			Valid = ValidDirectoryName(t->Internal.FUB.Copy.pDestDir)
				&&ValidDirectoryName(t->Internal.FUB.Copy.pSrcDir)
				&&ValidDirectoryName(t->Internal.FUB.Copy.pSrcDev)
				&&ValidDirectoryName(t->Internal.FUB.Copy.pDestDev);

			t->Internal.FUB.CopyFile.enable = Valid && t->IN.PAR.SrcName[0]!='\\';
			t->Internal.FUB.Copy.enable   = Valid && t->IN.PAR.SrcName[0]=='\\'; 
			
			if(t->Internal.FUB.Copy.enable==0 && t->Internal.FUB.CopyFile.enable==0){
				DirIOSetError(t,fiERR_INVALID_DIRECTORY);
			}			
		}
		else if(t->IN.CMD.CreateDirectory){
			
			t->Internal.NewFileIndex = 0;	

			strcpy(t->Internal.DstName,t->IN.PAR.DestPath);
			if(strcmp(t->IN.PAR.DestName,"")==0){
				strcat(t->Internal.DstName,"NewFolder");			
			}
			else{
				strcat(t->Internal.DstName,t->IN.PAR.DestName);			
			}
			
			t->Internal.FUB.Create.enable   = 
				ValidDirectoryName(t->Internal.FUB.Create.pDevice)
				&& ValidDirectoryName(t->Internal.FUB.Create.pName);
			
			if(t->Internal.FUB.Create.enable==0){
				DirIOSetError(t,fiERR_INVALID_DIRECTORY);
			}			
		}
		else if(t->IN.CMD.Delete){

			strcpy(t->Internal.DstName,t->IN.PAR.DestPath);
			strcat(t->Internal.DstName,t->IN.PAR.DestName);
						
			Valid   = ValidDirectoryName(t->Internal.FUB.Delete.pDevice)
				&& ValidDirectoryName(t->Internal.FUB.Delete.pName);			

			t->Internal.FUB.DeleteFile.enable = Valid && t->IN.PAR.DestName[0]!='\\';
			t->Internal.FUB.Delete.enable   = Valid && t->IN.PAR.DestName[0]=='\\'; 
			
			if(t->Internal.FUB.Delete.enable==0 && t->Internal.FUB.DeleteFile.enable==0){
				DirIOSetError(t,fiERR_INVALID_DIRECTORY);
			}			
		}
		else if(t->IN.CMD.Rename){

			strcpy(t->Internal.SrcName,t->IN.PAR.DestPath);
			strcat(t->Internal.SrcName,t->IN.PAR.SrcName);

			strcpy(t->Internal.DstName,t->IN.PAR.DestPath);
			strcat(t->Internal.DstName,t->IN.PAR.DestName);			

			Valid = 
				ValidDirectoryName(t->Internal.FUB.Rename.pDevice)
				&& ValidDirectoryName(t->Internal.FUB.Rename.pName)
				&& ValidDirectoryName(t->Internal.FUB.Rename.pNewName);

			t->Internal.FUB.RenameFile.enable  = Valid && t->IN.PAR.SrcName[0]!='\\';
			t->Internal.FUB.Rename.enable     = Valid && t->IN.PAR.SrcName[0]=='\\';
  
			if(t->Internal.FUB.Rename.enable==0 && t->Internal.FUB.RenameFile.enable==0){
				DirIOSetError(t,fiERR_INVALID_DIRECTORY);
			}			
		}
		else if(t->IN.CMD.ConvertToDataObject){

			strcpy(t->Internal.SrcName,t->IN.PAR.SrcPath);
			strcat(t->Internal.SrcName,t->IN.PAR.SrcName);

			strcpy(t->Internal.DstName,t->IN.PAR.DestName);			

			Valid = 
				ValidDirectoryName((UDINT)t->Internal.SrcName)
				&& ValidDirectoryName((UDINT)t->Internal.DstName);

			if(t->IN.PAR.SrcName[0]=='\\'){
				t->Internal.FUB.ConvertFolderToDataObject.Execute=	Valid;	
			}
			else{			
				t->Internal.FUB.ConvertToDataObject.Execute=	Valid;	
			}			
			if(t->Internal.FUB.ConvertToDataObject.Execute==0
				&& t->Internal.FUB.ConvertFolderToDataObject.Execute==0
			){
				DirIOSetError(t,fiERR_INVALID_DIRECTORY);
			}			
			
		}
		else if(t->IN.CMD.ConvertToFile){

			strcpy(t->Internal.SrcName,t->IN.PAR.SrcName);

			strcpy(t->Internal.DstName,t->IN.PAR.DestPath);
			strcat(t->Internal.DstName,t->IN.PAR.DestName);			

			Valid = ValidDirectoryName((UDINT)t->Internal.SrcName);

			t->Internal.FUB.ConvertToFile.Execute=	Valid;	
			
			if(t->Internal.FUB.ConvertToFile.Execute==0){
				DirIOSetError(t,fiERR_INVALID_DIRECTORY);
			}						
		}
		else if(t->IN.CMD.ExtractDataObject ){

			strcpy(t->Internal.SrcName,t->IN.PAR.SrcName);

			strcpy(t->Internal.DstName,t->IN.PAR.DestPath);
			strcat(t->Internal.DstName,t->IN.PAR.DestName);			

			Valid = ValidDirectoryName((UDINT)t->Internal.SrcName);

			t->Internal.FUB.ConvertToFolder.Execute=	Valid;	
			
			if(t->Internal.FUB.ConvertToFile.Execute==0){
				DirIOSetError(t,fiERR_INVALID_DIRECTORY);
			}						
		}
		else if(t->IN.CMD.Zip){

			strcpy(t->Internal.SrcName,t->IN.PAR.SrcPath);
			strcat(t->Internal.SrcName,t->IN.PAR.SrcName);

			strcpy(t->Internal.DstName,t->IN.PAR.DestPath);
			strcat(t->Internal.DstName,t->IN.PAR.DestName);			
			strcat(t->Internal.DstName,	".tar.gz");			

			Valid = ValidDirectoryName((UDINT)t->Internal.SrcName)
				&&	ValidDirectoryName((UDINT)t->Internal.DstName);

			t->Internal.FUB.Zip.enable=	Valid;	
			
			if(t->Internal.FUB.Zip.enable==0){
				DirIOSetError(t,fiERR_INVALID_DIRECTORY);
			}			
		}
		else if(t->IN.CMD.Unzip){
			strcpy(t->Internal.SrcName,t->IN.PAR.SrcPath);
			strcat(t->Internal.SrcName,t->IN.PAR.SrcName);

			strcpy(t->Internal.DstName,t->IN.PAR.DestPath);
			strcat(t->Internal.DstName,t->IN.PAR.DestName);			

			Valid = ValidDirectoryName((UDINT)t->Internal.SrcName)
				&&	ValidDirectoryName((UDINT)t->Internal.DstName);

			t->Internal.FUB.Unzip.enable=	Valid;	
			
			if(t->Internal.FUB.Unzip.enable==0){
				DirIOSetError(t,fiERR_INVALID_DIRECTORY);
			}			
		}
	}
	
	//////////////////////////////////////////////
	//Setup the parameters for the function blocks
	//////////////////////////////////////////////
	t->Internal.FUB.Copy.option = fiRECURSIVE + fiOVERWRITE;
	t->Internal.FUB.Copy.pSrcDev = (UDINT)&t->IN.PAR.SrcDevice;
	t->Internal.FUB.Copy.pDestDev = (UDINT)&t->IN.PAR.DestDevice;
	t->Internal.FUB.Copy.pSrcDir = (UDINT)t->Internal.SrcName;
	t->Internal.FUB.Copy.pDestDir = (UDINT)t->Internal.DstName;
	
	t->Internal.FUB.CopyFile.pSrcDev = (UDINT)&t->IN.PAR.SrcDevice;
	t->Internal.FUB.CopyFile.pDestDev = (UDINT)&t->IN.PAR.DestDevice;
	t->Internal.FUB.CopyFile.pSrc = (UDINT)t->Internal.SrcName;
	t->Internal.FUB.CopyFile.pDest = (UDINT)t->Internal.DstName;

	t->Internal.FUB.Create.pDevice  = (UDINT)&t->IN.PAR.DestDevice;
	t->Internal.FUB.Create.pName    = (UDINT)t->Internal.DstName;
	
	t->Internal.FUB.DeleteFile.pDevice = (UDINT)&t->IN.PAR.DestDevice;
	t->Internal.FUB.DeleteFile.pName = (UDINT)t->Internal.DstName;
	
	t->Internal.FUB.Delete.pDevice = (UDINT)&t->IN.PAR.DestDevice;
	t->Internal.FUB.Delete.pName = (UDINT)t->Internal.DstName;
	
	t->Internal.FUB.RenameFile.pDevice = (UDINT)&t->IN.PAR.DestDevice;
	t->Internal.FUB.RenameFile.pName= (UDINT)t->Internal.SrcName;
	t->Internal.FUB.RenameFile.pNewName = (UDINT)t->Internal.DstName;

	t->Internal.FUB.Rename.pDevice = (UDINT)&t->IN.PAR.DestDevice;
	t->Internal.FUB.Rename.pName    = (UDINT)t->Internal.SrcName;		
	t->Internal.FUB.Rename.pNewName = (UDINT)t->Internal.DstName;
	
	strcpy(t->Internal.FUB.ConvertToDataObject.DataObjectName,t->Internal.DstName);
	strcpy(t->Internal.FUB.ConvertToDataObject.FileDevice,t->IN.PAR.SrcDevice);
	strcpy(t->Internal.FUB.ConvertToDataObject.FileName,t->Internal.SrcName);

	strcpy(t->Internal.FUB.ConvertFolderToDataObject.DataObjectName,t->Internal.DstName);
	strcpy(t->Internal.FUB.ConvertFolderToDataObject.FileDevice,t->IN.PAR.SrcDevice);
	strcpy(t->Internal.FUB.ConvertFolderToDataObject.FileName,t->Internal.SrcName);
	strcpy(t->Internal.FUB.ConvertFolderToDataObject.TempFileDevice,t->IN.PAR.SrcDevice);
		
	strcpy(t->Internal.FUB.ConvertToFile.DataObjectName,t->Internal.SrcName);
	strcpy(t->Internal.FUB.ConvertToFile.FileName,t->Internal.DstName);
	strcpy(t->Internal.FUB.ConvertToFile.FileDevice,t->IN.PAR.DestDevice);

	strcpy(t->Internal.FUB.ConvertToFolder.DataObjectName,t->Internal.SrcName);
	strcpy(t->Internal.FUB.ConvertToFolder.FileName,t->Internal.DstName);
	strcpy(t->Internal.FUB.ConvertToFolder.FileDevice,t->IN.PAR.DestDevice);
	strcpy(t->Internal.FUB.ConvertToFolder.DestPath,	t->IN.PAR.DestPath);
	strcpy(t->Internal.FUB.ConvertToFolder.TempFileDevice,t->IN.PAR.DestDevice);
	//////////////////////////////////////////////

	
	//////////////////////////////////////////////
	//Call Function blocks
	//////////////////////////////////////////////
	DirCopy(&t->Internal.FUB.Copy);	
	DirCreate(&t->Internal.FUB.Create);	
	DirDeleteEx(&t->Internal.FUB.Delete);	
	DirRename(&t->Internal.FUB.Rename);
	
	FileCopy(&t->Internal.FUB.CopyFile);
	FileDelete(&t->Internal.FUB.DeleteFile);
	FileRename(&t->Internal.FUB.RenameFile);

	FileToDataObj(&t->Internal.FUB.ConvertToDataObject);

	DataObjToFile(&t->Internal.FUB.ConvertToFile);
	//////////////////////////////////////////////
	
	//////////////////////////////////////////////
	//Set status if the function is enabled
	//////////////////////////////////////////////	
	if(t->Internal.FUB.Copy.status !=  65534){
		DirIOSetError(t,t->Internal.FUB.Copy.status);
	}
	else if(t->Internal.FUB.CopyFile.status !=  65534){
		DirIOSetError(t,t->Internal.FUB.CopyFile.status);
	}
	else if(t->Internal.FUB.Create.status !=  65534){
		DirIOSetError(t,t->Internal.FUB.Create.status);	
	}
	else if(t->Internal.FUB.Delete.status !=  65534){
		DirIOSetError(t,t->Internal.FUB.Delete.status);	
	}
	else if(t->Internal.FUB.DeleteFile.status !=  65534){
		DirIOSetError(t,t->Internal.FUB.DeleteFile.status);	
	}
	else if(t->Internal.FUB.Rename.status !=  65534){
		DirIOSetError(t,t->Internal.FUB.Rename.status);	
	}
	else if(t->Internal.FUB.RenameFile.status !=  65534){
		DirIOSetError(t,t->Internal.FUB.RenameFile.status);	
	}
	else if(t->Internal.FUB.ConvertToDataObject.Status !=  65534){
		DirIOSetError(t,t->Internal.FUB.ConvertToDataObject.Status);	
	}
	else if(t->Internal.FUB.ConvertToFile.Status !=  65534){
		DirIOSetError(t,t->Internal.FUB.ConvertToFile.Status);			
	}
	else if(t->Internal.FUB.Zip.Status !=  65534
	&& t->Internal.FUB.Zip.Active == 1	
	){
		DirIOSetError(t,t->Internal.FUB.Zip.Status);
	}
	else if(t->Internal.FUB.Unzip.Status !=  65534
	&& t->Internal.FUB.Unzip.Active == 1
	){
		DirIOSetError(t,t->Internal.FUB.Unzip.Status);	
	}
	else if(t->Internal.FUB.ConvertFolderToDataObject.Status !=  65534
	&& t->Internal.FUB.ConvertFolderToDataObject.Active == 1	
	){
		DirIOSetError(t,t->Internal.FUB.ConvertFolderToDataObject.Status);	
	}
	else if(t->Internal.FUB.ConvertToFolder.Status !=  65534
	&& t->Internal.FUB.ConvertToFolder.Active == 1		
	){
		DirIOSetError(t,t->Internal.FUB.ConvertToFolder.Status);	
	}		
		//If no function is busy and there isn't a command, reset status
	else if(memcmp(&t->IN.CMD,&t->Internal.CMD,sizeof(t->IN.CMD))==0){
		t->OUT.STAT.Busy			= 0;
		t->OUT.STAT.Done			= 0;
		t->OUT.STAT.ErrorID			= 0;
		t->OUT.STAT.Error			= 0;
		strcpy(t->OUT.STAT.ErrorString,"");
	}
	else{
		t->OUT.STAT.Busy			= 0;	
	}
	//////////////////////////////////////////////
	//Reset the enable if the command is accepted
	//////////////////////////////////////////////		
	if(t->Internal.FUB.Delete.status != 65535
	&& t->Internal.FUB.Delete.status != 65534
	){
		t->Internal.FUB.Delete.enable   = 0;
	}
	if(t->Internal.FUB.DeleteFile.status != 65535
	&& t->Internal.FUB.DeleteFile.status != 65534
	){
		t->Internal.FUB.DeleteFile.enable   = 0;
		t->IN.CMD.Move = 0;
	}
	if(t->Internal.FUB.Copy.status != 65535
	&& t->Internal.FUB.Copy.status != 65534
	){
		t->Internal.FUB.Copy.enable     = 0;		
	}
	if(t->Internal.FUB.CopyFile.status != 65535
		&& t->Internal.FUB.CopyFile.status != 65534
	){
		t->Internal.FUB.CopyFile.enable     = 0;
		if(t->Internal.FUB.CopyFile.status == ERR_OK && t->Internal.CMD.Move){
			// Do delete
			strcpy(t->Internal.DstName,t->Internal.SrcName);
						
			Valid   = ValidDirectoryName(t->Internal.FUB.Delete.pDevice)
				&& ValidDirectoryName(t->Internal.FUB.Delete.pName);			

			t->Internal.FUB.DeleteFile.enable = Valid && t->IN.PAR.DestName[0]!='\\';
			t->Internal.FUB.Delete.enable   = Valid && t->IN.PAR.DestName[0]=='\\'; 
			
			if(t->Internal.FUB.Delete.enable==0 && t->Internal.FUB.DeleteFile.enable==0){
				DirIOSetError(t,fiERR_INVALID_DIRECTORY);
			}
			t->Internal.CMD.Move = 0;
		}
	}	
	if(t->Internal.FUB.ConvertToDataObject.Status != 65535
	&& t->Internal.FUB.ConvertToDataObject.Status != 65534
	){
		t->Internal.FUB.ConvertToDataObject.Execute     = 0;		
	}
	if(t->Internal.FUB.ConvertFolderToDataObject.Status != 65535
		&& t->Internal.FUB.ConvertFolderToDataObject.Status != 65534
	){
		t->Internal.FUB.ConvertFolderToDataObject.Execute     = 0;		
	}
	if(t->Internal.FUB.ConvertToFile.Status != 65535
	&& t->Internal.FUB.ConvertToFile.Status != 65534
	){
		t->Internal.FUB.ConvertToFile.Execute     = 0;		
	}
	if(t->Internal.FUB.ConvertToFolder.Status != 65535
		&& t->Internal.FUB.ConvertToFolder.Status != 65534
	){
		t->Internal.FUB.ConvertToFolder.Execute     = 0;		
	}
	if(t->Internal.FUB.Unzip.Status != 65535
	&& t->Internal.FUB.Unzip.Status != 65534
	){
		t->Internal.FUB.Unzip.enable= 0;		
	}
	if(t->Internal.FUB.Zip.Status != 65535
	&& t->Internal.FUB.Zip.Status != 65534
	){
		t->Internal.FUB.Zip.enable= 0;		
	}
	
	if(t->Internal.FUB.Create.status != 65535){
		//If the file already exists, append a number
		if(t->Internal.FUB.Create.status==20725 && t->Internal.NewFileIndex<20){
			t->Internal.NewFileIndex++;
			brsitoa(t->Internal.NewFileIndex,(UDINT)&stringIndex);
			strcpy(t->Internal.DstName,t->IN.PAR.DestPath);
			if(strcmp(t->IN.PAR.DestName,"")==0){
				strcat(t->Internal.DstName,"NewFolder");			
			}
			else{
				strcat(t->Internal.DstName,t->IN.PAR.DestName);			
			}						
			strcat(t->Internal.DstName,stringIndex);
			t->Internal.FUB.Create.enable   = 0;		
			DirCreate(&t->Internal.FUB.Create);	
			t->Internal.FUB.Create.enable   = 1;		
		}
		else{
			t->Internal.FUB.Create.enable   = 0;		
		}
	}
	if(t->Internal.FUB.Rename.status != 65535){
		t->Internal.FUB.Rename.enable   = 0;		
	}
	if(t->Internal.FUB.RenameFile.status != 65535){
		t->Internal.FUB.RenameFile.enable   = 0;		
	}
	//////////////////////////////////////////////
	
	return t->OUT.STAT.ErrorID;
	
}

unsigned short DirIOSetError(struct DIOWrap_typ* t, UINT ErrorID )
{
	t->OUT.STAT.ErrorID = ErrorID;
	t->OUT.STAT.Error = 1;
	t->OUT.STAT.Busy = 1;
	
	switch (ErrorID)
	{
		case 0: 
			t->OUT.STAT.Done = 1;						
			t->OUT.STAT.Busy = 0;
		case 65535: 
			t->OUT.STAT.Error =  0;			
			strcpy(t->OUT.STAT.ErrorString,"");break;	
		case fiERR_INVALID_PATH:		strcpy(t->OUT.STAT.ErrorString,"Invalid path."); break;
		case fiERR_DATA_SIZE:			strcpy(t->OUT.STAT.ErrorString,"Data length is too small (DirRead)."); break;
		case fiERR_NO_MORE_ENTRIES:		strcpy(t->OUT.STAT.ErrorString,"No more file handles available."); break;
		case fiERR_NOT_SUPPORTED:		strcpy(t->OUT.STAT.ErrorString,"System error."); break;
		case fiERR_INVALID_TYP:			strcpy(t->OUT.STAT.ErrorString,"Invalid type."); break;
		case fiERR_EXIST:				strcpy(t->OUT.STAT.ErrorString,"File already exists."); break;
		case fiERR_ACCESS:				strcpy(t->OUT.STAT.ErrorString,"Access not possible with the desired mode."); break;
		case fiERR_MODE:				strcpy(t->OUT.STAT.ErrorString,"Illegal mode."); break;
		case fiERR_FILE_NOT_FOUND:		strcpy(t->OUT.STAT.ErrorString,"File not found. Check IN.PAR.FileName."); break;
		case fiERR_FILE_DEVICE:			strcpy(t->OUT.STAT.ErrorString,"File device not found. Check IN.PAR.FileDevice."); break;
		case fiERR_SPACE:				strcpy(t->OUT.STAT.ErrorString,"Not enough memory to write file."); break;
		case fiERR_SEEK:				strcpy(t->OUT.STAT.ErrorString,"Invalid file offset."); break;
		case fiERR_FILE:				strcpy(t->OUT.STAT.ErrorString,"File not allowed."); break;
		case fiERR_LESS_VIRTUAL_MEMORY:	strcpy(t->OUT.STAT.ErrorString,"Not enough memory to write file."); break;
		case fiERR_COM_FILE_OPEN:		strcpy(t->OUT.STAT.ErrorString,"System error."); break;
		case fiERR_COM_FILE_CLOSE:		strcpy(t->OUT.STAT.ErrorString,"System error."); break;
		case fiERR_COM_FILE_READ:		strcpy(t->OUT.STAT.ErrorString,"System error."); break;
		case fiERR_COM_FILE_WRITE:		strcpy(t->OUT.STAT.ErrorString,"System error."); break;
		case fiERR_COM_FILE_IOCTL:		strcpy(t->OUT.STAT.ErrorString,"System error."); break;
		case fiERR_DATA:				strcpy(t->OUT.STAT.ErrorString,"Invalid parameter. Check IN.PAR.pData."); break;
		case fiERR_ASYNC_MANAGER:		strcpy(t->OUT.STAT.ErrorString,"System error."); break;
		case fiERR_FILE_NOT_OPENED:		strcpy(t->OUT.STAT.ErrorString,"File not opened."); break;
		case fiERR_INVALID_DIRECTORY:	strcpy(t->OUT.STAT.ErrorString,"Invalid directory. Check IN.PAR.FileDevice and IN.PAR.FileName."); break;
		case fiERR_DIR_NOT_EXIST:		strcpy(t->OUT.STAT.ErrorString,"Directory does not exist. Check IN.PAR.FileDevice and IN.PAR.FileName."); break;
		case fiERR_DIR_NOT_EMPTY:		strcpy(t->OUT.STAT.ErrorString,"Directory is not empty."); break;
		case fiERR_DIR_ALREADY_EXIST:	strcpy(t->OUT.STAT.ErrorString,"Directory already exists."); break;
		case fiERR_DETMEMINFO:			strcpy(t->OUT.STAT.ErrorString,"System error."); break;
		case fiERR_NOT_ENOUGH_FREEMEM:	strcpy(t->OUT.STAT.ErrorString,"Not enough memory to write file."); break;
		case fiERR_DIR_INVALID_HANDLE:	strcpy(t->OUT.STAT.ErrorString,"System error."); break;
		case fiERR_PARAMETER:			strcpy(t->OUT.STAT.ErrorString,"System error."); break;
		case fiERR_DEVICE_ALREADY_EXIST:strcpy(t->OUT.STAT.ErrorString,"Device already exists."); break;
		case fiERR_DEVICE_INVALID_HANDLE:strcpy(t->OUT.STAT.ErrorString,"Invalid device handle."); break;
		case fiERR_NETIO_IP_UNEQUAL:	strcpy(t->OUT.STAT.ErrorString,"IP address error. Check hostname."); break;
		case fiERR_INIT:				strcpy(t->OUT.STAT.ErrorString,"System error."); break;
		case fiERR_DEVICE_DRIVER:		strcpy(t->OUT.STAT.ErrorString,"System error."); break;
		case fiERR_DEVICE_MANAGER:		strcpy(t->OUT.STAT.ErrorString,"File device error. Check IN.PAR.FileDevice."); break;
		case fiERR_SYSTEM:				strcpy(t->OUT.STAT.ErrorString,"System error."); break;	
		default:
			strcpy(t->OUT.STAT.ErrorString,"Unknown error."); break;
	}
	return ErrorID;
}

