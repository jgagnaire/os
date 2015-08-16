/*
** JG - kernel.c
*/

void	_start(void)
{
  char	*fb;

  fb = (void *)0xB8A01;
  *(fb + 2) = 'L';
  *(fb + 3) = 0B000101;
  *(fb + 4) = 'E';
  *(fb + 5) = 0B000101;
  *(fb + 6) = 'L';
  *(fb + 7) = 0B000101;
  while (1);
}
