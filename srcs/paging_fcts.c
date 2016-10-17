/*
** JG - paging_fcts.c
*/

#include "paging.h"
#include "utils.h"

void		init_paging(void)
{
  /*
  ** We need 1024 bytes for our page directory,
  ** 1024 * 4096 = 4 194 304 bytes
  */
  unsigned	*page_directory = (unsigned *)PAGE_DIR_ADDR;
  unsigned	*page_table = (unsigned *)PAGE_TABLE_ADDR;
  unsigned	i, page_addr;

  /*
  ** Each page directory entry contains the address
  ** of the page table on 20 bits + some attributes
  ** on 12 bits. See note on paging for details.
  ** 0B000000000011 <=> Read/Write + Present
  */
  page_directory[0] = PAGE_TABLE_ADDR | 0B000000000011;

  for (i = 1;i < 1024;++i)
    page_directory[i] = 0;

  for (i = 0, page_addr = 0;i < 1024;++i, page_addr += 4096)
    page_table[i] = page_addr | 0B000000000011;

  /*
  ** To enable pagin we have to:
  **   -> put the address of the page directory in cr3
  **   -> enable the paging bit (PG-bit) of cr0
  */
  asm volatile ("mov cr3, %0 \n"
  		"mov eax, cr0 \n"
  		"or eax, %1 \n"
  		"mov cr0, eax \n"::"a"(PAGE_DIR_ADDR), "b"(1 << 31));

  putstr("Paging enabled !\n");
}
