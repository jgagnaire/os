/*
** JG - utils.h
*/

#ifndef		UTILS_H_
# define	UTILS_H_

struct
{
  char	x;
  char	y;
  char	attr;
}	g_cursor;

void		clear_screen(void);
void		putstr(const char *);

#endif		/* !UTILS_H_ */
