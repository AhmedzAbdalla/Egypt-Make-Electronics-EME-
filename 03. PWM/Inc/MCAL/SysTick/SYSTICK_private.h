#ifndef INC_MCAL_SYSTICK_SYSTICK_PRIVATE_H_
#define INC_MCAL_SYSTICK_SYSTICK_PRIVATE_H_

#include "../../LIB/STD_TYPES.h"

#define SYSTICK_BASE  0xE000E010


/*
This code defines a structure named "sSYSTICK_type" that represents the layout of the SysTick registers. It contains three volatile 32-bit unsigned integer variables: STCTRL, STRELOAD, and STCURRENT. The "volatile" keyword indicates that the variables can be modified by external sources, and the "uint32" type represents an unsigned 32-bit integer.
*/
typedef struct
{
    volatile uint32 STCTRL;
    volatile uint32 STRELOAD;
    volatile uint32 STCURRENT;
}sSYSTICK_type;


/*
This line defines a macro named "SYSTICK" that casts the base address (SYSTICK_BASE) to a pointer of type "sSYSTICK_type". It allows convenient access to the SysTick registers by dereferencing the pointer.
*/
#define SYSTICK ((sSYSTICK_type *)SYSTICK_BASE)

#endif /* INC_MCAL_SYSTICK_SYSTICK_PRIVATE_H_ */
