NAME = checkerpush.a
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
TARGET = checker
RM = rm -f
AR = ar -rc

LIBFT_PATH = ./libft
LIBFT = $(LIBFT_PATH)/libft.a

SRCS = checker.c \
	errors.c \
	ft_splitandjoin.c \
	get_next_line.c \
	list_utils.c \
	reverse_rotate.c \
	rotate.c \
	switch_push.c \

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
