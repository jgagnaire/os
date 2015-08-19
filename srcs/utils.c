/*
** JG - putstr.c
*/

#include "utils.h"

void		clear_screen(void)
{
  int		*fb;
  unsigned	i;

  /*
  ** The text memory buffer is located at 0xB8000
  ** each character is coded on 2 bytes, 1 for the ASCII
  ** code and one for the attributes
  */
  fb = (int *)0xB8000;

  /*
  ** It is made of 25 rows, each of them having 80 chars + 80 attr bytes,
  ** which gives us 25 * (80 + 80) = 4000 bytes, that we divide by sizeof(int)
  ** to avoid a lot of iterations
  */
  for (i = 0;i < (4000 / sizeof(int));++i)
    fb[i] = 0;

  /*
  ** And we reset the cursor position
  */
  g_cursor.y = g_cursor.x = 0;
}

void	putstr(const char *s)
{
  (void)s;
}
