/*
** JG - kernel_main.c
*/

#include "utils.h"

extern void	init_system(void);
void		kmain(void);

void	_start(void)
{
   init_system();

  /*
  ** Sets the stack pointer esp - We have to do it here because
  ** at the end of init_system(), gcc uses the 'leave' instruction
  ** which overwrites esp with ebp.
  **
  ** 0x18 is the offset of the stack segment in the new GDT,
  ** 0x1FFFF is an arbitrary address chosen so that the stack frame
  ** doesn't overwrite the other segments
  */
  asm volatile ("mov ax, 0x18 \n"
                "mov ss, ax \n"
		"mov esp, 0x1FFFF \n");
  /*
  ** We use an extra function call to start with a
  ** clean stack frame since we modified it earlier
  */
  kmain();
}

void	kmain(void)
{
  asm volatile ("hlt \n");
  while (1);
}
