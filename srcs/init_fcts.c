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

  g_gdt[index].base_low_part = base & 0xFFFF;
  g_gdt[index].base_middle_part = (base & 0xFF0000) >> 16;

  g_gdt[index].access = access;

  g_gdt[index].limit_high_part = (limit & 0xF0000) >> 16;

  g_gdt[index].granularity = granularity & 0xF;

  g_gdt[index].base_high_part = (base & 0xFF000000) >> 24;
}

static inline void	reset_gdt(void)
{
  set_gdt_segment(NULL_SEGMENT, 0, 0, 0, 0);
  set_gdt_segment(CODE_SEGMENT, 0, 0xFFFFF, 0x9B, 0x0D);
  set_gdt_segment(DATA_SEGMENT, 0, 0xFFFFF, 0x93, 0x0D);
  set_gdt_segment(STCK_SEGMENT, 0, 0x0, 0x97, 0x0D);

  g_gdtptr.limit = 4 * 8;
  g_gdtptr.base = (int)&g_gdt;

  asm volatile ("lgdt g_gdtptr \n"

		"movw ax, 0x10 \n"
		"movw ds, ax \n"
		"movw es, ax \n"
		"movw fs, ax \n"
		"movw gs, ax \n"
		"movw ss, ax \n"

		"ljmp 0x08:next_line \n"

		"next_line: \n");
}

void	init_system(void)
{

  /*
  ** As the kernel is not supposed to know the GDT,
  ** it has to define its own to be able to fully
  ** rely on it
  */
  reset_gdt();
  clear_screen();

  /*
  ** Default text attribute: white char on black background
  */
  g_cursor.attr = 0B000111;
}
