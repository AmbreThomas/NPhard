CC = gcc
SRC = *.c
OBJ = $(SRC:.c=.o)
HEADERS = dsatur.h
NAME = dsat

.PHONY: all, clean, fclean, re

all : $(NAME)

$(NAME) : $(OBJ)
	@$(CC) -o $(NAME) $(SRC)

$(OBJ) : $(SRC) $(HEADERS)
	@$(CC) -c $(SRC)

clean :
	@/bin/rm -f $(OBJ)

fclean : clean
	@/bin/rm -f $(NAME)

re : fclean all
