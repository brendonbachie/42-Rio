CC=cc
CFLAGS=-Wall -Werror -Wextra
HEADER = ft_printf.h
SRC = ft_convert_char.c \
      ft_convert_num.c \
      ft_hex_fd.c \
      ft_numlen.c \
      ft_printf.c \
      ft_put_uns_fd.c \
      ft_convert_string.c \
      ft_verify.c \
      ft_putchar_fd.c \
      ft_putnbr_fd.c \
      ft_putstr_fd.c \
      ft_strchr.c \
      ft_strlen.c \
      
OBJ=$(SRC:.c=.o)
NAME=libftprintf.a

all: $(NAME)

.o: .c $(HEADER)
	$(CC) -c $(CFLAGS) $< -o $@

$(NAME): $(OBJ)
	ar rcs $(NAME) $(OBJ)

clean:
	rm -f $(OBJ)
fclean:
	rm -f $(NAME)
	
re: fclean all
