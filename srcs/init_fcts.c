/*
** JG - init_fcts.c
*/

#include "utils.h"
#include "gdt.h"

static inline void	reinit_gdt(void)
{
  ;
}

void	init_system(void)
{
  reinit_gdt();
  clear_screen();
  g_cursor.attr = 0B000111;
}
