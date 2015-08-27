/*
** JG - init_fcts.c
*/

#include "utils.h"

static inline void	reinit_gdt(void)
{
  ;
}

void	init_system(void)
{

  /*
  ** As the kernel is not supposed to know the GDT,
  ** it has to define its own to be able to fully
  ** rely on it
  */
  reinit_gdt();
  clear_screen();

  /*
  ** Default text attribute: white char on black background
  */
  g_cursor.attr = 0B000111;
}
