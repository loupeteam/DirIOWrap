(*
 * File: DirIOWrap.fun
 * Copyright (c) 2023 Loupe
 * https://loupe.team
 * 
 * This file is part of DirIOWrap, licensed under the MIT License.
 *)

FUNCTION DirIO : UINT (*Wrapper for common directory functions*) (*$GROUP=User*)
	VAR_IN_OUT
		t : DIOWrap_typ;
	END_VAR
END_FUNCTION

FUNCTION ValidDirectoryName : BOOL (*TODO: Add your comment here*) (*$GROUP=User*)
	VAR_INPUT
		Name : UDINT;
	END_VAR
END_FUNCTION

FUNCTION_BLOCK ReadFileVolatile (*TODO: Add your comment here*) (*$GROUP=User*)
	VAR_INPUT
		Enable : BOOL;
		FileDevice : STRING[80];
		FileName : STRING[80];
	END_VAR
	VAR_OUTPUT
		pData : UDINT;
		DataLen : UDINT;
		Status : UINT;
	END_VAR
	VAR
		internal : LoadFileVolatile_internal_typ;
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK ReadDataObjectVolatile (*TODO: Add your comment here*) (*$GROUP=User*)
	VAR_INPUT
		Enable : BOOL;
		DataObjectName : STRING[10];
	END_VAR
	VAR_OUTPUT
		pData : UDINT;
		DataLen : UDINT;
		Status : UINT;
	END_VAR
	VAR
		internal : LoadDatObjVolatile_internal_typ;
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK DeleteDataObject (* *) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		Execute : BOOL;
		DataObjectName : STRING[10];
	END_VAR
	VAR_OUTPUT
		Status : UINT;
	END_VAR
	VAR
		internal : DeleteDatObj_internal_typ;
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK CreateFile (*Create a file and write data to it*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		Execute : BOOL;
		FileDevice : STRING[80];
		FileName : STRING[80];
		pData : UDINT;
		DataLen : UDINT;
	END_VAR
	VAR_OUTPUT
		Status : UINT;
	END_VAR
	VAR
		internal : CreateWriteFile_internal_typ;
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK FileToDataObj (*Convert dataobject to file*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		Execute : BOOL;
		FileDevice : STRING[80];
		FileName : STRING[80];
		DataObjectName : STRING[10];
	END_VAR
	VAR_OUTPUT
		Status : UINT;
	END_VAR
	VAR
		internal : FileToDataObj_internal_typ;
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK DataObjToFile (*Convert a file to a dataobject*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		Execute : BOOL;
		FileDevice : STRING[80];
		FileName : STRING[80];
		DataObjectName : STRING[10];
	END_VAR
	VAR_OUTPUT
		ExportFileName : STRING[80];
		Status : UINT;
	END_VAR
	VAR
		internal : DataObjToFile_internal_typ;
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK CreateDataObject (*Create Data object from data*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		Execute : BOOL;
		DataObjectName : STRING[10];
		pData : UDINT;
		DataLen : UDINT;
	END_VAR
	VAR_OUTPUT
		Status : UINT;
	END_VAR
	VAR
		internal : CreateDataObject_internal_typ;
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK AppendDataObject (* *) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		Execute : BOOL;
		DataObjectName : STRING[10];
		pData : UDINT;
		DataLen : UDINT;
	END_VAR
	VAR_OUTPUT
		Status : UINT;
	END_VAR
	VAR
		internal : AppendDataObject_internal_typ;
	END_VAR
END_FUNCTION_BLOCK
