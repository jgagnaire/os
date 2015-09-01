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
  g_cursor.attr = 0B000111;

  /*
  ** As the kernel is not supposed to know the GDT,
  ** it has to define its own to be able to fully
  ** rely on it
  */
  reset_gdt();

  /*
  ** Initializes the PIC(s) - Programmable Interrupt Controller
  */
  init_pic();
}
