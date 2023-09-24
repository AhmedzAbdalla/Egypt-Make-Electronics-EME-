/***************************************************/
/******** Author      : Hosseni            *********/
/******** Date        : 13/8/2023          *********/
/******** version     : 1.0 V              *********/
/******** Description : STD_TYPES          *********/
/***************************************************/

#ifndef INC_LIB_STD_TYPES_H_
#define INC_LIB_STD_TYPES_H_

#define NULL (unsigned int *)0

typedef unsigned char        uint8 ;
typedef unsigned short int   uint16;
typedef unsigned  int        uint32;

typedef signed char          sint8 ;
typedef signed short int     sint16;
typedef signed long  int     sint32;

typedef unsigned char	u8;
typedef unsigned short 	u16;
typedef unsigned int 	u32;

typedef signed char 	s8;
typedef signed short 	s16;
typedef signed int 		s32;

typedef float 			f32;
typedef double 			f64;

#define DISABLE 0
#define Enalbe  1


typedef uint8 Std_ReturnType;
#define OK          0u
#define NOT_OK      1u

#define WORD_LENGTH_BITS      32u
#define WORD_LENGTH_BYTES     4u
#define MSB_FIRST             0u    /* big endian bit ordering */
#define LSB_FIRST        1u    /* little endian bit ordering */

#endif /* INC_LIB_STD_TYPES_H_ */
