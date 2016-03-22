/*
** JG - utils.h
*/

#ifndef		UTILS_H_
# define	UTILS_H_

# define IN(port) ({						 \
      unsigned char _key;					 \
      asm volatile ("in %%ax, %%dx\n" : "=a"(_key) : "d"(port)); \
      _key;							 \
})

void		set_text_attr(char);
void		clear_screen(void);
void		memcpy(char *, char *, unsigned long);
void		putchar(char);
void		putstr(char *);
void		putnbr(int);
void		print_key(int);

/*
** escaped characters fcts
*/

#endif		/* !UTILS_H_ */
