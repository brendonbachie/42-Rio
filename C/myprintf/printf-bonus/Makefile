CC = cc
CFLAGS = -Wall -Werror -Wextra -g
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

BONUS_SRC = ft_convert_char_bonus.c \
            ft_convert_num_bonus.c \
            ft_hex_fd_bonus.c \
            ft_numlen_bonus.c \
            ft_printf_bonus.c \
            ft_put_uns_fd_bonus.c \
            ft_convert_string_bonus.c \
            ft_verify_bonus.c \
            ft_putnbr_fd_bonus.c \
            ft_putstr_fd_bonus.c \
            ft_strchr_bonus.c \
            ft_strlen_bonus.c \
            utils_bonus.c \
            ft_flags_bonus.c \
            ft_putchar_fd_bonus.c \
            putnbr_flags_bonus.c \
            ft_itoa_bonus.c \
            ft_itoh_bonus.c \
            utils2_bonus.c

OBJ = $(SRC:.c=.o)
BONUS_OBJ = $(BONUS_SRC:.c=.o)

NAME = libftprintf.a

all: $(NAME)

$(NAME): $(OBJ)
	ar rcs $(NAME) $(OBJ)

bonus: $(BONUS_OBJ)
	ar rcs $(NAME) $(BONUS_OBJ)

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(BONUS_OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus
