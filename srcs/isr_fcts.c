/*
** JG - isr.c
*/

#include "utils.h"

void	default_isr(void)
{
  putstr("default ISR\n");
}

void	keybd_isr(void)
{
  putstr("keyboard ISR\n");
}

void	clock_isr(void)
{
  ;
}
