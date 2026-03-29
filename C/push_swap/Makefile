NAME = libftpushswap.a

CC = cc
CFLAGS = -Wall -Wextra -Werror -g
RM = rm -f
AR = ar -rc

# ===== PATHS =====
LIBFT_PATH = ./libft
LIBFT = $(LIBFT_PATH)/libft.a
BONUS_PATH = ./push_swap_bonus

# ===== TARGETS =====
TARGET = push_swap
BONUS_TARGET = checker

# ===== SOURCES =====
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
	target.c

BONUS_SRCS = 	$(BONUS_PATH)/checker_bonus.c \
		$(BONUS_PATH)/errors_bonus.c \
		$(BONUS_PATH)/ft_splitandjoin_bonus.c \
		$(BONUS_PATH)/get_next_line_bonus.c \
		$(BONUS_PATH)/list_utils_bonus.c \
		$(BONUS_PATH)/reverse_rotate_bonus.c \
		$(BONUS_PATH)/rotate_bonus.c \
		$(BONUS_PATH)/switch_push_bonus.c \


OBJS = $(SRCS:.c=.o)
BONUS_OBJS = $(BONUS_SRCS:.c=.o)

# ===== RULES =====
all: $(TARGET)

$(LIBFT):
	@make -C $(LIBFT_PATH)

$(TARGET): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBFT)

bonus: all $(BONUS_TARGET)

$(BONUS_TARGET): $(BONUS_OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -o $(BONUS_TARGET) $(BONUS_OBJS) $(LIBFT)

clean:
	@make -C $(LIBFT_PATH) clean
	@$(RM) $(OBJS) $(BONUS_OBJS) $(NAME)

fclean: clean
	@make -C $(LIBFT_PATH) fclean
	@$(RM) $(TARGET) $(BONUS_TARGET) $(NAME)

re: fclean all

.PHONY: all bonus clean fclean re
