/*
****************************************************************************************************************************
FileName:			basetype.h
Author:				Liam.Nurx
Date:				2020.11.14

Description:

****************************************************************************************************************************
*/

#ifndef			BASE_TYPE_H
#define			BASE_TYPE_H

typedef		unsigned char				UINT8;
typedef		unsigned short				UINT16;
typedef		unsigned int				UINT32;
typedef		signed char					INT8;
typedef		signed short				INT16;
typedef		signed int					INT32;

#define		TRUE						0x01
#define		FALSE						0x00

typedef	struct
{
	UINT16		year;
	UINT8		month;
	UINT8		day;
	UINT8		hour;
	UINT8		minute;
	UINT8		second;
	UINT8		reserved;
}SYSTEM_TIME;							//	8 Bytes;

#endif			//	End of "BASE_TYPE_H"


































