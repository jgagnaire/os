/*
** JG - paging.h
*/

#ifndef		PAGING_H_
# define	PAGING_H_

# define	PAGE_DIR_ADDR	0x100000
# define	PAGE_TABLE_ADDR	0x101000

void	init_paging(void);

#endif		/* !PAGING_H_ */
