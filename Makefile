#
# JG - Makefile
#

CC		= gcc

LD		= ld

CFLAGS		= -W -Wall -Wextra -Werror -masm=intel

LDFLAGS		= --oformat binary -Ttext 1000

NAME		= floppy

BOOTLD_SRC	= bootloader.S

BOOTLD_NAME	= bootloader

KERNEL_NAME	= kernel

KERNEL_SRCS	= kernel.c

KERNEL_OBJS	= $(KERNEL_SRCS:.c=.o)

RM		= rm -f

NASM		= nasm

FORMAT_FLAG	= -f bin

all:		$(NAME)

$(BOOTLD_NAME):
		$(NASM) $(FORMAT_FLAG) bootloader.S

$(KERNEL_NAME):	$(KERNEL_OBJS)
		$(LD) $(LDFLAGS) $(KERNEL_OBJS) -o $(KERNEL_NAME)

$(NAME):	$(BOOTLD_NAME) $(KERNEL_NAME)
		cat $(BOOTLD_NAME) $(KERNEL_NAME) /dev/zero | dd of=$(NAME) bs=512 count=2880

clean:
		$(RM) $(BOOTLD_NAME) $(KERNEL_OBJS)

fclean:		clean
		$(RM) $(NAME)
		$(RM) $(KERNEL_NAME)

re:		fclean all

.PHONY:		re clean fclean all
