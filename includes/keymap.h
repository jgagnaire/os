/*
** Used to set keyboard layout
*/

#ifndef KEYMAP_H_
# define KEYMAP_H_

# define LSHIFT 0x2A
# define LCTRL 0x1D
# define LALT 0x38
# define RSHIFT 0x36
# define RCTRL 0xE01D
# define ALTGR 0xE038
# define GET_KEYCODE 0x60

# define KEY_RELEASE 0x80
# define LAYOUT_SIZE(S) (sizeof(S) / sizeof(S[0]))

enum		e_keyboardLayout
  {
    EN = 0,
    FR
  };

typedef struct	s_keypress
{
  unsigned 	key_press;
  unsigned 	ctrl_key;
  unsigned 	shift_key;
  unsigned 	alt_key;
  unsigned 	altgr_key;
  char		key_release;
}		t_keypress;

typedef struct	s_keyboard
{
  char		ctrl_pressed;
  char		shift_pressed;
  char	        alt_pressed;
  char	        altgr_pressed;
  char	        num_unlocked;
  char	        caps_unlocked;
  t_keypress	*keyboard_layout[2];
}		t_keyboard;

#endif /* !KEYMAP_H_ */
