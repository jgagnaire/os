/*
** JG - init_fcts.c
*/

#include "paging.h"
#include "utils.h"
#include "gdt.h"
#include "idt.h"

/*
** The interruption will come from the PIC 8259A which is
** connected to the PIT 8253 - Programmable Interval Timer
**
** This interruption frequency is 1 193 180 Hertz
*/
#define	PIT_FREQ	(1193180)

static inline void	init_timer(unsigned freq)
{
  unsigned		divisor = PIT_FREQ / freq;

  /*
  ** The PIT has 3 channel:
  **  -> channel 0 is connected to IRQ0
  **  -> channel 1 is used to control refresh rates for DRAM
  **  -> channel 2 controls the PC speaker
  */
  asm volatile (/*
		** 0x43 is the PIT command port, to which we send 0x36,
		** meaning that we want to change the PIT frequency
		*/
		"mov al, 0x43 \n"
		"xor dh, dh \n"
		"mov dl, 0x36 \n"
		"out dx, al \n"

		/*
		** We then fill the PIT data port 0x40, starting with
		** the low part of the divisor...
		*/
		"mov al, 0x40 \n"
		"xor dh, dh \n"
		"mov dl, %0 \n"
		"out dx, al \n"::"a"((char)(divisor & 0xFF)));

  asm volatile (/*
		** ...and then with its low part
		*/
		"xor dh, dh \n"
		"mov dl, %0 \n"
		"out dx, al \n"::"a"((char)((divisor >> 8) & 0xFF)));
}

void	init_system(void)
{
  clear_screen();

  /*
  ** Default text attribute: black background, white character
  */
  set_text_attr(0B000111);

  /*
  ** Initializes the PIC(s) - Programmable Interrupt Controller
  */
  init_pic();

  /*
  ** As the kernel is not supposed to know the GDT, it
  ** has to define its own to be able to fully rely on it
  */
  reset_gdt();

  /*
  ** Initializes the PIT - Programmable Interval Timer
  ** to 1 Hz <=> timeout every second
  */
  init_timer(1);

  /*
  ** Loads the pages directory and enables the paging
  */
  init_paging();
}
