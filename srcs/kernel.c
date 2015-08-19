/*
** JG - kernel.c
*/

#include "utils.h"

void	init_system(void);

void	_start(void)
{
  init_system();
  /* putstr("LEL\n"); */

  char	*fb;

  fb = (void *)0xB8A00;
  *(fb + 1) = 'L';
  *(fb + 2) = 0B000101;
  *(fb + 3) = 'E';
  *(fb + 4) = 0B000101;
  *(fb + 5) = 'L';
  *(fb + 6) = 0B000101;
  while (1);
}

void	init_system(void)
{
  g_cursor.x = 0;
  g_cursor.y = 0;
  g_cursor.attr = 0b000111;
  clear_screen();
}
