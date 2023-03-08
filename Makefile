##
## EPITECH PROJECT, 2021
## B-CCP-400-MPL-4-1-theplazza-bastien.boymond
## File description:
## Makefile
##

CC			= g++

SRC			= 	src/main.cpp		\
				src/Core.cpp		\
				src/Protocol.cpp	\
				src/Ai.cpp	\
				src/Evaluate.cpp	\

OBJ			= $(SRC:.cpp=.o)

NAME		= pbrain-gomoku-ai
CPPFLAGS	= -std=c++17 -W -Wall -Wextra -g3 -I./src

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

