NAME = interview-master
CC = cc
CFLAGS = -Wall -Wextra -Werror -std=c11
INCLUDES = -Iinclude
SRC = src/main.c src/app.c src/questions.c src/progress.c src/ui.c
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJ) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

test: $(NAME)
	./$(NAME) --self-test

run: $(NAME)
	./$(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all


.PHONY: all test run clean fclean re install
