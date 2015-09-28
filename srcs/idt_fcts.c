/*
** JG - idt.c
*/

#include "idt.h"
#include "utils.h"

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

static inline void	init_ocw(char port, char value)
{
  asm volatile (/*
		** We clear dh to use dx for the 'in' and 'out'
                ** instructions - same as the init_icw() function
                */
                "xor dh, dh \n"
                "mov dl, %0 \n"

                /*
		** We retrieve the value of the IMR,
                ** set it to the desired value,
                ** and write it to the given I/O port
                */
                "in al, dx \n"
                "and al, %1 \n"
		"xor dh, dh \n"
                "mov dl, al \n"
                "out dx, al \n"::"a"(port), "b"(value));
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

static inline void	init_ocw_registers(char port, char ocw1,
                                           char ocw2)
{
  init_ocw(port + 1, ocw1);
  init_ocw(port, ocw2);
}

static inline void	set_idt_segment(unsigned index,
					int segment_selector,
					int isr_addr, char type,
					char s_dpl_p)
{
  g_idt[index].offset_low_part = (isr_addr & 0x00FF);
  g_idt[index].segment_selector = segment_selector;
  g_idt[index].zero = 0;
  g_idt[index].type_s_dpl_p = (s_dpl_p << 4) | (type & 0B00001111);
  g_idt[index].offset_high_part = (isr_addr & 0xFF00) >> 16;
}

static inline void	load_idt(void)
{
  memcpy((char *)g_idtptr.base, (char *)g_idt, g_idtptr.limit);

  asm volatile ("lidt (g_idtptr) \n"
		"sti \n");
}

void		init_pic(void)
{
  /*
  ** The I/O master PIC port A is mapped at 0x20,
  ** slave port A at 0xA0
  */
  unsigned char	master_port_a = 0x20;
  unsigned char	slave_port_a = 0xA0;
  unsigned	i;

  /*
  ** 0B1000 means P-bit set to 1 = segment present
  */
  for (i = 0;i < IDT_SIZE;++i)
    if (i != CLOCK_IDX && i != KEYBD_IDX)
      set_idt_segment(i, 0x8, (int)&asm_default_isr, INT_GATE, 0B1000);
  set_idt_segment(CLOCK_IDX, 0x8, (int)&asm_clock_isr, INT_GATE, 0B1000);
  set_idt_segment(KEYBD_IDX, 0x8, (int)&asm_keybd_isr, INT_GATE, 0B1000);

  g_idtptr.limit = IDT_SIZE * 8;
  g_idtptr.base = IDT_ADDR;

  load_idt();

  /*
  ** Master PIC ICW (Initialization Command Words) registers initialization:
  **
  ** -> ICW1: 0b00010001 = with ICW4, several cascading PIC, and edge-triggered
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

  /*
  ** We then fill the OCW registers of the master PIC:
  **
  ** -> OCW1 represents the value in the IMR (Interrupt Mask Register)
  **    0B11101111 = interrupt 4 not masked, others are.
  **
  ** -> OCW2: - when the 3 leftmost bits are equal to 001:
  **                + the sent command will be an EOI (bit 6 = 1)
  **                + the sent command will be non-specific (bit 7 = 0)
  **                + rotate priorities off (bit 8 = 0)
  **          - the 3 rightmost bits specify the interrupt level
  **            for a specific command
  */
  init_ocw_registers(master_port_a, 0B11101111, 0B00100000);

  /*
  ** Same for the slave
  */
  init_ocw_registers(slave_port_a, 0B11101111, 0B00100000);
}
