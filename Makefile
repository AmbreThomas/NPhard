CC = gcc
SRC = *.c
FLAG = -Werror -Wextra -Wall
OBJ = $(SRC:.c=.o)
HEADERS = graphe.h
NAME = petriboxes

.PHONY: all, clean, fclean, re

all : $(NAME)

$(NAME) : $(OBJ)
	$(CC) -o $(NAME) $(OBJ)

$(OBJ) : $(SRC) $(HEADERS)
	$(CC) -c $(SRC) $(FLAG)

clean :
	/bin/rm -f $(OBJ)

fclean : clean
	/bin/rm -f $(NAME)

re : fclean all
