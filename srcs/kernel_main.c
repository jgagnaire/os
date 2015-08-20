/*
** JG - kernel_main.c
*/

#include "utils.h"

void	_start(void)
{
  init_system();
  putstr("Lel\blabite\n");
  while (1);
}
