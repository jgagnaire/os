/*
** JG - putstr.c
*/

#include "utils.h"

struct
{
  char x, y, attr;
} g_cursor;

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

void		putnbr(int nb)
{
  if (nb < 0)
    {
      putchar('-');
      nb *= -1;
    }
  if (nb / 10 > 0)
    putnbr(nb / 10);
  putchar((char)((nb % 10) + 48));
}

void		print_key(int key)
{
  if (!key)
    return ;
  else
    print_key(key >> 8);
  putchar(key);
}

void		putchar(char c)
{
  unsigned	i;
  char		*fbptr;

  /*
  ** If c is an escaped character
  ** '\b', '\t', '\n', '\v', '\f' or '\r'
  ** we have to deal with the cursor position
  */
  for (i = 0;g_esc_char_tab[i].fptr;++i)
    if (c == g_esc_char_tab[i].c)
      return (g_esc_char_tab[i].fptr());

  /*
  ** Otherwise, we just display it
  */
  fbptr = (char *)0xB8000;
  i = (2 * g_cursor.x + 160 * g_cursor.y);
  fbptr[i] = c;
  fbptr[i + 1] = g_cursor.attr;

  /*
  ** And increment the cursor position
  */
  if (!(g_cursor.x = ((g_cursor.x + 1) % 80)))
    g_cursor.y = ((g_cursor.y + 1) % 25);
}

void	putstr(char *s)
{
  for (;*s;++s)
    putchar(*s);
}

inline void	memcpy(char *addr, char *src, unsigned long size)
{
  unsigned	i;

  for (i = 0;i < size;++i)
    addr[i] = src[i];
}

inline void	set_text_attr(char new_attr)
{
  g_cursor.attr = new_attr;
}
