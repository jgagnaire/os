/*
** JG - kernel.c
*/

void	_start(void)
{
  char	*fb;

  fb = (void *)0xB8A00;
  *(fb + 1) = 'L';
  *(fb + 2) = 0B000101;
  *(fb + 3) = 'E';
  *(fb + 4) = 0B000101;
  *(fb + 5) = 'L';
  *(fb + 6) = 0B000101;
  while (1);
}
