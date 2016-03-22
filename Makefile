#
# JG - Makefile
#

CC		= gcc

LD		= ld

NASM		= nasm

CFLAGS		= -masm=intel -nostdlib -fno-builtin -m32	\
		-W -Wall -Wextra -Werror -I$(INC_FLD)

ASMFLAGS	= -f elf32

LDFLAGS		= -m elf_i386 --oformat binary -Ttext 1000

NAME		= floppy

INC_FLD		= ./includes/

SRCS_FLD	= ./srcs/

BOOTLD_SRC	= $(SRCS_FLD)bootloader.S

BOOTLD_NAME	= bootloader

KERNEL_NAME	= kernel

KERNEL_SRCS	= $(SRCS_FLD)kernel_main.c		\
		$(SRCS_FLD)init_fcts.c			\
		$(SRCS_FLD)gdt_fcts.c			\
		$(SRCS_FLD)idt_fcts.c			\
		$(SRCS_FLD)utils_fcts.c			\
		$(SRCS_FLD)keyboard_layout.c		\
		$(SRCS_FLD)isr_fcts.c

ASM_SRCS	= $(SRCS_FLD)isr_asm.S

KERNEL_OBJS	= $(KERNEL_SRCS:.c=.o)

ASM_OBJS	= $(SRCS_FLD)isr_asm.o

RM		= rm -f

FORMAT_FLAG	= -f bin

all:		$(NAME)

$(BOOTLD_NAME):
		$(NASM) $(FORMAT_FLAG) $(BOOTLD_SRC) -o $(BOOTLD_NAME)

$(KERNEL_NAME):	$(KERNEL_OBJS)
		$(NASM) $(ASMFLAGS) $(ASM_SRCS)
		$(LD) $(LDFLAGS) $(KERNEL_OBJS) $(ASM_OBJS) -o $(KERNEL_NAME)

$(NAME):	$(BOOTLD_NAME) $(KERNEL_NAME)
		cat $(BOOTLD_NAME) $(KERNEL_NAME) /dev/zero | dd of=$(NAME) bs=512 count=2880

clean:
		$(RM) $(BOOTLD_NAME) $(KERNEL_OBJS) $(ASM_OBJS)

fclean:		clean
		$(RM) $(NAME)
		$(RM) $(KERNEL_NAME)

re:		fclean all

.PHONY:		re clean fclean all
