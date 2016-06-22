
/************************************************************************************
 *  common.h  
 *  
 *	descp : Commonly used data types, structs etc
 *
 *  auth  : javery
 *  
 *  path  : C:\Program Files\Microsoft Visual Studio\MyProjects\PrimaryCubes3D\common.h
 *
 * iDate  : 09/17/06 
 * version:
 * pre-alpha [ 10/26/06 ] added RGB mask macros
 ************************************************************************************/

#ifndef __COMMON_H__
#define __COMMON_H__

#define GET_R(rgb) ((rgb) & (0x0000001F))
#define GET_G(rgb) ((rgb) & (0x000003E0))
#define GET_B(rgb) ((rgb) & (0x00007C00))

typedef enum
{
	c_false=0,
	c_true=1
}c_boolean;

typedef struct Vect3_t
{ 
	float xPosition , yPosition , zPosition;
}*pVect3_t;	  

#endif
