/*
** JG - kernel_main.c
*/

#include "utils.h"

void	_start(void)
{
  init_system();
  putstr("lel\n");
  while (1);
}
