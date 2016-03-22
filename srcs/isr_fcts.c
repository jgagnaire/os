/*
** JG - isr.c
*/

#include "keymap.h"
#include "utils.h"

extern t_keypress	EN_layout[11];

static inline void	init_kbd_layouts(t_keyboard *kbd)
{
  kbd->keyboard_layout[EN] = EN_layout;
}

static void     key_management(t_keyboard *kbd,
			       unsigned long int keycode)
{
  if ((unsigned long)(keycode - KEY_RELEASE) < LAYOUT_SIZE(EN_layout) &&
      !kbd->keyboard_layout[EN][keycode - KEY_RELEASE].key_release)
    {
      kbd->keyboard_layout[EN][keycode].key_release = 1;
      return ;
    }
  if (kbd->ctrl_pressed)
    print_key(kbd->keyboard_layout[EN][keycode].ctrl_key);
  else if (kbd->shift_pressed)
    print_key(kbd->keyboard_layout[EN][keycode].shift_key);
  else if (kbd->alt_pressed)
    print_key(kbd->keyboard_layout[EN][keycode].alt_key);
  else if (kbd->altgr_pressed)
    print_key(kbd->keyboard_layout[EN][keycode].altgr_key);
  else
    print_key(kbd->keyboard_layout[EN][keycode].key_press);
  kbd->keyboard_layout[EN][keycode].key_release = 0;
}

void	default_isr(void)
{
  putstr("default ISR\n");
}

void			keybd_isr(void)
{
  static unsigned	keycode;
  static t_keyboard     kbd;

  if (!kbd.keyboard_layout[0])
    init_kbd_layouts(&kbd);
  if ((keycode |= IN(GET_KEYCODE)) == 0xE0)
    {
      keycode <<= 8;
      return ;
    }
  switch (keycode) {
  case (LSHIFT):
  case (RSHIFT):
  case (LSHIFT + KEY_RELEASE):
  case (RSHIFT + KEY_RELEASE):
    kbd.shift_pressed ^= 1;
    break ;
  case (LALT):
  case (LALT + KEY_RELEASE):
    kbd.alt_pressed ^= 1;
    break ;
  case (LCTRL):
  case (RCTRL):
  case (LCTRL + KEY_RELEASE):
  case (RCTRL + KEY_RELEASE):
    kbd.ctrl_pressed ^= 1;
    break ;
  default:
    key_management(&kbd, keycode);
    break ;
  }
  keycode = 0;
}

void	clock_isr(void)
{
}
