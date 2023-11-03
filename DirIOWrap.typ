(*
 * File: DirIOWrap.typ
 * Copyright (c) 2023 Loupe
 * https://loupe.team
 * 
 * This file is part of DirIOWrap, licensed under the MIT License.
 *)

TYPE
	DIOWrap_Int_FUB_typ : 	STRUCT 
		Create : DirCreate; (*Open a file and read its contents*)
		Rename : DirRename; (*Open a file and read its contents*)
		Copy : DirCopy; (*Open a file and read its contents*)
		Delete : DirDeleteEx; (*Open a file and read its contents*)
		RenameFile : FileRename; (*Open a file and read its contents*)
		CopyFile : FileCopy; (*Open a file and read its contents*)
		DeleteFile : FileDelete; (*Open a file and read its contents*)
		ConvertToFile : DataObjToFile;
		ConvertToDataObject : FileToDataObj;
		Zip : DIODelegate_typ;
		Unzip : DIODelegate_typ;
		ConvertFolderToDataObject : DIODelegate_typ;
		ConvertToFolder : DIODelegate_typ;
	END_STRUCT;
	DIOWrap_Internal_typ : 	STRUCT 
		FUB : DIOWrap_Int_FUB_typ;
		CMD : DIOWrap_IN_CMD_typ;
		NewFileIndex : UDINT;
		SrcName : STRING[DIOWRAP_STRLEN_NAME];
		DstName : STRING[DIOWRAP_STRLEN_NAME];
	END_STRUCT;
	DIOWrap_OUT_STAT_typ : 	STRUCT 
		Busy : BOOL;
		Done : BOOL;
		Error : BOOL;
		ErrorID : UINT;
		ErrorString : STRING[DIOWRAP_STRLEN_ERROR];
	END_STRUCT;
	DIOWrap_OUT_typ : 	STRUCT 
		STAT : DIOWrap_OUT_STAT_typ;
	END_STRUCT;
	DIOWrap_IN_PAR_typ : 	STRUCT 
		SrcDevice : STRING[DIOWRAP_STRLEN_NAME]; (*File device name*)
		SrcPath : STRING[DIOWRAP_STRLEN_NAME]; (*File name (with extension)*)
		SrcName : STRING[DIOWRAP_STRLEN_NAME]; (*File name (with extension)*)
		DestDevice : STRING[DIOWRAP_STRLEN_NAME]; (*File device name*)
		DestPath : STRING[DIOWRAP_STRLEN_NAME]; (*File name (with extension)*)
		DestName : STRING[DIOWRAP_STRLEN_NAME]; (*File name (with extension)*)
	END_STRUCT;
	DIOWrap_IN_CMD_typ : 	STRUCT 
		CreateDirectory : BOOL; (*Open a file and read its contents*)
		Rename : BOOL; (*Open a file and read its contents*)
		Copy : BOOL; (*Open a file and read its contents*)
		Move : BOOL;
		Delete : BOOL; (*Open a file and read its contents*)
		ConvertToFile : BOOL; (*Open a file and read its contents*)
		ExtractDataObject : BOOL; (*Open a file and read its contents*)
		ConvertToDataObject : BOOL; (*Open a file and read its contents*)
		Zip : BOOL; (*Open a file and read its contents*)
		Unzip : BOOL; (*Open a file and read its contents*)
	END_STRUCT;
	DIOWrap_IN_typ : 	STRUCT 
		CMD : DIOWrap_IN_CMD_typ;
		PAR : DIOWrap_IN_PAR_typ;
	END_STRUCT;
	DIOWrap_typ : 	STRUCT 
		IN : DIOWrap_IN_typ;
		OUT : DIOWrap_OUT_typ;
		Internal : DIOWrap_Internal_typ;
	END_STRUCT;
	DIODelegate_typ : 	STRUCT 
		Active : BOOL;
		enable : BOOL;
		Execute : USINT;
		DataObjectName : STRING[10];
		FileDevice : STRING[20];
		FileName : STRING[80];
		TempFileDevice : STRING[20];
		Status : UINT;
		DestPath : STRING[80];
	END_STRUCT;
	LoadFileVolatile_internal_typ : 	STRUCT 
		FileOpen : FileOpen;
		FileRead : FileRead;
		FileClose : FileClose;
		AllocStatus : UINT;
		_Open : BOOL;
		_Active : BOOL;
	END_STRUCT;
	LoadDatObjVolatile_internal_typ : 	STRUCT 
		DatObjInfo : DatObjInfo;
		DatObjRead : DatObjRead;
		AllocStatus : UINT;
		_Active : BOOL;
	END_STRUCT;
	DeleteDatObj_internal_typ : 	STRUCT 
		DatObjInfo : DatObjInfo;
		DatObjDelete : DatObjDelete;
		_Active : BOOL;
		_Execute : BOOL;
	END_STRUCT;
	DataObjToFile_internal_typ : 	STRUCT 
		ReadDataObject : ReadDataObjectVolatile;
		CreateFile : CreateFile;
		_Execute : BOOL;
	END_STRUCT;
	FileToDataObj_internal_typ : 	STRUCT 
		ReadFile : ReadFileVolatile;
		CreateDataObject : CreateDataObject;
		DataLength : UDINT;
		AllocStatus : UINT;
		pData : UDINT;
		_Execute : BOOL;
		_Active : BOOL;
	END_STRUCT;
	CreateWriteFile_internal_typ : 	STRUCT 
		FileCreate : FileCreate;
		FileWrite : FileWrite;
		FileClose : FileClose;
		_Active : BOOL;
		_Open : BOOL;
		_Execute : BOOL;
	END_STRUCT;
	CreateDataObject_internal_typ : 	STRUCT 
		CreateDataObject : DatObjCreate;
		_Execute : BOOL;
	END_STRUCT;
	AppendDataObject_internal_typ : 	STRUCT 
		ReadDataObject : ReadDataObjectVolatile;
		DeleteDataObject : DeleteDataObject;
		WriteDataObject : CreateDataObject;
		DataLength : UDINT;
		pData : UDINT;
		AllocStatus : UINT;
		_Execute : BOOL;
		_Active : BOOL;
	END_STRUCT;
	DataObjectFileHeader_typ : 	STRUCT 
		HEAD : STRING[7] := 'File';
		FileName : STRING[80];
		FileSize : UDINT;
	END_STRUCT;
END_TYPE
