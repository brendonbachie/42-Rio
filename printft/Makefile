CC=cc
CFLAGS=-Wall -Werror -Wextra
HEADER = libftprintf.h
SRC = ft_convert_char.c \
      ft_convert_num.c \
      ft_hex_fd.c \
      ft_numlen.c \
      ft_printf.c \
      ft_put_uns_fd.c \
      ft_convert_string.c \
      ft_verify.c \
      ft_plus.c
      
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
