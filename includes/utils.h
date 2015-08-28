/*
** JG - utils.h
*/

#ifndef		UTILS_H_
# define	UTILS_H_

void		set_text_attr(char);
void		clear_screen(void);
void		putchar(char);
void		putstr(char *);

/*
** escaped characters fcts
*/
void		esc_char_vertab_formfeed(void);
void		esc_char_carriageret(void);
void		esc_char_backspace(void);
void		esc_char_horiztab(void);
void		esc_char_newline(void);

struct
{
  char	x;
  char	y;
  char	attr;
}	g_cursor;

static const struct
{
  char	c;
  void	(*fptr)(void);
} g_esc_char_tab[7] =
  {
    {8, esc_char_backspace},
    {9, esc_char_horiztab},
    {10, esc_char_newline},
    {11, esc_char_vertab_formfeed},
    {12, esc_char_vertab_formfeed},
    {13, esc_char_carriageret},
    {0, 0}
  };

#endif		/* !UTILS_H_ */
