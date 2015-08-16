#
# JG - Makefile
#

NAME		= floppy

BOOTLD_SRC	= bootloader.S

KERNEL_SRC	= kernel.S

BOOTLD_NAME	= bootloader

KERNEL_NAME	= kernel

RM		= rm -f

NASM		= nasm

FORMAT_FLAG	= -f bin

all:		$(NAME)

$(BOOTLD_NAME):
		$(NASM) $(FORMAT_FLAG) bootloader.S

$(KERNEL_NAME):
		$(NASM) $(FORMAT_FLAG) kernel.S

$(NAME):	$(BOOTLD_NAME) $(KERNEL_NAME)
		cat $(BOOTLD_NAME) $(KERNEL_NAME) /dev/zero | dd of=$(NAME) bs=512 count=2880

clean:
		$(RM) $(BOOTLD_NAME) $(KERNEL_NAME)

fclean:		clean
		$(RM) $(NAME)

re:		fclean all

.PHONY:		re clean fclean all
