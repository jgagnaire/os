/*
** JG - utils.h
*/

#ifndef		UTILS_H_
# define	UTILS_H_

void		clear_screen(void);
void		init_system(void);
void		putchar(char);
void		putstr(char *);

/*
** esc_char_fcts
*/
void		esc_char_carriageret(void);
void		esc_char_backspace(void);
void		esc_char_horiztab(void);
void		esc_char_formfeed(void);
void		esc_char_newline(void);
void		esc_char_vertab(void);

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
    {7, esc_char_backspace},
    {9, esc_char_horiztab},
    {10, esc_char_newline},
    {11, esc_char_vertab},
    {12, esc_char_formfeed},
    {13, esc_char_carriageret},
    {0, 0}
  };

#endif		/* !UTILS_H_ */
