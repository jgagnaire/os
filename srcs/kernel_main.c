/*
** JG - kernel_main.c
*/

#include "utils.h"

extern void	init_system(void);

void	_start(void)
{
  init_system();
  putstr("lel\n");
  while (1);
}
