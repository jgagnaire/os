/*
** JG - putstr.c
*/

#include "utils.h"

void		clear_screen(void)
{
  char		*fb;
  unsigned	i;

  fb = (char *)0xB8000;
  for (i = 0;i < 4000;++i)
    fb[i] = 0;
  g_cursor.x = 0;
  g_cursor.y = 0;
}

void	putstr(const char *s)
{
  (void)s;
}
