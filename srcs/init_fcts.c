/*
** JG - init_fcts.c
*/

#include "utils.h"
#include "gdt.h"

/*
** Sets a descriptor parameters
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
		** 'ljmp' to set the new code segment selector in cs
		*/
		"ljmp 0x08:next_line \n"
		"next_line: \n");
}

static inline void	reset_gdt(void)
{
  set_gdt_segment(NULL_SEGMENT, 0, 0, 0, 0);

  /*
  ** Code and data segments sizes are set to the maximal addressable
  ** memory size in 32-bit mode, so their limit is 32 bits set to 1
  */
  set_gdt_segment(CODE_SEGMENT, 0, 0xFFFF, 0B10011011, 0B1101);
  set_gdt_segment(DATA_SEGMENT, 0, 0xFFFF, 0B10010011, 0B1101);

  /*
  ** The stack segment is initialized as a data segment that grows
  ** downward thanks to the E-bit (Expand) - see note about the GDT
  */
  set_gdt_segment(STCK_SEGMENT, 0, 0x0000, 0B10010111, 0B1101);

  /*
  ** Since we have 4 segments of 8 bytes each,
  ** the limit of our GDT will be 4 * 8
  */
  g_gdtptr.limit = 4 * 8;
  g_gdtptr.base = (int)&g_gdt;

  load_new_gdt();
}

static inline void	init_icw(char port, char value)
{
  asm volatile ("mov al, %0 \n"

		/*
		** We will use dx to specify the I/O port address
		** to 'out' in dl. So we first clear dh
		*/
		"xor dh, dh \n"
		"mov dl, %1 \n"

		/*
		** The 'out' instruction writes the value in al
		** to the I/O port contained in dx
		*/
		"out dx, al \n"::"a"(value), "b"(port));
}

static inline void	init_icw_registers(char port, char icw1,
					   char icw2, char icw3,
					   char icw4)
{
  init_icw(port, icw1);

  /*
  ** port + 1 to get I/O port B
  */
  init_icw(port + 1, icw2);
  init_icw(port + 1, icw3);
  init_icw(port + 1, icw4);
}

static inline void	init_pic(void)
{
  /*
  ** The I/O master PIC port A is mapped at 0x20,
  ** slave port A at 0xA0
  */
  unsigned char			master_port_a = 0x20;
  unsigned char			slave_port_a = 0xA0;

  /*
  ** Master PIC ICW (Initialization Command Words) registers initialization: 
  **
  ** -> ICW1: 0b00010001 = with ICW4, several cascading PIC,
  **    and edge-triggered
  **
  ** -> ICW2: An offset in the IDT (used to calculate the base address of the IVT)
  **    is stored in the 5 most significant bits. The other 3 are used for
  **    the interrupt number: 8 interrupts, nb 0 to 7 = 000 to 111.
  **    On x86, the 32 first bytes of the IDT are used to handle exceptions. 
  **
  ** -> ICW3: Slave is connected to the third pin, so the third bit is set to 1
  **
  ** -> ICW4: mode AEOI - Automatic End Of Interrupt
  */
  init_icw_registers(master_port_a, 0B00010001, 32, 0B00000100, 1);

  /*
  ** Slave ICW registers initialization: 
  **
  ** -> ICW1, ICW2 and ICW4 are the same as the master
  **
  ** -> ICW3: indicates to the slave to which pin of the master PIC
  **    it is connected, starting from 0 (so 2 = 3rd pin)
  */
  init_icw_registers(slave_port_a, 0B00010001, 32, 2, 1);
}

void	init_system(void)
{
  clear_screen();

  /*
  ** Default text attribute: black background, white character
  */
  g_cursor.attr = 0B000111;

  /*
  ** As the kernel is not supposed to know the GDT,
  ** it has to define its own to be able to fully
  ** rely on it
  */
  reset_gdt();

  /*
  ** Initializes the PIC(s) - Programmable Interrupt Controller
  */
  init_pic();
}
