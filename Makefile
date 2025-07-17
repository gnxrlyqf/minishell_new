CC = cc
CFLAGS = -Wall -Wextra -Werror
INC = -I./includes
LIB = -lreadline
SRC = $(wildcard src/*/*.c) src/main.c
NAME = minishell

.PHONY: all clean fclean re bonus
.SECONDARY:

all: $(NAME)

$(NAME): $(SRC:.c=.o)
	$(CC) $^ $(LIB) -o $(NAME)

%.o: %.c
	$(CC) $(INC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(SRC:.c=.o) $(BONUS:.c=.o)

fclean: clean
	rm -rf $(NAME) $(BNAME)

re: fclean all