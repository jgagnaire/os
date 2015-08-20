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
  g_cursor.x = 0;
  g_cursor.y = 0;
}

void		putchar(char c)
{
  unsigned	i;
  char		*fbptr;

  for (i = 0;g_esc_char_tab[i].fptr;++i)
    if (c == g_esc_char_tab[i].c)
      return (g_esc_char_tab[i].fptr());
  fbptr = (char *)0xB8000;
  i = 2 * g_cursor.x + 160 * g_cursor.y;
  fbptr[i] = c;
  fbptr[i + 1] = g_cursor.attr;
  if (!(g_cursor.x = (g_cursor.x + 1) % 80))
    g_cursor.y = (g_cursor.y + 1) % 25;
}

void	putstr(char *s)
{
  for (;*s;++s)
    putchar(*s);
}

void	init_system(void)
{
  clear_screen();
  g_cursor.attr = 0B000111;
}