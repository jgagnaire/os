#
# JG - Makefile
#

CC		= gcc

LD		= ld

CFLAGS		= -masm=intel -nostdlib -fno-builtin -m32	\
		-W -Wall -Wextra -Werror -I$(INC_FLD)

LDFLAGS		= -melf_i386 --oformat binary -Ttext 1000

NAME		= floppy

INC_FLD		= ./includes/

SRCS_FLD	= ./srcs/

BOOTLD_SRC	= $(SRCS_FLD)bootloader.S

BOOTLD_NAME	= bootloader

KERNEL_NAME	= kernel

KERNEL_SRCS	= $(SRCS_FLD)kernel_main.c	\
		$(SRCS_FLD)utils.c		\
		$(SRCS_FLD)esc_char_fcts.c

KERNEL_OBJS	= $(KERNEL_SRCS:.c=.o)

RM		= rm -f

NASM		= nasm

FORMAT_FLAG	= -f bin

all:		$(NAME)

$(BOOTLD_NAME):
		$(NASM) $(FORMAT_FLAG) $(BOOTLD_SRC) -o $(BOOTLD_NAME)

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
