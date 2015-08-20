/*
** JG - kernel_main.c
*/

#include "utils.h"

void	_start(void)
{
  init_system();
  putstr("Lel\n\0");
  while (1);
}
