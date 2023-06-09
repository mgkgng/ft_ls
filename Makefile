CC = gcc
CFLAGS = -Wall -Wextra -Werror
TARGET = ft_ls
SRC = src/main.c src/parse.c src/ls.c src/display.c src/utils.c src/libft.c
OBJ = $(SRC:.c=.o)
INC = -Iinclude/

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(INC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(TARGET)

re: fclean all

.PHONY: all clean fclean re