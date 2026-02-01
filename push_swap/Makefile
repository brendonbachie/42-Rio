NAME = libftpushswap.a
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
TARGET = push_swap
RM = rm -f
AR = ar -rc

LIBFT_PATH = ./libft
LIBFT = $(LIBFT_PATH)/libft.a

SRCS = cost_calc.c \
	errors.c \
	list_utils.c \
	utils.c \
	small_numbers.c \
	ft_splitandjoin.c \
	chose_move.c \
	switch_push.c \
	rotate.c \
	reverse_rotate.c \
	push_swap.c \
	algorithm.c \
	target.c \

OBJS = $(SRCS:.c=.o)

all: $(TARGET) $(NAME)

$(TARGET): $(OBJS) $(NAME)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(NAME)

$(LIBFT):
	@make -C $(LIBFT_PATH)

$(NAME): $(LIBFT)
	@cp $(LIBFT) $(NAME)
	@$(CC) $(CFLAGS) -c $(SRCS)
	@$(AR) $(NAME) $(OBJS)

clean: 
	@make -C $(LIBFT_PATH) clean
	@$(RM) $(OBJS)

fclean: clean
	@make -C $(LIBFT_PATH) fclean
	@$(RM) $(NAME)
	@$(RM) $(TARGET)

re: fclean all

.PHONY: all clean fclean re
