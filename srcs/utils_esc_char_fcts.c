/*
** JG - utils_esc_char_fcts.c
*/

#include "utils.h"

extern struct { char x, y, attr; } g_cursor;

void	esc_char_backspace(void)
{
  g_cursor.x = !g_cursor.x ? 0 : g_cursor.x - 1;
}

void	esc_char_horiztab(void)
{
  g_cursor.x = (g_cursor.x + 8) % 80;
}

void	esc_char_newline(void)
{
  g_cursor.x = 0;
  g_cursor.y = (g_cursor.y + 1) % 25;
}

void	esc_char_vertab_formfeed(void)
{
  g_cursor.y = (g_cursor.y + 1) % 25;
}

void	esc_char_carriageret(void)
{
  g_cursor.x = 0;
}
