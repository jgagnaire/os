/*
** JG - gdt.h
*/

#ifndef		GDT_H_
# define	GDT_H_

# define	NULL_SEGMENT		0
# define	CODE_SEGMENT		1
# define	DATA_SEGMENT		2
# define	STCK_SEGMENT		3

struct
{
  short		limit_low_part;
  short		base_low_part;
  char		base_middle_part;
  char		access;
  char		limit_high_part : 4;
  char		granularity : 4;
  char		base_high_part;
}__attribute__((packed)) g_gdt[4];

struct
{
  short		limit;
  int		base;
}__attribute__((packed)) g_gdtptr;

#endif		/* !GDT_H_ */
