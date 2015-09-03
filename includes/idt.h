/*
** JG - idt.h
*/

#ifndef		IDT_H_
# define	IDT_H_

# define	IDT_SIZE	256
# define	CLOCK_IDX	32
# define	KEYBD_IDX	33

/*
** The last 4 bits of INT_GATE represent the gate type.
** 1110 means 32-bit interrupt gate
*/
# define	INT_GATE	0B1110

struct
{
  short		offset_low_part;
  short		segment_selector;
  char		zero;
  char		type_s_dpl_p;
  short		offset_high_part;
}__attribute__((packed)) g_idt[IDT_SIZE];

struct
{
  short		limit;
  int		base;
}__attribute__((packed)) g_idtptr;

extern void	asm_default_isr(void);
extern void	asm_clock_isr(void);
extern void	asm_keybd_isr(void);

void		init_pic(void);

#endif		/* !IDT_H_ */
