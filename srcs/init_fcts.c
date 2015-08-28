/*
** JG - init_fcts.c
*/

#include "utils.h"
#include "gdt.h"

static inline void	set_gdt_segment(unsigned index, int base,
					int limit, char access,
					char granularity)
{
  g_gdt[index].limit_low_part = limit & 0xFFFF;
  g_gdt[index].limit_high_part = (limit >> 16) & 0xF;

  g_gdt[index].base_low_part = base & 0xFFFF;
  g_gdt[index].base_middle_part = (base >> 16) & 0xFF;
  g_gdt[index].base_high_part = (base >> 24) & 0xFF;

  g_gdt[index].access = access;

  g_gdt[index].granularity = granularity & 0xF;
}

static inline void	load_new_gdt(void)
{
  /*
  ** 'volatile' so that gcc does not perform
  ** optimization nor erases the assembly code
  */
  asm volatile ("lgdt (g_gdtptr) \n"

		/*
		** At offset 0x10 in the GDT is the data segment
		** previously set
		*/
		"mov ax, 0x10 \n"
		"mov ds, ax \n"
		"mov es, ax \n"
		"mov fs, ax \n"
		"mov gs, ax \n"
		
		/*
		** 'ljmp' to empty the processor internal caches
		*/
		"ljmp 0x08:next_line \n"
		"next_line: \n");
}

static inline void	reset_gdt(void)
{
  putstr("Loading GDT from the kernel...");

  set_gdt_segment(NULL_SEGMENT, 0, 0, 0, 0);
  set_gdt_segment(CODE_SEGMENT, 0, 0xFFFFF, 0x9B, 0x0D);
  set_gdt_segment(DATA_SEGMENT, 0, 0xFFFFF, 0x93, 0x0D);

  /*
  ** Since we have 3 segments of 8 bytes each,
  ** the limit of our GDT will be 3 * 8
  */
  g_gdtptr.limit = 3 * 8;
  g_gdtptr.base = (int)&g_gdt;

  load_new_gdt();

  putstr(" Done !\n");
}

void	init_system(void)
{
  clear_screen();

  /*
  ** Default text attribute: white char on black background
  */
  g_cursor.attr = 0B000111;

  /*
  ** As the kernel is not supposed to know the GDT,
  ** it has to define its own to be able to fully
  ** rely on it
  */
  reset_gdt();
}
