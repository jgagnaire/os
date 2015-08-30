/*
** JG - init_fcts.c
*/

#include "utils.h"
#include "gdt.h"

/*
** Sets a descriptor parameters - see note about the GDT
*/
static inline void	set_gdt_segment(unsigned index, int base,
					int limit, char access,
					char granularity)
{

  /*
  ** limit_low_part is on 16 bits (short), to which we add
  ** limit_high_part, a bitfield on 4 bits, in order to
  ** get the 20 bits composing the 'limit' field size in the GDT
  */
  g_gdt[index].limit_low_part = limit;
  g_gdt[index].limit_high_part = limit >> 16;

  /*
  ** Same for the base, on 32-bit : short + char + char <=> 16 + 8 + 8
  */
  g_gdt[index].base_low_part = base;
  g_gdt[index].base_middle_part = base >> 16;
  g_gdt[index].base_high_part = base >> 24;

  g_gdt[index].access = access;

  g_gdt[index].granularity = granularity;
}

static inline void	load_new_gdt(void)
{
  /*
  ** 'volatile' so that gcc does not perform
  ** optimization on the assembly code
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
  putstr("Loading new GDT from the kernel...");

  set_gdt_segment(NULL_SEGMENT, 0, 0, 0, 0);

  /*
  ** Code and data segments sizes are set to the maximal addressable
  ** memory size in 32-bit mode, so their limit is 32 bits set to 1
  */
  set_gdt_segment(CODE_SEGMENT, 0, 0xFFFF,
		  0B10011011, 0B1101);
  set_gdt_segment(DATA_SEGMENT, 0, 0xFFFF,
		  0B10010011, 0B1101);

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
