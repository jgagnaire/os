/*
** JG - init_fcts.c
*/

#include "utils.h"
#include "gdt.h"
#include "idt.h"

void	init_system(void)
{
  clear_screen();

  /*
  ** Default text attribute: black background, white character
  */
  set_text_attr(0B000111);

  /*
  ** Initializes the PIC(s) - Programmable Interrupt Controller
  */
  init_pic();

  /*
  ** As the kernel is not supposed to know the GDT,
  ** it has to define its own to be able to fully
  ** rely on it
  */
  reset_gdt();
}
